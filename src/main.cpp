// Definición de los registros básicos de video de la GBA
// Añadimos paréntesis exteriores para evitar errores matemáticos
#define REG_DISPCNT (*(volatile unsigned short*)0x04000000)
#define MODE_3 3
#define BG2_ENABLE (1 << 10)

// Puntero a la memoria de video de la pantalla (VRAM)
// ¡Aquí estaban los paréntesis que faltaban!
#define VRAM ((volatile unsigned short*)0x06000000)

// Función para convertir colores RGB a formato de 15 bits de GBA
constexpr unsigned short RGB15(unsigned int r, unsigned int g, unsigned int b) {
    return (r & 0x1F) | ((g & 0x1F) << 5) | ((b & 0x1F) << 10);
}

int main() {
    // Configurar la GBA en el Modo de Video 3 y activar el fondo 2
    REG_DISPCNT = MODE_3 | BG2_ENABLE;

    // Pintar toda la pantalla de la GBA de azul
    for (int i = 0; i < 240 * 160; i++) {
        VRAM[i] = RGB15(0, 15, 31);
    }

    // Bucle infinito para que el juego no se cierre
    while (1) {
    }

    return 0;
}
