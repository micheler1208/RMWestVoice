/*
  ==============================================================================

    MonoNoteStack.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <cstddef>
#include <vector>

namespace RMWestVoice
{
class MonoNoteStack
{
public:
    static constexpr int noNote = -1;

    enum class Priority
    {
        lastNote,
        lowNote
    };

    struct UpdateResult
    {
        int previousActiveNote { noNote };
        int activeNote { noNote };
        bool hadActiveNote { false };
        bool hasActiveNote { false };
        bool activeNoteChanged { false };
        bool isLegatoNoteOn { false };
        bool noteWasAlreadyHeld { false };
    };

    void clear();

    UpdateResult setPriority(Priority newPriority);
    Priority getPriority() const noexcept { return priority; }

    UpdateResult noteOn(int midiNoteNumber);
    UpdateResult noteOff(int midiNoteNumber);

    bool hasActiveNote() const noexcept { return getActiveNote() != noNote; }
    int getActiveNote() const noexcept;

    bool isHeld(int midiNoteNumber) const;
    std::size_t getNumHeldNotes() const noexcept { return heldNotes.size(); }

private:
    static bool isValidMidiNote(int midiNoteNumber) noexcept;

    UpdateResult makeResult(int previousActiveNote, bool isNoteOn, bool noteWasAlreadyHeld) const noexcept;

    std::vector<int> heldNotes;
    Priority priority { Priority::lastNote };
};
} // namespace RMWestVoice
