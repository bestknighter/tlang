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
	private:
		std::string tokenText;
		int line;
		int column;
		int validAs;
		static std::regex Label;
		static std::regex Number;
		static std::regex Arithm;
};

#endif // TOKEN_HPP