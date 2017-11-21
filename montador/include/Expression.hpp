#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <array>

/**
 * Struct Expression
 * Essa estrutura armazena e valida uma linha de código completa composta de
 * LABEL*: OPERAÇÃO OPERANDO1* OFFSET1*, OPERANDO2* OFFSET2*
 * '*' indica elementos opcionais no caso geral
 */
struct Expression {
	public:
		Expression();
		Expression( bool isValid, unsigned int lineInSource
				  , std::string label, std::string operation
				  , std::string operand1 = "", std::string operand2 = ""
				  , int offset1 = 0, int offset2 = 0 );

		bool IsValid();
		void Invalidate(); // Para caso os operandos sejam alterados
		unsigned int GetLineInSource();
		std::string GetLabel();
		std::string GetOperation();
		std::array< std::string, 2 > GetOperands();
		std::array< int, 2 > GetOffsets();
		
		void ReplaceOperand( int whichOne, std::string substitute );

		explicit operator std::string(); // Facilita a transformação para impressão tanto na tela como no arquivo
	private:
		unsigned int lineInSource;
		bool isValid;
		std::string label;
		std::string operation;
		std::array< std::string, 2 > operands;
		std::array< int, 2 > offsets;
};

#endif // EXPRESSION_HPP
