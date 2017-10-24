#include "Token.hpp"

const std::regex Token::Label = std::regex( "^[A-Z_]+[A-Z0-9_]*:?$", std::regex_constants::ECMAScript | std::regex_constants::optimize );
const std::regex Token::Number = std::regex( "^[+-]?(?:0X)?[0-9]+$", std::regex_constants::ECMAScript | std::regex_constants::optimize );
const std::regex Token::Arithm = std::regex( "^[+-]$", std::regex_constants::ECMAScript | std::regex_constants::optimize );

Token::Token()
		: tokenText( "" )
		, line( -1 )
		, column( -1 )
		, validAs( 0 ) {}

Token::Token( std::string text, int line, int column )
					: tokenText( text )
					, line( line )
					, column( column )
					, validAs( 0 ) {
	if( std::regex_match( tokenText, Label ) ) validAs |= Validity::LABEL;
	if( std::regex_match( tokenText, Number ) ) validAs |= Validity::NUMBER;
	if( std::regex_match( tokenText, Arithm ) ) validAs |= Validity::ARITHM;
	if( ":" == tokenText || "," == tokenText ) validAs |= Validity::SPEC_C;
}

int Token::GetLine() {
	return line;
}

int Token::GetColumn() {
	return column;
}

int Token::GetValidity() {
	return validAs;
}

std::string Token::GetText() {
	return tokenText;
}
