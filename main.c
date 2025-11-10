#include <stdio.h>
#include "nixJson.h"

int main(void){
    printf("\nInicializado\n");

    NIXJSON* json = NixJson_create();
    NixJson_readJson(json, "teste.json");
    NIXJSON* primeiraCamada =  NixJson_parseJson(json);
    NixJson_free(json);

    NIXJSON* segundaCamada = NixJson_GetItemArray(NixJson_GetObjectItem(primeiraCamada, "perguntas"), 0);
    NixJson_free(primeiraCamada);

    printf("Resposta: %s\n",NixJson_GetObjectItem(segundaCamada, "dica"));
    return 0;
}