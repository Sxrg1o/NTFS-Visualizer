# Variables
PYTHON = python3
VENV = .venv/bin/python
CXX = g++
CXXFLAGS = -Wall -std=c++17 -fvisibility=default -Iinclude $(shell python3 -m pybind11 --includes)
TARGET = proyecto
SRC_DIR = src/core
INCLUDE_DIR = include
BINDINGS_DIR = bindings
GUI_DIR = src/gui
ASSETS_DIR = assets

# Archivos fuente
CORE_SRCS = $(wildcard $(SRC_DIR)/*.cpp)
READER_SRCS = $(wildcard $(SRC_DIR)/imageReader/*.cpp)
NTFS_SRCS = $(wildcard $(SRC_DIR)/ntfs/*.cpp)
SRCS = $(CORE_SRCS) $(READER_SRCS) $(NTFS_SRCS)

# Nombre del módulo de Python
MODULE_NAME = bindings

# Obtener el directorio actual
CURRENT_DIR = $(shell pwd)

# Objetivos principales
all: c-build run

# Compilar el código C++ y los bindings
c-build: $(BINDINGS_DIR)
	$(CXX) $(CXXFLAGS) -shared -fPIC $(SRCS) -o $(BINDINGS_DIR)/$(MODULE_NAME)$(shell python3-config --extension-suffix)

$(BINDINGS_DIR):
	mkdir -p $(BINDINGS_DIR)

# Ejecutar la aplicación Python
run:
	PYTHONPATH=$(CURRENT_DIR):$(CURRENT_DIR)/$(BINDINGS_DIR) $(VENV) $(GUI_DIR)/main_window.py

# Limpiar archivos generados
clean:
	rm -f $(TARGET)
	rm -rf **pycache**
	rm -rf $(BINDINGS_DIR)/__pycache__
	rm -f $(BINDINGS_DIR)/$(MODULE_NAME).so

# Crear el entorno virtual
venv:
	python3 -m venv .venv
	$(VENV) -m pip install --upgrade pip
	$(VENV) -m pip install -r requirements.txt

# Ayuda
help:
	@echo "Opciones disponibles:"
	@echo "  make all       - Compila y ejecuta el proyecto"
	@echo "  make c-build   - Compila el código C++"
	@echo "  make run       - Ejecuta la aplicación Python"
	@echo "  make clean     - Limpia archivos generados"
	@echo "  make venv      - Crea y configura el entorno virtual"
	@echo "  make help      - Muestra esta ayuda"

.PHONY: all c-build run clean venv help