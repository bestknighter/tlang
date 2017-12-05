# tlang

[tlang](https://github.com/bestknighter/tlang) is toolchain for assembling codes for a toy ISA

Developed for Windows 10 x64 but with Linux compatiblity in mind.

Made by Gabriel Barbosa (12/0050935), Victor de Carvalho (12/0062232) and Adarley Grando (11/0007344)

## Compilation instructions

***ATTENTION***: You'll need a g++ version compatible with the directive -std=c++14

- Run, in the root folder, `make release` or `make debug` or simply `make`. If you're using Windows with mingw32, run `mingw32-make release` or `mingw32-make debug` or simply `mingw32-make`.
- A `./bin/[montador|ligador|carregador]`(Linux) \ `[montador|ligador|carregador].exe` (Windows) will show up on the same directory. You can call it as specified below.
- If you want to change any flag passed to the inner makefiles, you can run as demonstrated below.
  - Let's say I want to change the compiler and my debug flags: `make debug MAKEFLAGS="COMPILER=clang DBFLAGS=-g DBFLAGS+=-O0"`

## Calling instructions

- For calling `montador`, you'll need to pass 1 or more `.asm` files. It'll generate one `.o` for each file. Windows example:
  - Windows: `.\montador.exe '.\Arquivos Teste\fat_mod_A.asm' '.\Arquivos Teste\fat_mod_B.asm'`
  - Linux: `./bin/montador ./Arquivos\ Teste/fat_mod_A.asm ./Arquivos\ Teste/fat_mod_B.asm`

- For calling `ligador`, you'll need to pass 1 or more `.o` files. It'll generate a single file without extension with the same name as the first file. Windows example:
  - Windows `.\ligador.exe '.\Arquivos Teste\fat_mod_A.o' '.\Arquivos Teste\fat_mod_B.o'`
  - Linux: `./bin/ligador ./Arquivos\ Teste/fat_mod_A.o ./Arquivos\ Teste/fat_mod_B.o`

- For calling `carregador`, you'll need to pass one file without extension followed by memory chunk info. It'll generate a `.im` file. Windows example:
  - Windows: `.\carregador.exe '.\Arquivos Teste\fat_mod_A' 3 10 10 10 100 200 300`
  - Linux: `./bin/carregador ./Arquivos\ Teste/fat_mod_A 3 10 10 10 100 200 300`
  - In this case, we have 3 chunks with 10 bytes each located at address 100, 200 and 300, respectively.
