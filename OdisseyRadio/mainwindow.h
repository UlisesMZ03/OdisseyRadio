#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ReproductorMP3.h" // Incluye el encabezado de ReproductorMP3
#include <QMap> // Para almacenar los datos de las canciones

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void cargarCancion();
    void mostrarInformacionCargada();
    void detenerReproduccion();
    void iniciarReproduccion();
    void cargarCancionSeleccionada();
    // Declara la función para cargar una canción

private:
    QMap<QString, SongInfo> songData;  // Contenedor para los datos de las canciones
    void cargarDatosCSV();
    mutable QString selectedSongPath;
QStringList getAllSongFiles(const QString &folderPath);

    ReproductorMP3 reproductorMP3; // Declara una instancia de ReproductorMP3
    void setSelectedSongPath(const QString &path);
};

#endif // MAINWINDOW_H
