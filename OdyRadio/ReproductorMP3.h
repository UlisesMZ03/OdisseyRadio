#ifndef REPRODUCTORMP3_H
#define REPRODUCTORMP3_H

#include <SDL2/SDL.h>
#include <mpg123.h>

class ReproductorMP3 {
public:
    ReproductorMP3();
    ~ReproductorMP3();

    bool cargarCancion(const char* archivo);
    void reproducir();
    void pausar();
    void reanudar();
    void detener();

    void reproducirDesdeSegundo(int segundo);
    double obtenerDuracionTotal();
    double obtenerDuracion();

private:

    mpg123_handle* mh;
    SDL_AudioDeviceID audioDevice;
    bool enReproduccion; // Para mantener un seguimiento del estado de reproducción
};

#endif // REPRODUCTORMP3_H
