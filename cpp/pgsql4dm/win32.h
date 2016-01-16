#ifndef SRC_WIN32_H_
#define SRC_WIN32_H_

#ifdef _WIN32
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#define _WIN32_WINNT 0x501
	#include <windows.h>
	#include <tchar.h>

	#define EXPORT __declspec(dllexport)

	#include <stdlib.h>

	void intToChar(int a, char* b, int size)
	{
		_itoa_s(a, b, size, 10);
	}
#else
	#define EXPORT
	#include <string>

	void intToChar(int a, char* b, int size)
	{
		snprintf(b, size, "%d", a);
	}
#endif

#endif