#################################################
#												#
#	Created by Gabriel Barbosa for SB - 2/2017	#
#	 https://github.com/bestknighter/tlang		#
#												#
#################################################

#-------------------------------------------------------------
# Assumindo UNIX											 |
#-------------------------------------------------------------

# Remover pastas
RMDIR = rm -rf

# Remover arquivos
RM = rm -f

# Mudar diretório
CD = cd

# Make
MAKE = make

PROGS = montador ligador carregador tlang

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

.PHONY: clean clean_this clean_inner release do_release debug do_debug copy
.IGNORE: clean clean_inner clean_this

release: do_release copy

do_release:
	-@$(foreach PROG,$(PROGS),$(call run_cmd,$(CD) $(PROG) && $(MAKE) release))

debug: do_debug copy

do_debug:
	-@$(foreach PROG,$(PROGS),$(call run_cmd,$(CD) $(PROG) && $(MAKE) debug))

copy:
ifeq ($(OS), Windows_NT)
	-@$(foreach PROG,$(PROGS),$(call run_cmd,copy $(PROG)\$(PROG).exe $(PROG).exe))
else
	@mkdir -p bin
	-@$(foreach PROG,$(PROGS),$(call run_cmd,cp $(PROG)/$(PROG) ./bin/$(PROG)))
endif

clean: clean_inner clean_this

clean_inner:
	-@$(foreach PROG,$(PROGS),$(call run_cmd,$(CD) $(PROG) && $(MAKE) clean))

clean_this:
ifeq ($(OS), Windows_NT)
	-@$(foreach PROG,$(PROGS),$(call run_cmd,$(RM) $(PROG).exe))
else
	-@$(foreach PROG,$(PROGS),$(call run_cmd,$(RM) ./bin/$(PROG)))
	@rm -rf bin
endif

# A linha em branco abaixo de $(1) é de extrema importância! Não apagar!
define run_cmd
	$(1)
	
endef

# Regra pra debug
print-%:
	@echo $* = $($*)
