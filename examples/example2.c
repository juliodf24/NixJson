#include <stdio.h>
#include <stdlib.h>
#include "nixJson.h"

int main(void){
    printf("\nInicializado\n");

    NIXJSON* json = NixJson_create();
    NixJson_readJson(json, "data.json");

    NIXJSON* primeiraCamada =  NixJson_parseJson(json);
    NixJson_free(json);

    char* perguntas = NixJson_GetObjectItem(primeiraCamada, "niveis");

    int tamanhoArray = NixJson_GetArraySize(perguntas);
    printf("tamanho array: %d\n", tamanhoArray);

    NIXJSON* segundaCamada = NixJson_GetArrayItem(perguntas, 1);
    NixJson_free(primeiraCamada);
    printf("resultado: %s\n", segundaCamada->stringJson);


    return 0;
}