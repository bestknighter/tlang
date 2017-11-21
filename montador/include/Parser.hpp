#ifndef PARSER_HPP
#define PARSER_HPP

#include "Expression.hpp"
#include "Scanner.hpp"
#include "Token.hpp"

#include <vector>

/**
 * Class Singleton Parser
 * É o responsável de pegar todos os tokens que o scanner retornar que fazem parte da mesma linha
 * e transformar numa expressão e avaliar a sua validade.
 */
class Parser {
	public:
		static Parser& GetInstance();

		bool GetNextExpression( Expression& exp ); // Uma expressão é uma partícula lógica de código

		~Parser();
	private:
		Parser();
		static Parser* instance;
		Scanner& s;

		Expression CreateExpression( std::vector< Token > tokens ); // Cria a expressão em si

		Token* nextToken;
		int lineBeingProcessed;
		bool eof;
};

#endif //PARSER_HPP
