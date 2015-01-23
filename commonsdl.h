/*
 * commonsdl.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once

#include <iostream>

// A simple function that prints a message, the error code returned by SDL,
// and quits the application
// A simple function that prints a message, the error code returned by SDL,
// and quits the application
inline void sdldie(const char *msg)
{
	std::cout << msg << ": " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
}

inline void checkSDLError(int line = -1)
{
#ifndef NDEBUG
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		std::cout << "SDL Error: " << error << std::endl;
		if (line != -1)
			std::cout << " + line: " << line << std::endl;
		SDL_ClearError();
	}
#endif
}



