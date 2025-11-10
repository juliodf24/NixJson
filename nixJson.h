#ifndef _NIXJSON_H_
#define _NIXJSON_H_

#include "dicionario.h"


//Estrutura principal da biblioteca NixJSON
//Representa um objeto JSON carregado em memória.
typedef struct NixJson {
    DICIONARIO* dicionario;   
    char* stringJson;      
} NIXJSON;


//Cria uma nova instância de NixJSON.
NIXJSON* NixJson_create();

//lê um arquivo json
void NixJson_readJson(NIXJSON* nixJson,char* path);

//Lê e interpreta um arquivo JSON.
NIXJSON* NixJson_parseJson(NIXJSON* nixJson);

// pega um item em uma array
NIXJSON* NixJson_GetItemArray(char* stringJson, int index);

// // pega o valor de uma chave 
char* NixJson_GetObjectItem(NIXJSON* nixJson, char * chave);

// retorna a quantidade de itens na array
NIXJSON* NixJson_GetArraySize(NIXJSON* nixJson);

//Libera toda a memória associada.
void NixJson_free(NIXJSON* nixJson);


/* -------- feramentas --------- */
int charcatmalloc(char caractere, char** palavra);



typedef struct conjunto{
    int itensAbertos;
    int itensFechado;
}CONJUNTO;


#endif
