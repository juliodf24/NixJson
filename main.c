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

// int ehJson(char* json, int tamanhoJson){
//     if(json[0] == '{' && json[tamanhoJson -1] == '}'){
//         return 1; // verdadeiro
//     }
//     return 0; // falso
// }


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

DICIONARIO* ObterItemArray(char* stringJson, int numeroItem){
    DICIONARIO* dicionario = malloc(sizeof(DICIONARIO));
    if (!dicionario) {
        perror("Erro ao alocar memória para o dicionário");
        exit(1);
    }
    dicionario->qtdItens = 0;
    dicionario->item = NULL; 
    char* chave = NULL;
    char* valor = NULL;
    limparPalavra(&chave);
    limparPalavra(&valor);
    int gravandoValor = 0;
    int gravando = 0;
    int contador = 48;
    int qtdItens = 0;

    int tamanhoString = strlen(stringJson);
    printf("tamanho string: %d\n", tamanhoString);

    for(int i = 0; i < tamanhoString; i++){
        char caractere = stringJson[i];

        if(caractere == '{'){
            if( (i==0 || stringJson[i-1] == '[' || (stringJson[i-1] == ',' && stringJson[i-2] == '}')) && gravandoValor == 0){
                gravando = 1;
                contador++;
                adicionarCaractereNaPalavra(contador, &chave);
                gravandoValor = 1;
            }
        }
        if(caractere == ','){
            if(stringJson[i-1] == '}' && stringJson[i+1] == '{'){
                gravando = 0;
                qtdItens++;
                adicionarItem(dicionario, chave, valor);
                limparPalavra(&chave);
                limparPalavra(&valor);
                gravandoValor = 0;
                continue;
            }
        }
        if(caractere == '}'){
            if(stringJson[i+1] == ']'){
                gravando = 0;
                qtdItens++;
                adicionarCaractereNaPalavra(caractere, &valor);
                adicionarItem(dicionario, chave, valor);
                limparPalavra(&chave);
                limparPalavra(&valor);
                gravandoValor = 0;
                contador++;
                continue;
            }
        }

        if(gravando){
            if(gravandoValor){
                adicionarCaractereNaPalavra(caractere, &valor);
            }
        }


    }
    dicionario->qtdItens = qtdItens;
    printf("qtd itens: %d\n\n", dicionario->qtdItens);
    for(int i = 0; i < dicionario->qtdItens ; i++){
        printf("Chave: - %s - \nvalor: - %s -\n\n", dicionario->item[i].chave, dicionario->item[i].valor );
    }

}


void parseJson(DICIONARIO* dicionario, char* stringJson){
    int tamanhoStringJson = removerEspacosEmBrancoIgnorandoConteudoAspas(stringJson);
    int qtdChaves = 0;
    int qtdAspas = 2;
    int qtdColchete = 2;
    int qtdColcheteAberto = 0;
    int qtdColcheteFechado = 0;
    int gravandoValor = 0;
    int gravando = 0;
    int modo = 0;
    char* chave = NULL;
    char* valor = NULL;
    limparPalavra(&chave);
    limparPalavra(&valor);
    printf("stringJson: %s\n\n", stringJson);

    for(int i = 0; i < tamanhoStringJson; i++){
        int caractere = stringJson[i];
        // printf("caractere observado: %c\n", caractere);
        
        if(caractere == '{') {
            qtdChaves++;
            if(qtdChaves == 1){
                continue;
            }
        }
        if(caractere == '}') {

            if(stringJson[i+1] == '\0' || stringJson[i+1] == '}'){
                adicionarItem(dicionario, chave, valor);
                limparPalavra(&chave);
                limparPalavra(&valor);
                continue;
            }
        };
        if(caractere == '['){
            qtdColcheteAberto++;
            if(!gravando){
                gravando = 1;
            }
            qtdColchete++;
        }
        if(caractere == ']'){
            qtdColcheteFechado++;
            qtdColchete++;
        }
        if(caractere == '"'){
            if(gravando){
                if(qtdColcheteAberto == qtdColcheteFechado){
                    gravando = 0;
                    continue;
                }
            } else {
                gravando = 1;
                continue;
            }
        }
        if(caractere == ':'){
            if(!gravandoValor){
                gravandoValor = 1;
            }
        }
        if(caractere == ','){
            if(gravandoValor){
                 if(qtdColcheteAberto == qtdColcheteFechado){
                    gravandoValor = 0;
                    gravando = 0;
                    adicionarItem(dicionario, chave, valor);
                    limparPalavra(&chave);
                    limparPalavra(&valor);
                }
            } else {
                continue;
            }
        }
        if(gravando){
            if(gravandoValor){
                adicionarCaractereNaPalavra(caractere, &valor);
            } else {
                adicionarCaractereNaPalavra(caractere, &chave);
            }
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

    // for(int i = 0; i< dicionario->qtdItens;i++){
    //     printf("Chave: - %s - \n valor: - %s -\n\n", dicionario->item[i].chave, dicionario->item[i].valor );
    // }

    ObterItemArray(dicionario->item[1].valor, 0);

    return 0;
}
