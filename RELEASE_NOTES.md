# RM West Voice Release Notes

## 1.0.0 Pre-Release Snapshot

This snapshot completes the post-research implementation roadmap through Task
12. It is a buildable instrument milestone, not a public distribution candidate
until the asset license blockers are resolved.

## Highlights

- Replaced the original prototype parameter surface with a cleaner v2 APVTS
  surface.
- Added a custom mono lead engine with note stack behavior and controlled MIDI
  handling.
- Added selectable mono note priority, rate-based glide, discrete pitch bend
  range choices, mod-wheel vibrato, fade-in, and optional aftertouch
  contribution.
- Reworked the oscillator path into a two-oscillator lead core with curated
  wave choices.
- Added LP24 lead filtering with drive, key tracking, and filter envelope
  movement.
- Added Analog, Worm, and Hybrid character modes with original synthesized
  color behavior.
- Added post-voice width, delay, and reverb processing.
- Rebuilt the editor around Performance, Tone, Filter, FX, and Output sections.
- Added five host-visible factory programs.

## Factory Programs

The initial program list is documented in [FACTORY_PRESETS.md](FACTORY_PRESETS.md):

- Classic Worm
- Smooth Whine
- Hybrid Lead
- Dry Analog
- Wide Mix Lead

## Verification

Before cutting a release candidate, run:

```powershell
cmake -S "RM West Voice" -B "RM West Voice\Builds\VisualStudio2022" -G "Visual Studio 17 2022" -A x64
cmake --build "RM West Voice\Builds\VisualStudio2022" --config Debug --target RMWestVoiceTests RMWestVoiceProcessorTests RMWestVoice_Standalone RMWestVoice_VST3 -- /m
ctest --test-dir "RM West Voice\Builds\VisualStudio2022" -C Debug --output-on-failure
cmake --build "RM West Voice\Builds\VisualStudio2022" --config Release --target RMWestVoiceTests RMWestVoiceProcessorTests RMWestVoice_Standalone RMWestVoice_VST3 -- /m
ctest --test-dir "RM West Voice\Builds\VisualStudio2022" -C Release --output-on-failure
```

The Release standalone should also be smoke-tested by launching it, keeping it
open briefly, and closing it cleanly.

## Known Release Blockers

- Bundled fonts and background image are not cleared for public distribution.
  See [ASSET_LICENSE_AUDIT.md](ASSET_LICENSE_AUDIT.md).
- There is no standalone preset browser or import/export file format; factory
  presets are exposed through host programs.
- Final sound-design tuning should happen after asset replacement and before
  any public binary release.
