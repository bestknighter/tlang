#include "Instruction.hpp"

Instruction::Instruction( int opcode, std::string mnemonic, int argc )
				: opcode( opcode )
				, mnemonic( mnemonic )
				, argc( argc )
				, OPs( argc ) {}

Intruction Intruction::ADD() {
	return Instruction( 1, "ADD", 1 );
}

Intruction Intruction::SUB() {
	return Instruction( 2, "SUB", 1 );
}

Intruction Intruction::MULT() {
	return Instruction( 3, "MULT", 1 );
}

Intruction Intruction::DIV() {
	return Instruction( 4, "DIV", 1 );
}

Intruction Intruction::JMP() {
	return Instruction( 5, "JMP", 1 );
}

Intruction Intruction::JMPN() {
	return Instruction( 6, "JMPN", 1 );
}

Intruction Intruction::JMPP() {
	return Instruction( 7, "JMPP", 1 );
}

Intruction Intruction::JMPZ() {
	return Instruction( 8, "JMPZ", 1 );
}

Intruction Intruction::COPY() {
	return Instruction( 9, "COPY", 2 );
}

Intruction Intruction::LOAD() {
	return Instruction( 10, "LOAD", 1 );
}

Intruction Intruction::STORE() {
	return Instruction( 11, "STORE", 1 );
}

Intruction Intruction::INPUT() {
	return Instruction( 12, "INPUT", 1 );
}

Intruction Intruction::OUTPUT() {
	return Instruction( 13, "OUTPUT", 1 );
}

Intruction Intruction::STOP() {
	return Instruction( 14, "STOP", 0 );
}
