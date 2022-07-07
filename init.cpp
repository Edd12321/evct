#pragma once

#include "global.hpp"
#if defined(_WIN32)
#include <Windows.h>

/** This function enables ANSI/VT-100 escape sequences on Windows.
 * By default, they aren't enabled.
 *
 * @param none
 * @return void
 */

inline void
init_tty(void)
{
	/* init step */
	HANDLE output;
	DWORD  mode;

	/* get ready to set new term mode */
	output = GetStdHandle(STD_OUTPUT_HANDLE);
        mode   = 0;

	/* set mode 0x0004 (enables virtual term processing) */
	GetConsoleMode(output, &mode);
        mode |= 0x0004;
        SetConsoleMode(output, mode);
}

/** Initialize the two variables "rows" and "columns"
 *
 * @param  none
 * @return void
 */

inline void
init_vars(void)
{
	/* init step */
	CONSOLE_SCREEN_BUFFER_INFO buf_info;
	HANDLE output;

	/* get info */
	output = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(output, &buf_info);

	/* finally ... */
	rows    = 1+(buf_info.srWindow.Bottom - buf_info.srWindow.Top);
	columns = 1+(buf_info.srWindow.Right  - buf_info.srWindow.Left);
}

#else
#include <iostream>
extern "C" {
 #include <sys/ioctl.h>
}

#ifndef STDOUT_FILENO
  #define STDOUT_FILENO 1
#endif

/** Initialize the two variables "rows" and "columns"
 *
 * @param  none
 * @return void
 */

inline void
init_vars(void)
{
	struct winsize term;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &term);

	rows    = term.ws_row;
	columns = term.ws_col;
}

#endif

