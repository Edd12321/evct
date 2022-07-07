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

#define ever (;;)

/** plot twist **/
using namespace std;

/** global vars begin **/
string filename;
vector<string> buffer;

ll rows;
ll columns;
/** global vars end **/

static inline vector<string>
stream2buf(istream& in)
{
	string tmp;
	vector<string> vec;
	while (getline(in, tmp))
		vec.emplace_back(tmp);
	return vec;
}

static inline void
usage(void)
{
	cerr << "SYNOPSIS: evct[file]\n";
	exit(EXIT_FAILURE);
}

void
cls(void)
{
	cout << "\x1b[2J\x1b[H";
}

int
main(int argc, char *argv[]) {
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
	ifstream fin(argv[1]);
	buffer = stream2buf(fin);

	filename = argv[1];
	/** MAIN-LOOP **/

	load_conf(argv[0]);       // editor.cpp
	for ever {
		init_vars();      // init.cpp
		rows -= 2;

		display_buffer(); // editor.cpp
		statusbar();      // editor.cpp
		cout << flush;

		input();          // editor.cpp
		fix_cursor();     // editor.cpp
		cls();            // editor.cpp
	}

	return EXIT_SUCCESS;
}
