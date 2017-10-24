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
	private:
		Semantico();
		static Semantico* instance;

		Parser& p;
		enum CurrentSection {
			NONE,
			TEXT,
			DATA
		} currentSection;
		int macroStart;
		std::map< std::string, int > EQUs;
		std::map< std::string, std::tuple< unsigned int, unsigned int > > Macros;
};

#endif // SEMANTICO_HPP
