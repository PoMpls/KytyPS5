# ASTRO BOT: Bryan updates and preserved v4 fixes

This branch combines BryanKAdams/BryKytyPS5 main through `a9e1ae5d` with the local fixes from the previously tested ASTRO BOT v4 Windows build. It is not based on latest official main. The exact old v4 reference is commit `b9ea80ae`.

Local fixes: staged recovery of Windows file reads rejected with 998/1784; Windows critical-section spin count zero; bounded scratch-allocation retry releasing only idle completed buffers; estimated adaptive trigger states from effect and pressure.

The twelve local fix files remain unchanged from the preserved v4 working source, ignoring line-ending representation. Bryan update `a9e1ae5d` adds Windows guest red-zone protection around emulated SSE4a instructions on hosts without SSE4a; it does not change the graphics path. The existing tested Windows executable is retained locally with SHA256 `C9520CF74BB6C6612BE99DADC665F394527C4BD290247F9B817A771EA1EDB382`.

The original official-based port is on branch `astro-bot-windows-fixes`. It passes its unit tests but the user observed pink images at runtime. Do not treat that branch as visually equivalent to this reference.

This branch includes the graphics and performance work already in Bryan's base and preserves its original Git history and credits. It does not include a game, save data, cache, private credentials or the user's separate non-RT game patch. Previous user reports of about 53 FPS are scene dependent; memory usage and CPU bottlenecks remain open.

Updated build validation: Windows Release compilation passed; pad_haptics, windows_file_read, virtual_memory_allocation and guest_red_zone_patcher passed (4/4). The updated binary has not yet been compared in-game with the preserved stable v4; no additional FPS or memory reduction is claimed.
