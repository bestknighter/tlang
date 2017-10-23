#include "Instruction.hpp"

Instruction::Instruction( int opcode, std::string mnemonic, int argc )
				: opcode( opcode )
				, mnemonic( mnemonic )
				, argc( argc )
				, OPs( argc ) {}

Instruction Instruction::ADD() {
	return Instruction( 1, "ADD", 1 );
}

Instruction Instruction::SUB() {
	return Instruction( 2, "SUB", 1 );
}

Instruction Instruction::MULT() {
	return Instruction( 3, "MULT", 1 );
}

Instruction Instruction::DIV() {
	return Instruction( 4, "DIV", 1 );
}

Instruction Instruction::JMP() {
	return Instruction( 5, "JMP", 1 );
}

Instruction Instruction::JMPN() {
	return Instruction( 6, "JMPN", 1 );
}

Instruction Instruction::JMPP() {
	return Instruction( 7, "JMPP", 1 );
}

Instruction Instruction::JMPZ() {
	return Instruction( 8, "JMPZ", 1 );
}

Instruction Instruction::COPY() {
	return Instruction( 9, "COPY", 2 );
}

Instruction Instruction::LOAD() {
	return Instruction( 10, "LOAD", 1 );
}

Instruction Instruction::STORE() {
	return Instruction( 11, "STORE", 1 );
}

Instruction Instruction::INPUT() {
	return Instruction( 12, "INPUT", 1 );
}

Instruction Instruction::OUTPUT() {
	return Instruction( 13, "OUTPUT", 1 );
}

Instruction Instruction::STOP() {
	return Instruction( 14, "STOP", 0 );
}
