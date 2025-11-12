#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dicionario.h"

void adicionarItem(DICIONARIO* dicionario, const char* chave, const char* valor) {
    size_t novaQtdItens = dicionario->qtdItens + 1;
    dicionario->item = realloc(dicionario->item, novaQtdItens * sizeof(*dicionario->item));
    if (!dicionario->item) {
        perror("Erro ao alocar memória");
        exit(1);
    }
    dicionario->qtdItens = novaQtdItens;
    dicionario->item[dicionario->qtdItens - 1].chave = strdup(chave);
    dicionario->item[dicionario->qtdItens - 1].valor = strdup(valor);
    printf("\nValor Adicionado: Chave: %s, Valor: %s\n", dicionario->item[dicionario->qtdItens - 1].chave, dicionario->item[dicionario->qtdItens - 1].valor);
}

int buscarItem(DICIONARIO* dicionario,char* chave){
    for(int i = 0; i< dicionario->qtdItens; i++){
        if(strcmp(dicionario->item[i].chave, chave) == 0){
            // printf("\nencontrada: %s\n", dicionario->item[i].valor);
            return i;
        }
    }
    perror("valor não encontrado no dicionario (buscarItem Dicionario) ");
    exit(1);
}
