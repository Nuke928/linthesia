#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "keyboard.h"

namespace linthesia {

const static unsigned NUMBER_OCTAVES = 5;
const static unsigned NBR_WHITE_KEYS = 35;
const static unsigned NBR_BLACK_KEYS = 25;
const static unsigned NBR_WHITE_KEYS_BY_OCTAVE = 7;
const static unsigned NBR_BLACK_KEYS_BY_OCTAVE = 5;

const static unsigned NOTES_PER_OCTAVE = 12;

const static unsigned KEYBOARD_OFFSET = 36;

enum Keys {
    C = 0,
    C_SHARP,
    D,
    D_SHARP,
    E,
    F,
    F_SHARP,
    G,
    G_SHARP,
    A,
    A_SHARP,
    B
};

/**
 *
 */
Keyboard::Keyboard():
    whiteKeys(NBR_WHITE_KEYS),
    blackKeys(NBR_BLACK_KEYS)
{
    const unsigned WHITE_KEY_WIDTH = 20;
    const unsigned BLACK_KEY_WIDTH = 12;
    unsigned xWhite = 0;

    for (auto &oneWhiteKey : whiteKeys) {
        oneWhiteKey.setPosition(xWhite, 0);
        xWhite += WHITE_KEY_WIDTH;
    };


    // we draw octave by octave
    float xBlack = WHITE_KEY_WIDTH - (BLACK_KEY_WIDTH / 2.0f);
    for (
        unsigned currentOctave = 0;
        currentOctave < NUMBER_OCTAVES;
        currentOctave++
    ) {
        const unsigned offset = currentOctave * NBR_BLACK_KEYS_BY_OCTAVE;

        //Note: I could have used a loop with a if
        //but I think the unrolled version looks more "visual" to what it
        //actually do (hence the two statements on one line)
        blackKeys[0 + offset].setPosition(xBlack, 0); xBlack += WHITE_KEY_WIDTH;
        blackKeys[1 + offset].setPosition(xBlack, 0); xBlack += WHITE_KEY_WIDTH;
        xBlack += WHITE_KEY_WIDTH;
        blackKeys[2 + offset].setPosition(xBlack, 0); xBlack += WHITE_KEY_WIDTH;
        blackKeys[3 + offset].setPosition(xBlack, 0); xBlack += WHITE_KEY_WIDTH;
        blackKeys[4 + offset].setPosition(xBlack, 0); xBlack += WHITE_KEY_WIDTH;
        xBlack += WHITE_KEY_WIDTH;
    }
}

/**
 *
 */
void Keyboard::keyPressed(
    unsigned noteNumber,
    const sf::Color &color
) {
    if (isOutOfKeyboard(noteNumber)) {
        return;
    }

    //TODO: hack
    //this line works only because the offset of note is exactly X octave
    noteNumber -= KEYBOARD_OFFSET;

    unsigned octave = noteNumber / NOTES_PER_OCTAVE;
    unsigned noteBase = noteNumber % NOTES_PER_OCTAVE;

    unsigned baseIndex = noteToIndex(noteBase);

    if (isBlackKey(noteBase)) {
        unsigned index = baseIndex + NBR_BLACK_KEYS_BY_OCTAVE*octave;
        blackKeys[index].pressed(color);
    } else {
        unsigned index = baseIndex + NBR_WHITE_KEYS_BY_OCTAVE*octave;
        whiteKeys[index].pressed(color);
    }
}

/**
 *
 */
void Keyboard::keyReleased(
    unsigned noteNumber
) {
    if (isOutOfKeyboard(noteNumber)) {
        return;
    }
    //TODO: hack
    //this line works only because the offset of note is exactly X octave
    noteNumber -= KEYBOARD_OFFSET;

    //TODO should be possible to refactor with keyPressed

    unsigned octave = noteNumber / NOTES_PER_OCTAVE;
    unsigned noteBase = noteNumber % NOTES_PER_OCTAVE;


    unsigned baseIndex = noteToIndex(noteBase);
    if (isBlackKey(noteBase)) {
        unsigned index = baseIndex + NBR_BLACK_KEYS_BY_OCTAVE*octave;
        blackKeys[index].released();
    } else {
        unsigned index = baseIndex + NBR_WHITE_KEYS_BY_OCTAVE*octave;
        whiteKeys[index].released();
    }

}

/**
 *
 */
unsigned Keyboard::noteToIndex(unsigned baseNoteNumber) {

    switch(baseNoteNumber) {

        // white keys
        case Keys::C : return 0;
        case Keys::D : return 1;
        case Keys::E : return 2;
        case Keys::F : return 3;
        case Keys::G : return 4;
        case Keys::A : return 5;
        case Keys::B : return 6;

        //black keys

        case Keys::C_SHARP : return 0;
        case Keys::D_SHARP : return 1;
        case Keys::F_SHARP : return 2;
        case Keys::G_SHARP : return 3;
        case Keys::A_SHARP : return 4;
    }

    throw new std::exception();
}

/**
 *
 */
bool Keyboard::isBlackKey(unsigned baseNoteNumber) {

    return
        baseNoteNumber == Keys::C_SHARP ||
        baseNoteNumber == Keys::D_SHARP ||
        baseNoteNumber == Keys::F_SHARP ||
        baseNoteNumber == Keys::G_SHARP ||
        baseNoteNumber == Keys::A_SHARP;
}


/**
 *
 */
void Keyboard::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    for (const auto& oneWhiteKey: whiteKeys) {
        target.draw(oneWhiteKey, states);
    }
    for (const auto& oneBlackKey: blackKeys) {
        target.draw(oneBlackKey, states);
    }
}

/**
 *
 */
bool Keyboard::isOutOfKeyboard(unsigned noteNumber) {

    return
        // too low or..
        (noteNumber < KEYBOARD_OFFSET) ||
        // to high
        (noteNumber > NUMBER_OCTAVES * NOTES_PER_OCTAVE + KEYBOARD_OFFSET);
}


} // end of namespace