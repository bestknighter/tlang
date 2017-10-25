#ifndef SEMANTICO_HPP
#define SEMANTICO_HPP

#include "Parser.hpp"
#include "Expression.hpp"

#include <vector>
#include <map>
#include <string>
#include <tuple>

class Semantico {
	public:
		static Semantico& GetInstance();

		bool PassagemZero( std::vector< Expression >& preproCode);
		std::string PassagemUnica( std::vector< Expression >& code );
	private:
		Semantico();
		static Semantico* instance;
		bool LabelExists( std::string label );
		bool LabelExists( std::string label, auto map);

		Parser& p;
		enum CurrentSection {
			NONE = -1,
			TEXT = 0,
			DATA = 1
		} currentSection;
		int macroStart;
		std::map< std::string, int > EQUs;
		std::map< std::string, std::tuple< unsigned int, unsigned int > > Macros;
		std::map< std::string, bool > dataLabels; // Temp pra deteccao de erros na primeira passagem
		std::map< std::string, bool > textLabels; // Temp pra deteccao de erros na primeira passagem (bool é dummy)
		std::vector< std::tuple< int, int > > offsets; // Temp para fazer offsets corretamente
		std::map< std::string, std::tuple< unsigned int, bool, bool, int > > Symbols; // Endereco, definido, const, lista
};

#endif // SEMANTICO_HPP
