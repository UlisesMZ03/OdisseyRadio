#include <QApplication>
#include "mainwindow.h" // Asegúrate de incluir el encabezado de MainWindow

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Crear una instancia de MainWindow
    MainWindow mainWindow;
    mainWindow.show(); // Mostrar la ventana principal

    return app.exec(); // Iniciar el ciclo de eventos de la aplicación
}
