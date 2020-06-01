#include <cctype>
#include <map>
using std::map;

#include "lex.h"

static map<Token,string> tokenPrint = {
		{ PRINT, "PRINT" },
		{ PRINTLN, "PRINTLN" },
		{ REPEAT, "REPEAT" },
		{ BEGIN, "BEGIN" },
		{ END, "END" },

		{ IDENT, "IDENT" },

		{ ICONST, "ICONST" },
		{ SCONST, "SCONST" },

		{ PLUS, "PLUS" },
		{ MINUS, "MINUS" },
		{ STAR, "STAR" },
		{ SLASH, "SLASH" },
		{ EQ, "EQ" },
		{ LPAREN, "LPAREN" },
		{ RPAREN, "RPAREN" },
            
		{ SC, "SC" },

		{ ERR, "ERR" },

		{ DONE, "DONE" },
};


ostream& operator<<(ostream& out, const Tok& tok) {
	Token tt = tok.GetToken();
	out << tokenPrint[ tt ];
	if( tt == IDENT || tt == ICONST || tt == SCONST || tt == ERR ) {
		out << "(" << tok.GetLexeme() << ")";
	}
	return out;
}
