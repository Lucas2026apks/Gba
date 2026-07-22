# Rutas oficiales dentro del contenedor Docker de devkitPro
export DEVKITPRO ?= /opt/devkitpro
export DEVKITARM ?= $(DEVKITPRO)/devkitARM

# Añadir las herramientas al PATH para que make las encuentre de inmediato
export PATH := $(DEVKITARM)/bin:$(PATH)

PREFIX  := arm-none-eabi-
CXX     := $(PREFIX)g++
OBJCOPY := $(PREFIX)objcopy

# Configuraciones de arquitectura para GBA
ARCH    := -mthumb -mthumb-interwork
CFLAGS  := $(ARCH) -O2 -Wall
CXXFLAGS:= $(CFLAGS) -fno-rtti -fno-exceptions
LDFLAGS := $(ARCH) -specs=gba.specs

# Archivos de entrada y salida
TARGET  := juego
SOURCES := src/main.cpp

# Reglas de compilación
all: $(TARGET).gba

$(TARGET).elf: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SOURCES) -o $@

$(TARGET).gba: $(TARGET).elf
	$(OBJCOPY) -v -O binary $< $@
