// Definición de registros de video (igual que antes, con paréntesis de seguridad)
#define REG_DISPCNT (*(volatile unsigned short*)0x04000000)
#define MODE_3 3
#define BG2_ENABLE (1 << 10)
#define VRAM ((volatile unsigned short*)0x06000000)

// --- REGISTROS DE SINCRONIZACIÓN (VBLANK) ---
// Este registro cuenta qué línea de la pantalla está dibujando la GBA
#define REG_VCOUNT (*(volatile unsigned short*)0x04000006)

// --- REGISTROS DE ENTRADA (BOTONES) ---
// Este registro contiene el estado de todos los botones
#define REG_KEYINPUT (*(volatile unsigned short*)0x04000130)

// Definiciones de botones (los bits están invertidos, 0 = presionado)
#define KEY_RIGHT (1 << 4)
#define KEY_LEFT  (1 << 5)
#define KEY_UP    (1 << 6)
#define KEY_DOWN  (1 << 7)

// Función auxiliar para colores
constexpr unsigned short RGB15(unsigned int r, unsigned int g, unsigned int b) {
    return (r & 0x1F) | ((g & 0x1F) << 5) | ((b & 0x1F) << 10);
}

// Función para sincronizar con la tasa de refresco de la pantalla (60fps)
// Espera a que la GBA entre en el periodo de "VBlank"
void vblank_sync() {
    // Espera a que termine de dibujar las 160 líneas activas
    while (REG_VCOUNT >= 160) {}
    // Espera a que la GBA empiece a dibujar de nuevo (esto sucede 60 veces por segundo)
    while (REG_VCOUNT < 160) {}
}

// Función para pintar un píxel de forma segura
void setPixel(int x, int y, unsigned short color) {
    if (x >= 0 && x < 240 && y >= 0 && y < 160) {
        VRAM[y * 240 + x] = color;
    }
}

// Función para dibujar un cuadrado (nuestro "hombresito" básico)
void drawBox(int x, int y, int width, int height, unsigned short color) {
    for (int dy = 0; dy < height; dy++) {
        for (int dx = 0; dx < width; dx++) {
            setPixel(x + dx, y + dy, color);
        }
    }
}

int main() {
    // Configurar video Modo 3 (como antes)
    REG_DISPCNT = MODE_3 | BG2_ENABLE;

    // Colores
    unsigned short colorFondo = RGB15(10, 10, 10);   // Gris oscuro
    unsigned short colorHombresito = RGB15(31, 31, 0); // Amarillo brillante

    // Estado del personaje (X, Y) y tamaño
    int p_x = 120; // Centro de la pantalla X
    int p_y = 80;  // Centro de la pantalla Y
    int p_ancho = 8;
    int p_alto = 8;
    int p_velocidad = 2; // Píxeles por frame

    // Pintar fondo inicial (solo una vez)
    for (int i = 0; i < 240 * 160; i++) {
        VRAM[i] = colorFondo;
    }

    // --- EL BUCLE DEL JUEGO PRINCIPAL ---
    while (1) {
        // 1. Sincronizar (esperar el frame)
        vblank_sync();

        // 2. BORRAR la posición vieja
        drawBox(p_x, p_y, p_ancho, p_alto, colorFondo);

        // 3. LEER botones
        // REG_KEYINPUT tiene los bits en 1 por defecto; se ponen en 0 cuando se presiona
        unsigned short keys = REG_KEYINPUT;

        // 4. ACTUALIZAR posición según flechas (notas la inversión del operador NOT '!')
        if (!(keys & KEY_RIGHT)) p_x += p_velocidad;
        if (!(keys & KEY_LEFT))  p_x -= p_velocidad;
        if (!(keys & KEY_DOWN))  p_y += p_velocidad;
        if (!(keys & KEY_UP))    p_y -= p_velocidad;

        // Limites de pantalla (no dejar que el hombresito se escape)
        if (p_x < 0) p_x = 0;
        if (p_x > (240 - p_ancho)) p_x = 240 - p_ancho;
        if (p_y < 0) p_y = 0;
        if (p_y > (160 - p_alto)) p_y = 160 - p_alto;

        // 5. DIBUJAR nueva posición
        drawBox(p_x, p_y, p_ancho, p_alto, colorHombresito);
    }

    return 0;
}
