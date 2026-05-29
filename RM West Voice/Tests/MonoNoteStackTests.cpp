#include "Engine/MonoNoteStack.h"

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
} // namespace

int main()
{
    testLastNotePriority();
    testLowNotePriority();
    testRepeatedNoteRefreshesLastPriority();
    testPrioritySwitchRecalculatesActiveNote();
    testInvalidNotesAreIgnored();

    if (failures != 0)
        return 1;

    std::cout << "MonoNoteStack tests passed\n";
    return 0;
}
