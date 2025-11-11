![Logo do NixJSON](./img/nixJson.png)
---
**NixJSON** é uma biblioteca minimalista em **C** para leitura e parsing de arquivos **JSON**, projetada para ser leve, rápida e fácil de integrar.
Apenas para fins didaticos. Não aborda todos os tipos de json.
---

## 🚀 Características
- Leitura e parsing de JSON puro em C
- Estrutura de dados simples e eficiente
- Código limpo e bem documentado
- Suporte a valores aninhados e arrays
---

## 📦 Instalação
Clone o repositório e compile os arquivos `.c` normalmente:

```bash
git clone https://github.com/seuusuario/nixjson.git
cd nixjson
gcc -o exemplo exemplo.c nixjson.c
```

---

## 🧩 Exemplo de uso

```c
#include "nixjson.h"

int main() {
    JSON *data = json_read("dados.json");

    if (data) {
        const char *nome = json_get_string(data, "usuario.nome");
        printf("Nome: %s\n", nome);

        json_free(data);
    }

    return 0;
}
```

---

## 🛠 Estrutura do Projeto

```
📂 nixjson/
 ├── nixjson.c
 ├── nixjson.h
 ├── exemplo.c
 ├── README.md
 └── LICENSE
```

---

## ⚖️ Licença

Este projeto é licenciado sob a licença **MIT** — veja o arquivo [LICENSE](LICENSE) para mais detalhes.

---

## 💡 Sobre

Criado por **Júlio César** — inspirado em bibliotecas minimalistas e de alta performance para C.

> *NixJSON: JSON parsing no estilo C — simples, direto e eficiente.*
