#include <stdio.h>
#include "nixJson.h"

int main(void){
    printf("\nInicializado\n");

    NIXJSON* json = NixJson_create();
    NixJson_readJson(json, "teste.json");
    NIXJSON* primeiraCamada =  NixJson_parseJson(json);
    
    return 0;
}