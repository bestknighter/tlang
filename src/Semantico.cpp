#include "Semantico.hpp"

#include "Config.hpp"
#include "Directives.hpp"
#include "Instruction.hpp"
#include "Error.hpp"

#include <stdexcept>

Semantico* Semantico::instance = nullptr;

Semantico::Semantico()
			: p( Parser::GetInstance() )
			, currentSection( CurrentSection::NONE )
			, macroStart( -1 ) {}

Semantico& Semantico::GetInstance() {
	if( nullptr == instance ) {
		instance = new Semantico();
	}
	return *instance;
}

bool Semantico::PassagemZero( std::vector< Expression >& preproCode) {
	std::vector< Expression > preprocessedCode;

	std::string macroLabel;
	Expression e;
	bool eof;
	bool validCode = true;
	do {
		eof = p.GetNextExpression(e);
		if( Directives::Validate( e ) ) {
			// Executa diretivas que se executam na passagem zero
			switch( Directives::GetCode( e.GetOperation() ) ) {
				case 1: { // SECTION
					if( e.GetOperands()[0] == "TEXT" ) {
						currentSection = CurrentSection::TEXT;
					} else if( e.GetOperands()[0] == "DATA" ) {
						currentSection = CurrentSection::DATA;
					}
					preprocessedCode.push_back( e );
					break;
				}
				case 4: { // EQU
					EQUs[e.GetLabel()] = std::stoi( e.GetOperands()[0] ) + e.GetOffsets()[0];
					break;
				}
				case 5: { // IF
					try {
						if( !EQUs.at( e.GetOperands()[0] ) ) {
							eof = p.GetNextExpression( e );
							continue;
						}
					} catch( std::out_of_range &err ) {
						Error::Semantico( "Label nao definido.", e, e.GetOperation().size()+1, std::string(e).size() );
						validCode = false;
					}
					break;
				}
				case 6: { // MACRO
					if( 2 <= Config::numSteps ) {
						if( CurrentSection::TEXT != currentSection ) {
							Error::Semantico( "Definicao de macro permitida somente na secao texto.", e, 1, std::string(e).size() );
							validCode = false;
						} else if( -1 == macroStart ) {
							macroStart = preprocessedCode.size();
							macroLabel = e.GetLabel();
						} else {
							Error::Semantico( "Proibida definicao de macro dentro de macro.", e, 1, std::string(e).size() );
							validCode = false;
						}
					} else {
						preprocessedCode.push_back(e);
					}
					break;
				}
				case 7: { // END
					if( 2 <= Config::numSteps ) {
						if( CurrentSection::TEXT != currentSection ) {
							Error::Semantico( "Definicao de macro permitida somente na secao texto.", e, 1, std::string(e).size() );
							validCode = false;
						} else if( -1 == macroStart ) {
							Error::Semantico( "Macro nao estava sendo definida ao se encontrar END.", e, 1, std::string(e).size() );
							validCode = false;
						} else {
							Macros[macroLabel] = std::make_tuple( macroStart, preprocessedCode.size() );
							macroStart = -1;
						}
					} else {
						preprocessedCode.push_back(e);
					}
					break;
				}
				default: {
					preprocessedCode.push_back(e);
				}
			}
		} else if( Instruction::Validate( e ) ) {
			// Falta substituir labels de EQU
			preprocessedCode.push_back( e );
		} else {
			try { // Expansao de macros
				auto bounds = Macros.at( e.GetOperation() );
				for( unsigned int i = std::get<0>(bounds); i < std::get<1>(bounds); i++ ) {
					preprocessedCode.push_back( preprocessedCode[i] );
				}
			} catch( std::out_of_range &err ) {
				int column = e.GetLabel().size();
				column += 2*(column>0) + 1;
				Error::Semantico( "Instrucao, diretiva ou macro nao reconhecida.", e, column, column+e.GetOperation().size()-1 );
				validCode = false;
			}
		}
	} while( !eof );

	preproCode = preprocessedCode;
	return validCode;
}
