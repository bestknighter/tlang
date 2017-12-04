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

		void Reset(); // Limpa todas as tabelas para refazer o processo

		bool PassagemZero( std::vector< Expression >& preproCode); // Recebe lista de Expressoes e faz o pré-processamento sobre eles. Retorna se é o código é válido.
		std::vector< int > PassagemUnica( std::vector< Expression >& code ); // Recebe lista de Expressoes já pré-processadas (com seção DATA após a seção TEXT, por exemplo)
																	  // e retorna um vetor com os valores do código objeto
		std::vector< std::tuple< std::string, unsigned int > > GetTabelaDefinicao(); // Retorna todos do Symbols que sao PUBLIC
		std::vector< std::tuple< std::string, unsigned int > > GetTabelaUso(); // Retorna todos do Symbols que sao EXTERN
		std::string GetMapaBits(); // Retorna o mapa de bits
		bool GetTeveBegin();
		bool GetTeveEnd();
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
		std::map< std::string, bool > dataLabels; // Temp pra deteccao de erros na primeira passagem (bool eh const)
		std::map< std::string, bool > textLabels; // Temp pra deteccao de erros na primeira passagem (bool é dummy)
							//	pos, offset
		std::vector< std::tuple< int, int > > offsets; // Temp para fazer offsets corretamente
										// Endereço   , def, const, lista, ext, public
		std::map< std::string, std::tuple< unsigned int, bool, bool, int, bool, bool > > Symbols;
		std::vector< std::tuple< std::string, unsigned int > > TabelaDefinicao;
		std::vector< std::tuple< std::string, unsigned int > > TabelaUso;
		std::string MapaBits;
		bool teveBegin;
		bool teveEnd;
};

#endif // SEMANTICO_HPP
