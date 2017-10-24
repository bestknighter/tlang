#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "Expression.hpp"

#include <string>
#include <map>

struct Instruction {
	public:
		static bool Validate( Expression exp );
		static int GetOpcode( std::string key );
	private:
		const static std::map< std::string, unsigned int > mnemToOpcode;
		static bool ValidateNoArg( Expression exp );
		static bool ValidateSingleArg( Expression exp );
		static bool ValidateTwoArg( Expression exp );
};

#endif // INSTRUCTION_HPP
