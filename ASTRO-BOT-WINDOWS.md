# ASTRO BOT: local Windows compatibility changes

Based on official KytyPS5 `719e02574a8cdcccf47dbd3118290a3e2ebd965e`.

This branch ports three changes from the user's previously tested ASTRO BOT v4 build (BryanKAdams/BryKytyPS5 `719050e`):

- Recover Windows ReadFile errors 998/1784 with a bounded host staging buffer, then copy through the guest memory fault handler. Retry only when the failed direct read transferred zero bytes.
- Use spin count zero for Windows critical sections. The performance benefit previously reported by the user is scene dependent and has not been established for this official base.
- Return adaptive trigger states estimated from trigger effects and pressure, allowing software to observe resistance zones instead of constant zero. SDL does not provide the controller's exact physical state; this remains an approximation.

The official controller intensity/volume settings remain available. The old v4 scratch-buffer retry uses Bryan's allocator pool, which is absent from this official base; it is not included here. No Wolverine/Senaxx code is included.

## Validation

Build targets: `launcher`, `pad_haptics_tests`, `windows_file_read_tests`.
Run: `ctest --test-dir <build> --output-on-failure -R "^(pad_haptics|windows_file_read)$"`.

Runtime comparison settings: 2560x1440, Immediate, vblank 60, Performance shader optimization, redzone and shader validation. The user's existing non-RT patch is supplied separately in the local test package. This repository contains no game, saves, caches or personal local configuration.

The previous v4 user's result (working punches and around 53 FPS in one scene) is not a measured result for this branch. Check scene rendering, adaptive trigger interactions, streaming and stability before relying on it.
