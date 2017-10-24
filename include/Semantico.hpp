#ifndef SEMANTICO_HPP
#define SEMANTICO_HPP

#include "Parser.hpp"
#include "Expression.hpp"

class Semantico {
	public:
		static Semantico& GetInstance();

		bool GetNextExpression(Expression &exp);
	private:
		Semantico();
		static Semantico* instance;

		Parser& p;
};

#endif // SEMANTICO_HPP
