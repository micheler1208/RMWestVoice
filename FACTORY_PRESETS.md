# RM West Voice Factory Presets

Task 12 adds the initial factory vocabulary as host-visible plugin programs.
These presets are not external preset files and they do not introduce a custom
preset browser. A host can select them through the plugin program API, while
DAW/session state continues to persist through APVTS parameter serialization.

## Preset List

| Program | Name | Purpose |
| --- | --- | --- |
| 1 | Default Coast Lead | Balanced default patch matching the current parameter defaults. |
| 2 | Tight Dry Analog | Dry, immediate analog saw lead with minimal width and no ambience. |
| 3 | Worm Glide Hook | Resonant glide lead with stronger vocal filter movement and light space. |
| 4 | Hybrid Glass Lead | Hybrid character patch with Saw+Pulse color, brighter width, and short ambience. |
| 5 | Dub Echo Lead | Slower glide patch with audible delay and reverb for post-voice presentation. |

## Implementation

The preset definitions live in:

```text
RM West Voice/Source/Presets/FactoryPresets.h
RM West Voice/Source/Presets/FactoryPresets.cpp
```

`RMWestVoiceAudioProcessor` exposes them through:

- `getNumPrograms`
- `getCurrentProgram`
- `setCurrentProgram`
- `getProgramName`

Changing a program writes the preset values into the existing APVTS parameters.
The selected factory program index is stored alongside the APVTS XML state, but
the parameter values remain the source of truth when a DAW session is restored.

## Policy

The preset names are descriptive and original. They intentionally avoid naming
specific records, artists, brands, or protected recordings.

Future preset work should keep source-backed sound-design claims out of preset
names and descriptions unless the claim has been explicitly researched and
documented.
