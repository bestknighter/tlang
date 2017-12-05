#include <string>
#include <vector>
#include <iostream>

int main(int argc, const char **argv) {
	std::string cmdMont = "";
	std::string cmdLig = "";

	for( int i = 1; i < argc; i++ ) {
		std::string digitado = std::string(argv[i]);
		#ifdef _WIN32
		cmdMont += std::string("\"") + digitado + "\" ";
		cmdLig += std::string("\"") + digitado.substr(0, digitado.find_last_of('.')) + ".o\" ";
		#else
		cmdMont += digitado + " ";
		cmdLig += digitado.substr(0, digitado.find_last_of('.')) + ".o ";
		#endif
	}


	// Montador
	#ifdef _WIN32
	system( (std::string("montador.exe ") + cmdMont).c_str() );
	#else
	system( (std::string("./bin/montador ") + cmdMont).c_str() );
	#endif
	std::cout << "Arquivos montados." << std::endl;

	// Ligador
	#ifdef _WIN32
	system( (std::string("ligador.exe ") + cmdLig).c_str() );
	#else
	system( (std::string("./bin/ligador ") + cmdLig).c_str() );
	#endif
	std::cout << "Arquivos ligados." << std::endl;

	return 0;
}
