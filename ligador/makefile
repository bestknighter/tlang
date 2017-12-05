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

FLAGS = -std=c++14 -Wall -Wextra -fmax-errors=5 -Wno-unused-parameter -fdiagnostics-color -Werror=init-self
DPFLAGS = -M -MT $(OBJ_PATH)/$(*F).o -MP -MF $@ $(FLAGS)
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
EXEC = ligador


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

.PHONY: clean release debug folders
.IGNORE: clean

release: FLAGS += $(RLFLAGS)
release: $(EXEC)

debug: FLAGS += $(DBFLAGS)
debug: $(EXEC)

clean:
	-@$(RMDIR) "$(DEP_PATH)"
	-@$(RMDIR) "$(OBJ_PATH)"
	-@$(RM) $(EXEC)

folders:
ifeq ($(OS), Windows_NT)
	@if NOT exist $(DEP_PATH) ( mkdir $(DEP_PATH) )
	@if NOT exist $(OBJ_PATH) ( mkdir $(OBJ_PATH) )
	@if NOT exist $(INC_PATH) ( mkdir $(INC_PATH) )
	@if NOT exist $(SRC_PATH) ( mkdir $(SRC_PATH) )
else
	@mkdir -p $(DEP_PATH) $(OBJ_PATH) $(INC_PATH) $(SRC_PATH)
endif

$(DEP_PATH)/%.d: $(SRC_PATH)/%.cpp $(INC_PATH)/%.hpp | folders
	$(COMPILER) $(INC_PATHS) $< $(DPFLAGS)
$(DEP_PATH)/%.d: $(SRC_PATH)/%.cpp | folders
	$(COMPILER) $(INC_PATHS) $< $(DPFLAGS)
$(DEP_PATH)/%.d: $(SRC_PATH)/%.hpp | folders
	$(COMPILER) $(INC_PATHS) $< $(DPFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp $(DEP_PATH)/%.d | folders
	$(COMPILER) $(INC_PATHS) $< -c $(FLAGS) -o $@

$(EXEC): $(DEP_FILES) $(OBJ_FILES)
	$(COMPILER) -o $@ $(OBJ_FILES) $(LIBS) $(FLAGS)


# Regra pra debug
print-%:
	@echo $* = $($*)

-include $$(DEP_FILES)
	
