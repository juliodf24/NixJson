#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct lista{
    char* chave;
    char* valor;
};


struct lista* lista = NULL;

int qtdItens = 0;

int adicionar(struct lista** lista, const char* chave, const char* valor) {
    qtdItens++;
    *lista = realloc(*lista, qtdItens * sizeof(struct lista));
    if (!*lista) {
        perror("Erro ao alocar memória");
        exit(1);
    }

    (*lista)[qtdItens - 1].chave = strdup(chave);
    (*lista)[qtdItens - 1].valor = strdup(valor);
    return 0;
}

void lerAquivoJson(char** json){
    FILE* f; 
    f = fopen("teste.json", "r");
    if(f == 0){
        printf("! Erro ao ler o Json. !\n\n");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long tamanhoJson = ftell(f);
    fseek(f,0, SEEK_SET);

    *json = malloc(tamanhoJson+1);
    if(!*json){
        printf("! Erro ao alocar memória para o JSON. !\n\n");
        fclose(f);
        exit(1);
    }

    fread(*json, sizeof(char), tamanhoJson, f);

    (*json)[tamanhoJson] = '\0';
    fclose(f);
}

int removerEspacos(char *str) {
    int qtdchar = 0;
    char *origem = str;
    char *destino = str;
    int contadorAspas = 2;

    while (*origem != '\0') {
        if(*origem == '"'){
            contadorAspas++;
        }
        if ((*origem != ' ' && *origem != '\n' && *origem != '\t' && *origem != '\r') || contadorAspas%2 != 0) {
            qtdchar++;
            *destino++ = *origem; 
        }
        origem++; 
    }
    *destino = '\0';
    return qtdchar;
}

int ehJson(char* json, int qtdChar){
    if(json[0] == '{' && json[qtdChar -1] == '}'){
        return 1; // verdadeiro
    }
    return 0; // falso
}

void parseJson(char* json,  int qtdChar){
    char digito;
    char* chave;
    char* valor;
    int qtdAspas =0;
    int esperandovalor = 0;
    for(int i = 0; i < qtdChar; i++){
        digito = json[i];
        switch(digito)
        {
            case '"':
                qtdAspas++;
                if(qtdAspas%4 == 0){
                    // salva chave e valor na lista
                    adicionar(&lista, "chaveAdicionado", "valorAdicionado");
                    break;
                }
                if (qtdAspas%2 == 0){
                    // para de gravar na chave e grava no valor
                    break;
                }
                // continua

                break;
            case ':':
                esperandovalor++;

                break;
            case ',':
                /* code */
                break;
            
            default:
                break;
        }
    }

}

int main() {
    char* json;
    char* destino;
    int qtdchar;
    lerAquivoJson(&json);

    qtdchar = removerEspacos(json);
    parseJson(json, qtdchar);
    
    printf("\njason: %d\n\n", ehJson(json, qtdchar));

    for(int i = 0; i<qtdItens;i++){
        printf("Item na lista: Chave: %s, valor: %s\n", lista[i].chave,lista[i].valor );
    }

    return 0;
}
