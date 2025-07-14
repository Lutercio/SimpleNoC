# Makefile for NoC Simulation with SystemC

# SystemC installation path
SYSTEMC_HOME = C:/SystemC/install

# Compiler and flags
CXX = C:/msys64/mingw64/bin/g++.exe
CXXFLAGS = -std=c++17 -Wall -Wextra -I$(SYSTEMC_HOME)/include
LDFLAGS = -L$(SYSTEMC_HOME)/lib -lsystemc -lpthread

# Source and build directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Target executable
TARGET = $(BIN_DIR)/noc_simulation

.PHONY: all clean run test analyze

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

# Execute automated test suite
test: all
	@echo Executando suite de testes automatizados...
	scripts\run_tests.bat

# Execute extended test suite (20 tests)
test-extended: all
	@echo Executando suite estendida de testes (20 cenarios)...
	scripts\run_extended_tests.bat

# Execute comprehensive test suite (50 tests, 4x4 to 16x16)
test-50: all
	@echo Executando 50 testes (malhas 4x4 ate 16x16)...
	scripts\run_tests_50.bat

# Analyze test results (requires Python)
analyze:
	@echo Analisando resultados dos testes...
	@C:\msys64\mingw64\bin\python.exe scripts\analyze_results.py

# Analyze specific timestamp
analyze-timestamp:
	@echo Analisando resultados com timestamp especifico...
	@C:\msys64\mingw64\bin\python.exe scripts\analyze_results.py $(TIMESTAMP)