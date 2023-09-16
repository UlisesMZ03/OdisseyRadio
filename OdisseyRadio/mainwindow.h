#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ReproductorMP3.h" // Incluye el encabezado de ReproductorMP3

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void cargarCancion(); // Declara la función para cargar una canción

private:
    ReproductorMP3 reproductorMP3; // Declara una instancia de ReproductorMP3
};

#endif // MAINWINDOW_H
