# Tasks API — REST em C++ com Crow + SQLite

API REST de tarefas (CRUD completo) escrita em **C++17**, usando o micro-framework **[Crow](https://crowcpp.org/)** para HTTP/JSON e a biblioteca **`libsqlite3`** (C API) para persistência.

## Funcionalidades

- CRUD completo de `tasks` (criar, listar, buscar, substituir, atualizar parcialmente, remover)
- Filtro (`done`) e paginação (`limit`/`offset`) na listagem
- Validação de entrada com respostas de erro em JSON
- *Prepared statements* em todas as consultas (sem SQL injection)
- Conexão SQLite com RAII (`sqlite3_stmt` sempre finalizado) e proteção por `std::mutex`, pronta para o modo `multithreaded()` do Crow
- Configuração por variáveis de ambiente

## Estrutura

```
.
├── CMakeLists.txt
├── README.md
├── src/
│   ├── main.cpp          # rotas HTTP, validação e serialização JSON
│   ├── database.hpp      # interface da camada de dados (Task, TaskPatch, Database)
│   └── database.cpp      # implementação com libsqlite3
└── third_party/
    └── crow/include/     # coloque aqui crow.h + pasta crow/ (se não usar vcpkg)
```

## Pré-requisitos

- Compilador com C++17 (GCC 9+, Clang 10+, MSVC 2019+)
- CMake ≥ 3.16
- SQLite3 (`libsqlite3`)
- Crow (≥ 1.0) e Asio standalone

### Windows — MSYS2 / UCRT64

No terminal **MSYS2 UCRT64**:

```bash
pacman -S --needed \
  mingw-w64-ucrt-x86_64-toolchain \
  mingw-w64-ucrt-x86_64-cmake \
  mingw-w64-ucrt-x86_64-ninja \
  mingw-w64-ucrt-x86_64-sqlite3 \
  mingw-w64-ucrt-x86_64-asio
```

Depois, baixe o Crow em <https://github.com/CrowCpp/Crow/releases> e copie o conteúdo da pasta `include/` (o arquivo `crow.h` **e** a pasta `crow/`) para `third_party/crow/include/`, de modo que exista `third_party/crow/include/crow.h`.

### Linux (Debian/Ubuntu)

```bash
sudo apt install build-essential cmake ninja-build libsqlite3-dev libasio-dev
git clone --depth 1 https://github.com/CrowCpp/Crow.git /tmp/Crow
cp -r /tmp/Crow/include/* third_party/crow/include/
```

### Alternativa: vcpkg

```bash
vcpkg install crow sqlite3
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake
```

O `CMakeLists.txt` tenta `find_package(Crow)` primeiro e só usa `third_party/` como alternativa.

## Compilar e executar

```bash
cmake -S . -B build -G Ninja
cmake --build build
./build/tasks_api          # no Windows: .\build\tasks_api.exe
```

A API sobe em `http://localhost:8080` e cria o arquivo `tasks.db` no diretório atual.

### Configuração

| Variável   | Padrão     | Descrição                      |
|------------|------------|--------------------------------|
| `PORT`     | `8080`     | Porta HTTP                     |
| `HOST`     | `0.0.0.0`  | Endereço de bind               |
| `DB_PATH`  | `tasks.db` | Caminho do arquivo SQLite      |

Exemplo: `PORT=3000 DB_PATH=/tmp/dev.db ./build/tasks_api`

## Modelo

| Campo         | Tipo    | Observações                                 |
|---------------|---------|---------------------------------------------|
| `id`          | inteiro | Gerado automaticamente                      |
| `title`       | string  | Obrigatório, 1–200 caracteres (sem espaços nas pontas) |
| `description` | string  | Opcional, até 2000 caracteres, padrão `""`  |
| `done`        | boolean | Opcional, padrão `false`                    |
| `created_at`  | string  | ISO-8601 em UTC, gerado pelo banco          |

## Endpoints

| Método   | Rota          | Descrição                              | Sucesso          |
|----------|---------------|----------------------------------------|------------------|
| `GET`    | `/health`     | Verificação de saúde                   | `200`            |
| `POST`   | `/tasks`      | Cria uma tarefa                        | `201` + `Location` |
| `GET`    | `/tasks`      | Lista tarefas (`done`, `limit`, `offset`) | `200`         |
| `GET`    | `/tasks/{id}` | Busca uma tarefa                       | `200`            |
| `PUT`    | `/tasks/{id}` | Substitui (exige `title`; omitidos voltam ao padrão) | `200` |
| `PATCH`  | `/tasks/{id}` | Atualiza só os campos enviados         | `200`            |
| `DELETE` | `/tasks/{id}` | Remove                                 | `204`            |

Parâmetros de `GET /tasks`: `done=true|false`, `limit` (1–200, padrão 50), `offset` (≥ 0, padrão 0).

### Erros

Sempre em JSON: `{"error": "mensagem"}`.

| Código | Quando                                                          |
|--------|-----------------------------------------------------------------|
| `400`  | JSON inválido, campo com tipo/tamanho inválido, parâmetro inválido |
| `404`  | Tarefa ou rota inexistente                                      |
| `500`  | Erro interno (ex.: falha do SQLite)                             |

## Exemplos (curl)

> No PowerShell use `curl.exe` (e não o alias `curl`) e escape as aspas do JSON, ou use o terminal MSYS2.

```bash
# Criar
curl -i -X POST localhost:8080/tasks \
  -H "Content-Type: application/json" \
  -d '{"title":"Estudar Crow","description":"Fase 6 do roadmap"}'

# Listar (apenas concluídas, 10 por página)
curl "localhost:8080/tasks?done=true&limit=10&offset=0"

# Buscar
curl localhost:8080/tasks/1

# Marcar como concluída (parcial)
curl -X PATCH localhost:8080/tasks/1 \
  -H "Content-Type: application/json" -d '{"done":true}'

# Substituir
curl -X PUT localhost:8080/tasks/1 \
  -H "Content-Type: application/json" \
  -d '{"title":"Novo título","description":"Nova descrição","done":false}'

# Remover
curl -i -X DELETE localhost:8080/tasks/1
```

Resposta de `POST /tasks`:

```json
{
  "id": 1,
  "title": "Estudar Crow",
  "description": "Fase 6 do roadmap",
  "done": false,
  "created_at": "2026-11-09T22:13:00Z"
}
```

## Decisões de projeto

- **Camada de dados separada** (`Database`): `main.cpp` não conhece SQL, o que facilita testar e trocar o armazenamento.
- **Uma conexão + mutex**: simples e correta para SQLite. O banco usa `WAL` e `busy_timeout` para conviver bem com leituras concorrentes.
- **`PUT` vs `PATCH`**: `PUT` é substituição completa; `PATCH` altera apenas o que foi enviado. Ambos usam o mesmo parser/validador (`TaskPatch`).
- **Uma rota por caminho**: o Crow trata os métodos de cada rota num único handler (`req.method`), evitando conflito de rotas duplicadas.

## Problemas comuns

| Sintoma | Solução |
|---|---|
| `asio.hpp não encontrado` | Instale o Asio (`pacman -S mingw-w64-ucrt-x86_64-asio` / `apt install libasio-dev`) |
| `Crow não encontrado` | Confirme que `third_party/crow/include/crow.h` existe ou instale via vcpkg |
| `undefined reference to WSAStartup` / `ws2_32` | Já tratado no `CMakeLists.txt`; use CMake, não compile o `.cpp` manualmente |
| `sqlite3.h` não encontrado | Instale o pacote de desenvolvimento do SQLite (`...-sqlite3` / `libsqlite3-dev`) |
| IntelliSense do VSCode sem achar `crow.h` | O CMake gera `build/compile_commands.json`; no VSCode, defina `"C_Cpp.default.compileCommands": "${workspaceFolder}/build/compile_commands.json"` (ou use a extensão CMake Tools) |
| Porta em uso | Defina outra com `PORT=3000` |

## Próximos passos

- Autenticação (JWT / API key) via *middleware* do Crow
- CORS e rate limiting
- Testes automatizados (Catch2/GoogleTest) para a camada `Database`
- Migrações versionadas do schema
- Dockerfile e GitHub Actions para CI
