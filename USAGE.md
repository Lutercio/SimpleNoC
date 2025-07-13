# Instruções de Uso

## Como evitar que arquivos desnecessários voltem:

1. **O projeto agora está sob controle do Git** - isso garante que apenas os arquivos essenciais sejam mantidos.

2. **Estrutura final do projeto:**
   ```
   ChipEmRede/
   ├── .gitignore         # Ignora arquivos desnecessários
   ├── Makefile           # Sistema de build
   ├── README.md          # Documentação
   ├── trab2.txt         # Especificação do trabalho
   └── src/              # Código-fonte essencial
   ```

3. **Para compilar:**
   ```bash
   mingw32-make
   ```

4. **Para executar:**
   ```bash
   .\bin\noc_simulation.exe [opções]
   ```

5. **Se arquivos indesejados aparecerem novamente:**
   ```bash
   git status           # Ver o que mudou
   git checkout .       # Restaurar estado limpo
   git clean -fd        # Remover arquivos não versionados
   ```

## Arquivos que foram removidos permanentemente:
- ✅ Scripts .bat desnecessários (9 arquivos)
- ✅ CMakeLists.txt (usamos Makefile)
- ✅ SETUP.md (estava vazio)
- ✅ test_compiler.cpp (arquivo de teste)
- ✅ node.cpp e router.cpp (estavam vazios)
- ✅ Pastas build/ e bin/ (temporárias)
- ✅ Configurações .vscode/ (específicas do usuário)

O projeto agora está limpo e organizado! 🚀
