#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP

#include "Expression.hpp"

#include <string>
#include <map>

struct Directives {
	public:
		static bool Validate( Expression exp );
		static int GetCode( std::string key );
	private:
		const static std::map< std::string, int > mnemToCode;
};

#endif // INSTRUCTION_HPP
