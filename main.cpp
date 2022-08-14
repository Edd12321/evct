/** main.cpp
 *
 * @version v1.0
 * @author Edd12321 <sanduedi309@gmail.com>
 */

#pragma GCC optimize("Ofast")
#pragma GCC target("avx,avx2,fma")

#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <string>
#include <vector>

extern "C" {
  #include <stdlib.h>
  #include <unistd.h>
}
#if defined(_WIN32)
  #include <conio.h>
  #include <Windows.h>
#else
  #include <limits.h>
#endif // defined

#include "global.hpp"
#include "init.cpp"
#include "editor.cpp"

/***** MACRO BEGIN *****/
#define ever (;;)
/***** MACRO END *****/

/** global arg **/
int    argc;
char **argv;

//char *argv[];

/** plot twist **/
using namespace std;

/** global vars begin **/
string filename;

vector<string> buffer;
vector<string> syntax;

ll rows;
ll columns;
ll lines;
/** global vars end **/

static inline vector<string>
stream2buf(istream& in)
{
	string tmp;
	vector<string> vec;
	while (getline(in, tmp)) {
		/* add to buffer */
		vec.emplace_back(tmp);
		++lines;
	}
	return vec;
}

static inline void
usage(void)
{
	cerr << "SYNOPSIS: "<<argv[0]<<"[file]\n";
	exit(EXIT_FAILURE);
}

void
cls(void)
{
	cout << "\x1b[2J\x1b[H";
}

int
main(int argc, char *argv[]) {
	/* use global arg instead */
	::argc = argc;
	::argv = argv;

	ios_base::sync_with_stdio(false);
#if defined(_WIN32)
	  /** performance on win-based oses **/
	  init_tty();
	  setvbuf(stdout, 0, _IOLBF, 4096);
#endif

	/* trigger message on wrong args */
	if (argc < 2)
		usage();

	/* if file doesn't exist, create it */
	if (access(argv[1], F_OK) != 0) {
		ofstream fout;

		fout.open(argv[1], fstream::app);
		fout << '\n';
		fout.close();
	}

	/* init textbuffer */
	ifstream      fin(argv[1]);
	buffer      = stream2buf(fin);
	ll buf_len  = buffer.size();

	/* init highlighting */
	syntax = buffer;
	syntax_highlight(0, buf_len-1);

	filename = argv[1];
	/** MAIN-LOOP **/

	load_conf(argv[0]);   // editor.cpp
	for ever {
		init_vars();      // init.cpp
		rows -= 2;


		display_buffer(); // editor.cpp
		statusbar();      // editor.cpp
		cout << flush;

		fix_cursor();    //editor.cpp
		input();          //-"-
		fix_cursor();     //-"-

		cls();            // editor.cpp
	}

	return EXIT_SUCCESS;
}
