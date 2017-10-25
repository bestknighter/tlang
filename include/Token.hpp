#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <regex>
#include <string>

/**
 * Struct Token
 * Essa classe foi feita para se armazenar um elemento minimo de codigo.
 * Ela possui deteccao de erro proprio, apesar de nao notificar ao usuario.
 * Eh usado pelo scanner e ele informara ao usuario do erro.
 */
struct Token {
	public:
		Token();
		Token(std::string text, int line = -1, int column = -1);
		enum Validity {
			LABEL = 1, // Começa com letra ou '_' e é seguido por letra, número ou '_'
			NUMBER = 1 << 1, // Número decimal ou hexadecimal (formato 0x000) com ou sem sinal
			ARITHM = 1 << 2, // '+' ou '-' para o offset
			SPEC_C = 1 << 3 // Separadores que mudam o significado de tokens anteriores, ou seja, ':' e ','
		};
		int GetLine(); // Retorna a linha do código fonte que esse token foi encontrado
		int GetColumn(); // Retorna onde esse token começa na linha do código fonte
		int GetValidity(); // Retorna qual é a validade desse token como um bitset.
		std::string GetText(); // Obtem a string do Token (já tudo em maiusculas)
		const static std::regex Label; // Regex para verificar se é label. Por label, entenda diretiva, instrução, label ou qualquer outro símbolo estilo palavra
		const static std::regex Number; // Regex para verificar se é número.
		const static std::regex Arithm; // Regex para verificar se é operação de soma ou subtração
	private:
		std::string tokenText;
		int line;
		int column;
		int validAs;
};

#endif // TOKEN_HPP
