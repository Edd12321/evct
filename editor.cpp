#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "global.hpp"

/** MACROS **/
#define $INPUT_COL "\e[1;33m"
#define $LINUM     "\e[1;34m"
#define $REVERSE   "\e[7m"
#define $UNDERLINE "\e[4;37m"
#define $RESET     "\e[0m"
#define $TILDE     "\e[1;34m~"

#define $KEY_ESC       27
#if defined(_WIN32)
  #define $KEY_RETURN  13
#else
  #define $KEY_RETURN  '\n'
#endif
#define $KEY_BACKSPACE 127
#define $KEY_ALTBSPACE 8

#define $IS_SELECTED i == y_pos && j == x_pos

#define $GO_UP() {              \
	if (y_pos) {            \
	       	--y_pos,        \
	       	--y_lim;        \
       	}                       \
	if (y_lim < 0)          \
       		y_lim = 0;      \
       	if (!y_lim && y_scr)    \
       		--y_scr;        \
}

#define $GO_DOWN() {            \
	++y_pos,                \
	++y_lim;                \
       	if (y_lim > rows)       \
		y_lim = rows;   \
       	if (y_lim == rows)      \
       		++y_scr;        \
}

#define $GO_RIGHT() {           \
       	if (x_pos < x_len)      \
       		++x_pos;        \
}

#define $GO_LEFT()  {           \
       	if (x_pos)              \
       		--x_pos;        \
}

/* getch() */
#if defined(_WIN32)
  #include <conio.h>
#else
  #include <termios.h>
#endif

/** another plot twist **/
using namespace std;

/** X, Y coordinates in textbuffer **/
static ll x_pos; // X buffer coord
static ll x_len; // length of current line
static ll y_pos; // Y buffer coord
static ll y_scr; // scroll control
static ll y_lim; // limit for scroll

static int keypress;

/*****************************************************
 * There are two types of "modes" for evct:
 *
 * Primary modes (Normal, Insert, Visual)
 * Secondary modes, which can be enabled (ex. Linum)
 */

typedef enum{
	NORMAL,
	INSERT,
	VISUAL
}State;
State state = State::NORMAL;

unordered_map<string, bool>
smodes;

/*****************************************************/

/** Displays the file contents
 *
 * @param  none
 * @return void
 */

extern inline void
display_buffer(void)
{
	ll rows_copy = rows+y_scr;
	ll hi = buffer.size();

	for (ll i = y_scr; i < rows_copy; ++i) {
		/** is it less than the buffer height? **/
		if (i < hi) {
			/* Show line numbers */
			if (smodes["linum"])
				cout << $LINUM << setw(5) << i+1 << $RESET << ' ';

			/* line length */
			ll oo = buffer[i].length();
			ll oo_copy = oo;
			/* line-wrap */
			while (oo > columns && rows_copy) {
				oo -= columns;
				--rows_copy;
			}
			/** print line **/
			for (int j = 0; j < oo_copy; ++j) {
				if ($IS_SELECTED)
					cout << $REVERSE << buffer[i][j] << $RESET;
				else
					cout << buffer[i][j];
			}
		}
		/* empty lines marked by a sign */
		if (i >= hi || !buffer[i].length()) {
			if (i == y_pos) {
				cout << $REVERSE; /* cursor */
			}
			cout << $TILDE << $RESET;
		}
		cout << '\n';
	}
}

/** Displays the statusbar
 *
 * @param  none
 * @return void
 */

extern inline void
statusbar(void)
{
	cout << $REVERSE << '*' << filename << '*' << setw(columns-(filename.length()+2)) << to_string(y_pos)+", "+to_string(x_pos) << $RESET << '\n';
}

/* define func here instead */
inline void interpret(string str);

/** Gets user input
 *
 * @param  none
 * @return void
 */

#if !defined(_WIN32)
#ifndef STDIN_FILENO
  #define STDIN_FILENO 0
#endif

char
getch(void)
{
	/* init */
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);

	/* Disable canonic mode and echo mode */
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);

	/* set new mode and read to buffer */
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	char ch;
	std::cin.get(ch);

	/* restore old mode */
	term.c_lflag |= ICANON;
	term.c_lflag |= ECHO;

	tcsetattr(STDIN_FILENO, TCSADRAIN, &term);
	return ch;
}
#endif // defined

extern inline void
input(void)
{
	/** display current pmode of evct **/
	cout << $INPUT_COL;
	switch(state) {
	case NORMAL:
		cout << (char)keypress;
		break;
	case INSERT:
		cout << "-- INSERT MODE --";
		break;
	case VISUAL:
		cout << "-- VISUAL MODE --";
		break;
	}
	cout << $RESET << flush;

	/** read char and parse **/
	keypress = getch();
	switch(state) {
	case NORMAL:
		if (tolower(keypress) == 'i') {
			state = State::INSERT;
		} else if (tolower(keypress) == 'v') {
			state = State::VISUAL;
		} else if (keypress == ':') {
			string command;
			getline(cin, command, '\n');

			interpret(command);
		}
		break;
	case INSERT:
		if (keypress != $KEY_ESC) {
			/** avoid getting out of bounds **/
			if (!buffer[y_pos].length())
				buffer[y_pos] = " ";
			if (keypress == $KEY_BACKSPACE || keypress == $KEY_ALTBSPACE) {
				if (x_pos) {
					/* regular deletion */
					buffer[y_pos].erase(x_pos-1, 1);
					--x_pos;
				} else if (y_pos) {
					/* merging two lines */
					x_pos = buffer[y_pos-1].length(); /* set new x_pos */

					buffer[y_pos-1].insert(buffer[y_pos-1].length(), buffer[y_pos]);
					buffer.erase(buffer.begin()+y_pos);

					--y_pos;
				}
			} else if (keypress == $KEY_RETURN) {
				/* return key support */
				string before_enter = buffer[y_pos].substr(0, x_pos);
				string after_enter = buffer[y_pos].substr(x_pos, buffer[y_pos].length());

				buffer[y_pos] = before_enter;
				buffer.insert(buffer.begin()+y_pos+1, after_enter);

				/* goto next line */
				++y_pos;
				x_pos = 0;
			} else {
				/* trademark Ed overcomplication */
				string str = "";
				str += keypress;
				buffer[y_pos].insert(x_pos, str);

				++x_pos;
			}
		}
		break;
	case VISUAL:
		break;
	}

	/** flushes output **/
	cout << flush << $RESET;

#if defined(_WIN32)
        /** Arrow key format: \224X, where X is H, P, K, M **/
	if (keypress == 224) {
		switch(getch()) {
		case 'H':
			$GO_UP();
			break;
		case 'P':
			$GO_DOWN();
			break;
		case 'K':
			$GO_LEFT();
			break;
		case 'M':
			$GO_RIGHT();
			break;
		}
	}

	if (keypress == $KEY_ESC)
		state = State::NORMAL;
#else
        /** Arrow key format: \033[X, where X is A, B, C or D **/
        if (keypress == $KEY_ESC) {
		if (getch() == '[') {
			switch(getch()) {
			case 'A':
				$GO_UP();
				break;
			case 'B':
				$GO_DOWN();
				break;
			case 'C':
				$GO_RIGHT();
				break;
			case 'D':
				$GO_LEFT();
				break;
			}
		} else {
			state = State::NORMAL;
		}
	}
#endif // _WIN32

	/* update var */
	x_len = buffer[y_pos].length()-1;
}

/** "Parses" user input
 *
 * @param {string} str
 * @return void
 */

inline void
interpret(string str)
{
	ll hi = buffer.size();

	if (str == ":w" || str == ":wq") {
		ofstream fout(filename.c_str());
		for (ll i = 0; i < hi; ++i)
			fout << buffer[i] << '\n';
	}

	if (str == ":wq" || str == ":q!")
		exit(0);

	/** If all else fails, parse words **/
	vector<string> args;
	size_t first = str.find_first_not_of(" ");
	size_t last  = first;

	while (first != string::npos) {
		last = str.find(" ", first);
		args.push_back(str.substr(first, last-first));
		first = str.find_first_not_of(" ", last);
	}

	if (args[0] == ":set")
		smodes[args[1]] = !smodes[args[1]];
}

/** Fix cursor position
 *
 * @param  none
 * @return void
 */

inline void
fix_cursor(void)
{
	/* cursor is still in buffer and outside of line length */
	if (y_pos < rows-1 && x_pos > buffer[y_pos].length()-1)
		x_pos = buffer[y_pos].length()-1;
}


/** (2 functions) Load evct configuration file
 *
 * @param  none
 * @return void
 */

string
get_file_path(char *argv0)
{
        /* get name of exec dir */
#if defined(_WIN32)
	  char separator = '\\';
#else
	  char separator = '/';
#endif

	string new_argv0(argv0);
	string wd = new_argv0.substr(0, new_argv0.find_last_of(separator));

	return wd;
}

extern void
load_conf(char *argv0)
{
	ifstream fin((get_file_path(argv0)+"/evct.conf").c_str());
	string com;

	while (getline(fin, com))
		interpret(com);
}
