#ifndef PARSER_HPP
#define PARSER_HPP

#include "Scanner.hpp"

class Parser {
	public:
		static Parser& GetInstance();

	private:
		Parser();
		static Parser* instance;
		Scanner& s;
};

#endif //PARSER_HPP