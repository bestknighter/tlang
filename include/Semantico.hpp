#ifndef SEMANTICO_HPP
#define SEMANTICO_HPP

#include "Parser.hpp"
#include "Expression.hpp"

#include <vector>

class Semantico {
	public:
		static Semantico& GetInstance();

		std::vector< Expression > PassagemZero();
	private:
		Semantico();
		static Semantico* instance;

		Parser& p;
};

#endif // SEMANTICO_HPP
