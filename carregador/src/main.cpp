//
//  main.cpp
//  Carregador
//
//  Created by Adarley Grando on 02/12/17.
//  Copyright © 2017 Adarley Grando. All rights reserved.
//

#include <iostream>
#include <fstream>

using namespace std;

size_t find_nth(const string&, size_t, const string&, size_t);
void vetorMemoria(string, int[500], long, int);

int main(int argc, const char * argv[]) {
    
    std::string fileTitle = argv[1];
    // fileTitle = fileTitle + ".o";
    std::ifstream input( fileTitle );
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
    
    //carregar valores dos enderecos do programa para um vetor
    int memoria[500] = {0};
    cout << "Vetor carregado: ";
    vetorMemoria(codigo, memoria, 0, 0);
    for (int i = 0; i < tamanho; i++) {
        cout << memoria[i] << " ";
    }
    cout << endl;
    
    /*************
     * Simulacao *
     *************/
    cout << "---- Inicio Simulacao:" << endl;
    bool executar = true;
    long pos = 0;
    int acumulador = 0;
    while (executar) {
        //tratar operando
        string op;
        long nextPos = codigo.find(" ",pos);
        op = codigo.substr(pos,nextPos-pos);
        pos = nextPos + 1;
        int opInt = stoi(op);
        if (opInt == 14) {
            cout << "STOP." << endl;
            executar = false;
        }
        else if (opInt == 9) {
            //COPY, dois operandos
            nextPos = codigo.find(" ",pos);
            string operando = codigo.substr(pos,nextPos-pos);
            int operandoInt = stoi(operando);
            pos = nextPos + 1;
            cout << "COPY: " << memoria[operandoInt] << ", ";
            nextPos = codigo.find(" ",pos);
            operando = codigo.substr(pos,nextPos-pos);
            int operandoInt2 = stoi(operando);
            pos = nextPos + 1;
            cout << memoria[operandoInt2] << endl;
            memoria[operandoInt2] = memoria[operandoInt];
        }
        else {
            //um operando
            nextPos = codigo.find(" ",pos);
            string operando = codigo.substr(pos,nextPos-pos);
            pos = nextPos + 1;
            int operandoInt = stoi(operando);
            
            if (opInt == 1) {
                cout << "ADD. MEM[OP]: " << memoria[operandoInt] << ", Acumulador: " << acumulador;
                acumulador += memoria[operandoInt];
                cout << ", Resultado: " << acumulador << endl;
            }
            else if (opInt == 2) {
                cout << "SUB. MEM[OP]: " << memoria[operandoInt] << ", Acumulador: " << acumulador;
                acumulador = acumulador - memoria[operandoInt];
                cout << ", Resultado: " << acumulador << endl;
            }
            else if (opInt == 3) {
                cout << "MULT. MEM[OP]: " << memoria[operandoInt] << ", Acumulador: " << acumulador;
                acumulador = acumulador * memoria[operandoInt];
                cout << ", Resultado: " << acumulador << endl;
            }
            else if (opInt == 4) {
                cout << "DIV. MEM[OP]: " << memoria[operandoInt] << ", Acumulador: " << acumulador;
                acumulador = acumulador / memoria[operandoInt];
                cout << ", Resultado: " << acumulador << endl;
            }
            else if (opInt == 5) {
                long posicaoJmp = find_nth(codigo, 0, " ", operandoInt-1);
                cout << "JMP. Operando: " << operandoInt << endl;
                pos = posicaoJmp+1;
            }
            else if (opInt == 6) {
                if (acumulador < 0) {
                    long posicaoJmp = find_nth(codigo, 0, " ", operandoInt-1);
                    cout << "JMPN. Operando: " << operandoInt << endl;
                    pos = posicaoJmp+1;
                }
            }
            else if (opInt == 7) {
                if (acumulador > 0) {
                    long posicaoJmp = find_nth(codigo, 0, " ", operandoInt-1);
                    cout << "JMPP. Operando: " << operandoInt << endl;
                    pos = posicaoJmp+1;
                }
            }
            else if (opInt == 8) {
                if (acumulador == 0) {
                    long posicaoJmp = find_nth(codigo, 0, " ", operandoInt-1);
                    cout << "JMPZ. Operando: " << operandoInt << endl;
                    pos = posicaoJmp+1;
                }
            }
            else if (opInt == 10) {
                acumulador = memoria[operandoInt];
                cout << "LOAD. Pos. memoria: " << operandoInt << ", Valor: " << acumulador << endl;
            }
            else if (opInt == 11) {
                memoria[operandoInt] = acumulador;
                cout << "STORE. Pos. memoria: " << operandoInt << ", Valor: " << acumulador << endl;
            }
            else if (opInt == 12) {
                cout << "INPUT. Entre com valor: ";
                string textoInput;
                cin >> textoInput;
                cout << "Pos. memoria: " << operandoInt << ", Valor: " << textoInput << endl;
                memoria[operandoInt] = stoi(textoInput);
            }
            else if (opInt == 13) {
                cout << "OUTPUT. Pos. memoria: " << operandoInt << ", Valor: " << memoria[operandoInt] << endl;
                cout << memoria[operandoInt] << endl;
            }
        }
    }
  
    cout << "---- Fim Simulacao." << endl << "Espelho de memoria apos execucao:" << endl;
    for (int i = 0; i < tamanho; i++) {
        cout << memoria[i] << " ";
    }
    cout << endl << endl;
    
    /******************
     * Calcula chunks *
     ******************/
    int chunks = std::atoi(argv[2]);
    int tamanhoDisponivelMem = 0;
    int chunkUnica = -1;
    for (int i = 0; i < chunks; i++) {
        int valor = atoi(argv[3+i]);
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
            int enderecoIn = atoi(argv[3+chunks+chunkUnica]);
            for (int i = 0; i < tamanho; i++) {
                size_t strInicio;
                if(i > 0) strInicio = find_nth(codigo, 0, " ", i-1)+1;
                else strInicio = 0;
                size_t strFim = find_nth(codigo, 0, " ", i);
                if (strFim == string::npos) {
                    strFim = codigo.length();
                }
                int valor = stoi(codigo.substr(strInicio, strFim - strInicio));
                if (mapaBitsRel[i] == '1') {
                    //realocar endereco relativo
                    valor = valor + enderecoIn;
                }
                if (i > 0) {
                    codigoNovo = codigoNovo + " " + to_string(valor);
                }
                else {
                    codigoNovo = to_string(valor);
                }
            }
            cout << codigoNovo;
        }
        else {
            //programa tem que ser dividido em chunks
            int tamanhoEscrito = 0;
            for (int chunkAtual = 0; chunkAtual < chunks && tamanhoEscrito < tamanho; chunkAtual++) {
                int tamanhoChunkAtual = atoi(argv[3+chunkAtual]);
                int enderecoIn = atoi(argv[3+chunks+chunkAtual]);
                for (int tamanhoChunkPreenchido = 0; tamanhoChunkPreenchido < tamanhoChunkAtual && tamanhoEscrito < tamanho; tamanhoChunkPreenchido++) {
                    size_t strInicio;
                    if(tamanhoEscrito > 0) strInicio = find_nth(codigo, 0, " ", tamanhoEscrito-1)+1;
                    else strInicio = 0;
                    size_t strFim = find_nth(codigo, 0, " ", tamanhoEscrito);
                    if (strFim == string::npos) {
                        strFim = codigo.length();
                    }
                    int valor = stoi(codigo.substr(strInicio, strFim - strInicio));
                    if (mapaBitsRel[tamanhoEscrito] == '1') {
                        //realocar endereco relativo
                        valor = valor + enderecoIn;
                    }
                    if (tamanhoEscrito > 0) {
                        codigoNovo = codigoNovo + " " + to_string(valor);
                    }
                    else {
                        codigoNovo = to_string(valor);
                    }
                    tamanhoEscrito++;
                }
            }
          
            cout << "Codigo apos correcao de enderecos: " << endl << codigoNovo << endl << endl;
            std::ofstream output( fileTitle + ".im" );
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

void vetorMemoria(string codigo, int vetor[500], long pos, int pc) {
    size_t nextPos = codigo.find(" ",pos);
    if(string::npos == nextPos) {
        //nao encontrou mais
        return;
    }
    string op = codigo.substr(pos,nextPos-pos);
    vetor[pc] = stoi(op);
    pc += 1;
    pos = nextPos + 1;
    vetorMemoria(codigo, vetor, pos, pc);
}

size_t find_nth(const string& texto, size_t pos, const string& needle, size_t nth)
{
    size_t found_pos = texto.find(needle, pos);
    if(0 == nth || string::npos == found_pos)  return found_pos;
    return find_nth(texto, found_pos+1, needle, nth-1);
}
