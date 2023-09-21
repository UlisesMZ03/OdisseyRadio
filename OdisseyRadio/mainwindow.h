#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ReproductorMP3.h" // Incluye el encabezado de ReproductorMP3
#include "qlistwidget.h"
#include "qslider.h"
#include <QMap> // Para almacenar los datos de las canciones
#include "QLabel"
#include <QLabel>
#include <QSlider>

struct SongInfo {
    QString name;
    QString artist;
    QString genre;
    // Agrega más campos según sea necesario
};

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    QSet<QString> obtenerGenerosUnicos();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void actualizarMemoriaEnUso();
    void actualizarBarraProgreso();
    void cambiarPosicionReproduccion(int value);
    void cargarCancion();
    void detenerReproduccion();
    void iniciarReproduccion();
    void cargarCancionSeleccionada();
    std::string formatTiempo(double tiempoSegundos);


    // Declara la función para cargar una canción

private:
    QString songFilePath;
    QString folderPath;
    QMap<QString, QList<SongInfo>> artistSongs;
    QListWidget *leftListViewFolders;
    QLabel *memoryUsageLabel;
    long long memoryUsage = 0;
    QTimer* timer ;
    double tiempoActual;
    int currentSongIndex;
    QSlider* progressSlider;
    QLabel* currentTimeLabel;  // Declaración de currentTimeLabel
    QMap<QString, SongInfo> songData;  // Contenedor para los datos de las canciones
    void cargarDatosCSV(const QString &folderPath);
    mutable QString selectedSongPath;
    mutable QString selectedPathSong;

    QStringList getAllSongFiles(const QString &folderPath);

    ReproductorMP3 reproductorMP3; // Declara una instancia de ReproductorMP3
    void setSelectedSongPath(const QString &path);

};

#endif // MAINWINDOW_H
