#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dicionario.h"
#include "nixJson.h"

NIXJSON* NixJson_create() {
    NIXJSON* nixJson = malloc(sizeof(NIXJSON));
    if (!nixJson) {
        perror("Erro ao alocar memória para NIXJSON");
        exit(1);
    }

    nixJson->dicionario = malloc(sizeof(DICIONARIO));
    if (!nixJson->dicionario) {
        perror("Erro ao alocar memória para o dicionário");
        free(nixJson);
        exit(1);
    }

    nixJson->dicionario->qtdItens = 0;
    nixJson->dicionario->item = NULL;
    nixJson->stringJson = NULL;

    return nixJson;
}

void NixJson_readJson(NIXJSON* nixJson, char* path) {
    FILE* f = fopen(path, "r");
    if (!f) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long tamanhoString = ftell(f);
    fseek(f, 0, SEEK_SET);

    nixJson->stringJson = malloc(tamanhoString + 1);
    if (!nixJson->stringJson) {
        perror("Erro ao alocar memória para stringJson");
        fclose(f);
        exit(1);
    }

    fread(nixJson->stringJson, sizeof(char), tamanhoString, f);
    nixJson->stringJson[tamanhoString] = '\0';
    fclose(f);
}

int charcatmalloc(char caractere, char* palavra){
    if(palavra == NULL){
        perror("falha ao obter palavra (charcatmalloc)");
        exit(1);
    } 

    size_t tamanhoPalavra = strlen(palavra);
    palavra = realloc(palavra, tamanhoPalavra + 2);

    palavra[tamanhoPalavra] = caractere;
    palavra[tamanhoPalavra+1] = '\0';
    
    if(strlen(palavra) != tamanhoPalavra + 1){
        perror("falha ao alocar memoria (charcatmalloc)");
        exit(1);
    };

    return 0;
}

int removeWhitespace(char* str){
    int stringSize = 0;
    char *origem = str;
    char *destino = str;
    int aspas = 2;

    while (*origem != '\0'){
        if(*origem == '"') aspas++;
        if(!(*origem == ' ' || *origem == '\n' || *origem == '\t' || *origem == '\r') || aspas%2 != 0){
            *destino++ = *origem;
            stringSize++;
        }
        origem++;
    }

    *destino = '\0';
    return stringSize;
}

void limparPalavra(char** palavra){
    if(*palavra) free(*palavra);
    *palavra = malloc(1);
    (*palavra)[0] = '\0';
}

NIXJSON* NixJson_parseJson(NIXJSON* nixJson){
    CONJUNTO chaves;
    chaves.itensAbertos = 0;
    chaves.itensFechado = 0;
    CONJUNTO colchetes;
    colchetes.itensAbertos = 0;
    colchetes.itensFechado = 0;
    char* chave = NULL;
    char* valor = NULL;
    limparPalavra(&chave);
    limparPalavra(&valor);
    int stringSize = removeWhitespace(nixJson->stringJson);

    printf("\njson: %s\n", nixJson->stringJson);

    for(int i = 0; i < stringSize; i++){
        char caractere = nixJson->stringJson[i];

        if(caractere == '{'){
            if(i == 0) continue;
            chaves.itensAbertos++;
        }

        if(caractere == '}'){
            if(i == (stringSize-1)){
                adicionarItem(nixJson->dicionario, chave, valor);
                limparPalavra(&chave);
                limparPalavra(&valor);
                continue;
            };
            chaves.itensFechado++;
        }

        if(caractere == '['){
            colchetes.itensAbertos++;
        }

        if(caractere == '['){
            colchetes.itensFechado++;
        }


    }
}



