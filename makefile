#################################################
#												#
#  Created by Gabriel Barbosa for SB - 2/2017	#
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

FLAGS = -std=c++11 -Wall -Wextra -fmax-errors=5 -Wno-unused-parameter -fdiagnostics-color=auto -Werror=init-self
DPFLAGS = -M -MT $(OBJ_PATH)/$(*F).o -MP -MF $@
RLFLAGS = -O3 -DDEBUG=0
DBFLAGS = -ggdb -O0 -DDEBUG=1
LIBS = 

SRC_PATH = src
INC_PATH = include
DEP_PATH = dep
OBJ_PATH = bin

SRC_FILES = $(wildcard $(SRC_PATH)/*.cpp)
INC_FILES = $(wildcard $(INC_PATH)/*.hpp)
DEP_FILES = $(addprefix $(DEP_PATH)/,$(notdir $(SRC_FILES:.cpp=.d)))
OBJ_FILES = $(addprefix $(OBJ_PATH)/,$(notdir $(SRC_FILES:.cpp=.o)))

INC_PATHS = -I$(INC_PATH)


# Nome do executável
EXEC = montador


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

FLAGS += -mconsole
LIBS := -lmingw32 $(LIBS)

# Nome do executável
EXEC := $(EXEC).exe

endif

#############################################################################################
#############################################################################################

.PHONY: clean release debug
.IGNORE: clean

release: FLAGS += $(RLFLAGS)
release: $(EXEC)

debug: FLAGS += $(DBFLAGS)
debug: $(EXEC)

clean:
	-@$(RM) "$(OBJ_PATH)\*.o"
	-@$(RM) "$(DEP_PATH)\*.d"
	-@$(RM) $(EXEC)

$(DEP_PATH)/%.d: $(INC_PATH)/%.hpp $(SRC_PATH)/%.cpp
	$(COMPILER) $(INC_PATHS) $< $(DPFLAGS)

$(OBJ_PATH)/%.o: $(DEP_PATH)/%.d
	$(COMPILER) $(INC_PATHS) $< -c $(FLAGS) -o $@

$(EXEC): $(DEP_FILES) $(OBJ_FILES)
	$(COMPILER) -o $@ $(OBJ_FILES) $(LIBS)


# Regra pra debug
print-%:
	@echo $* = $($*)

-include $(DEP_FILES)
	