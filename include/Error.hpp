#ifndef ERROR_H
#define ERROR_H

#include "Expression.hpp"
#include "Token.hpp"

#include <string>

/**
 * Struct Error
 * Estrutura de suporte para se modularizar a formatação das mensagens de erro.
 */
struct Error {
	public:
		static void Lexico( std::string errorMsg, Token t );
		static void Sintatico( std::string errorMsg, Token t );
		static void Semantico( std::string errorMsg, Expression exp, unsigned int highlightStart, unsigned int highlightEnd );
		static void PrintError( std::string type, std::string errorMsg
							  , unsigned int line, unsigned int column
							  , std::string lineOfCode
							  , unsigned int highlightStart
							  , unsigned int highlightEnd );
		static void PrintError( std::string type, std::string errorMsg );
};

#endif // ERROR_H
