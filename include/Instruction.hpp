#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <string>
#include <vector>

struct Instruction {
	public:
		const int opcode;
		const std::string mnemonic;
		const int argc;
		std::vector< int > OPs;
		
		static Instruction ADD();
		static Instruction SUB();
		static Instruction MULT();
		static Instruction DIV();
		static Instruction JMP();
		static Instruction JMPN();
		static Instruction JMPP();
		static Instruction JMPZ();
		static Instruction COPY();
		static Instruction LOAD();
		static Instruction STORE();
		static Instruction INPUT();
		static Instruction OUTPUT();
		static Instruction STOP();
	private:
		Instruction(int opcode, std::string mnemonic, int argc);
};

#endif // INSTRUCTION_HPP