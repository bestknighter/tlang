#ifndef SEMANTICO_HPP
#define SEMANTICO_HPP

#include "Expression.hpp"
#include "Parser.hpp"

#include <map>
#include <string>
#include <tuple>
#include <vector>

/**
 * Class Singleton Semantico
 * Essa classe é o analisador semântico. É onde ocorrem as passagens zero e única.
 */
class Semantico {
	public:
		static Semantico& GetInstance();

		bool PassagemZero( std::vector< Expression >& preproCode); // Recebe lista de Expressoes e faz o pré-processamento sobre eles. Retorna se é o código é válido.
		std::string PassagemUnica( std::vector< Expression >& code ); // Recebe lista de Expressoes já pré-processadas (com seção DATA após a seção TEXT, por exemplo)
																	  // e retorna uma string com os valores em decimal já do código objeto
	private:
		Semantico();
		static Semantico* instance;
		bool LabelExists( std::string label ); // Retorna se a Label já foi adicionada a algum dos mapas não temporários.
		bool LabelExists( std::string label, auto map); // Retorna se a Label existe dentro do mapa fornecido.

		Parser& p;
		enum CurrentSection { // Registra qual seção está montando no momento.
			NONE = -1, // Nenhuma seção
			TEXT = 0, // Seção TEXT
			DATA = 1 // Seção DATA
		} currentSection;
		int macroStart; // Temp para armazenar a linha onde a macro começou a ser definida.
		std::map< std::string, int > EQUs; // Armazena todos os sinônimos declarados.
		std::map< std::string, std::tuple< unsigned int, unsigned int > > Macros; // Armazena o começo e o fim das macros declaradas.
							// const
		std::map< std::string, bool > dataLabels; // Temp pra deteccao de erros na primeira passagem
		std::map< std::string, bool > textLabels; // Temp pra deteccao de erros na primeira passagem (bool é dummy)
							//	pos, offset
		std::vector< std::tuple< int, int > > offsets; // Temp para fazer offsets corretamente
										// Endereço   , def, const, lista
		std::map< std::string, std::tuple< unsigned int, bool, bool, int > > Symbols;
};

#endif // SEMANTICO_HPP
