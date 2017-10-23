#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <array>

struct Expression {
	public:
		Expression();
		Expression( bool isValid, std::string label, std::string operation, std::string operand1 = "", std::string operand2 = "", int offset1 = 0, int offset2 = 0 );

		bool IsValid();
		std::string GetLabel();
		std::string GetOperation();
		std::array< std::string, 2 > GetOperands();
		std::array< int, 2 > GetOffsets();
	private:
		bool isValid;
		std::string label;
		std::string operation;
		std::array< std::string, 2 > operands;
		std::array< int, 2 > offsets;
};

#endif // EXPRESSION_HPP
