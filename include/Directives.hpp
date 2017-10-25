#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP

#include "Expression.hpp"

#include <map>
#include <string>

/**
 * Struct Directives
 * Estrutura para auxílio na hora de validar e executar as diretivas. Similar a Struct Instruction mas para diretivas.
 */
struct Directives {
	public:
		static bool Validate( Expression exp );
		static int GetCode( std::string key );
	private:
		const static std::map< std::string, int > mnemToCode;
};

#endif // INSTRUCTION_HPP
