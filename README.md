# RMWestVoice

RMWestVoice is a JUCE-based software synthesizer plugin focused on a West Coast hip hop lead sound. The current project is an early but buildable instrument plugin, not a finished commercial release. Its immediate purpose is to provide a clean technical baseline for deeper sound-design research and later refinement.

The plugin is currently configured as a MIDI-controlled synth instrument with two supported build products:

- Standalone desktop application
- VST3 plugin

It does not produce MIDI output and it is not an audio effect. Audio is generated internally from incoming MIDI notes, then shaped by the current oscillator, envelope, filter, and volume controls.

## Current Status

This repository has been stabilized around JUCE8 and a Visual Studio 2022 build flow. The project can be generated and built through CMake, while the original `.jucer` file is kept aligned as project metadata.

The sound architecture is now moving through the post-research roadmap. The current runtime path uses a custom mono lead engine, while larger musical changes such as glide, pitch bend, controllable vibrato, saturation, effects, oscillator redesign, or historically informed G-funk lead modeling remain staged as separate tasks.

The post-research implementation sequence is tracked in [POST_RESEARCH_ROADMAP.md](POST_RESEARCH_ROADMAP.md).

## Product Scope

RMWestVoice is best understood as:

- A virtual instrument plugin.
- A subtractive-style lead synth prototype.
- A JUCE learning-to-production transition project.
- A pre-research technical baseline for a historically credible West Coast lead instrument.

RMWestVoice is not currently:

- A general-purpose workstation synth.
- A sampler.
- A MIDI effect.
- A polished commercial preset instrument.
- A faithful emulation of a specific copyrighted recording, soundtrack, or commercial synthesizer.

## Repository Layout

```text
.
|-- README.md
|-- ARCHITECTURE.md
|-- LICENSE
|-- .gitignore
`-- RM West Voice
    |-- CMakeLists.txt
    |-- RM West Voice.jucer
    |-- TECHNICAL_STABILIZATION.md
    |-- Source
    |   |-- PluginProcessor.*
    |   |-- PluginEditor.*
    |   |-- PluginParameters.h
    |   |-- SynthVoice.*
    |   |-- SynthSound.h
    |   |-- Engine
    |   |   |-- MonoLeadEngine.*
    |   |   `-- MonoNoteStack.*
    |   |-- Data
    |   |   |-- AdsrData.*
    |   |   |-- FilterData.*
    |   |   `-- OscData.*
    |   |-- UI
    |   |   |-- AdsrComponent.*
    |   |   |-- FilterComponent.*
    |   |   |-- OscComponent.*
    |   |   |-- CustomLookAndFeel*.h
    |   |   `-- CustomSlider.h
    |   |-- font
    |   `-- img
    |-- Tests
    |   `-- MonoNoteStackTests.cpp
    `-- Builds
```

`RM West Voice/Builds` is generated build output and is ignored by Git.

For a deeper description of the code structure, audio flow, parameters, and known technical debt, see [ARCHITECTURE.md](ARCHITECTURE.md).

## Requirements

The current Windows build expects:

- Windows
- Visual Studio 2022 with the C++ desktop workload
- CMake 3.22 or newer
- JUCE8 available at:

```text
C:/D/MIKE-VST/WORKSPACE JUCE/JUCE8/JUCE
```

The default JUCE path is defined in `RM West Voice/CMakeLists.txt` as `JUCE_ROOT`. If JUCE lives somewhere else, pass a different path during CMake configure:

```powershell
cmake -S "RM West Voice" -B "RM West Voice\Builds\VisualStudio2022" -G "Visual Studio 17 2022" -A x64 -DJUCE_ROOT="C:/path/to/JUCE"
```

## Build Instructions

From the repository root:

```powershell
cmake -S "RM West Voice" -B "RM West Voice\Builds\VisualStudio2022" -G "Visual Studio 17 2022" -A x64
```

Build Debug:

```powershell
cmake --build "RM West Voice\Builds\VisualStudio2022" --config Debug --target RMWestVoice_Standalone RMWestVoice_VST3 -- /m
```

Build Release:

```powershell
cmake --build "RM West Voice\Builds\VisualStudio2022" --config Release --target RMWestVoice_Standalone RMWestVoice_VST3 -- /m
```

The generated Visual Studio solution is:

```text
RM West Voice/Builds/VisualStudio2022/RMWestVoice.sln
```

## Test Instructions

The CMake build also defines a small logic test executable for non-audio engine code.

Build the tests:

```powershell
cmake --build "RM West Voice\Builds\VisualStudio2022" --config Debug --target RMWestVoiceTests -- /m
```

Run the tests:

```powershell
ctest --test-dir "RM West Voice\Builds\VisualStudio2022" -C Debug --output-on-failure
```

## Expected Build Outputs

Release standalone:

```text
RM West Voice/Builds/VisualStudio2022/RMWestVoice_artefacts/Release/Standalone/RM West Voice.exe
```

Release VST3:

```text
RM West Voice/Builds/VisualStudio2022/RMWestVoice_artefacts/Release/VST3/RM West Voice.vst3
```

The VST3 target uses the VST3 SDK bundled with JUCE8. No external Steinberg SDK checkout is required for the current CMake build.

`COPY_PLUGIN_AFTER_BUILD` is disabled, so the VST3 bundle is not automatically copied into the system VST3 plugin folder. This is deliberate: the build should first produce a local artifact before any install/distribution workflow is added.

## Plugin Metadata

Current technical metadata:

| Field | Value |
| --- | --- |
| Product name | RM West Voice |
| Project name | RMWestVoice |
| Manufacturer | RM Audio |
| Bundle identifier | `com.rmaudio.RMWestVoice` |
| Version | `1.0.0` |
| Plugin type | Instrument / Synth |
| Formats | Standalone, VST3 |
| MIDI input | Enabled |
| MIDI output | Disabled |
| Audio input | Not used |

## Current Controls

The existing control surface exposes a small number of parameters. These names are important because they are used by the `AudioProcessorValueTreeState` and may be seen by DAWs.

| Parameter ID | Label | Current purpose |
| --- | --- | --- |
| `WAVE` | Wave | Selects the current Tri/Saw oscillator branch. |
| `DETUNE_CENTS` | Detune | Applies oscillator detune in cents. |
| `AMP_ATTACK` | Amp Attack | ADSR attack time, shown in seconds. |
| `AMP_DECAY` | Amp Decay | ADSR decay time, shown in seconds. |
| `AMP_SUSTAIN` | Amp Sustain | ADSR sustain level. |
| `AMP_RELEASE` | Amp Release | ADSR release time, shown in seconds. |
| `FILTER_CUTOFF` | Filter Cutoff | Main low-pass filter cutoff. |
| `FILTER_RESONANCE` | Filter Resonance | Main low-pass filter resonance. |
| `OUTPUT_HIGHPASS_CUTOFF` | Output Highpass Cutoff | Fixed output high-pass cleanup cutoff. |
| `OUTPUT_HIGHPASS_RESONANCE` | Output Highpass Resonance | Fixed output high-pass cleanup resonance. |
| `OUTPUT_GAIN` | Output Gain | Final output gain. |

Task 02 introduced this pre-release v2 parameter surface. The old `DAY`/`NIGHT`, `DETUNE`, dormant `OSC1FM*`, `LP_FILTER*`, `HP_FILTER*`, and `VOLUME` IDs are intentionally not preserved.

## State Persistence

Plugin state is stored through `AudioProcessorValueTreeState`.

The processor serializes state by:

1. Copying the APVTS state.
2. Converting it to XML.
3. Writing the XML into the DAW-provided binary block.

State is restored by:

1. Reading XML from the binary block.
2. Checking that the XML tag matches the APVTS state type.
3. Replacing the APVTS state.

This means DAW projects and plugin presets should restore the current parameter values.

## Development Notes

The current build entrypoint is CMake:

```text
RM West Voice/CMakeLists.txt
```

The `.jucer` file remains useful as historical project metadata and has been updated to point at JUCE8 modules, but the stabilized build flow is the generated Visual Studio solution from CMake.

When changing the project:

- Keep sound-design changes separate from build and infrastructure changes.
- Treat the v2 parameter IDs as stable from this point forward unless a later task explicitly documents a breaking pre-release change.
- Add pure logic tests for engine behavior before wiring that behavior into the audio path where practical.
- Do not introduce new historical claims into the instrument without a source-backed research pass.
- Keep generated build output out of Git.

## Known Deferred Work

The following items are intentionally not solved in this stabilization pass:

- Historically informed oscillator and lead architecture.
- Glide/portamento and legato behavior.
- Pitch bend behavior.
- Controllable vibrato behavior.
- Saturation, chorus, delay, reverb, or other effects.
- Preset management beyond DAW state recall.
- Full UI redesign.
- Licensing audit for bundled fonts and image assets.
- Installer or deployment flow for the VST3 bundle.

## Verification Snapshot

The stabilized project has been checked with:

```powershell
cmake --build "RM West Voice\Builds\VisualStudio2022" --config Debug --target RMWestVoice_Standalone RMWestVoice_VST3 -- /m
cmake --build "RM West Voice\Builds\VisualStudio2022" --config Release --target RMWestVoice_Standalone RMWestVoice_VST3 -- /m
```

The Release standalone has also been smoke-tested by launching it, keeping it open briefly, and closing it cleanly.

## License

See [LICENSE](LICENSE).

Before public distribution, bundled fonts and image assets should receive a separate licensing review.
