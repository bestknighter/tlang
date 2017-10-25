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
			NONE,
			TEXT,
			DATA
		} currentSection;
		int macroStart;
		std::map< std::string, int > EQUs;
		std::map< std::string, std::tuple< unsigned int, unsigned int > > Macros;
		std::map< std::string, unsigned int > CodeLabels;
		std::map< std::string, unsigned int > DataLabels;
};

#endif // SEMANTICO_HPP
