# RM West Voice Post-Research Task Record

## Summary

Original baseline before the post-research work: JUCE8 buildable plugin, one
generic synth voice, simple Day/Night oscillator branch, detune toggle, ADSR,
state-variable low-pass/high-pass filtering, volume control, APVTS state
persistence, and limited UI coverage.

Research-backed target: true mono note behavior, glide/legato, wider pitch bend
choices, mod-wheel vibrato, a two-oscillator subtractive lead core, LP24-style
filtering with drive and key/envelope modulation, optional hybrid color,
post-voice stereo presentation, a small preset vocabulary, and complete UI
coverage for the musical controls.

Current implementation status: Tasks 1-12 are implemented in the current source
tree. The old public parameter surface was intentionally replaced because the
project is still pre-release.

## Implemented Task Sequence

1. **Roadmap Commit**
   Added a tracked implementation roadmap that maps the research requirements to
   small engineering tasks and records the pre-release parameter cleanup policy.
   No DSP behavior changed in this task.

2. **Parameter Surface V2**
   Moved APVTS parameter IDs and ranges into `PluginParameters.h`. Replaced
   legacy `DAY`/`NIGHT`, `DETUNE`, old filter IDs, `VOLUME`, and unused FM
   parameters with the current v2 surface.

3. **Test Harness + Mono Note Stack**
   Added CMake test targets and implemented `MonoNoteStack` with last-note
   default behavior, low-note option, note-off fallback, and legato detection.

4. **Custom Mono Lead Engine**
   Replaced the generic `juce::Synthesiser` scheduling path with
   `MonoLeadEngine`, routing MIDI note-on/off events through `MonoNoteStack`.

5. **Glide / Portamento**
   Added `GLIDE_MODE` with Off / Always / Auto-Legato choices and `GLIDE_TIME`
   as rate-based seconds per octave. Default mode is Auto-Legato.

6. **Oscillator Core**
   Reworked `OscData` into a two-oscillator lead source with Saw, Tri, Saw+Tri,
   and Saw+Pulse options, oscillator mix, detune in cents, glide integration,
   and no always-on fixed LFO behavior.

7. **Pitch Bend + Expressive Vibrato**
   Added bend range choices `2 / 5 / 7 / 12`, default `5`. Implemented smoothed
   pitch wheel handling, CC1 mod-wheel vibrato depth, vibrato rate, fade-in, and
   optional channel/key aftertouch contribution to vibrato depth.

8. **Filter + Drive Stage**
   Replaced the lead low-pass path with `juce::dsp::LadderFilter` in LPF24 mode.
   Added cutoff, resonance, key tracking, filter envelope amount, and drive.
   The current implementation uses contained filter drive and fixed voice/output
   gain rather than a separate gain-compensation block.

9. **Character + Hybrid Color**
   Added a `CHARACTER` control with Analog / Worm / Hybrid behavior. The Hybrid
   branch is original synthesis only: a low-level digital/flute-string style
   color layer blended behind the mono source, with no samples.

10. **Post-Voice FX**
   Added stereo presentation after the mono voice: conservative width/doubler,
   short delay with feedback, modest reverb, and output gain. The current delay
   is fixed-time in seconds and does not include a dedicated sync mode or
   high-cut filter.

11. **UI Pass**
   Rebuilt the editor around Performance, Tone, Filter, FX, and Output sections.
   The UI exposes the current musical APVTS controls, including note priority
   and aftertouch amount. There is no exposed glide-curve control.

12. **Factory Presets + Release Hygiene**
   Added five host-visible factory programs: Classic Worm, Smooth Whine, Hybrid
   Lead, Dry Analog, and Wide Mix Lead. Updated README, architecture notes,
   factory preset documentation, release notes, and bundled asset license audit.
   Public-facing materials avoid official Rockstar/GTA branding and do not claim
   exact soundtrack emulation.

## Public Interface

The current v1 APVTS surface is:

- `CHARACTER`
- `WAVE`
- `OSC_MIX`
- `DETUNE_CENTS`
- `GLIDE_MODE`
- `GLIDE_TIME`
- `NOTE_PRIORITY`
- `BEND_RANGE`
- `VIB_DEPTH`
- `VIB_RATE`
- `VIB_FADE`
- `VIB_AFTERTOUCH`
- `AMP_ATTACK`
- `AMP_DECAY`
- `AMP_SUSTAIN`
- `AMP_RELEASE`
- `FILTER_CUTOFF`
- `FILTER_RESONANCE`
- `DRIVE`
- `FILTER_KEYTRACK`
- `FILTER_ENV_AMOUNT`
- `OUTPUT_HIGHPASS_CUTOFF`
- `OUTPUT_HIGHPASS_RESONANCE`
- `WIDTH`
- `DELAY_MIX`
- `DELAY_TIME`
- `DELAY_FEEDBACK`
- `REVERB_MIX`
- `REVERB_SIZE`
- `REVERB_DAMPING`
- `OUTPUT_GAIN`

Current internal modules include `MonoNoteStack`, `MonoLeadEngine`,
`GlideState`, `PitchModulationState`, `FilterModulationState`,
`CharacterState`, `OscData`, `LeadFilterData`, `PostVoiceFxState`,
`PostVoiceFxData`, and `FactoryPresets`. CMake remains the source of truth;
`.jucer` is updated only when source lists or plugin metadata require it.

## Verification

The implemented scope is covered by:

- `RMWestVoiceTests` for pure logic coverage, including note priority, glide,
  pitch modulation, character state, post-voice FX state, and factory preset
  definitions.
- `RMWestVoiceProcessorTests` for headless render coverage, including MIDI
  rendering, finite/nonzero output, bounded levels, preset state restore, and
  release-to-silence behavior.
- Debug and Release builds for Standalone and VST3 targets.

Verification run on 2026-05-30:

```powershell
cmake --build "RM West Voice\Builds\VisualStudio2022" --config Release --target RMWestVoiceTests RMWestVoiceProcessorTests -- /m
cmake --build "RM West Voice\Builds\VisualStudio2022" --config Release --target RMWestVoice_Standalone -- /m
cmake --build "RM West Voice\Builds\VisualStudio2022" --config Release --target RMWestVoice_VST3 -- /m
ctest --test-dir "RM West Voice\Builds\VisualStudio2022" -C Release --output-on-failure
cmake --build "RM West Voice\Builds\VisualStudio2022" --config Debug --target RMWestVoiceTests RMWestVoiceProcessorTests -- /m
cmake --build "RM West Voice\Builds\VisualStudio2022" --config Debug --target RMWestVoice_Standalone RMWestVoice_VST3 -- /m
ctest --test-dir "RM West Voice\Builds\VisualStudio2022" -C Debug --output-on-failure
```

All commands above completed successfully. The separate Release standalone
launch smoke test remains part of release-candidate verification.

## Assumptions

The project is still pre-release, so parameter cleanup is allowed. The
instrument stays an original synthesis plugin with no third-party audio samples
and no Rockstar/GTA branding. The source voice remains mono; stereo width,
delay, and reverb happen only after the voice.
