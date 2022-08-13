#pragma once

#include <string>
#include <unordered_map>
#include <vector>

/***** MACRO BEGIN *****/
#define $SYN_NULL        0
#define $SYN_SQUARE      1
#define $SYN_PARENTHESIS 2
#define $SYN_BRACKET     3
#define $SYN_ANGLED      4
#define $SYN_NUMBER      5
#define $SYN_MATH        6
#define $SYN_LOGIC_BIT   7
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
	{ $SYN_NULL       ,         "" }, //nothing
	{ $SYN_SQUARE     , "\e[0;31m" },
	{ $SYN_PARENTHESIS, "\e[0;32m" },
	{ $SYN_BRACKET    , "\e[0;33m" },
	{ $SYN_ANGLED     , "\e[0;34m" },
	{ $SYN_NUMBER     , "\e[0;35m" },
	{ $SYN_MATH       , "\e[0;36m" },
	{ $SYN_LOGIC_BIT  , "\e[0;37m" },
};


/** Stores a 2D matrix of syntax highlighting for all chars in the buffer.
 *
 * @param {ll}first,{ll}last
 * @return void
 */

extern void
syntax_highlight(ll first, ll last)
{
	for (ll i = first; i <= last; ++i) {
		/** copy over memory to fit in syntax[] **/
		syntax[i] = buffer[i];

		int len = buffer[i].length();
		for (int j = 0; j < len; ++j) {
			/* color numbers differently */
			if (buffer[i][j] >= '0'
			 && buffer[i][j] <= '9')
			{ syntax[i][j] = $SYN_NUMBER; }

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
			case '<': case '>': //FALLTHROUGH
			case '=':
				syntax[i][j] = $SYN_ANGLED;
			case '+': case '-': //FALLTHROUGH
			case '*': case '/': case '%':
				syntax[i][j] = $SYN_MATH;
				break;
			case '&': case '|': //FALLTHROUGH
			case '!': case '^': case '~':
				syntax[i][j] = $SYN_LOGIC_BIT;
				break;
			default:
				syntax[i][j] = $SYN_NULL;
				break;
			}
		}
	}
}
