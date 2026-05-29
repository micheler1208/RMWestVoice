#include "Engine/GlideState.h"
#include "Engine/FilterModulationState.h"
#include "Engine/MonoNoteStack.h"
#include "Engine/PitchModulationState.h"

#include <cmath>
#include <iostream>
#include <string>

namespace
{
int failures = 0;

void expect(bool condition, const std::string& message)
{
    if (condition)
        return;

    std::cerr << "FAIL: " << message << '\n';
    ++failures;
}

void expectActiveNote(const RMWestVoice::MonoNoteStack& stack, int expectedNote, const std::string& context)
{
    expect(stack.getActiveNote() == expectedNote, context + " active note");
    expect(stack.hasActiveNote() == (expectedNote != RMWestVoice::MonoNoteStack::noNote), context + " active state");
}

void expectNear(float actual, float expected, float tolerance, const std::string& message)
{
    expect(std::abs(actual - expected) <= tolerance, message);
}

void testLastNotePriority()
{
    RMWestVoice::MonoNoteStack stack;

    expectActiveNote(stack, RMWestVoice::MonoNoteStack::noNote, "empty stack");

    auto result = stack.noteOn(60);
    expectActiveNote(stack, 60, "first note-on");
    expect(result.activeNoteChanged, "first note-on changes active note");
    expect(! result.isLegatoNoteOn, "first note-on is not legato");

    result = stack.noteOn(64);
    expectActiveNote(stack, 64, "last-note priority second note-on");
    expect(result.previousActiveNote == 60, "last-note priority previous active note");
    expect(result.isLegatoNoteOn, "overlapping note-on is legato");

    result = stack.noteOff(64);
    expectActiveNote(stack, 60, "last-note priority fallback");
    expect(result.activeNoteChanged, "releasing active note falls back");

    result = stack.noteOff(60);
    expectActiveNote(stack, RMWestVoice::MonoNoteStack::noNote, "all notes released");
    expect(result.hadActiveNote, "final note-off had active note before release");
    expect(! result.hasActiveNote, "final note-off leaves no active note");
}

void testLowNotePriority()
{
    RMWestVoice::MonoNoteStack stack;
    stack.setPriority(RMWestVoice::MonoNoteStack::Priority::lowNote);

    auto result = stack.noteOn(64);
    expectActiveNote(stack, 64, "low-note first note-on");
    expect(! result.isLegatoNoteOn, "low-note first note-on is not legato");

    result = stack.noteOn(60);
    expectActiveNote(stack, 60, "low-note lower note takes priority");
    expect(result.isLegatoNoteOn, "low-note overlapping lower note-on is legato");

    result = stack.noteOn(67);
    expectActiveNote(stack, 60, "low-note higher note does not steal priority");
    expect(! result.activeNoteChanged, "higher note does not change low-note active note");
    expect(result.isLegatoNoteOn, "higher overlapping note-on is still legato");

    result = stack.noteOff(60);
    expectActiveNote(stack, 64, "low-note fallback after releasing lowest");
    expect(result.activeNoteChanged, "low-note release falls back to next-lowest");
}

void testRepeatedNoteRefreshesLastPriority()
{
    RMWestVoice::MonoNoteStack stack;

    stack.noteOn(60);
    stack.noteOn(64);

    auto result = stack.noteOn(60);
    expectActiveNote(stack, 60, "repeated note refresh");
    expect(result.noteWasAlreadyHeld, "repeated note reports already-held state");
    expect(stack.getNumHeldNotes() == 2, "repeated note does not duplicate held note");

    stack.noteOff(60);
    expectActiveNote(stack, 64, "repeated note fallback preserves other held note");
}

void testPrioritySwitchRecalculatesActiveNote()
{
    RMWestVoice::MonoNoteStack stack;

    stack.noteOn(72);
    stack.noteOn(60);
    stack.noteOn(67);
    expectActiveNote(stack, 67, "last-note before priority switch");

    auto result = stack.setPriority(RMWestVoice::MonoNoteStack::Priority::lowNote);
    expectActiveNote(stack, 60, "low-note after priority switch");
    expect(result.previousActiveNote == 67, "priority switch previous active note");
    expect(result.activeNoteChanged, "priority switch active note changed");

    result = stack.setPriority(RMWestVoice::MonoNoteStack::Priority::lastNote);
    expectActiveNote(stack, 67, "last-note after priority switch");
    expect(result.activeNoteChanged, "priority switch back active note changed");
}

void testInvalidNotesAreIgnored()
{
    RMWestVoice::MonoNoteStack stack;

    stack.noteOn(60);
    auto result = stack.noteOn(128);
    expectActiveNote(stack, 60, "invalid note-on ignored");
    expect(! result.activeNoteChanged, "invalid note-on does not change active note");

    result = stack.noteOff(-1);
    expectActiveNote(stack, 60, "invalid note-off ignored");
    expect(! result.activeNoteChanged, "invalid note-off does not change active note");
}

void testGlideOffJumpsImmediately()
{
    RMWestVoice::GlideState glide;
    glide.prepare(10.0);
    glide.setMode(RMWestVoice::GlideState::Mode::off);
    glide.setTimeSecondsPerOctave(1.0f);

    glide.startTransition(100.0f, false);
    glide.startTransition(200.0f, true);

    expect(! glide.isGliding(), "off mode does not glide");
    expectNear(glide.getCurrentFrequency(), 200.0f, 0.01f, "off mode jumps to target");
}

void testGlideAlwaysUsesSecondsPerOctave()
{
    RMWestVoice::GlideState glide;
    glide.prepare(10.0);
    glide.setMode(RMWestVoice::GlideState::Mode::always);
    glide.setTimeSecondsPerOctave(1.0f);

    glide.startTransition(100.0f, false);
    glide.startTransition(200.0f, false);

    expect(glide.isGliding(), "always mode glides without legato");
    expect(glide.getRemainingSamples() == 10, "one octave glide uses one second at 10 Hz sample rate");

    for (int i = 0; i < 10; ++i)
        glide.getNextFrequency();

    expect(! glide.isGliding(), "always mode glide completes");
    expectNear(glide.getCurrentFrequency(), 200.0f, 0.01f, "always mode reaches target");
}

void testGlideAutoLegatoRequiresLegato()
{
    RMWestVoice::GlideState glide;
    glide.prepare(10.0);
    glide.setMode(RMWestVoice::GlideState::Mode::autoLegato);
    glide.setTimeSecondsPerOctave(1.0f);

    glide.startTransition(100.0f, false);
    glide.startTransition(200.0f, false);
    expect(! glide.isGliding(), "auto-legato does not glide for separated notes");
    expectNear(glide.getCurrentFrequency(), 200.0f, 0.01f, "auto-legato non-legato jumps");

    glide.startTransition(400.0f, true);
    expect(glide.isGliding(), "auto-legato glides for overlapping notes");
    expect(glide.getRemainingSamples() == 10, "auto-legato one octave glide uses rate timing");
}

void testGlideRateScalesWithOctaveDistance()
{
    RMWestVoice::GlideState glide;
    glide.prepare(10.0);
    glide.setMode(RMWestVoice::GlideState::Mode::always);
    glide.setTimeSecondsPerOctave(1.0f);

    glide.startTransition(100.0f, false);
    glide.startTransition(400.0f, false);

    expect(glide.getRemainingSamples() == 20, "two-octave glide takes twice as many samples as one octave");
}

void testPitchWheelSmoothsTowardTarget()
{
    RMWestVoice::PitchModulationState pitchModulation;
    pitchModulation.prepare(1000.0);
    pitchModulation.setPitchBendRangeSemitones(12.0f);
    pitchModulation.setPitchWheel(1.0f);

    pitchModulation.getNextPitchRatio();

    expect(pitchModulation.getCurrentPitchBendSemitones() > 0.0f, "pitch wheel starts moving toward target");
    expect(pitchModulation.getCurrentPitchBendSemitones() < 12.0f, "pitch wheel is smoothed");

    float pitchRatio = 1.0f;
    for (int i = 0; i < 220; ++i)
        pitchRatio = pitchModulation.getNextPitchRatio();

    expectNear(pitchModulation.getCurrentPitchBendSemitones(), 12.0f, 0.01f, "pitch wheel reaches configured bend range");
    expectNear(pitchRatio, 2.0f, 0.01f, "one-octave bend produces two-times pitch ratio");
}

void testVibratoRequiresModWheelAndFadesIn()
{
    RMWestVoice::PitchModulationState pitchModulation;
    pitchModulation.prepare(4.0);
    pitchModulation.setVibratoDepthCents(40.0f);
    pitchModulation.setVibratoRateHz(1.0f);
    pitchModulation.setVibratoFadeSeconds(1.0f);
    pitchModulation.noteStarted(false);

    pitchModulation.getNextPitchRatio();
    expectNear(pitchModulation.getCurrentVibratoCents(), 0.0f, 0.001f, "vibrato is silent with mod wheel down");

    pitchModulation.setModWheel(1.0f);
    pitchModulation.noteStarted(false);
    pitchModulation.getNextPitchRatio();
    expectNear(pitchModulation.getCurrentVibratoCents(), 10.0f, 0.01f, "vibrato fades in from note start");
}

void testAftertouchCanDriveVibratoWhenEnabled()
{
    RMWestVoice::PitchModulationState pitchModulation;
    pitchModulation.prepare(4.0);
    pitchModulation.setVibratoDepthCents(50.0f);
    pitchModulation.setVibratoRateHz(1.0f);
    pitchModulation.setVibratoFadeSeconds(0.0f);
    pitchModulation.setAftertouch(1.0f);
    pitchModulation.noteStarted(false);

    pitchModulation.getNextPitchRatio();
    expectNear(pitchModulation.getCurrentVibratoCents(), 0.0f, 0.001f, "aftertouch does not drive vibrato until enabled");

    pitchModulation.setVibratoAftertouchAmount(1.0f);
    pitchModulation.noteStarted(false);
    pitchModulation.getNextPitchRatio();
    expectNear(pitchModulation.getCurrentVibratoCents(), 50.0f, 0.01f, "enabled aftertouch drives vibrato depth");
}

void testFilterKeyTrackingRaisesCutoffByOctave()
{
    RMWestVoice::FilterModulationState filterModulation;
    filterModulation.prepare(44100.0);
    filterModulation.setBaseCutoffHz(1000.0f);
    filterModulation.setKeyTracking(1.0f);
    filterModulation.setEnvelopeAmountOctaves(0.0f);
    filterModulation.setCurrentMidiNote(72);

    expectNear(filterModulation.getCutoffHz(0.0f), 2000.0f, 0.01f, "full key tracking doubles cutoff one octave above C4");
}

void testFilterEnvelopeAmountAddsOctaves()
{
    RMWestVoice::FilterModulationState filterModulation;
    filterModulation.prepare(44100.0);
    filterModulation.setBaseCutoffHz(1000.0f);
    filterModulation.setKeyTracking(0.0f);
    filterModulation.setEnvelopeAmountOctaves(2.0f);
    filterModulation.setCurrentMidiNote(60);

    expectNear(filterModulation.getCutoffHz(0.5f), 2000.0f, 0.01f, "half envelope level with two-octave amount doubles cutoff");
}

void testFilterCutoffIsClampedBelowNyquist()
{
    RMWestVoice::FilterModulationState filterModulation;
    filterModulation.prepare(1000.0);
    filterModulation.setBaseCutoffHz(20000.0f);
    filterModulation.setKeyTracking(1.0f);
    filterModulation.setEnvelopeAmountOctaves(4.0f);
    filterModulation.setCurrentMidiNote(127);

    expectNear(filterModulation.getCutoffHz(1.0f), 450.0f, 0.01f, "filter cutoff is clamped to a safe fraction of sample rate");
}
} // namespace

int main()
{
    testLastNotePriority();
    testLowNotePriority();
    testRepeatedNoteRefreshesLastPriority();
    testPrioritySwitchRecalculatesActiveNote();
    testInvalidNotesAreIgnored();
    testGlideOffJumpsImmediately();
    testGlideAlwaysUsesSecondsPerOctave();
    testGlideAutoLegatoRequiresLegato();
    testGlideRateScalesWithOctaveDistance();
    testPitchWheelSmoothsTowardTarget();
    testVibratoRequiresModWheelAndFadesIn();
    testAftertouchCanDriveVibratoWhenEnabled();
    testFilterKeyTrackingRaisesCutoffByOctave();
    testFilterEnvelopeAmountAddsOctaves();
    testFilterCutoffIsClampedBelowNyquist();

    if (failures != 0)
        return 1;

    std::cout << "Engine tests passed\n";
    return 0;
}
