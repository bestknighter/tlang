#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

/**
 * Struct Config
 * Estrutura para armazenar de forma a se ter fácil aceso por todo
 * o código das configuraçãoes setadas por linha de comando.
 */
struct Config {
	public:
		static std::string inputFile;
		static std::string outputFile;
		static std::string ext;
		static bool beginEndRequired;
};

#endif // CONFIG_HPP
