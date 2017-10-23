#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

struct Config {
	public:
		static std::string inputFile;
		static std::string outputFile;
		static std::string ext;
		static int numSteps;
};

#endif // CONFIG_HPP