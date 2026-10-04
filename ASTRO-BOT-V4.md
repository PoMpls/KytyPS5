# ASTRO BOT v4: preserved working reference

This branch keeps BryanKAdams/BryKytyPS5 commit `719050e2` and the local changes used by the previously tested ASTRO BOT v4 Windows build. It is not based on latest official main.

Local fixes: staged recovery of Windows file reads rejected with 998/1784; Windows critical-section spin count zero; bounded scratch-allocation retry releasing only idle completed buffers; estimated adaptive trigger states from effect and pressure.

All twelve modified source/test files match the preserved v4 working source, ignoring line-ending representation. The existing tested Windows executable is retained locally with SHA256 `C9520CF74BB6C6612BE99DADC665F394527C4BD290247F9B817A771EA1EDB382`.

The original official-based port is on branch `astro-bot-windows-fixes`. It passes its unit tests but the user observed pink images at runtime. Do not treat that branch as visually equivalent to this reference.

This branch includes the graphics and performance work already in Bryan's base and preserves its original Git history and credits. It does not include a game, save data, cache, private credentials or the user's separate non-RT game patch. Previous user reports of about 53 FPS are scene dependent; memory usage and CPU bottlenecks remain open.
