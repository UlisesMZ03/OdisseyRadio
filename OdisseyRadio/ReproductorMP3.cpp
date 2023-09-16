#include <SDL2/SDL.h>
#include <mpg123.h>
#include <iostream>

class ReproductorMP3 {
public:
    ReproductorMP3();
    ~ReproductorMP3();

    bool cargarCancion(const char* archivo);
    void reproducir();
    void detener();

private:
    mpg123_handle* mh;
    SDL_AudioDeviceID audioDevice;
};

ReproductorMP3::ReproductorMP3() {
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

    while (mpg123_read(mh, audio_buffer, sizeof(audio_buffer), &buffer_size) == MPG123_OK) {
        SDL_QueueAudio(audioDevice, audio_buffer, buffer_size);
    }

    SDL_Delay(3000);  // Reproduce durante 3 segundos (puedes ajustar esto).
}

void ReproductorMP3::detener() {
    SDL_CloseAudioDevice(audioDevice);
    mpg123_close(mh);
    mpg123_delete(mh);
}

int reproductorody(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " archivo.mp3" << std::endl;
        return 1;
    }

    ReproductorMP3 reproductor;

    if (reproductor.cargarCancion(argv[1])) {
        reproductor.reproducir();
        reproductor.detener();
    }

    return 0;
}
