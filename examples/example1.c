#include <stdio.h>
#include "nixJson.h"

int main(void){
    printf("\nInicializado\n");

    NIXJSON* json = NixJson_create();
    NixJson_readJson(json, "teste.json");

    NIXJSON* primeiraCamada =  NixJson_parseJson(json);
    NixJson_free(json);

    char* perguntas = NixJson_GetObjectItem(primeiraCamada, "perguntas");

    int tamanhoArray = NixJson_GetArraySize(perguntas);
    printf("tamanho array: %d\n", tamanhoArray);

    NIXJSON* segundaCamada = NixJson_GetArrayItem(perguntas, 0);
    NixJson_free(primeiraCamada);

    char* alternativas = NixJson_GetObjectItem(segundaCamada, "alternativa");

    int tamanhoArray2 = NixJson_GetArraySize(alternativas);
    printf("string array: %s\n", alternativas);
    printf("tamanho array: %d\n", tamanhoArray2);

    NIXJSON* alternativa = NixJson_GetArrayItem(alternativas, 0);

    // printf("Resposta: %s\n",  );
    return 0;
}