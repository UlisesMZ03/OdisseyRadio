#include <SDL2/SDL.h>
#include <mpg123.h>
#include <iostream>
#include <thread>
#include <chrono>

class ReproductorMP3 {
public:
    ReproductorMP3();
    ~ReproductorMP3();

    bool cargarCancion(const char* archivo);
    void reproducir();
    void pausar();
    void reanudar();

    double obtenerDuracion();
    double obtenerDuracionTotal();
    void detener();
    void reproducirDesdeSegundo(int segundo);

private:
    mpg123_handle* mh;
    SDL_AudioDeviceID audioDevice;
    bool pausado;
    double duracionCancion;
};

ReproductorMP3::ReproductorMP3() : pausado(false) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    mpg123_init();
    mh = mpg123_new(NULL, NULL);
    if (mh == NULL) {
        std::cerr << "Error al inicializar mpg123" << std::endl;
        exit(1);
    }
}

ReproductorMP3::~ReproductorMP3() {
    mpg123_exit();
    SDL_Quit();
}

// Agregar esta función para avanzar la reproducción a partir de un segundo específico
void ReproductorMP3::reproducirDesdeSegundo(int segundo) {
    unsigned char audio_buffer[4096];
    size_t buffer_size;
    double tiempoTranscurrido = 0.0;

    // Leer y descartar los primeros 'segundo' segundos de audio
    while (tiempoTranscurrido < segundo) {
        if (mpg123_read(mh, audio_buffer, sizeof(audio_buffer), &buffer_size) == MPG123_OK) {
            tiempoTranscurrido += static_cast<double>(buffer_size) / (44100.0 * 2 * 2);
        } else {
            break;
        }
    }

    // Luego, reproducir normalmente
    while (mpg123_read(mh, audio_buffer, sizeof(audio_buffer), &buffer_size) == MPG123_OK) {
        if (!pausado) {
            SDL_QueueAudio(audioDevice, audio_buffer, buffer_size);
        }
    }
}


bool ReproductorMP3::cargarCancion(const char* archivo) {
    if (mpg123_format_none(mh) != MPG123_OK ||
        mpg123_format(mh, 44100, MPG123_STEREO, MPG123_ENC_SIGNED_16) != MPG123_OK) {
        std::cerr << "Error al configurar el formato de mpg123" << std::endl;
        return false;
    }

    if (mpg123_open(mh, archivo) != MPG123_OK) {
        std::cerr << "Error al abrir el archivo MP3: " << mpg123_strerror(mh) << std::endl;
        return false;
    }

    SDL_AudioSpec wanted_spec, spec;
    long rate;
    int channels, encoding;
    mpg123_getformat(mh, &rate, &channels, &encoding);

    wanted_spec.freq = rate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = channels;
    wanted_spec.samples = 4096;
    wanted_spec.callback = NULL;

    audioDevice = SDL_OpenAudioDevice(NULL, 0, &wanted_spec, &spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (audioDevice == 0) {
        std::cerr << "Error al abrir el dispositivo de audio: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_PauseAudioDevice(audioDevice, 0);
    return true;
}

void ReproductorMP3::reproducir() {
    unsigned char audio_buffer[4096];
    size_t buffer_size;

    duracionCancion =0;
    while (mpg123_read(mh, audio_buffer, sizeof(audio_buffer), &buffer_size) == MPG123_OK) {
        if (!pausado) {
            duracionCancion += static_cast<double>(buffer_size) / (44100.0 * 2 * 2); // Frecuencia * canales * bytes por muestra (16 bits)
            SDL_QueueAudio(audioDevice, audio_buffer, buffer_size);
        }
    }
}

void ReproductorMP3::pausar() {
    // Detener la lectura de audio desde el archivo MP3.


    // Vaciar la cola de audio del dispositivo de audio.
    SDL_ClearQueuedAudio(audioDevice);

    // Pausar el dispositivo de audio.
    SDL_PauseAudioDevice(audioDevice, 1);  // El segundo parámetro es 1 para pausar.
    pausado = true;
}


void ReproductorMP3::reanudar() {
    pausado = false;
}




double ReproductorMP3::obtenerDuracion() {
    return duracionCancion;
}

double ReproductorMP3::obtenerDuracionTotal() {
    return mpg123_length(mh);
}

void ReproductorMP3::detener() {
    pausar();  // Pausa la reproducción (en caso de que esté reproduciendo).

    // Limpia los recursos relacionados con la reproducción de MP3.
    SDL_CloseAudioDevice(audioDevice);
    mpg123_close(mh);

}


int reproductormp3(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " archivo.mp3" << std::endl;
        return 1;
    }

    ReproductorMP3 reproductor;

    if (reproductor.cargarCancion(argv[1])) {
        reproductor.reproducir();
        std::cout << "Reproduciendo..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3)); // Reproduce durante 3 segundos (puedes ajustar esto).
        reproductor.pausar();
        std::cout << "Pausado" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Pausado durante 2 segundos (puedes ajustar esto).
        reproductor.reanudar();
        std::cout << "Reanudado" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Reproduciendo durante 2 segundos (puedes ajustar esto).
        //reproductor.adelantar(10); // Adelanta 10 segundos (puedes ajustar esto).
        std::cout << "Adelantando 10 segundos..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        //reproductor.atrasar(5); // Retrocede 5 segundos (puedes ajustar esto).
        std::cout << "Retrocediendo 5 segundos..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Tiempo actual: " << reproductor.obtenerDuracion() << " segundos" << std::endl;
        std::cout << "Duración total: " << reproductor.obtenerDuracionTotal() << " segundos" << std::endl;
                                                                                                    reproductor.detener();
    }

    return 0;
}
