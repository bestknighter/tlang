#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "Expression.hpp"

#include <map>
#include <string>
#include <tuple>

/**
 * Struct Instruction
 * Serve para facilitar a validação e obtenção dos valores dos opcodes de cada instrução
 */
struct Instruction {
	public:
		static bool Validate( Expression exp );
		static int GetOpcode( std::string key );
		static int GetOpsize( std::string key );
	private:										//	Opcode		, tamanho
		const static std::map< std::string, std::tuple< unsigned int, unsigned int > > instMap;
		static bool ValidateNoArg( Expression exp );
		static bool ValidateSingleArg( Expression exp );
		static bool ValidateTwoArg( Expression exp );
};

#endif // INSTRUCTION_HPP
