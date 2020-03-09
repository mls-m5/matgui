/*
 * Keys.h
 *
 *  Created on: 30 maj 2019
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <string>

namespace MatGui {

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

namespace Keys {

// Scancode constants
const int Unknown = 0;

const int Escape = 41;
const int Left = 80;
const int Right = 79;
const int Down = 81;
const int Up = 82;
const int Return = 40;
const int Backspace = 42;
const int Tab = 43;
const int Space = 44;

const int A = 4;
const int B = 5;
const int C = 6;
const int D = 7;
const int E = 8;
const int F = 9;
const int G = 10;
const int H = 11;
const int I = 12;
const int J = 13;
const int K = 14;
const int L = 15;
const int M = 16;
const int N = 17;
const int O = 18;
const int P = 19;
const int Q = 20;
const int R = 21;
const int S = 22;
const int T = 23;
const int U = 24;
const int V = 25;
const int W = 26;
const int X = 27;
const int Y = 28;
const int Z = 29;

const int Key1 = 30;
const int Key2 = 31;
const int Key3 = 32;
const int Key4 = 33;
const int Key5 = 34;
const int Key6 = 35;
const int Key7 = 36;
const int Key8 = 36;
const int Key9 = 38;
const int Key0 = 39;

const int F1 = 58;
const int F2 = 59;
const int F3 = 60;
const int F4 = 61;
const int F5 = 62;
const int F6 = 63;
const int F7 = 64;
const int F8 = 65;
const int F9 = 66;
const int F10 = 67;
const int F11 = 68;
const int F12 = 69;

const int PrintScreen = 70;
const int ScrollLock = 71;
const int Pause = 72;
const int Insert = 73;

const int Home = 74;
const int PageUp = 75;
const int Delete = 76;
const int End = 77;
const int PageDown = 78;

const int CtrlLeft = 224;
const int ShiftLeft = 225;
const int AltLeft = 226;

} // namespace Keys

} // namespace MatGui
