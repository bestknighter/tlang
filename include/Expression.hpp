#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "Token.hpp"

#include <vector>
#include <array>

struct Expression {
	public:
		Expression( std::vector< Token > args );

		bool IsValid();
		std::string to_string();
	private:
		bool isValid;
		Token label;
		Token operation;
		std::array< std::vector< Token >, 2 > OPs;
};

#endif // EXPRESSION_HPP
