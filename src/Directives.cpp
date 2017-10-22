#include "Directives.hpp"

Directives::Directives( std::string mnemonic, int argc )
				: mnemonic( mnemonic )
				, argc( argc )
				, OPs( argc ) {}

Directives Directives::SECTION() {
	return Directives( "SECTION", 0 );
}

Directives Directives::SPACE() {
	return Directives( "SPACE", 1 );
}

Directives Directives::CONST() {
	return Directives( "CONST", 1 );
}

Directives Directives::EQU() {
	return Directives( "EQU", 1 );
}

Directives Directives::IF() {
	return Directives( "IF", 1 );
}

Directives Directives::MACRO() {
	return Directives( "MACRO", 0 );
}

Directives Directives::END() {
	return Directives( "END", 0 );
}
