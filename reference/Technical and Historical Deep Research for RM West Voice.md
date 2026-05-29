# Technical and Historical Deep Research for RM West Voice

Converted from: `Deep research tecnica e storica per RM West Voice.pdf`

Original PDF metadata:

- Title: `Deep research tecnica e storica per RM West Voice`
- Author: `ChatGPT Deep Research`
- Producer: `WeasyPrint 68.0`
- Pages: 10

RM West Voice, if it wants to be historically credible without becoming an illegitimate copy of a specific track, should not chase a single "mythical preset". It should instead target a family of sonic behaviors born from the intersection of at least three historical lines: the ARP Pro Soloist / "Funky Worm" branch of early synthesized funk, the Minimoog / P-Funk / Dr. Dre branch of the expressive monophonic lead with glide and bend, and the late-1980s hybrid workstation branch represented by instruments such as the Yamaha SY77. That last branch matters because Colin Wolfe later challenged the idea that the famous tone in "Nuthin' But a 'G' Thang" was performed with a Moog, indicating instead a Yamaha SY77. For a plugin dedicated to that vocabulary, then, the smartest choice is not "Minimoog only", but a mono-subtractive core plus optional hybrid digital coloration.

## Historical Frame and Sound Philology

From a historiographic point of view, G-funk is described as a West Coast production style that emerged with Dr. Dre and is deeply rooted in the sonic memory of Parliament-Funkadelic and the previous funk era. It is not only a matter of beats, but of synth basses, whining leads, hi-fi arrangement, and played parts. Reverb's article on hip-hop's move into a "hi-fi" sound places *The Chronic* inside a process of hybridization between sampling and studio musicians. The quotes attributed to Colin Wolfe, and the historical account around them, insist that Dre worked with basses, keys, horns, guitars, an SSL desk, and a strong focus on mix polish.

The genealogy of the so-called "worm" or "West Coast whistle" starts much more clearly from Ohio Players' "Funky Worm" than from 1990s rap. In a Red Bull Music Academy interview, Junie Morrison says he found an ARP Soloist, immediately heard a worm-like riff in it, and brought that synth into the studio to record the track. Google Arts & Culture summarizes that sound as a Pro Soloist whose characteristic effect depends decisively on glide and solo-lead behavior, to the point that many other machines can imitate it even if they do not reproduce it identically. This is a crucial point: the identity of the sound family is more behavioral than iconographic.

The Moog/Minimoog contribution enters the story not because every West Coast timbre was a Minimoog, but because the expressive subtractive monosynth became the most natural vehicle for those phrases: pitch bend, vibrato, glide, low-pass filtering, and a single foreground note. The Moog Foundation notes that by the early 1970s the Minimoog had already become a reference instrument for many working musicians. MusicRadar and other journalistic sources explicitly connect it to Bernie Worrell's language and, through aesthetic descent, to Dre's productions. Archived quotes from the Colin Wolfe interview also contain direct references to "prominent Moog" in *Chronic*-era tracks such as "Deeez Nuuuts", "Dre Day", and the "high Moog part" in "Bitches Ain't Shit".

The most important correction is this: the most famous lead popularly associated with the West Coast does not, by itself, prove the exclusive role of the Minimoog. In secondary sources that point back to a recent Anthony Marinelli interview, Colin Wolfe is presented as saying that he did not use a Moog on "G Thang", but a Yamaha SY77. A later secondary reconstruction connects that result to the AFM plus AWM2 side of the instrument. In addition, in a quote derived from the Wax Poetics interview about the making of the record, Wolfe describes the live contribution on "Nuthin' but a 'G' Thang" as a synth string part and live bass, not as a "classic Moog lead". Therefore, for RM West Voice, the most serious reading is this: Minimoog and analog monos are central to the G-funk family, but the "G Thang" case introduces a hybrid/digital branch that must not be ignored.

For the GTA San Andreas reference, the cleanest primary source is Michael Hunter's official site. There, Hunter credits himself as composer and performer of the themes for *Grand Theft Auto: San Andreas* and *Grand Theft Auto IV*, and in the "Music from Grand Theft Auto" section he lists "The Theme From San Andreas" along with other game cues. However, among the sources recovered here, there is no equally solid primary source that identifies the exact instrumental chain of the theme lead. The design consequence is clear: the plugin can legitimately move inside the same West Coast/G-funk sound family, but it should not advertise itself as an "official" or "exact" replica of the San Andreas lead.

## Design Evidence Matrix

| Sonic characteristic | Historical or technical evidence | Design decision | Confidence and sources |
| --- | --- | --- | --- |
| Soloistic monophony | Minimoog, ARP Pro Soloist, and SH-101 are monophonic instruments or instruments oriented toward a single lead line. The Pro Soloist also had its own note-priority logic. | Strictly mono engine with a dedicated note stack; modern default: last-note priority; legacy option: low-note priority. | High |
| Glide as the primary signature | The "worm" is historically tied to glide/portamento. Moog service notes describe glide in seconds per octave. The SH-101 documents AUTO mode, active only during legato. | Expose Glide Time plus Mode: Off / Always / Auto-Legato. Historical default: rate-based. | High |
| Pitch bend as a gesture, not an ornament | Minimoog brochure: minimum bend range of 5 semitones. ARP Pro Soloist had programmable bend through touch sensitivity. | Offer configurable bend, with "Classic" presets wider than the standard +/-2. Suggested values: 2 / 5 / 7 / 12 semitones. | Medium-high |
| Expressive vibrato controlled by the performer | Minimoog centers pitch wheel and modulation wheel. ARP Pro Soloist allowed vibrato, wow, growl, and bend through touch sensor/aftertouch. | Use mod wheel -> vibrato depth as the default. Use aftertouch -> extra vibrato / brilliance / growl as an option. | High |
| Low-pass lead with moderate resonance | Minimoog: 24 dB/oct ladder LPF. SH-101 block diagram: classic VCF in a mono chain. SY77: 12/24 dB digital filters with EG/LFO. | Main filter: 24 dB low-pass, moderate resonance, contained envelope amount, adjustable key tracking. | High |
| Timbre not reducible to "Moog only" | Colin Wolfe connects "G Thang" to the SY77. Yamaha describes the SY77 as an AWM + FM hybrid with digital filtering. | Add an optional auxiliary digital layer for "hybrid whine/string" presets instead of selling the synth as a Minimoog clone. | High |
| Original source mostly mono, width afterward | Historical sources point to mono instruments. The most plausible modern reconstruction stereoizes the whistle through a double pass and opposite-side reverbs. | Chain: mono voice in the center, stereo FX after the voice. | Medium |

## Timbre Recipe and Expressive Controls

For the analog core of RM West Voice, the strongest historically grounded recipe is a monophonic subtractive lead with two or three oscillators, performance priority, a 24 dB low-pass filter, obvious glide, and wide pitch bend. The Moog documentation confirms the central role of the three oscillators, modulation assignable to pitch and cutoff, the 24 dB/oct ladder filter, and the two contour generators. The SH-101 block confirms that even a simpler architecture, one VCO, sub, noise, VCF, VCA, LFO, bender, and portamento, is enough to produce credible lead behavior. The ARP chain likewise shows that the "worm" side depends on enriched monophonic synthesis with filtering and modulation, not on enormous structural complexity.

On waveforms, the most prudent reading is this: saw and triangle are the most plausible base options for a singing lead. Pulse/square should not be excluded, but historically it appears more as a variant or as an ingredient in specific presets, especially in the ARP world. Google Arts & Culture's synthesis of the "Funky Worm" sound describes it as essentially a single sawtooth wave set to glide. The Minimoog manual offers a rich waveform set in the oscillator bank. The Pro Soloist service manual shows preset routing based on saw/pulse, filters, and a resonator bank. For the plugin, then, it is better to avoid an encyclopedic UI and provide a few highly curated combinations: Saw, Tri, Saw+Tri, and Saw+Pulse, with a "Character" macro that narrows the range musically.

On the filter, the most important thing is not to turn it into the real protagonist when the historical track often foregrounded glide, bend, and vibrato. A 24 dB low-pass remains the most credible default choice. Resonance should be able to "sing", but rarely to self-oscillation as the default. The presence, in the Pro Soloist, of a resonator bank, HPF, and more complex preset logic suggests that a simple ladder does not cover 100% of the vocabulary. For this reason, a good software solution is to introduce, internally, a pre-emphasis/formant option or a hidden legacy formant mode, useful for more nasal presets, without exposing an overly technical panel.

Typical envelopes, from a design point of view, do not need to be "brass staccato". For a West Coast lead, the most plausible behavior is a very short attack, short or medium-short decay, high sustain, and short but non-zero release, so that the line remains smooth during legato. On the filter, the envelope amount should stay moderate: too much contour pushes the timbre toward classic synth brass, while too little makes it sterile. As a practical factory-preset initialization, I would use near-zero amp attack, high sustain, short release; near-zero filter attack, short decay, medium or medium-low sustain; and key tracking between one third and half travel. This is not a documented "historical measurement", but an engineering synthesis consistent with the controls offered by historical monosynths and with the lead behavior reconstructed in modern sources.

For glide/portamento, the most useful source for a software decision is the contrast between two historical models: Minimoog equals rate-based, with service material expressing glide in seconds per octave; SH-101 equals AUTO mode during legato; and later Studio Electronics equals glide/auto glide, linear or exponential. Translated into product terms: RM West Voice should have three behavior choices in the main UI, Off, Always, and Auto, and at least two internal curve models, Rate and possibly Time, with Rate as the "historical" default. If you want an extra touch without making the UI heavier, Linear/Exponential can live in an Advanced menu.

For vibrato, the historically sensible behavior is not "always on", but activated or intensified by the performer. Minimoog and Pro Soloist are both expressive instruments in the physical sense of the term: pitch wheel, modulation wheel, touch sensitivity, wow, growl, brilliance, and vibrato. For the plugin this means vibrato should be implemented as a pitch LFO with fade-in and mod-wheel control, ideally with a medium-fast rate and moderate depth. Aftertouch is historically very coherent, but not essential if you want to stay minimal. The best choice is: mod wheel = depth, aftertouch = additional depth or brightness, delay/fade according to the preset.

On pitch bend range, the sources argue against dogma. Minimoog service material speaks of at least 5 semitones. Modern Minimoog-emulation documentation defines 7 semitones as the "authentic" setting for its own model. For a real plugin this leads to a simple practical decision: do not fix everything to +/-2 semitones as many generic synths do. The strongest choice is factory presets around +/-5, with quick selections for 2 / 5 / 7 / 12. That covers natural phrasing, wide scoops, and more theatrical effects without making the user's life harder.

Finally, if you want to include the 1989-1992 hybrid branch, the correct reference is not "model the whole SY77", but offer a small auxiliary digital source that brings a more flute/string/rompler-FM color into the plugin. Yamaha describes the SY77 as an instrument where an AWM generator and an FM generator work together with a digital filter. Sound On Sound describes it with 12/24 dB filters controlled by EG or LFO. In RM West Voice, this can become a low-volume optional layer, used only in some presets or called up by a Hybrid macro, enough to cover the historical case "it was not a Moog".

## Effects and Treatment in a G-Funk Context

In the classic West Coast context, the lead is easier to understand if it is separated into two layers: source and presentation. The historical source is often mono, physical, and highly performative. The presentation becomes wider and more polished inside hi-fi productions such as the ones described for *The Chronic*, with musicians, engineers, an SSL desk, and very high attention to finish. This means effects matter, but they should not replace the synth behavior. First comes the gesture, glide, bend, vibrato, and articulation. Then comes the stereo environment.

Saturation/drive is among the most plausible effects as an internal function, especially if implemented as pre-filter or mixer overdrive rather than high-gain guitar distortion. The SE-1 manual notes that distortion can appear by overloading the filter input. The "Red Eye" specifications even cite dedicated fuzz/overdrive hardware. In practice: it makes sense to have a Drive control that pushes the mixer/VCF into a slightly rough area, with automatic gain compensation. This contributes much more to the plugin's credibility than a huge rack of effects.

On chorus, doubler, and widening, the research supports an intermediate position. Sweetwater's reconstruction of the West Coast language discusses liberal use of stereo effects, a double pass of the whistle panned hard-left/hard-right, and crossed reverb sends. This suggests that a certain constructed stereo image is plausible, but as post-voice treatment, not as the original character of the lead. Consequently, a Width/Double module makes sense only if it stays discreet and possibly inactive in the more "analog worm" presets, while becoming much more plausible in Hybrid or Smooth Whine presets.

For delay and reverb, the most credible behavior is again moderate. Sweetwater uses slapback and throw delay as stylistically compatible devices in the West Coast world, and applies reverb to give depth to call-and-response leads. This does not justify putting a huge ping-pong delay on every preset. The best compromise for RM West Voice is: short/syncable but unobtrusive delay, contained feedback, internal high-cut; small or medium reverb, with low mix, intended more to place the sound than to transform it.

For EQ and compression, the research suggests caution before turning them into the synth's "sonic identity". The Reverb article and the Sweetwater reconstruction show that much of G-funk's hi-fi character depended on arrangement, layering, studio process, drum reinforcement, and mix processing, not on one synth containing everything already. Therefore, inside RM West Voice I would keep, at most, an output tone or tilt EQ and, if you really want it, a gentle output compressor used sparingly. But that would already be a secondary choice, not essential for historical credibility.

## Design Decisions for RM West Voice

The strongest proposal for JUCE/C++ is a custom mono engine rather than a generic polyphonic synth disguised as mono. JUCE documentation explains that `juce::Synthesiser` works with `SynthesiserSound` and `SynthesiserVoice`, and that `AudioProcessorValueTreeState` provides the standard way to manage persistent parameters. `juce::dsp::Oversampling` exists specifically to raise the internal sample rate around nonlinear processes. All of that is useful, but for a historical lead, the important behaviors are the ones generic synths often handle poorly: note priority, legato detection, envelope retriggering, historical portamento, and sample-accurate bend/vibrato. I therefore recommend either a `juce::Synthesiser` wrapper with one rigidly controlled voice, or, even better, a dedicated `MonoLeadEngine` integrated directly into the `AudioProcessor`.

The audio architecture can be very lean.

### MIDI / Performance Layer

Use a `MonoNoteStack` with selectable priority, legato detection, bend and mod-wheel smoothing, and optional aftertouch. This is where the engine decides whether a new note-on retriggers the amp/filter envelopes or only the filter, and whether glide starts only on overlapping notes or always. Historically, this is where credibility is won or lost.

### Tone Core

Use two main oscillators and an optional auxiliary layer. The first oscillator should always be present. The second can provide detune, interval, or register doubling. The auxiliary layer is there to color "SY-style" presets, not to turn the plugin into a workstation. My recommendation is therefore: band-limited analog oscillators plus a one-shot/static digital color layer callable through a macro. Everything should remain controllable with a small parameter set.

### Filter and Drive Stage

Use LP24 as the default, with level-vs-resonance compensation and pre-filter or intra-filter drive. Overdrive should be treated as an integral part of the timbre. Extra modes, formant, pre-emphasis, or legacy resonator, can be hidden behind a Character macro or a Legacy switch.

### Modulation Stage

One musical LFO dedicated to vibrato/brilliance is more coherent than a huge modulation matrix. Expose rate, depth, fade/delay, and a base destination to pitch. Optionally add a small amount to cutoff in the more vocal presets. Aftertouch can act on vibrato or brilliance as a nod to the Pro Soloist. MPE through `juce::MPESynthesiserVoice` is a future possibility, but not essential for the first release.

### FX / Output Stage

Use dry mono center first, then stereo width/doubler, delay, and reverb. Oversampling should be applied around nonlinear blocks, not necessarily across the entire chain, to contain CPU cost.

For the UI, I would expose only the parameters that genuinely change phrasing and the timbre family:

- Character
- Wave
- Osc Mix/Detune
- Glide Time
- Glide Mode
- Cutoff
- Resonance
- Filter Env
- Key Track
- Amp Contour
- Vibrato Depth
- Vibrato Rate
- Vibrato Fade
- Drive
- Delay
- Reverb
- Width
- Bend Range

I would keep the following parameters internal or place them in an Advanced panel:

- Note Priority
- Glide Curve
- Envelope Retrigger Policy
- Oversampling Factor
- Gain Compensation
- Analog Drift Amount
- Hybrid Layer Balance

The general rule is simple: show the user only what changes expressivity; keep implementation-shaping controls behind the scenes.

## Implementation Priorities

| Priority | Feature to implement | Why it is genuinely needed | Confidence and sources |
| --- | --- | --- | --- |
| Essential | Mono engine with note stack and priority | Without this, the plugin sounds like a generic synth, not a historical lead. | High |
| Essential | Glide Off / Always / Auto-Legato | It is a behavioral signature of the sound family. | High |
| Essential | LP24 with key tracking and drive | Covers the Minimoog/analog-mono branch credibly. | High |
| Essential | Mod-wheel vibrato with fade-in | This is the most historically grounded way to make the lead sing. | High |
| Essential | Configurable bend range: 2 / 5 / 7 / 12 | The modern +/-2 synth default is too narrow for many leads in this family. | Medium-high |
| Strongly recommended | Character macro with Analog and Hybrid branches | Resolves the Minimoog-vs-SY77 contradiction without bloating the interface. | High |
| Strongly recommended | Stereo FX after the voice | The source stays mono, but the presentation can widen. | Medium |
| Strongly recommended | Aftertouch -> vibrato/brilliance | Technically sensible nod to the Pro Soloist; improves playability. | Medium-high |
| Optional | Formant/resonator legacy mode | Useful for some "worm/vocal" presets, but not indispensable for release 1.0. | Medium |
| Optional | MPE or advanced performance macros | Interesting for future evolution, not required for baseline credibility. | Medium |

## Annotated Bibliography

**Bob Moog Foundation / Moog Music, historical Minimoog material.** The 1972 brochure and the Minimoog service notes are the most useful primary sources for fixing the design cornerstones: performer-oriented monophony, glide in seconds per octave, wide pitch bend, 24 dB low-pass filter, and classic contour generators. For RM West Voice they are core sources, not decorative references.

**Moog Minimoog Model D User Manual on ManualsLib.** This is not the best historical source for 1972 philology, but it is extremely useful because it makes the sections on oscillator bank, controllers, filter, contour, and performance tips readable in HTML. It is the most practical source for translating Minimoog behavior into software parameters.

**ARP Pro Soloist Service Manual.** This is a fundamental manual for understanding why the "worm" is not reducible to "a sawtooth with glide". The voice flow chart shows saw/pulse, HPF, LPF, resonator bank, ADSR/AR. The truth tables show preset logic and modulations such as vibrato and growl. Keep it close during the design of any legacy/formant modes.

**Roland SH-101 Owner's/Service Manual.** Excellent source for a function many mono plugins implement poorly: the manual states that AUTO portamento is active only during legato. The block diagram also documents how a relatively simple monosynth is already enough for this vocabulary.

**Yamaha SY77 official and historical documentation.** The Yamaha manual library confirms the existence of the official manual. Yamaha's synth history and the Sound On Sound technical review show the SY77's role as an AWM + FM + digital-filter hybrid, therefore as a distinct but relevant sonic branch for the late-1980s/early-1990s vocabulary. This is the source that justifies the idea of an auxiliary digital layer in the plugin.

**Studio Electronics SE-1 / SE-1X manuals and specifications.** These sources do not, by themselves, prove the SE-1 was used in any specific track, but they are valuable for understanding how the Minimoog inheritance was transformed into a programmable studio monosynth: triangle/saw/pulse, auto glide, linear/exponential choices, overdrive/fuzz. They are very useful as a model for "respectful modernization" of the historical mono lead.

**Junie Morrison interview, Red Bull Music Academy.** Almost a primary source for the birth of the "worm". It is historically strong because it directly connects the riff and the instrument to the author's experience. It prevents anachronisms: the West Coast vocabulary did not begin in 1992, it was reactivated.

**Google Arts & Culture, classic synthesizers behind iconic tracks.** Secondary source, but very clear and concise. It connects "Funky Worm" to the Pro Soloist and stresses that the patch's simplicity makes many imitations possible. Useful for explaining why RM West Voice should model behaviors, not only a synth brand.

**Reverb, "When Hip-Hop Went Hi-Fi".** Strong historical context source: live musicians, SSL desk, 808 reinforcement, Colin Wolfe's role, and Dre's attention to hi-fi finish. It is not a synthesis manual, but it helps avoid designing the plugin in a vacuum.

**Colin Wolfe / Wax Poetics, as quoted in secondary sources.** The archived quotes that pass through Gearspace and later Reverb reprocessing are useful because they directly connect Dre, Wolfe, P-Funk, live basses, synth string parts, and prominent Moog parts. They are very informative, but since they arrive here through secondary sources, they should be used with some methodological caution.

**Michael Hunter official site.** The best available primary source here for San Andreas credits. It confirms Hunter's role and the existence of the game cues, including "The Theme From San Andreas". Very useful for delimiting the field: artistic connection, yes; arbitrary reverse engineering, no.

**Sweetwater InSync, modern reconstruction of West Coast hip-hop.** Not a primary source, but the best technical reconstruction recovered in readable form for plausible waveforms, glide, subtle vibrato, double-pass stereo, reverb, and support percussion. Use it as a plausible contemporary recipe, not as definitive historical proof.

**JUCE documentation.** The official pages for `Synthesiser`, `SynthesiserVoice`, `SynthesiserSound`, `AudioProcessorValueTreeState`, and `dsp::Oversampling` are the correct sources for deciding the plugin's C++/JUCE skeleton. They are the bridge from sonic philology to maintainable implementation.

## Weak Sources, Contradictions, and Legal Constraints

The most important contradiction that emerged from the research is this: the popular narrative "that is the Minimoog sound" is useful as a timbral shortcut, but methodologically too crude. Concrete example: contemporary tutorials and recipes attribute the "G Thang" lead to a Minimoog, but sources connected to Colin Wolfe overturn that simplification by indicating a Yamaha SY77. Therefore, in this project, forum sources, Reddit, commercial recipes, and YouTube tutorials should be treated as operational folklore. They help reveal user expectations, but they do not establish historical truth. The detail of the specific SY77 presets, often reported as C-46 Flute plus D-61 WideString, remains, in the state of this research, a weak or secondary claim, not strong enough to be promoted into confirmed fact.

Legally, the line is much simpler: if you want to avoid problems, do not sample masters, stems, or recognizable fragments of pre-existing recordings. The U.S. Copyright Office educational guide and WIPO's summary agree that unauthorized sampling typically touches two distinct rights: the sound recording and the composition/song. Therefore the correct approach for RM West Voice is original synthesis, possible non-identifying stylistic interpolation, and no third-party audio asset without clearance.

On the practical/legal side, it is also wise to avoid any ambiguous branding with Rockstar, Grand Theft Auto, GTA, logos, or official artwork in the plugin's public materials. Rockstar/Take-Two copyright and trademark notices explicitly state that those marks and distinctive signs are protected. In other words: internally, you can say you are pursuing a 1990s West Coast sound family; publicly, it is much less prudent to use commercial messaging that appears to officially associate the plugin with the franchise.

For fonts, images, and visual resources, the cleanest path is to use only material with very clear licenses. The official Google Fonts FAQ states that fonts in the library are open source and usable commercially according to their specific license. The official SIL Open Font License text and FAQ explain the conditions, including Reserved Font Names when fonts are modified. For promotional images, the Creative Commons chooser and search engines such as Openverse are enough to set up a documentable and clean pipeline. Operationally: keep a `licenses/` folder, include the license texts distributed with fonts and assets, and maintain an internal spreadsheet with origin, source URL, license, author, and proof of purchase or authorization.

## Final Recommendation

The final result with the highest ratio of historical credibility to implementability is this: a lead-oriented, behavior-first monosynth, with a historical subtractive core, serious glide, expressive bend and vibrato, an LP24 filter with drive, a dry mono source, measured stereo FX, and a small optional digital hybrid layer to cover the SY77 branch. Everything else, large modulation matrices, polyphony, too-literal imitations of specific tracks, and "San Andreas"-style branding, is secondary or counterproductive.

## Source URL Appendix

The PDF contains repeated URL instances because of footnote and link rendering. This appendix preserves every unique source target extracted from the PDF annotations and visible source pages.

- https://chatgpt.com/?utm_src=deep-research-pdf
- https://daily.redbullmusicacademy.com/2015/04/junie-morrison-interview/
- https://www.jstor.org/stable/853712
- https://moogfoundation.org/thom-holmes-keith-emerson-an-appreciation-of-his-moog-musicianship/
- https://www.youtube.com/shorts/Qv6dYcExPvo
- https://www.olbapmusic.com/
- https://www.manualslib.com/manual/1164307/Moog-Minimoog-Model-D.html
- https://artsandculture.google.com/story/what-s-that-sound-the-classic-synthesizers-behind-10-iconic-tracks-musikinstrumenten-museum/bAXhk5CyN9w2KA?hl=en
- https://moogfoundation.org/wp-content/uploads/1972-Minimoog-INstrument.pdf
- https://archive.org/stream/synthmanual-moog-minimoog-service-notes/moogminimoogservicenotes_djvu.txt
- https://es.yamaha.com/es/musical-instruments/keyboards/explore/synth-40th/history/chapter03/
- https://reverb.com/news/when-hip-hop-went-hi-fi
- https://anaphonic.com/wp-content/uploads/studioelectronics_se1_om.pdf
- https://www.sweetwater.com/insync/california-love-re-creating-90s-west-coast-hip-hop-beats/
- https://docs.juce.com/master/classjuce_1_1Synthesiser.html
- https://synthfool.com/docs/Roland/SH_Series/Roland%20SH-101%20Owners%20%26%20Service%20Manuals.pdf
- https://docs.cherryaudio.com/cherry-audio/instruments/pro-soloist/getting-started
- https://docs.juce.com/master/classjuce_1_1dsp_1_1Oversampling.html
- https://juce.com/tutorials/tutorial_audio_processor_value_tree_state/
- https://www.vintagesynth.com/arp/pro-soloist
- https://synthfool.com/docs/Arp/ARP_Pro_Soloist-Service_Manual.pdf
- https://docs.juce.com/master/classjuce_1_1MPESynthesiserVoice.html
- https://usa.yamaha.com/products/contents/music_production/downloads/manuals/index.html?c=music_production&k=sy77&l=en&p=1
- https://www.studioelectronics.com/assets/pdf/SE-1X/SE-1X_UserManual.pdf
- https://gearspace.com/board/rap-hip-hop-engineering-and-production/847217-heres-og-version-snoops-quot-aint-no-fun-quot-before-dr-dre-got-4.html
- https://www.syntorial.com/preset-recipe/dr-dre-nuthin-but-a-g-thang-lead/
- https://www.copyright.gov/music-modernization/educational-materials/Sampling-Interpolations-Beat-Stores-and-More-An-Introduction-for-Musicians-Using-Preexisting.pdf
- https://store.rockstargames.com/game/gta-plus
- https://developers.google.com/fonts/faq
