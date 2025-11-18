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
    nixJson->string = NULL;

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

int charcatmalloc(char caractere, char** palavra) {

    if (*palavra == NULL) {
        perror("falha ao obter palavra (charcatmalloc)");
        exit(1);
    }

    size_t tamanhoPalavra = strlen(*palavra);
    char* temp = realloc(*palavra, tamanhoPalavra + 2);
    if (temp == NULL) {
        perror("falha ao alocar memoria (charcatmalloc)");
        exit(1);
    }

    *palavra = temp;
    (*palavra)[tamanhoPalavra] = caractere;
    (*palavra)[tamanhoPalavra + 1] = '\0';

    if (strlen(*palavra) != tamanhoPalavra + 1) {
        perror("erro de tamanho (charcatmalloc)");
        exit(1);
    }

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
    int gravando = 0;
    int gravarValor = 0;
    NIXJSON* result = NixJson_create();
    result->stringJson = strdup(nixJson->stringJson);
    int stringSize = removeWhitespace(result->stringJson);

    // printf("\njson: %s\n", nixJson->stringJson);

    for(int i = 0; i < stringSize; i++){
        char caractere = result->stringJson[i];

        if(caractere == '{'){
            if(i == 0) continue;
            chaves.itensAbertos++;
        }

        if(caractere == '}'){
            if(i == (stringSize-1)){
                adicionarItem(result->dicionario, chave, valor);
                limparPalavra(&chave);
                limparPalavra(&valor);
                continue;
            };
            chaves.itensFechado++;
        }

        if(caractere == '['){
            colchetes.itensAbertos++;
            if(!gravando){
                gravando = 1;
            }
        }

        if(caractere == ']'){
            colchetes.itensFechado++;
        }

        if(caractere == '"'){
            if(gravando){
                if(colchetes.itensAbertos == colchetes.itensFechado){
                    gravando = 0;
                    continue;
                }
            } else {
                gravando = 1;
                continue;
            }
        }

        if(caractere == ':'){
            if(!gravarValor){
                gravarValor = 1;
            }
        }

        if(caractere == ','){
            if(gravarValor){
                if(colchetes.itensAbertos == colchetes.itensFechado){
                    gravarValor = 0;
                    gravando = 0;
                    adicionarItem(result->dicionario, chave, valor);
                    limparPalavra(&chave);
                    limparPalavra(&valor);
                }
            } else {
                continue;
            }
        }
        if(gravando){
            if(gravarValor){
                charcatmalloc(caractere, &valor);
            } else{
                charcatmalloc(caractere, &chave);
            }
        }


    }
    return result;
}

int NixJson_GetArraySize(char* stringJson){
    NIXJSON* array = NixJson_GetArrayItem(stringJson, -2);
    int result = array->dicionario->qtdItens;
    NixJson_free(array);
    return result;
}


NIXJSON* NixJson_GetArrayItem(char* stringJson, int index){
    char* chave = NULL;
    char* valor = NULL;
    limparPalavra(&chave);
    limparPalavra(&valor);
    int gravandoValor = 0;
    int gravando = 0;
    int contador = 48;
    int aspas = 0;
    CONJUNTO colchetes;
    colchetes.itensAbertos = 0;
    colchetes.itensFechado = 0;
    CONJUNTO chaves;
    chaves.itensAbertos = 0;
    chaves.itensFechado = 0;
    NIXJSON* nixJson = NixJson_create();
    nixJson->stringJson = strdup(stringJson);
    if (!nixJson->stringJson) {
        perror("Erro ao duplicar stringJson");
        exit(1);
    }
    int stringSize = removeWhitespace(nixJson->stringJson);

    if(stringJson[0] != '[' && stringJson[stringSize] != ']'){
        fprintf(stderr, "\033[1;31m"); 
        perror("O item não é um array. Verifique se o valor inicia e termina com []. Caso utilize {}, use a função NixJson_parseJson.");
        fprintf(stderr, "\033[0m"); 
        exit(1);
    }

    for(int i = 0; i < stringSize; i++){
        char caractere = nixJson->stringJson[i];
        // tipo 1 ["teste", "teste", "teste"]
        if(nixJson->stringJson[1] != '{'){
            if(caractere == '['){
                colchetes.itensAbertos++;
            }
            if(caractere == '"'){
                aspas++;
                if(aspas%2 != 0){
                    gravando = 1;
                    contador++;
                    charcatmalloc(contador, &chave);
                    gravandoValor = 1;
                    continue;
                } else {
                    gravando = 0;
                    gravandoValor = 0;
                    adicionarItem(nixJson->dicionario, chave, valor);
                    limparPalavra(&chave);
                    limparPalavra(&valor);
                    continue;
                }
            }
            if(gravando){
                if(gravandoValor){
                    charcatmalloc(caractere, &valor);
                }
            }
            continue;
        }
        //////
        // tipo 2 [{},{},{}]
        // printf("caractare: %c    gravando = %d\n",caractere, gravando );
        if(caractere == '{'){
                if(i==0 || (chaves.itensAbertos == chaves.itensFechado) && gravando == 0){
                    gravando = 1;
                    contador++;
                    charcatmalloc(contador, &chave);
                    gravandoValor = 1;
                }
                chaves.itensAbertos++;
            // if( (i==0 || nixJson->stringJson[i-1] == '[' || (nixJson->stringJson[i-1] == ',' && nixJson->stringJson[i-2] == '}')) && gravandoValor == 0){
            //     gravando = 1;
            //     contador++;
            //     charcatmalloc(contador, &chave);
            //     gravandoValor = 1;
            // }
        }

        if(caractere == ','){
            if((nixJson->stringJson[i-1] == '}' && nixJson->stringJson[i+1] == '{') && chaves.itensAbertos == chaves.itensFechado){
                gravando = 0;
                // printf("Adicionado: chave: (%s) valor: (%s)", chave, valor);
                // adicionarItem(nixJson->dicionario, chave, valor);
                // limparPalavra(&chave);
                // limparPalavra(&valor);
                gravandoValor = 0;
                continue;
            }
        }

        if(caractere == '}'){
            chaves.itensFechado++;
            if((nixJson->stringJson[i+1] == ']' || nixJson->stringJson[i+1] == ',') && chaves.itensAbertos == chaves.itensFechado){
                gravando = 0;
                charcatmalloc(caractere, &valor);
                adicionarItem(nixJson->dicionario, chave, valor);
                limparPalavra(&chave);
                limparPalavra(&valor);
                gravandoValor = 0;
                contador++;
                continue;
            }
        }

        if(gravando){
            if(gravandoValor){
                charcatmalloc(caractere, &valor);
            }
        }
    }
    // printf("qtdItens: %d\n", nixJson->dicionario->qtdItens);
    if(index >= nixJson->dicionario->qtdItens && index != -2){
        fprintf(stderr, "\033[1;31m"); 
        perror("index Não encontrado (NixJson_GetArrayItem)");
        fprintf(stderr, "\033[0m"); 
        exit(1);
    }
    // printf("palavra: %s\n", nixJson->dicionario->item[index].valor);
    NIXJSON* result = NixJson_create();
    if(index == -2){
        result->dicionario->qtdItens = nixJson->dicionario->qtdItens;
    } else {
        result->stringJson = strdup(nixJson->dicionario->item[index].valor);
        // printf("resolvendo: %s\n", result->stringJson);
    }
    NixJson_free(nixJson);
    if(index == -2){
        return result;
    }
    return NixJson_parseJson(result);
    // return result;
    // resultado->stringJson = strdup(nixJson->dicionario->item[index].valor);
    // NixJson_free(nixJson);

}

char* NixJson_GetObjectItem(NIXJSON* nixJson, char * chave){
    int indice = buscarItem(nixJson->dicionario, chave);
    int stringSize = strlen(nixJson->dicionario->item[indice].valor);
    char* result = malloc(stringSize + 1);
    strncpy(result, nixJson->dicionario->item[indice].valor, stringSize);
    result[stringSize] = '\0';
    return result;
}

void NixJson_free(NIXJSON* nixJson) {
    if (!nixJson) return;

    if (nixJson->dicionario) {
        if (nixJson->dicionario->item) {
            for (int i = 0; i < nixJson->dicionario->qtdItens; i++) {
                free(nixJson->dicionario->item[i].chave);
                free(nixJson->dicionario->item[i].valor);
            }
            free(nixJson->dicionario->item);
        }
        free(nixJson->dicionario);
    }

    free(nixJson->stringJson);
    free(nixJson);
}

void NixJson_version(){
    printf("Verção do NixJson: V1.1.1");
}



