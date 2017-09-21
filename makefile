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

# Touch
TOUCH = touch

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

# Touch
TOUCH = echo $nul >>

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
release: cache/all.txt

debug: FLAGS += $(DBFLAGS)
debug: cache/all.txt

cache/deps.txt: $(DEP_FILES)
	-@$(TOUCH) cache/deps.txt

cache/objs.txt: cache/deps.txt $(OBJ_FILES)
	-@$(TOUCH) cache/objs.txt

cache/all.txt: $(EXEC)
	-@$(TOUCH) cache/all.txt

clean:
	-@$(RM) "$(OBJ_PATH)\*"
	-@$(RM) "$(DEP_PATH)\*"
	-@$(RM) "cache\*"
	-@$(RM) $(EXEC)

$(DEP_PATH)/%.d: $(SRC_PATH)/%.cpp
	$(COMPILER) $(INC_PATHS) $< $(DPFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(COMPILER) $(INC_PATHS) $< -c $(FLAGS) -o $@

$(EXEC): cache/objs.txt
	$(COMPILER) -o $@ $(OBJ_FILES) $(LIBS)


# Regra pra debug
print-%:
	@echo $* = $($*)

-include $(DEP_FILES)
	