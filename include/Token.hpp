#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <regex>

struct Token {
	public:
		Token();
		Token(std::string text, int line = -1, int column = -1);
		enum Validity {
			LABEL = 1,
			NUMBER = 1 << 1,
			ARITHM = 1 << 2,
			SPEC_C = 1 << 3
		};
		int GetLine();
		int GetColumn();
		int GetValidity();
		std::string GetText();
		const static std::regex Label;
		const static std::regex Number;
		const static std::regex Arithm;
	private:
		std::string tokenText;
		int line;
		int column;
		int validAs;
};

#endif // TOKEN_HPP