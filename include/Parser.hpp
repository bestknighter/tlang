#ifndef PARSER_HPP
#define PARSER_HPP

#include "Scanner.hpp"
#include "Expression.hpp"
#include "Token.hpp"

#include <vector>

class Parser {
	public:
		static Parser& GetInstance();

		bool GetNextExpression( Expression& exp );

		~Parser();
	private:
		Parser();
		static Parser* instance;
		Scanner& s;

		Expression CreateExpression( std::vector< Token > tokens );

		Token* nextToken;
		int lineBeingProcessed;
		bool eof;
};

#endif //PARSER_HPP