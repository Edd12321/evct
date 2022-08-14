#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

/***** MACRO BEGIN *****/
#define $IS_QUOTE()      buffer[i][j] == '"'
#define $IS_APOSTROPHE() j < len-4 && buffer[i][j] == '\''
#define $IS_ESCAPE()     j < len-2 && buffer[i][j] == '\\'

#define $SYN_NULL        0
#define $SYN_SQUARE      1
#define $SYN_PARENTHESIS 2
#define $SYN_BRACKET     3
#define $SYN_MATH        4
#define $SYN_NUMBER      5
#define $SYN_STRING      6
#define $SYN_ESCAPE      7

#define ll long long
#define byte char
/***** MACRO END *****/
extern std::string filename;
extern std::vector<std::string> buffer;
extern std::vector<std::string> syntax;

extern ll rows;
extern ll columns;
extern ll maxl;
extern ll lines;

std::unordered_map<byte, std::string>
highlighter_hashtable = {
	{ $SYN_NULL       ,         ""  }, //nothing
	{ $SYN_SQUARE     , "\e[0;31m"  },
	{ $SYN_PARENTHESIS, "\e[0;32m"  },
	{ $SYN_BRACKET    , "\e[0;33m"  },
	{ $SYN_MATH       , "\e[0;34m"  },
	{ $SYN_NUMBER     , "\e[0;35m"  },
	{ $SYN_STRING     , "\e[0;36m"  },
	{ $SYN_ESCAPE     , "\e[0;37m"  }
};

/** Stores a 2D matrix of syntax highlighting for all chars in the buffer.
 *
 * @param {ll}first,{ll}last
 * @return void
 */

extern void
syntax_highlight(ll first, ll last)
{
	//using std::tuple;
	  using std::string;
	  using std::vector;
	/* Don't syntax-highlight whilst in a string/comment... */
	bool in_string  = false;
	bool in_char    = false;

	for (ll i = first; i <= last; ++i) {
		/* Copy over some memory to fit in syntax[]. */
		syntax[i] = buffer[i];
		ll len = buffer[i].length();

		for (ll j = 0; j < len; ++j) {
			 /***********************/
			 /** CHECK FOR STRINGS **/
			 /***********************/
			if ($IS_QUOTE()) {
				//apostrope/quote init
				syntax[i][j] = $SYN_STRING;
				in_string = !in_string;
			} else if ($IS_APOSTROPHE()) {
				in_char = !in_char;
				if (in_char) {
					int k = 3;
					while (k--)
						syntax[i][j+k] = $SYN_STRING;
					j += 2;
					continue;
				}
			} else if (in_string) {
				//"undo" quote reversal
				if ($IS_ESCAPE()) {
					//escape sequence
					syntax[i][j] = syntax[i][++j] = $SYN_ESCAPE;
					continue;
				} else {
					//apostrophe/quote contents
					syntax[i][j] = $SYN_STRING;
				}
			} else {
				/***********************/
				/** CHECK FOR NUMBERS **/
				/***********************/
				if (buffer[i][j] >= '0' && buffer[i][j] <= '9')
					syntax[i][j] = $SYN_NUMBER;

				/* check tokens */
				switch(buffer[i][j]) {
				case '(': case ')': //FALLTHROUGH
					syntax[i][j] = $SYN_PARENTHESIS;
					break;
				case '{': case '}': //FALLTHROUGH
					syntax[i][j] = $SYN_BRACKET;
					break;
				case '[': case ']': //FALLTHROUGH
					syntax[i][j] = $SYN_SQUARE;
					break;
				case '+': case '-': case '*': case '/': case '%': //FALLTHROUGH
				case '&': case '|': case '!': case '^': case '~':
				case '<': case '=': case '>':
					syntax[i][j] = $SYN_MATH;
					break;
				default:
					syntax[i][j] = $SYN_NULL;
					break;
				}
			}
		}
	}
}
