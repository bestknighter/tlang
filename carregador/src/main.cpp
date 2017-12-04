//
//  main.cpp
//  Carregador
//
//  Created by Adarley Grando on 02/12/17.
//  Copyright © 2017 Adarley Grando. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

size_t find_nth(const std::string&, size_t, const std::string&, size_t);
void vetorMemoria(std::string, int[500], long, int);

int main(int argc, const char * argv[]) {
    
    std::string fileTitle = argv[1];
    // fileTitle = fileTitle + ".o";
    std::ifstream input;
    input.open( fileTitle.c_str() );
    std::string line;
    
    /**********************************
     * Obtem informacoes do cabecalho *
     **********************************/
    std::string nomeArquivo;
    getline( input, line );
    nomeArquivo = line.substr(3,line.length() - line.find("."));
    std::cout << "Nome arquivo: " << nomeArquivo << std::endl;
    
    getline( input, line );
    int tamanho = std::stoi(line.substr(3));
    std::cout << "Tamanho do programa em bytes: " << tamanho << std::endl;
    
    getline( input, line );
    std::string mapaBitsRel = line.substr(3);
    std::cout << "Mapa de Bits: " << mapaBitsRel << std::endl;
    
    getline( input, line );
    std::string codigo = line.substr(3);
    //std::cout << "Código Inicial: " << codigo << std::endl;
    
    input.close();

    //carregar valores dos enderecos do programa para um vetor
    int memoria[500] = {0};
    std::cout << "Vetor carregado: ";
    vetorMemoria(codigo, memoria, 0, 0);
    for (int i = 0; i < tamanho; i++) {
        std::cout << memoria[i] << " ";
    }
    std::cout << std::endl;
    
    /*************
     * Simulacao *
     *************/
    std::cout << "---- Inicio Simulacao:" << std::endl;
    bool executar = true;
    long pos = 0;
    int acumulador = 0;
    while (executar) {
        //tratar operando
        std::string op;
        long nextPos = codigo.find(" ",pos);
        op = codigo.substr(pos,nextPos-pos);
        pos = nextPos + 1;
        int opInt = std::stoi(op);
        if (opInt == 14) {
            std::cout << "STOP." << std::endl;
            executar = false;
        }
        else if (opInt == 9) {
            //COPY, dois operandos
            nextPos = codigo.find(" ",pos);
            std::string operando = codigo.substr(pos,nextPos-pos);
            int operandoInt = std::stoi(operando);
            pos = nextPos + 1;
            std::cout << "COPY: " << memoria[operandoInt] << ", ";
            nextPos = codigo.find(" ",pos);
            operando = codigo.substr(pos,nextPos-pos);
            int operandoInt2 = std::stoi(operando);
            pos = nextPos + 1;
            std::cout << memoria[operandoInt2] << std::endl;
            memoria[operandoInt2] = memoria[operandoInt];
        }
        else {
            //um operando
            nextPos = codigo.find(" ",pos);
            std::string operando = codigo.substr(pos,nextPos-pos);
            pos = nextPos + 1;
            int operandoInt = std::stoi(operando);
            
            if (opInt == 1) {
                std::cout << "ADD. MEM[OP]: " << memoria[operandoInt] << ", Acumulador: " << acumulador;
                acumulador += memoria[operandoInt];
                std::cout << ", Resultado: " << acumulador << std::endl;
            }
            else if (opInt == 2) {
                std::cout << "SUB. MEM[OP]: " << memoria[operandoInt] << ", Acumulador: " << acumulador;
                acumulador = acumulador - memoria[operandoInt];
                std::cout << ", Resultado: " << acumulador << std::endl;
            }
            else if (opInt == 3) {
                std::cout << "MULT. MEM[OP]: " << memoria[operandoInt] << ", Acumulador: " << acumulador;
                acumulador = acumulador * memoria[operandoInt];
                std::cout << ", Resultado: " << acumulador << std::endl;
            }
            else if (opInt == 4) {
                std::cout << "DIV. MEM[OP]: " << memoria[operandoInt] << ", Acumulador: " << acumulador;
                acumulador = acumulador / memoria[operandoInt];
                std::cout << ", Resultado: " << acumulador << std::endl;
            }
            else if (opInt == 5) {
                long posicaoJmp = find_nth(codigo, 0, " ", operandoInt-1);
                std::cout << "JMP. Operando: " << operandoInt << std::endl;
                pos = posicaoJmp+1;
            }
            else if (opInt == 6) {
                if (acumulador < 0) {
                    long posicaoJmp = find_nth(codigo, 0, " ", operandoInt-1);
                    std::cout << "JMPN. Operando: " << operandoInt << std::endl;
                    pos = posicaoJmp+1;
                }
            }
            else if (opInt == 7) {
                if (acumulador > 0) {
                    long posicaoJmp = find_nth(codigo, 0, " ", operandoInt-1);
                    std::cout << "JMPP. Operando: " << operandoInt << std::endl;
                    pos = posicaoJmp+1;
                }
            }
            else if (opInt == 8) {
                if (acumulador == 0) {
                    long posicaoJmp = find_nth(codigo, 0, " ", operandoInt-1);
                    std::cout << "JMPZ. Operando: " << operandoInt << std::endl;
                    pos = posicaoJmp+1;
                }
            }
            else if (opInt == 10) {
                acumulador = memoria[operandoInt];
                std::cout << "LOAD. Pos. memoria: " << operandoInt << ", Valor: " << acumulador << std::endl;
            }
            else if (opInt == 11) {
                memoria[operandoInt] = acumulador;
                std::cout << "STORE. Pos. memoria: " << operandoInt << ", Valor: " << acumulador << std::endl;
            }
            else if (opInt == 12) {
                std::cout << "INPUT. Entre com valor: ";
                std::string textoInput;
                std::cin >> textoInput;
                std::cout << "Pos. memoria: " << operandoInt << ", Valor: " << textoInput << std::endl;
                memoria[operandoInt] = std::stoi(textoInput);
            }
            else if (opInt == 13) {
                std::cout << "OUTPUT. Pos. memoria: " << operandoInt << ", Valor: " << memoria[operandoInt] << std::endl;
                std::cout << memoria[operandoInt] << std::endl;
            }
        }
    }
  
    std::cout << "---- Fim Simulacao." << std::endl << "Espelho de memoria apos execucao:" << std::endl;
    for (int i = 0; i < tamanho; i++) {
        std::cout << memoria[i] << " ";
    }
    std::cout << std::endl << std::endl;
    
    /******************
     * Calcula chunks *
     ******************/
    int chunks = std::atoi(argv[2]);
    int tamanhoDisponivelMem = 0;
    int chunkUnica = -1;
    for (int i = 0; i < chunks; i++) {
        int valor = std::atoi(argv[3+i]);
        tamanhoDisponivelMem += valor;
        if (valor >= tamanho) {
            chunkUnica = i;
        }
    }
  
    std::cout << "Memoria disponivel em bytes para execucao:\t" << tamanhoDisponivelMem << std::endl;
    
    /******************************************************
     * Aloca de acordo com o espaco disponivel, se houver *
     ******************************************************/
    if (tamanhoDisponivelMem >= tamanho) {
        //tudo certo, carregar programa em memoria
        std::string codigoNovo;
        if (chunkUnica > 0) {
            //programa pode ser alocado em um chunk
            int enderecoIn = std::atoi(argv[3+chunks+chunkUnica]);
            for (int i = 0; i < tamanho; i++) {
                size_t strInicio;
                if(i > 0) strInicio = find_nth(codigo, 0, " ", i-1)+1;
                else strInicio = 0;
                size_t strFim = find_nth(codigo, 0, " ", i);
                if (strFim == std::string::npos) {
                    strFim = codigo.length();
                }
                int valor = std::stoi(codigo.substr(strInicio, strFim - strInicio));
                if (mapaBitsRel[i] == '1') {
                    //realocar endereco relativo
                    valor = valor + enderecoIn;
                }
                if (i > 0) {
                    codigoNovo = codigoNovo + " " + std::to_string(valor);
                }
                else {
                    codigoNovo = std::to_string(valor);
                }
            }
            std::cout << codigoNovo;
        }
        else {
            //programa tem que ser dividido em chunks
            int tamanhoEscrito = 0;
            for (int chunkAtual = 0; chunkAtual < chunks && tamanhoEscrito < tamanho; chunkAtual++) {
                int tamanhoChunkAtual = std::atoi(argv[3+chunkAtual]);
                int enderecoIn = std::atoi(argv[3+chunks+chunkAtual]);
                for (int tamanhoChunkPreenchido = 0; tamanhoChunkPreenchido < tamanhoChunkAtual && tamanhoEscrito < tamanho; tamanhoChunkPreenchido++) {
                    size_t strInicio;
                    if(tamanhoEscrito > 0) strInicio = find_nth(codigo, 0, " ", tamanhoEscrito-1)+1;
                    else strInicio = 0;
                    size_t strFim = find_nth(codigo, 0, " ", tamanhoEscrito);
                    if (strFim == std::string::npos) {
                        strFim = codigo.length();
                    }
                    int valor = std::stoi(codigo.substr(strInicio, strFim - strInicio));
                    if (mapaBitsRel[tamanhoEscrito] == '1') {
                        //realocar endereco relativo
                        valor = valor + enderecoIn;
                    }
                    if (tamanhoEscrito > 0) {
                        codigoNovo = codigoNovo + " " + std::to_string(valor);
                    }
                    else {
                        codigoNovo = std::to_string(valor);
                    }
                    tamanhoEscrito++;
                }
            }
          
            std::cout << "Codigo apos correcao de enderecos: " << std::endl << codigoNovo << std::endl << std::endl;
            std::ofstream output;
            output.open( (fileTitle + ".im").c_str() );
            output << codigoNovo;
            output.close();
        }
    }
    else {
        //erro, memoria insuficiente
        std::cout << "OUT OF MEMORY - YOUR PROGRAM WILL NOT BE LOADED" << std::endl;
    }
    
    return 0;
}

void vetorMemoria(std::string codigo, int vetor[500], long pos, int pc) {
    size_t nextPos = codigo.find(" ",pos);
    if(std::string::npos == nextPos) {
        //nao encontrou mais
        return;
    }
    std::string op = codigo.substr(pos,nextPos-pos);
    vetor[pc] = std::stoi(op);
    pc += 1;
    pos = nextPos + 1;
    vetorMemoria(codigo, vetor, pos, pc);
}

size_t find_nth(const std::string& texto, size_t pos, const std::string& needle, size_t nth)
{
    size_t found_pos = texto.find(needle, pos);
    if(0 == nth || std::string::npos == found_pos)  return found_pos;
    return find_nth(texto, found_pos+1, needle, nth-1);
}
