#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dicionario.h"


void lerAquivoParaString(char** arquivo, char* path){
    FILE* f = fopen(path, "r");
    if(f == 0){
        perror("Erro ao ler o arquivo.");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long tamanhoArquivo = ftell(f);
    fseek(f,0, SEEK_SET);

    *arquivo = malloc(tamanhoArquivo+1);
    if(!*arquivo){
        perror("Erro ao alocar memória para o arquivo.");
        fclose(f);
        exit(1);
    }

    fread(*arquivo, sizeof(char), tamanhoArquivo, f);

    (*arquivo)[tamanhoArquivo] = '\0';
    fclose(f);
}

int ehCaractereBranco(char caractere){
    if(caractere == ' ' || caractere == '\n' || caractere == '\t' || caractere == '\r'){
        return 1;
    }
    return 0;
}

int oConjuntoEstaFechado(int qtdDoCaractereAberto){
    if(qtdDoCaractereAberto%2 == 0) return 1;
    return 0;
}

int removerEspacosEmBrancoIgnorandoConteudoAspas(char *str) {
    int tamanhoString = 0;
    char *origem = str;
    char *destino = str;
    int qtdAspas = 2;

    while (*origem != '\0') {
        if(*origem == '"') qtdAspas++;
        if (!ehCaractereBranco(*origem) || !oConjuntoEstaFechado(qtdAspas)) {
            tamanhoString++;
            *destino++ = *origem; 
        }
        origem++; 
    }
    *destino = '\0';
    return tamanhoString;
}

int ehJson(char* json, int tamanhoJson){
    if(json[0] == '{' && json[tamanhoJson -1] == '}'){
        return 1; // verdadeiro
    }
    return 0; // falso
}


void adicionarCaractereNaPalavra(char caractere, char** palavra) {
    size_t tamanho = strlen(*palavra);
    *palavra = realloc(*palavra, tamanho + 2); 
    (*palavra)[tamanho] = caractere;
    (*palavra)[tamanho + 1] = '\0';
}


void limparPalavra(char** palavra) {
    if (*palavra) free(*palavra); 
    *palavra = malloc(1);
    (*palavra)[0] = '\0';
}



void parseJson(DICIONARIO* dicionario, char* stringJson){
    int tamanhoStringJson = removerEspacosEmBrancoIgnorandoConteudoAspas(stringJson);
    int qtdAspas = 2;
    int qtdColchete = 2;
    int gravarValor = 0;
    int modo = 0;
    char* chave = NULL;
    char* valor = NULL;
    limparPalavra(&chave);
    limparPalavra(&valor);
    printf("stringJson: %s\n\n", stringJson);

    for(int i = 0; i < tamanhoStringJson; i++){
        int caractere = stringJson[i];
        // printf("caractere observado: %c\n", caractere);
        switch(caractere)
        {
            case '"':
                qtdAspas++;
                break;
            case '[':
                qtdColchete++;
                goto gravarCaractere;
                break;
            case ']':
                qtdColchete++;
                goto gravarCaractere;
                break;
            case ':':
                gravarValor = 1;

                if(i+1 < tamanhoStringJson){
                    // printf("\n\n %d  casa, valor: %c", i+1, stringJson[i+1]);
                    if(stringJson[i+1] == '"') modo = 0;
                    if(stringJson[i+1] == '[') {
                        printf("trocou modo para 1");
                        modo = 1;
                    }
                }

                break;
            case ',':
                if(modo == 0){
                    gravarValor = 0;
                    adicionarItem(dicionario, chave, valor);
                    limparPalavra(&chave);
                    limparPalavra(&valor);
                } else {
                    goto gravarCaractere;
                }
                break;
            case '}':
                adicionarItem(dicionario, chave, valor);
                limparPalavra(&chave);
                limparPalavra(&valor);
                break;
            gravarCaractere:
            default:
                // if(modo == 0) {
                //     if(oConjuntoEstaFechado(qtdAspas)) break;
                // }
                if(gravarValor == 0){
                    adicionarCaractereNaPalavra(caractere, &chave);
                    break;
                }
                if(gravarValor == 1){
                    adicionarCaractereNaPalavra(caractere, &valor);
                    break;
                }
                break;
        }
    }

}

int main() {
    DICIONARIO* dicionario = malloc(sizeof(DICIONARIO));
    if (!dicionario) {
        perror("Erro ao alocar memória para o dicionário");
        exit(1);
    }
    dicionario->qtdItens = 0;
    dicionario->item = NULL; 
    char* stringjson;

    lerAquivoParaString(&stringjson, "teste.json");
    printf("string Json: %s\n\n", stringjson);
    parseJson(dicionario, stringjson);

    for(int i = 0; i< dicionario->qtdItens;i++){
        printf("Chave: %s\n valor: %s\n\n", dicionario->item[i].chave, dicionario->item[i].valor );
    }

    return 0;
}
