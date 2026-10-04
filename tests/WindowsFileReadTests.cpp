#include "common/platform/sysFileIO.h"
#include <windows.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void* guarded = nullptr;
static bool fault_seen = false;
static void Check(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "%s\n", message); std::abort(); }
}
static LONG CALLBACK HandleWrite(EXCEPTION_POINTERS* ex) {
    const auto* rec = ex->ExceptionRecord;
    if (rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && rec->NumberParameters >= 2 &&
        rec->ExceptionInformation[0] == 1 && rec->ExceptionInformation[1] >= reinterpret_cast<uintptr_t>(guarded) &&
        rec->ExceptionInformation[1] < reinterpret_cast<uintptr_t>(guarded) + 4096) {
        DWORD old = 0;
        if (VirtualProtect(guarded, 4096, PAGE_READWRITE, &old)) {
            fault_seen = true;
            return EXCEPTION_CONTINUE_EXECUTION;
        }
    }
    return EXCEPTION_CONTINUE_SEARCH;
}
int main() {
    wchar_t folder[MAX_PATH], filename[MAX_PATH];
    Check(GetTempPathW(MAX_PATH, folder) != 0 && GetTempFileNameW(folder, L"kft", 0, filename) != 0, "temporary file");
    std::array<unsigned char, 496> expected;
    for (size_t i = 0; i < expected.size(); ++i) expected[i] = static_cast<unsigned char>(i * 13);
    auto* output = SysFileOpenW(filename);
    uint32_t count = 0;
    SysFileWrite(expected.data(), expected.size(), *output, &count);
    Check(count == expected.size(), "write fixture");
    SysFileClose(output);
    auto* input = SysFileOpenR(filename);
    std::array<unsigned char, 496> ordinary {};
    SysFileRead(ordinary.data(), ordinary.size(), *input, &count);
    Check(count == expected.size() && ordinary == expected, "ordinary read");
    SysFileRead(ordinary.data(), ordinary.size(), *input, &count);
    Check(count == 0, "real EOF stays EOF");
    Check(SysFileSeek(*input, 0), "rewind");
    guarded = VirtualAlloc(nullptr, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    Check(guarded != nullptr, "allocate protected destination");
    auto handler = AddVectoredExceptionHandler(1, HandleWrite);
    DWORD old = 0;
    Check(handler != nullptr && VirtualProtect(guarded, 4096, PAGE_READONLY, &old), "protect destination");
    SysFileRead(guarded, expected.size(), *input, &count);
    Check(count == expected.size(), "protected destination must not become false EOF");
    Check(fault_seen && std::memcmp(guarded, expected.data(), expected.size()) == 0, "CPU copy invokes fault handler and preserves bytes");
    Check(SysFileTell(*input) == expected.size(), "offset advances exactly once");
    RemoveVectoredExceptionHandler(handler);
    VirtualFree(guarded, 0, MEM_RELEASE);
    SysFileClose(input);
    DeleteFileW(filename);
    std::puts("WindowsFileReadTests: PASS");
}
