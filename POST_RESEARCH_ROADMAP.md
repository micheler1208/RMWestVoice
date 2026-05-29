# RM West Voice Post-Research Roadmap

This roadmap translates the technical and historical research in
`reference/Technical and Historical Deep Research for RM West Voice.md` into
small implementation tasks. Each task should be handled as its own branch,
commit, and push so the synth can evolve without large mixed changes.

## Starting Baseline Before Task Sequence

At the start of this roadmap, the project was a stabilized JUCE8 synth prototype:

- CMake is the reliable build entrypoint for Standalone and VST3 targets.
- `RMWestVoiceAudioProcessor` owns APVTS state, one `juce::Synthesiser`, one
  `SynthVoice`, one `SynthSound`, low-pass/high-pass filter objects, and final
  master gain.
- `SynthVoice` currently renders one oscillator through fixed voice gain and an
  ADSR envelope.
- `OscData` provides the Day/Night oscillator branch, a detune toggle, a fixed
  internal LFO, and dormant FM support.
- `FilterData` wraps a `juce::dsp::StateVariableTPTFilter<float>`; the processor
  applies a low-pass stage and a high-pass stage after the synth render.
- The UI exposes Day, Night, Detune, ADSR, low-pass cutoff, and volume; several
  APVTS parameters are not surfaced.
- Plugin state is serialized through APVTS XML state.

That baseline was buildable and useful, but not yet the historically
credible mono lead described by the research document.

## Research-Backed Target

The target instrument is a behavior-first West Coast lead synth:

- Strict mono performance layer with a note stack, selectable note priority, and
  legato detection.
- Glide/portamento with Off, Always, and Auto-Legato modes.
- Configurable pitch bend range, with wider classic options rather than only
  the generic +/-2 semitone default.
- Performer-controlled vibrato driven primarily by mod wheel, with fade-in and
  optional aftertouch influence.
- Two-oscillator subtractive tone core with curated wave choices: Saw, Tri,
  Saw+Tri, and Saw+Pulse.
- LP24 filter as the main filter voice, with resonance, key tracking, envelope
  amount, and musically contained drive.
- Optional hybrid/digital color layer for the late-1980s/early-1990s branch
  documented in the research, implemented through original synthesis only.
- Dry mono source first, then stereo width/doubler, delay, and reverb as
  post-voice presentation effects.
- A focused UI that exposes musical controls and keeps implementation controls
  compact or advanced.
- Factory presets that demonstrate the target vocabulary without copying any
  protected recording or using ambiguous external branding.

## Parameter Policy

RM West Voice is still pre-release, so the post-research implementation may
break the current parameter surface when the result is cleaner and easier to
maintain.

Default policy:

- Introduce a v2 APVTS parameter surface with clear, stable IDs.
- Do not preserve `DAY` / `NIGHT` as two public booleans unless a later task
  finds a concrete reason to keep them internally.
- Do not preserve dormant FM parameters as public parameters unless the new
  design intentionally reintroduces an FM or hybrid control.
- Prefer readable musical IDs such as `WAVE`, `GLIDE_MODE`, `GLIDE_TIME`,
  `BEND_RANGE`, `VIB_DEPTH`, `FILTER_CUTOFF`, `DRIVE`, `WIDTH`, and
  `OUTPUT_GAIN`.
- Document any intentional breaking change in the task that introduces it.

## Task Order

Task 1 is this roadmap commit. Tasks 2-12 have now been implemented and the
roadmap remains the reference checklist for the v1 feature scope.

| Task | Name | Scope |
| --- | --- | --- |
| 2 | Parameter Surface V2 | Move parameter declarations into a dedicated module and replace legacy IDs as features are brought forward. |
| 3 | Test Harness + Mono Note Stack | Add a small CMake test target and implement pure logic tests for mono note behavior. |
| 4 | Custom Mono Lead Engine | Route MIDI through a controlled mono engine path instead of relying on generic synth voice scheduling. |
| 5 | Glide / Portamento | Add Off, Always, and Auto-Legato glide with rate-based timing. |
| 6 | Oscillator Core | Replace the current single oscillator branch with a two-oscillator curated lead core. |
| 7 | Pitch Bend + Expressive Vibrato | Add bend range, smoothed pitch wheel, mod-wheel vibrato, fade-in, and optional aftertouch behavior. |
| 8 | Filter + Drive Stage | Move the lead path to LP24 ladder-style filtering with drive, key tracking, and filter envelope amount. |
| 9 | Character + Hybrid Color | Add Analog/Worm/Hybrid character behavior and an original synthesized hybrid color layer. |
| 10 | Post-Voice FX | Add measured width, delay, and reverb after the dry mono source. |
| 11 | UI Pass | Rebuild the editor around performance, tone, filter, FX, and output sections. |
| 12 | Factory Presets + Release Hygiene | Add initial presets, update documentation, verify assets/licenses, and prepare release notes. |

## Verification Discipline

For each implementation task:

- Start from a clean worktree.
- Keep the change scoped to the task.
- Build Standalone and VST3 in Debug and Release unless the task is explicitly
  documentation-only.
- Add or update tests when the task introduces logic that can be tested outside
  the plugin host.
- Review the diff before committing.
- Commit and push the task branch before starting the next task.

For documentation-only tasks, diff review is enough unless the change touches
build configuration or generated files.
