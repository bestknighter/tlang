#include "Parser.hpp"

Parser* Parser::instance = nullptr;

Parser::Parser() {}

Parser& Parser::GetInstance() {
	if( nullptr == instance ) {
		instance = new Parser();
	}
	return *instance;
}
