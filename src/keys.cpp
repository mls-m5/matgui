/*
 * keys.cpp
 *
 *  Created on: 9 mars 2020
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/keys.h"
#include <SDL2/SDL_keyboard.h>

namespace MatGui {

namespace Keys {

std::string GetKeyNameFromScancode(int scancode) {
    return SDL_GetScancodeName((SDL_Scancode)scancode);
}

std::string GetKeyNameFromKey(int key) {
    return SDL_GetKeyName(key);
}

int GetKeyFromName(const std::string &name) {
    return SDL_GetKeyFromName(name.c_str());
}

int GetScancodeFromName(const std::string &name) {
    return SDL_GetScancodeFromName(name.c_str());
}

int GetScancodeFromKey(int key) {
    return SDL_GetScancodeFromKey(key);
}

int GetKeyFromScancode(int scancode) {
    return SDL_GetKeyFromScancode((SDL_Scancode)scancode);
}

} // namespace Keys

} // namespace MatGui
