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
		validCode &= e.IsValid();
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
				case 2:
				case 3: { // SPACE e CONST mas so verifica se a label ja existe
					if( LabelExists( e.GetLabel() ) ) {
						Error::Semantico( "Esta label ja foi previamente definida.", e, 1, e.GetLabel().size() );
						validCode = false;
					}
					preprocessedCode.push_back( e );
					break;
				}
				case 4: { // EQU
					if( LabelExists( e.GetLabel() ) ) {
						Error::Semantico( "Esta label ja foi previamente definida.", e, 1, e.GetLabel().size() );
						validCode = false;
					} else {
						EQUs[e.GetLabel()] = std::stoi( e.GetOperands()[0] ) + e.GetOffsets()[0];
					}
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
					if( LabelExists( e.GetLabel() ) ) {
						Error::Semantico( "Esta label ja foi previamente definida.", e, 1, e.GetLabel().size() );
						validCode = false;
					} else {
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
					}
					break;
				}
				case 7: { // END
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
					break;
				}
				default: {
					preprocessedCode.push_back(e);
				}
			}
		} else if( Instruction::Validate( e ) ) {
			// Substituindo EQUs
			if( LabelExists( e.GetLabel() ) ) {
				Error::Semantico( "Esta label ja foi previamente definida.", e, 1, e.GetLabel().size() );
				validCode = false;
			}
			auto value = EQUs.find( e.GetOperands()[0] );
			if( value != EQUs.end() ) {
				e.ReplaceOperand( 0, std::to_string( value->second ) );
			}
			value = EQUs.find( e.GetOperands()[1] );
			if( value != EQUs.end() ) {
				e.ReplaceOperand( 1, std::to_string( value->second ) );
			}
			preprocessedCode.push_back( e );
		} else {
			if( 2 <= Config::numSteps ) {
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
			} else {
				preprocessedCode.push_back( e );
			}
		}
	} while( !eof );

	preproCode = preprocessedCode;
	return validCode;
}

std::string Semantico::PassagemUnica( std::vector< Expression >& code ) {
	std::vector< int > finalCode;

	for( unsigned int i = 0; i < code.size(); i++ ) {
		
	}
}

bool Semantico::LabelExists( std::string label, auto map ) {
	auto value = map.find( label );
	return value != map.end();
}

bool Semantico::LabelExists( std::string label ) {
	bool exists = false;

	exists |= LabelExists( label, DataLabels );
	exists |= LabelExists( label, CodeLabels );
	exists |= LabelExists( label, EQUs );
	exists |= LabelExists( label, Macros );

	return exists;
}
