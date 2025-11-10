#include <stdio.h>
#include "nixJson.h"

int main(void){
    printf("\nInicializado\n");

    NIXJSON* json = NixJson_create();
    NixJson_readJson(json, "teste.json");
    NIXJSON* primeiraCamada =  NixJson_parseJson(json);
    for(int i = 0; i < primeiraCamada->dicionario->qtdItens;i++){
        printf("Chave: %s   Valor: %s\n", primeiraCamada->dicionario->item[i].chave, primeiraCamada->dicionario->item[i].valor);
    }

    return 0;
}