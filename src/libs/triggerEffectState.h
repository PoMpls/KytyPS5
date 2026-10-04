#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

namespace Libs::Controller {

// SDL exposes pressure but not the device's adaptive-trigger status report.
// Approximate that status using the validated output effect's ten travel zones.
inline int32_t TriggerEffectState(const std::array<uint8_t, 11>& effect, int pressure) {
	const auto zone = static_cast<unsigned>(std::clamp(pressure, 0, 255)) * 10u / 256u;
	const unsigned zones = effect[1] | (static_cast<unsigned>(effect[2]) << 8u);
	if (effect[0] == 0x25 && zones != 0) {
		unsigned start = 0, end = 9;
		while (start < 9 && (zones & (1u << start)) == 0) ++start;
		while (end > start && (zones & (1u << end)) == 0) --end;
		return zone >= end ? 5 : zone >= start ? 4 : 3;
	}
	if (effect[0] == 0x21) return (zones & (1u << zone)) != 0 ? 2 : 1;
	if (effect[0] == 0x26) return (zones & (1u << zone)) != 0 ? 7 : 6;
	return 0;
}

} // namespace Libs::Controller
