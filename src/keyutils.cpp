/*
 * keys.cpp
 *
 *  Created on: 9 mars 2020
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/keyutils.h"
#include <SDL2/SDL_keyboard.h>

namespace matgui {

std::string GetKeyNameFromScancode(int scancode) {
    return SDL_GetScancodeName((SDL_Scancode)scancode);
}

std::string GetKeyNameFromKey(int key) {
    return SDL_GetKeyName(key);
}

int getKeyFromName(const std::string &name) {
    return SDL_GetKeyFromName(name.c_str());
}

int getScancodeFromName(const std::string &name) {
    return SDL_GetScancodeFromName(name.c_str());
}

int getScancodeFromKey(int key) {
    return SDL_GetScancodeFromKey(key);
}

int getKeyFromScancode(int scancode) {
    return SDL_GetKeyFromScancode((SDL_Scancode)scancode);
}

bool isUtfTail(char c) {
    // 0xC0 = 0b11000000
    // 0x80 = 0b10000000
    return ((c & 0xC0) == 0x80);
}

void beginTextEntry() {
    SDL_StartTextInput();
}

void endTextEntry() {
    SDL_StopTextInput();
}

} // namespace matgui
