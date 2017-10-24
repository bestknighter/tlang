#ifndef ERROR_H
#define ERROR_H

#include "Token.hpp"

#include <string>

class Error {
	public:
		static void Lexico( std::string errorMsg, Token t );
		static void Sintatico( std::string errorMsg, Token t );
		static void Semantico();
		static void PrintError( std::string type, std::string errorMsg
							  , unsigned int line, unsigned int column
							  , std::string lineOfCode
							  , unsigned int highlightStart
							  , unsigned int highlightEnd );
};

#endif // ERROR_H
