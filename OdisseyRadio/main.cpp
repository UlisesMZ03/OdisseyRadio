#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QFont>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Crear la ventana principal
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Mi Aplicación Spotify-Like");
        mainWindow.setGeometry(100, 100, 800, 600);

    // Crear el contenedor principal
    QWidget *centralWidget = new QWidget(&mainWindow);
    mainWindow.setCentralWidget(centralWidget);

    // Crear un diseño de cuadrícula
    QGridLayout *mainLayout = new QGridLayout(centralWidget);

    // Crear el QListView en el lado izquierdo (columna 0)
    QListWidget *leftListView = new QListWidget(centralWidget);
    QStringList leftPlaylist;
    leftPlaylist << "Lista de Reproducción 1" << "Lista de Reproducción 2" << "Lista de Reproducción 3";
    leftListView->addItems(leftPlaylist);
    mainLayout->addWidget(leftListView, 0, 0, 1, 2); // Cambia el ancho a 2 columnas

    // Crear el QListView en el lado derecho (columna 2)
    QListWidget *rightListView = new QListWidget(centralWidget);
    QStringList rightPlaylist;
    rightPlaylist << "Canción 1" << "Canción 2" << "Canción 3" << "Canción 4" << "Canción 5";
    rightListView->addItems(rightPlaylist);
    mainLayout->addWidget(rightListView, 0, 2, 1, 1); // Ocupa una columna

    // Crear el reproductor estilo Spotify en la parte inferior
    QWidget *playerWidget = new QWidget(centralWidget);
    mainLayout->addWidget(playerWidget, 1, 0, 1, 3); // Alinea con 3 columnas

    // Crear botones con símbolos en lugar de texto
    QPushButton *playButton = new QPushButton(playerWidget);
    playButton->setIcon(QIcon::fromTheme("media-playback-start")); // Símbolo de reproducción
    playButton->setIconSize(QSize(32, 32)); // Establece el tamaño del icono
    playButton->setFixedSize(32, 32); // Establece un tamaño fijo igual al tamaño del icono
    playButton->setFlat(true); // Botón sin bordes
    playButton->setCursor(Qt::PointingHandCursor); // Cambia el cursor al pasar sobre el botón
    playButton->setProperty("pressed", false); // Propiedad para rastrear si el botón está presionado

    QPushButton *pauseButton = new QPushButton(playerWidget);
    pauseButton->setIcon(QIcon::fromTheme("media-playback-pause")); // Símbolo de pausa
    pauseButton->setIconSize(QSize(32, 32)); // Establece el tamaño del icono
    pauseButton->setFixedSize(32, 32); // Establece un tamaño fijo igual al tamaño del icono
    pauseButton->setFlat(true); // Botón sin bordes
    pauseButton->setCursor(Qt::PointingHandCursor); // Cambia el cursor al pasar sobre el botón
    pauseButton->setProperty("pressed", false); // Propiedad para rastrear si el botón está presionado

    QPushButton *nextButton = new QPushButton(playerWidget);
    nextButton->setIcon(QIcon::fromTheme("media-skip-forward")); // Símbolo de siguiente
    nextButton->setIconSize(QSize(32, 32)); // Establece el tamaño del icono
    nextButton->setFixedSize(32, 32); // Establece un tamaño fijo igual al tamaño del icono
    nextButton->setFlat(true); // Botón sin bordes
    nextButton->setCursor(Qt::PointingHandCursor); // Cambia el cursor al pasar sobre el botón
    nextButton->setProperty("pressed", false); // Propiedad para rastrear si el botón está presionado

    QPushButton *prevButton = new QPushButton(playerWidget);
    prevButton->setIcon(QIcon::fromTheme("media-skip-backward")); // Símbolo de anterior
    prevButton->setIconSize(QSize(32, 32)); // Establece el tamaño del icono
    prevButton->setFixedSize(32, 32); // Establece un tamaño fijo igual al tamaño del icono
    prevButton->setFlat(true); // Botón sin bordes
    prevButton->setCursor(Qt::PointingHandCursor); // Cambia el cursor al pasar sobre el botón
    prevButton->setProperty("pressed", false); // Propiedad para rastrear si el botón está presionado

    // Aplicar hoja de estilo para hacer los botones redondos
    QString buttonStyleSheet =
        "QPushButton {"
        "   background-color: transparent;"
        "   border: none;"
        "   padding: 0;"
        "   border-radius: 16px;" // Establece un radio de borde para hacerlo redondo
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(0, 0, 0, 0.2);" // Cambiar el color de fondo cuando se presiona
        "}";

    playButton->setStyleSheet(buttonStyleSheet);
    pauseButton->setStyleSheet(buttonStyleSheet);
    nextButton->setStyleSheet(buttonStyleSheet);
    prevButton->setStyleSheet(buttonStyleSheet);

    // Agregar elementos al layout del reproductor
    QHBoxLayout *playerLayout = new QHBoxLayout(playerWidget);
    playerLayout->addWidget(prevButton);
    playerLayout->addWidget(playButton);
    playerLayout->addWidget(pauseButton);
    playerLayout->addWidget(nextButton);

    // Mostrar la ventana principal
    mainWindow.show();

    return app.exec();
}
