#include <iostream>
#include<strings.h>
#include <string>
#include <unordered_map>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <memory>
#include <sstream>
#include <iterator>
#include<vector>
using namespace std;

int main(int argc, const char **argv){


    string linha, codigo_completo, mapadebits_completo, mapadebits, tabela_uso_glob, aux_label;
    char *aux_linha, *nome_arquivo, *label , *aux_pos_label;
    int cont_linha, tamanho_arquivo, pos_label, fator_correcao=0, num_arquivos=argc-1, i=1;
    bool cabecalho_nome=true, cabecalho_tamanho=false, cabecalho_mapa=false;
    std::unordered_map<std::string,int> tabela_def_glob;
    std::unordered_map<std::string,int>::const_iterator gotLabel;

    aux_linha=(char *)malloc(sizeof(char)*200);
    nome_arquivo=(char *)malloc(sizeof(char)*200);
    aux_pos_label=(char *)malloc(sizeof(char)*200);
    label=(char *)malloc(sizeof(char)*100);


    while(num_arquivos>0){
        ifstream prog(argv[i]);
        if (!prog.is_open()){
            cout << "Nao foi possivel abrir o arquivo!";
            num_arquivos--;
            break;
        }
    // PRIMEIRA ETAPA: MONTAR TABELAS DE DEFINIÇÃO E USO DO CÓDIGO
        while (getline (prog,linha)){
            aux_linha=std::strtok(const_cast<char*>(linha.c_str())," ");

            //Armazena nome do programa
            if(!strcmp(aux_linha,"H:")&&cabecalho_nome){
                aux_linha=strtok(NULL, " ");
                strcpy(nome_arquivo,(const char*) aux_linha);
                cabecalho_nome=false;
                cabecalho_tamanho=true;
                continue;
            }
            //Armazena tamanho do programa
            if(!strcmp(aux_linha,"H:")&&cabecalho_tamanho){
                aux_linha=strtok(NULL, " ");
                tamanho_arquivo = atoi(aux_linha);
                cabecalho_tamanho=false;
                cabecalho_mapa=true;
                continue;
            }
            //Armazena mapa de bits e junta com o de outros programas
            if(!strcmp(aux_linha,"H:")&&cabecalho_mapa){
                aux_linha=strtok(NULL, " ");
                mapadebits=mapadebits+string(aux_linha);
                cabecalho_mapa=false;
                cabecalho_nome=true;
                continue;
            }

            //Armazena a tabela de definições em um mapa
            if(!strcmp(aux_linha,"TD:")){
                aux_linha=strtok(NULL, " ");
                 while(aux_linha!=NULL){
                    pos_label=atoi(strtok(NULL, " "))+fator_correcao;
                    tabela_def_glob.insert(make_pair(aux_linha,pos_label));
                    aux_linha=strtok(NULL, " ");

                 }
                 continue;
            }
            //Armazena a tabela de uso em um vetor
            if(!strcmp(aux_linha,"TU:")){
                aux_linha=strtok(NULL, " ");
                while(aux_linha!=NULL){
                    pos_label=atoi(strtok(NULL, " "))+fator_correcao;
                    tabela_uso_glob=tabela_uso_glob+string(aux_linha)+" "+to_string(pos_label)+" ";
                    aux_linha=strtok(NULL, " ");
                 }
                 continue;
            }
            //Armazena codigo montado e adiciona fator de correção
            if(!strcmp(aux_linha,"T:")){
                aux_linha=strtok(NULL, " ");
                int j=fator_correcao;
                while(aux_linha!=NULL){
                    if(mapadebits[j]=='1'){
                        codigo_completo=codigo_completo+to_string(atoi(aux_linha)+fator_correcao)+" ";
                        aux_linha=strtok(NULL, " ");
                        j++;
                    }

                    else{
                        codigo_completo=codigo_completo+string(aux_linha)+" ";
                        aux_linha=strtok(NULL, " ");
                        j++;
                    }
                }
                continue;
            }
        }
        fator_correcao=fator_correcao+tamanho_arquivo;
        prog.close();
        num_arquivos--;
        i++;
    }
    
    //ARRUMA OS ENDEREÇOS COM AS INFORMAÇÕES DA TABELA DE USO
    std::stringstream codigo(codigo_completo);
    std::istream_iterator<std::string> begin(codigo);
    std::istream_iterator<std::string> end;
    vector<std::string> vector_codigo(begin, end);

    label=std::strtok(const_cast<char*>(tabela_uso_glob.c_str())," ");
    while(label!=NULL){
        gotLabel=tabela_def_glob.find(string(label));
        pos_label=atoi(strtok(NULL," "));
        vector_codigo[pos_label].replace(vector_codigo[pos_label].begin(),vector_codigo[pos_label].end(),to_string(gotLabel->second));
        label=strtok(NULL," ");
    }

    //MONTAGEM DO ARQUIVO OBJETO LIGADO
    ofstream arquivo_saida;

    arquivo_saida.open(string(argv[1]));
    arquivo_saida << "H: " + string(argv[1])+"\n";
    arquivo_saida << "H: " + to_string(fator_correcao) + "\n";
    arquivo_saida << "H: " + mapadebits + "\n";
    arquivo_saida << "T: ";

    for(i=0;i<fator_correcao;i++)
        arquivo_saida << vector_codigo[i] + " ";


return 0;
}
