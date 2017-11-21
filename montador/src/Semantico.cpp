#include "Semantico.hpp"

#include "Config.hpp"
#include "Directives.hpp"
#include "Error.hpp"
#include "Instruction.hpp"

#include <cstdlib>
#include <iostream>
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
	std::array< std::vector< Expression >, 2 > preprocessedCode; // 0 = TEXT, 1 = DATA
	// Para facilitar colocar a seção DATA no final do código objeto
	
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
					preprocessedCode[currentSection].push_back( e );
					break;
				}
				case 2: { // SPACE mas so verifica se a label ja existe e se esta na secao certa
					if( LabelExists( e.GetLabel() ) ) {
						Error::Semantico( "Esta label ja foi previamente definida.", e, 1, e.GetLabel().size() );
						validCode = false;
					}
					// Anotando a existência dessa label
					if( !LabelExists( e.GetLabel(), dataLabels ) ) {
						dataLabels[e.GetLabel()] = false;
					}
					if( CurrentSection::DATA != currentSection ) {
						Error::Semantico( "Essa diretiva somente e valida na secao data.", e, 1, std::string( e ).size() );
						validCode = false;
					}
					if( CurrentSection::NONE != currentSection ) { // Não lança erro pois isso é feito em outro ponto já
						preprocessedCode[currentSection].push_back( e );
					}
					break;
				}
				case 3: { // CONST mas so verifica se a label ja existe e se esta na secao certa
					if( LabelExists( e.GetLabel() ) ) {
						Error::Semantico( "Esta label ja foi previamente definida.", e, 1, e.GetLabel().size() );
						validCode = false;
					}
					// Anotando a existência dessa label
					if( !LabelExists( e.GetLabel(), dataLabels ) ) {
						dataLabels[e.GetLabel()] = true;
					}
					if( CurrentSection::DATA != currentSection ) {
						Error::Semantico( "Essa diretiva somente e valida na secao data.", e, 1, std::string( e ).size() );
						validCode = false;
					}
					if( CurrentSection::NONE != currentSection ){ // Não lança erro pois isso é feito em outro ponto já
						preprocessedCode[currentSection].push_back( e );
					}
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
						} else if( -1 == macroStart ) { // Armazena quando a macro começa
							macroStart = preprocessedCode.size();
							macroLabel = e.GetLabel();
						} else {
							Error::Semantico( "Proibida definicao de macro dentro de macro.", e, 1, std::string(e).size() );
							validCode = false;
						}
					}
					if( CurrentSection::TEXT != currentSection ) {
						Error::Semantico( "Essa diretiva somente e valida na secao texto.", e, 1, std::string( e ).size() );
						validCode = false;
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
						// Salva a macro e prepara para poder receber outras
						Macros[macroLabel] = std::make_tuple( macroStart, preprocessedCode.size() );
						macroStart = -1;
					}
					if( CurrentSection::TEXT != currentSection ) {
						Error::Semantico( "Essa diretiva somente e valida na secao texto.", e, 1, std::string( e ).size() );
						validCode = false;
					}
					break;
				}
				default: { // Diretiva que só se executa na passagem única
					preprocessedCode[currentSection].push_back(e);
				}
			}
		} else if( Instruction::Validate( e ) ) {
			if( CurrentSection::TEXT != currentSection ) {
				Error::Semantico( "Essa instrucao somente e valida na secao texto.", e, 1, std::string( e ).size() );
				validCode = false;
			}
			// Substituindo EQUs
			if( LabelExists( e.GetLabel() ) ) {
				Error::Semantico( "Esta label ja foi previamente definida.", e, 1, e.GetLabel().size() );
				validCode = false;
			}
			// Anotando a existência dessa label
			if( 0 < e.GetLabel().size() && !LabelExists( e.GetLabel(), textLabels ) ) {
				textLabels[e.GetLabel()] = false;
			}
			// Substituição em si
			auto value = EQUs.find( e.GetOperands()[0] );
			if( value != EQUs.end() ) {
				e.ReplaceOperand( 0, std::to_string( value->second ) );
			}
			value = EQUs.find( e.GetOperands()[1] );
			if( value != EQUs.end() ) {
				e.ReplaceOperand( 1, std::to_string( value->second ) );
			}
			if( CurrentSection::NONE != currentSection ){
				preprocessedCode[currentSection].push_back( e );
			}
		} else {
			if( CurrentSection::TEXT != currentSection ) {
				Error::Semantico( "Essa instrucao somente e valida na secao texto.", e, 1, std::string( e ).size() );
				validCode = false;
			}
			if( 2 <= Config::numSteps ) {
				try { // Expansao de macros
					auto bounds = Macros.at( e.GetOperation() );
					for( unsigned int i = std::get<0>(bounds); i < std::get<1>(bounds); i++ ) {
						preprocessedCode[currentSection].push_back( preprocessedCode[currentSection][i] );
					}
				} catch( std::out_of_range &err ) {
					int column = e.GetLabel().size();
					column += 2*(column>0) + 1;
					Error::Semantico( "Instrucao, diretiva ou macro nao reconhecida.", e, column, column+e.GetOperation().size()-1 );
					validCode = false;
				}
			} else {
				if( CurrentSection::NONE != currentSection ){
					preprocessedCode[currentSection].push_back( e );
				}
			}
		}
	} while( !eof );

	if( 1 >= preprocessedCode[0].size() ) {
		Error::PrintError( "Semantico", "Secao TEXT ausente ou vazia." );
	}
	preproCode = preprocessedCode[0]; // Salva seção TEXT primeiro
	for( unsigned int i = 0; i < preprocessedCode[1].size(); i++ ) { // E seção DATA segundo
		preproCode.push_back( preprocessedCode[1][i] );
	}

	return validCode;
}

std::string Semantico::PassagemUnica( std::vector< Expression >& code ) {
	std::vector< int > finalCode;
	currentSection = CurrentSection::NONE;
	bool validCode = true;
	for( unsigned int i = 0; i < code.size(); i++ ) {
		switch( Directives::GetCode( code[i].GetOperation() ) ) {
			case 1: { // SECTION
				if( code[i].GetOperands()[0] == "TEXT" ) {
					currentSection = CurrentSection::TEXT;
				} else if( code[i].GetOperands()[0] == "DATA" ) {
					currentSection = CurrentSection::DATA;
				}
				continue;
			}
			case 2: { // SPACE
				if( !LabelExists( code[i].GetLabel() ) ) { // Label nao existia
					Symbols[code[i].GetLabel()] = {finalCode.size(), true, false, -1};
				} else { // Label ja existe
					auto value = Symbols[code[i].GetLabel()];
					if( !std::get<1>(value) ) { // Label so foi referenciado
						int next = std::get<3>(value);
						while( next != -1 ) {
							int aux = finalCode[next];
							finalCode[next] = finalCode.size();
							next = aux;
						}
						Symbols[code[i].GetLabel()] = {finalCode.size(), true, false, -1};
					} else { // Label ja foi declarado
						Error::Semantico( "Label ja foi previamente declarado.", code[i], 1, code[i].GetLabel().size() );
						validCode = false;
					}
				}
				// Insere os 0
				unsigned int amount = 0 == code[i].GetOperands()[0].size() ? 1 : std::stoi( code[i].GetOperands()[0] ) + code[i].GetOffsets()[0] ;
				for(unsigned int i = 0; i < amount ; i++ ) {
					finalCode.push_back( 0 );
				}
				continue;
			}
			case 3: { // CONST
				if( !LabelExists( code[i].GetLabel() ) ) { // Label nao existia
					Symbols[code[i].GetLabel()] = {finalCode.size(), true, true, -1};
				} else { // Label ja existe
					auto value = Symbols[code[i].GetLabel()];
					if( !std::get<1>(value) ) { // Label so foi referenciado
						int next = std::get<3>(value);
						while( next != -1 ) {
							int aux = finalCode[next];
							finalCode[next] = finalCode.size();
							next = aux;
						}
						Symbols[code[i].GetLabel()] = {finalCode.size(), true, true, -1};
					} else { // Label ja foi declarado
						Error::Semantico( "Label ja foi previamente declarado.", code[i], 1, code[i].GetLabel().size() );
						validCode = false;
					}
				}
				// Insere o valor
				finalCode.push_back( std::stoi( code[i].GetOperands()[0] ) + code[i].GetOffsets()[0] );
				continue;
			}
		}

		if( 0 < code[i].GetLabel().size() ) { // Tem label
			if( !LabelExists( code[i].GetLabel(), Symbols ) ) { // Label nao foi declarado e definido
				Symbols[code[i].GetLabel()] = {finalCode.size(), true, true, -1};
			} else { // Label foi declarado
				auto value = Symbols[code[i].GetLabel()];
				if( !std::get<1>(value) ) { // Label so foi referenciado
					int next = std::get<3>(value);
					while( next != -1 ) {
						int aux = finalCode[next];
						finalCode[next] = finalCode.size();
						next = aux;
					}
					Symbols[code[i].GetLabel()] = {finalCode.size(), true, true, -1};
				} else { // Label ja foi definido
					Error::Semantico( "Label ja foi previamente definido.", code[i], 1, code[i].GetLabel().size() );
					validCode = false;
				}
			}
		}

		int opCode = Instruction::GetOpcode( code[i].GetOperation() );
		finalCode.push_back( opCode );
		switch( opCode ) {
			case 1: // ADD
			case 2: // SUB
			case 3: // MULT
			case 4: // DIV
			case 10: // LOAD
			case 13: { // OUTPUT
				if( !LabelExists( code[i].GetOperands()[0], dataLabels ) ) {
					Error::Semantico( "Usado label de texto quando se esperava de data.", code[i], 1, std::string( code[i] ).size() );
					validCode = false;
				}
				break;
			}
			case 5: // JMP
			case 6: // JMPN
			case 7: // JMPP
			case 8: { // JMPZ
				if( !LabelExists( code[i].GetOperands()[0], textLabels ) ) {
					Error::Semantico( "Usado label de data quando se esperava de texto.", code[i], 1, std::string( code[i] ).size() );
					validCode = false;
				}
				break;
			}
			case 9: // COPY
			case 11: // STORE
			case 12: { // INPUT
				bool isData = LabelExists( code[i].GetOperands()[0], dataLabels );
				bool isConst = dataLabels[code[i].GetOperands()[0]];
				if( !isData || isConst ) {
					// So pode usar DATA e o primeiro nao pode ser CONST
					Error::Semantico( "Esperava label de data nao constante (para o primeiro argumento) e nao foi encontrado.", code[i], 1, std::string( code[i] ).size() );
					validCode = false;
				}
				break;
			}
		}

		if( 0 < code[i].GetOperands()[0].size() ) { // Tem operando
			if ( !LabelExists( code[i].GetOperands()[0], Symbols ) ) { // Nao foi declarado
				Symbols[code[i].GetOperands()[0]] = {0, false, false, finalCode.size()};
				code[i].ReplaceOperand(0, "-1" );
			} else { // Label foi declarado
				auto value = Symbols[code[i].GetOperands()[0]];
				if( !std::get<1>(value) ) { // Label so foi referenciado
					int next = std::get<3>(value);
					Symbols[code[i].GetOperands()[0]] = {0, false, false, finalCode.size()};
					code[i].ReplaceOperand(0, std::to_string( next ) );
				} else { // Label ja foi definido
					code[i].ReplaceOperand(0, std::to_string( std::get<0>( value ) ) );
				}
			}
			// Adiciona os offsets para serem computados ao final
			offsets.push_back( {finalCode.size(), code[i].GetOffsets()[0]} );

			int bin = std::stoi( code[i].GetOperands()[0] );
			if( 4 == opCode && 0 == bin ) {
				Error::Semantico( "Divisao por zero.", code[i], 1, std::string( code[i] ).size() );
				validCode = false;
			}
			finalCode.push_back( bin );
		}

		if( 0 < code[i].GetOperands()[1].size() ) { // Tem operando
			if ( !LabelExists( code[i].GetOperands()[1], Symbols ) ) { // Nao foi declarado
				Symbols[code[i].GetOperands()[1]] = {0, false, false, finalCode.size()};
				code[i].ReplaceOperand(1, "-1" );
			} else { // Label foi declarado
				auto value = Symbols[code[i].GetOperands()[1]];
				if( !std::get<1>(value) ) { // Label so foi referenciado
					int next = std::get<3>(value);
					Symbols[code[i].GetOperands()[1]] = {0, false, false, finalCode.size()};
					code[i].ReplaceOperand(1, std::to_string( next ) );
				} else { // Label ja foi definido
					code[i].ReplaceOperand(1, std::to_string( std::get<0>( value ) ) );
				}
			}
			// Adiciona os offsets para serem computados ao final
			offsets.push_back( {finalCode.size(), code[i].GetOffsets()[1]} );
			finalCode.push_back( std::stoi( code[i].GetOperands()[1] ) );
		}

	}

	for( auto it = Symbols.begin(); it != Symbols.end(); it++ ) {
		if( !std::get<1>( it->second ) ) {
			Error::PrintError( "Semantico", "Label " + it->first + " usada mas nunca definida.");
			validCode = false;
		}
	}
	
	if( validCode ) {
		// Se o código é válido, aplique os offsets
		for( unsigned int i = 0; i < offsets.size(); i++ ) {
			finalCode[std::get<0>(offsets[i])] += std::get<1>(offsets[i]);
		}
	
		// E gere a string
		std::string binary = std::to_string( finalCode[0] );
		for(unsigned int i = 1; i < finalCode.size(); i++ ) {
			binary += " ";
			binary += std::to_string( finalCode[i] );
		}
	
		return binary;
	} else {
		std::cout << "Codigo invalido. Codigo objeto nao gerado.\n";
		std::exit( EXIT_FAILURE );
	}

}

bool Semantico::LabelExists( std::string label, auto map ) {
	auto value = map.find( label );
	return value != map.end();
}

bool Semantico::LabelExists( std::string label ) {
	bool exists = false;

	exists |= LabelExists( label, Symbols );
	exists |= LabelExists( label, EQUs );
	exists |= LabelExists( label, Macros );

	return exists;
}
