#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP

#include <string>
#include <vector>

struct Directives {
	public:
		const std::string mnemonic;
		const argc;
		std::vector< int > OPs;
		
		static Directives SECTION();
		static Directives SPACE();
		static Directives CONST();
		static Directives EQU();
		static Directives IF();
		static Directives MACRO();
		static Directives END();
	private:
		Directives( std::string mnemonic, int argc);
};

#endif // INSTRUCTION_HPP
