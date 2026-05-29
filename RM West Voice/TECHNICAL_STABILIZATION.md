# RMWestVoice Technical Stabilization

This note tracks the pre-research cleanup phase. It intentionally avoids changing the synth architecture or voicing.

## Build Targets

- Primary targets: Standalone and VST3.
- Toolchain: Visual Studio 2022 via MSBuild.
- JUCE baseline: `C:/D/MIKE-VST/WORKSPACE JUCE/JUCE8/JUCE`.
- Build entrypoint: `CMakeLists.txt` generates the Visual Studio 2022 solution in `Builds/VisualStudio2022`.
- The `.jucer` metadata is kept aligned, but the tracked build entrypoint for this stabilization pass is CMake.
- Expected Release outputs:
  - `Builds/VisualStudio2022/RMWestVoice_artefacts/Release/Standalone/RM West Voice.exe`
  - `Builds/VisualStudio2022/RMWestVoice_artefacts/Release/VST3/RM West Voice.vst3`

## Stabilized Items

- Project metadata now identifies the plugin as an RM Audio instrument.
- MIDI output is disabled; the plugin remains a MIDI-controlled synth instrument.
- Plugin state is serialized through `AudioProcessorValueTreeState`.
- The Visual Studio build uses JUCE8 modules and the bundled JUCE8 VST3 SDK.
- Unused LFO headers were removed from the project; the later oscillator-core task also removed the hidden fixed LFO from `OscData`.

## Deferred Until Deep Research

- Lead architecture changes such as saturation, effects, and final voicing.
- Any future FM or hybrid color behavior, if the post-research design intentionally reintroduces it.
- Final licensing audit for bundled fonts and background image before distribution.
