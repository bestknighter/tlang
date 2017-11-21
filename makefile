#################################################
#												#
#	Created by Gabriel Barbosa for SB - 2/2017	#
#	 https://github.com/bestknighter/tlang		#
#												#
#################################################

#-------------------------------------------------------------
# Assumindo UNIX											 |
#-------------------------------------------------------------

# Compilador
COMPILER = g++

# Remover pastas
RMDIR = rm -rf

# Remover arquivos
RM = rm -f

# Mudar diretório
CD = cd

# Make
MAKE = make

#-------------------------------------------------------------
# Caso seja Windows											 |
#-------------------------------------------------------------
ifeq ($(OS),Windows_NT)

# Comando para remover um diretório recursivamente
RMDIR = rd /s /q

# Comando para deletar um único arquivo
RM = del /q

# Make
MAKE = mingw32-make

endif

#############################################################################################
#############################################################################################

.PHONY: clean release debug folders
.IGNORE: clean

release:
	$(CD) montador && $(MAKE) release
	$(CD) ligador && $(MAKE) release
	$(CD) carregador && $(MAKE) release
ifeq ($(OS), Windows_NT)
	copy montador\montador.exe montador.exe
	copy ligador\ligador.exe ligador.exe
	copy carregador\carregador.exe carregador.exe
else
	cp montador/montador .
	cp ligador/ligador .
	cp carregador/carregador .
endif

debug:
	$(CD) montador && $(MAKE) debug
	$(CD) ligador && $(MAKE) debug
	$(CD) carregador && $(MAKE) debug
ifeq ($(OS), Windows_NT)
	copy montador\montador.exe montador.exe
	copy ligador\ligador.exe ligador.exe
	copy carregador\carregador.exe carregador.exe
else
	cp montador/montador .
	cp ligador/ligador .
	cp carregador/carregador .
endif

clean: clean_this
clean:
	$(CD) montador && $(MAKE) clean
	$(CD) ligador && $(MAKE) clean
	$(CD) carregador && $(MAKE) clean

clean_this:
ifeq ($(OS), Windows_NT)
	$(RM) montador.exe
	$(RM) ligador.exe
	$(RM) carregador.exe
else
	$(RM) montador
	$(RM) ligador
	$(RM) carregador
endif

# Regra pra debug
print-%:
	@echo $* = $($*)
