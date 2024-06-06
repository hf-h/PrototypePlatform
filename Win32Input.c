#include "cutils/UtTypes.h"

#include "KeyboardInput.h"

#define KEYBOARD_SIZE 256

typedef struct KeyboardKey {
    i32 lastKeyState;
} KeyboardKey;

//TODO: Make a more robust keyboard representation, there is something I'm missing with multiple klicks per frame
void InitKeyboard(KeyboardKey *kb) {
    for (usize i = 0; i < KEYBOARD_SIZE; i++) {
        kb[i].lastKeyState = KEY_STATE_DOWN;
    }
}

void KeyboardKeyPressed(KeyboardKey *kb, u32 keyCode) {
    if (KEY_STATE_DOWN == kb[keyCode].lastKeyState) {
        return;
    }

    kb[keyCode].lastKeyState = KEY_STATE_DOWN;
}

void KeyboardKeyReleased(KeyboardKey *kb, u32 keyCode) {
    if (KEY_STATE_UP == kb[keyCode].lastKeyState) {
        return;
    }

    kb[keyCode].lastKeyState = KEY_STATE_UP;
}
