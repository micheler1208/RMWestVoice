/*
  ==============================================================================

    MonoNoteStack.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "MonoNoteStack.h"

#include <algorithm>

namespace RMWestVoice
{
void MonoNoteStack::clear()
{
    heldNotes.clear();
}

MonoNoteStack::UpdateResult MonoNoteStack::setPriority(Priority newPriority)
{
    const auto previousActiveNote = getActiveNote();
    priority = newPriority;

    return makeResult(previousActiveNote, false, false);
}

MonoNoteStack::UpdateResult MonoNoteStack::noteOn(int midiNoteNumber)
{
    const auto previousActiveNote = getActiveNote();

    if (! isValidMidiNote(midiNoteNumber))
        return makeResult(previousActiveNote, false, false);

    const auto noteWasAlreadyHeld = isHeld(midiNoteNumber);

    heldNotes.erase(std::remove(heldNotes.begin(), heldNotes.end(), midiNoteNumber), heldNotes.end());
    heldNotes.push_back(midiNoteNumber);

    return makeResult(previousActiveNote, true, noteWasAlreadyHeld);
}

MonoNoteStack::UpdateResult MonoNoteStack::noteOff(int midiNoteNumber)
{
    const auto previousActiveNote = getActiveNote();

    if (! isValidMidiNote(midiNoteNumber))
        return makeResult(previousActiveNote, false, false);

    heldNotes.erase(std::remove(heldNotes.begin(), heldNotes.end(), midiNoteNumber), heldNotes.end());

    return makeResult(previousActiveNote, false, false);
}

int MonoNoteStack::getActiveNote() const noexcept
{
    if (heldNotes.empty())
        return noNote;

    if (priority == Priority::lowNote)
        return *std::min_element(heldNotes.begin(), heldNotes.end());

    return heldNotes.back();
}

bool MonoNoteStack::isHeld(int midiNoteNumber) const
{
    return std::find(heldNotes.begin(), heldNotes.end(), midiNoteNumber) != heldNotes.end();
}

bool MonoNoteStack::isValidMidiNote(int midiNoteNumber) noexcept
{
    return midiNoteNumber >= 0 && midiNoteNumber <= 127;
}

MonoNoteStack::UpdateResult MonoNoteStack::makeResult(
    int previousActiveNote,
    bool isNoteOn,
    bool noteWasAlreadyHeld) const noexcept
{
    const auto activeNote = getActiveNote();

    return {
        previousActiveNote,
        activeNote,
        previousActiveNote != noNote,
        activeNote != noNote,
        previousActiveNote != activeNote,
        isNoteOn && previousActiveNote != noNote,
        noteWasAlreadyHeld
    };
}
} // namespace RMWestVoice
