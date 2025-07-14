# Makefile para Simulação NoC com SystemC

# Caminho de instalação do SystemC
SYSTEMC_HOME = C:/SystemC/install

# Compilador e flags
CXX = C:/msys64/mingw64/bin/g++.exe
CXXFLAGS = -std=c++17 -Wall -Wextra -I$(SYSTEMC_HOME)/include
LDFLAGS = -L$(SYSTEMC_HOME)/lib -lsystemc -lpthread

# Diretórios fonte e build
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Arquivos fonte
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Executável alvo
TARGET = $(BIN_DIR)/noc_simulation

.PHONY: all clean run test test-extended test-50 analyze analyze-timestamp graphs install-deps clean-graphs quick-analysis

all: dirs $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

dirs:
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	if not exist test_outputs mkdir test_outputs

clean:
	if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

run: all
	$(TARGET)

# Executar suite de testes automatizados
test: all
	@echo Executando suite de testes automatizados...
	scripts\run_tests.bat

# Executar suite estendida de testes (20 testes)
test-extended: all
	@echo Executando suite estendida de testes (20 cenarios)...
	scripts\run_extended_tests.bat

# Executar suite abrangente de testes (50 testes, 4x4 a 16x16)
test-50: all
	@echo Executando 50 testes (malhas 4x4 ate 16x16)...
	scripts\run_tests_50.bat

# Instalar dependências Python
install-deps:
	@echo Instalando dependencias Python...
	@C:\msys64\mingw64\bin\python.exe -m pip install -r requirements.txt

# Analisar resultados de testes e gerar gráficos (requer Python)
analyze:
	@echo Analisando resultados dos testes e gerando graficos...
	@C:\msys64\mingw64\bin\python.exe scripts\analyze_results.py

# Analisar timestamp específico
analyze-timestamp:
	@echo Analisando resultados com timestamp especifico...
	@C:\msys64\mingw64\bin\python.exe scripts\analyze_results.py $(TIMESTAMP)

# Gerar apenas gráficos (pular análise)
graphs:
	@echo Gerando todos os graficos dos resultados...
	@C:\msys64\mingw64\bin\python.exe scripts\generate_all_graphs.py

# Gerar apenas gráficos PNG
graphs-png:
	@echo Gerando graficos PNG...
	@C:\msys64\mingw64\bin\python.exe scripts\generate_png_graphs.py

# Gerar apenas gráficos HTML interativos
graphs-html:
	@echo Gerando graficos HTML interativos...
	@C:\msys64\mingw64\bin\python.exe scripts\generate_html_charts.py

# Gerar gráficos de comparação direta
graphs-compare:
	@echo Gerando graficos de comparacao direta...
	@C:\msys64\mingw64\bin\python.exe scripts\generate_direct_comparison.py

# Análise rápida com script batch
quick-analysis:
	@echo Executando analise rapida com graficos...
	scripts\run_analysis.bat

# Limpar diretório de gráficos
clean-graphs:
	@echo Limpando arquivos obsoletos do diretorio graphs...
	@C:\msys64\mingw64\bin\python.exe scripts\clean_graphs.py

# Limpar tudo (build + gráficos)
clean-all: clean clean-graphs
	@echo Limpeza completa concluida!
clean-graphs:
	@echo Limpando diretorio de graficos...
	@if exist graphs rmdir /s /q graphs