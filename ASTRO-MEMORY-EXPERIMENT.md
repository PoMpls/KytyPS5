# Opt-in ASTRO BOT memory-pressure experiment

Based on the stable Bryan 719050e + local v4 fixes, with Windows-only CI. The stable/default branch `astro-bot-bryan-v4` is unchanged by this experiment.

Set `KYTY_ASTRO_MEMORY_RELIEF=1` to notify Bryan's existing mip-stat relief policy when device memory reaches TextureCache's existing pressure threshold. Unset/0 keeps v4 behavior. No new garbage-collector thresholds or resource retirement rules are introduced. The existing 300-report in-use history, 15-second relief and 20-second cooldown remain in effect.

Rationale: v4 reports mip0 for every counter outside relief, allowing unused textures to remain at full detail until AMPR detects streaming thrash. Pressure-triggered relief may release that detail sooner, lowering guest streaming memory and device memory. Benefit is a hypothesis, not a measured improvement.

Tradeoff: unused counters can lead the guest streamer to reload detail later. Compare the same scene for visual quality, loading churn, RAM/VRAM and frame rate. Do not enable by default until tested. This is not accurate per-pixel mip feedback.

Validation: Release compilation passed. pad_haptics, windows_file_read, virtual_memory_allocation and guest_red_zone_patcher passed (4/4). These tests do not validate pressure-triggered runtime behavior. A local A/B package with drain-stats and mip-stats logging is prepared; no runtime result is claimed yet.
