#ifndef DICIONARIO_H
#define DICIONARIO_H

struct lista{
    char* chave;
    char* valor;
};

struct dicionario
{
    struct lista* item;
    int qtdItens;
};
typedef struct dicionario DICIONARIO;

void adicionarItem(DICIONARIO* dicionario, const char* chave, const char* valor);

int buscarItem(DICIONARIO* dicionario,char* chave);

#endif