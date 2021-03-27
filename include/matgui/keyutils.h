#pragma once

#include <string>

namespace matgui {

// Functions to convert between key and description string
std::string GetKeyNameFromScancode(int scancode);
std::string GetKeyNameFromKey(int scancode);
int getKeyFromName(const std::string &name);
int getScancodeFromName(const std::string &name);
int getScancodeFromKey(int key);
int getKeyFromScancode(int scancode);

void beginTextEntry();
void endTextEntry();
bool isUtfTail(char c);

} // namespace matgui
