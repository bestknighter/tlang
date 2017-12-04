#include "Directives.hpp"

#include "Error.hpp"
#include "Token.hpp"

#include <regex>
#include <stdexcept>

const std::map< std::string, int > Directives::mnemToCode = { {"SECTION", 1}, {"SPACE", 2}
															, {"CONST", 3}, {"EQU", 4}
															, {"IF", 5}, {"MACRO", 6}
															, {"ENDMACRO", 7}, {"EXTERN", 8}
															, {"PUBLIC", 9}, {"BEGIN", 10}
															, {"END", 11} };


int Directives::GetCode( std::string key ) {
	try {
		unsigned int code = mnemToCode.at( key );
		return int(code);
	} catch(std::out_of_range &e) {
		return -1;
	}
}

bool Directives::Validate( Expression exp ) {
	int opcode = GetCode( exp.GetOperation() );
	switch( opcode ) {
		case 1: { // SECTION
			if( 0 != exp.GetLabel().size() ) {
				Error::Semantico( "Essa diretiva nao pode ser referenciada por label.", exp, 1, std::string( exp ).size() );
				return false;
			}
			if( 0 == exp.GetOperands()[0].size() || 0 < exp.GetOperands()[1].size() ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva aceita (e precisa de) somente um argumento.", exp, column, std::string( exp ).size() );
				return false;
			}
			if ( 0 != exp.GetOffsets()[0] ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Argumento invalido para essa diretiva. (offsets nao sao aceitos)", exp, column, std::string( exp ).size() );
				return false;
			}
			if( "TEXT" != exp.GetOperands()[0] && "DATA" != exp.GetOperands()[0] ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva esperava TEXT ou DATA. Encontrou-se: " + exp.GetOperands()[0], exp, column, std::string( exp ).size() );
				return false;
			}
			return true;
			break;
		}
		case 2: { // SPACE
			if( 0 == exp.GetLabel().size() ) {
				Error::Semantico( "Essa diretiva precisa de ser referenciada por label.", exp, 1, std::string( exp ).size() );
				return false;
			}
			if( 0 < exp.GetOperands()[1].size() ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva nao aceita dois argumentos.", exp, column, std::string( exp ).size() );
				return false;
			}
			if ( (0 == exp.GetOperands()[0].size()) && 0 != exp.GetOffsets()[0] ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Argumento invalido para essa diretiva. (offsets nao sao aceitos)", exp, column, std::string( exp ).size() );
				return false;
			}
			if( (0 != exp.GetOperands()[0].size()) && !std::regex_match( exp.GetOperands()[0], Token::Number ) ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva espera nada ou um valor numerico de operando.", exp, column, std::string( exp ).size() );
				return false;
			}
			return true;
			break;
		}
		case 3:
		case 4: { // CONST e EQU
			if( 0 == exp.GetLabel().size() ) {
				Error::Semantico( "Essa diretiva precisa de ser referenciada por label.", exp, 1, std::string( exp ).size() );
				return false;
			}
			if( 0 == exp.GetOperands()[0].size() || 0 < exp.GetOperands()[1].size() ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva aceita (e precisa de) somente um argumento.", exp, column, std::string( exp ).size() );
				return false;
			}
			if( !std::regex_match( exp.GetOperands()[0], Token::Number ) ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva espera um valor numerico de operando.", exp, column, std::string( exp ).size() );
				return false;
			}
			return true;
			break;
		}
		case 5: { // IF
			if( 0 != exp.GetLabel().size() ) {
				Error::Semantico( "Essa diretiva nao pode ser referenciada por label.", exp, 1, std::string( exp ).size() );
				return false;
			}
			if( 0 == exp.GetOperands()[0].size() || 0 < exp.GetOperands()[1].size() ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva aceita (e precisa de) somente um argumento.", exp, column, std::string( exp ).size() );
				return false;
			}
			if( !std::regex_match( exp.GetOperands()[0], Token::Label ) ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva espera um label.", exp, column, std::string( exp ).size() );
				return false;
			}
			if ( 0 != exp.GetOffsets()[0] ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Argumento invalido para essa diretiva. (offsets nao sao aceitos)", exp, column, std::string( exp ).size() );
				return false;
			}
			return true;
			break;
		}
		case 6: { // MACRO
			if( 0 == exp.GetLabel().size() ) {
				Error::Semantico( "Essa diretiva precisa de ser referenciada por label.", exp, 1, std::string( exp ).size() );
				return false;
			}
			if( 0 < exp.GetOperands()[0].size() || 0 < exp.GetOperands()[1].size() ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva nao aceita argumentos.", exp, column, std::string( exp ).size() );
				return false;
			}
			return true;
			break;
		}
		case 7:
		case 11: { // ENDMACRO e END
			if( 0 != exp.GetLabel().size() ) {
				Error::Semantico( "Essa diretiva nao pode ser referenciada por label.", exp, 1, std::string( exp ).size() );
				return false;
			}
			if( 0 < exp.GetOperands()[0].size() || 0 < exp.GetOperands()[1].size() ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva nao aceita argumentos.", exp, column, std::string( exp ).size() );
				return false;
			}
			return true;
			break;
		}
		case 8: { // EXTERN
			if( 0 == exp.GetLabel().size() ) {
				Error::Semantico( "Essa diretiva tem que ser referenciada por label.", exp, 1, std::string( exp ).size() );
				return false;
			}
			if( 0 < exp.GetOperands()[0].size() || 0 < exp.GetOperands()[1].size() ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva nao aceita argumentos.", exp, column, std::string( exp ).size() );
				return false;
			}
			return true;
			break;
		}
		case 9: { // PUBLIC
			if( 0 != exp.GetLabel().size() ) {
				Error::Semantico( "Essa diretiva nao pode ser referenciada por label.", exp, 1, std::string( exp ).size() );
				return false;
			}
			if( 0 == exp.GetOperands()[0].size() || 0 < exp.GetOperands()[1].size() ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva aceita (e precisa de) somente um argumento.", exp, column, std::string( exp ).size() );
				return false;
			}
			return true;
			break;
		}
		case 10: { // BEGIN
			if( 0 == exp.GetLabel().size() ) {
				Error::Semantico( "Essa diretiva tem que ser referenciada por label.", exp, 1, std::string( exp ).size() );
				return false;
			}
			if( 0 < exp.GetOperands()[0].size() || 0 < exp.GetOperands()[1].size() ) {
				int column = exp.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Essa diretiva nao aceita argumentos.", exp, column, std::string( exp ).size() );
				return false;
			}
			return true;
			break;
		}
	}
	return -1 != opcode;
}
