#include "mainwindow.h"
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
#include <QFileDialog>
#include <QTimer>
#include <QDirIterator>
#include "ReproductorMP3.h" // Asegúrate de incluir correctamente el encabezado de ReproductorMP3
#include <QFile>
#include <QTextStream>
#include <QMap>  // Para almacenar los datos de las canciones
#include <QMessageBox>
#include <QRegularExpression>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    currentSongIndex = -1; // Inicialmente no hay ninguna canción seleccionada

    setWindowTitle("OdisseyRadio");
    setGeometry(100, 100, 800, 600);
    // Agrega un miembro de clase para almacenar la ruta de la canción seleccionada
    QString selectedSongPath;
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    cargarDatosCSV();
    QGridLayout *mainLayout = new QGridLayout(centralWidget);

    QListWidget *leftListViewFolders = new QListWidget(centralWidget);
    mainLayout->addWidget(leftListViewFolders, 0, 0, 1, 1);

    QListWidget *rightListViewSongs = new QListWidget(centralWidget);
    mainLayout->addWidget(rightListViewSongs, 0, 1, 1, 1);

    QPushButton *loadFolderButton = new QPushButton("Cargar Carpeta", centralWidget);
    mainLayout->addWidget(loadFolderButton, 0, 0, 1, 1);

    connect(loadFolderButton, &QPushButton::clicked, this, [=]() {
        QString folderPath = QFileDialog::getExistingDirectory(this, "Seleccionar Carpeta", QDir::homePath());
        if (!folderPath.isEmpty()) {
            leftListViewFolders->addItem(folderPath);
        }
    });

    // En la función para cargar la carpeta de canciones
    connect(leftListViewFolders, &QListWidget::itemSelectionChanged, this, [=]() {
        rightListViewSongs->clear();
        QListWidgetItem *selectedFolderItem = leftListViewFolders->currentItem();
        if (selectedFolderItem) {
            QString folderPath = selectedFolderItem->text();

            // Función para obtener todas las canciones de las subcarpetas
            QStringList allSongFiles = getAllSongFiles(folderPath);

            rightListViewSongs->addItems(allSongFiles);
        }
    });

// ...
    connect(rightListViewSongs, &QListWidget::itemSelectionChanged, this, [=]() {
        QListWidgetItem *selectedSongItem = rightListViewSongs->currentItem();

        if (selectedSongItem) {
            currentSongIndex = rightListViewSongs->row(selectedSongItem);
            QString selectedSongPath = selectedSongItem->text();

            // Obtén el nombre de la canción desde la ruta completa
            QString selectedSongName = QFileInfo(selectedSongPath).fileName();

            // Elimina la extensión ".mp3" del nombre de la canción usando QRegularExpression
            QRegularExpression re("\\.mp3$", QRegularExpression::CaseInsensitiveOption);
            selectedSongName.replace(re, "");

            // Intenta convertir el nombre de la canción en un entero para eliminar los ceros a la izquierda
            int songID = selectedSongName.toInt();

            // Convierte el número nuevamente a una cadena
            selectedSongName = QString::number(songID);

            // Imprime el nombre de la canción en la consola para verificarlo
            qDebug() << "Nombre de la canción seleccionada: " << selectedSongName;

                if (songData.contains(selectedSongName)) {
                SongInfo songInfo = songData.value(selectedSongName);
                QString artistName = songInfo.artist;
                QString genre = songInfo.genre;

                // Ahora tienes el "artist_name" y el "genre" correspondientes al "track_id"
                qDebug() << "Artista: " << artistName;
                qDebug() << "Album: " << genre;
            } else {
                // No se encontró información para el "track_id" dado
                qDebug() << "No se encontró información para el track_id: " << selectedSongName;
            }
        }
    });





    QWidget *playerWidget = new QWidget(centralWidget);
    mainLayout->addWidget(playerWidget, 1, 0, 1, 2);

    QPushButton *playButton = new QPushButton(playerWidget);
    playButton->setIcon(QIcon::fromTheme("media-playback-start"));
    playButton->setIconSize(QSize(32, 32));
    playButton->setFixedSize(32, 32);
    playButton->setFlat(true);
    playButton->setCursor(Qt::PointingHandCursor);

    connect(playButton, &QPushButton::clicked, this, &MainWindow::cargarCancionSeleccionada);


    QPushButton *pauseButton = new QPushButton(playerWidget);
    pauseButton->setIcon(QIcon::fromTheme("media-playback-pause"));
    pauseButton->setIconSize(QSize(32, 32));
    pauseButton->setFixedSize(32, 32);
    pauseButton->setFlat(true);
    pauseButton->setCursor(Qt::PointingHandCursor);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::detenerReproduccion);
    QPushButton *nextButton = new QPushButton(playerWidget);
    nextButton->setIcon(QIcon::fromTheme("media-skip-forward"));
    nextButton->setIconSize(QSize(32, 32));
    nextButton->setFixedSize(32, 32);
    nextButton->setFlat(true);
    nextButton->setCursor(Qt::PointingHandCursor);

    QPushButton *prevButton = new QPushButton(playerWidget);
    prevButton->setIcon(QIcon::fromTheme("media-skip-backward"));
    prevButton->setIconSize(QSize(32, 32));
    prevButton->setFixedSize(32, 32);
    prevButton->setFlat(true);
    prevButton->setCursor(Qt::PointingHandCursor);

    QString buttonStyleSheet =
        "QPushButton {"
        "   background-color: transparent;"
        "   border: none;"
        "   padding: 0;"
        "   border-radius: 16px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(0, 0, 0, 0.2);"
        "}";

    playButton->setStyleSheet(buttonStyleSheet);
    pauseButton->setStyleSheet(buttonStyleSheet);
    nextButton->setStyleSheet(buttonStyleSheet);
    prevButton->setStyleSheet(buttonStyleSheet);

    QHBoxLayout *playerLayout = new QHBoxLayout(playerWidget);
    playerLayout->addWidget(prevButton);
    playerLayout->addWidget(playButton);
    playerLayout->addWidget(pauseButton);
    playerLayout->addWidget(nextButton);

    connect(nextButton, &QPushButton::clicked, this, [=]() {
        if (currentSongIndex < rightListViewSongs->count() - 1) {
            currentSongIndex++;
            rightListViewSongs->setCurrentRow(currentSongIndex);
            cargarCancionSeleccionada(); // Llama a la función para cargar la canción seleccionada
        }
    });
    connect(prevButton, &QPushButton::clicked, this, [=]() {
        if (currentSongIndex < rightListViewSongs->count() - 1) {
            currentSongIndex--;
            rightListViewSongs->setCurrentRow(currentSongIndex);
            cargarCancionSeleccionada(); // Llama a la función para cargar la canción seleccionada
        }
    });


    connect(rightListViewSongs, &QListWidget::itemSelectionChanged, this, [=]() {
        QListWidgetItem *selectedSongItem = rightListViewSongs->currentItem();
        if (selectedSongItem) {
            QString selectedSongName = selectedSongItem->text();

            // Obtén la ruta de la carpeta en la que se encuentra la canción seleccionada
            QString selectedFolderPath = leftListViewFolders->currentItem()->text();

            // Combina la carpeta y el nombre del archivo para obtener la ruta completa
            QString selectedSongPath = selectedSongName;

            // Asigna la ruta completa a selectedSongPath
            this->selectedSongPath = selectedSongPath;

            // Imprime la ruta completa en la consola
            qDebug() << "Ruta completa de la canción: " << this->selectedSongPath;
        }
    });





      // En el constructor de MainWindow, después de crear los botones:
      progressSlider = new QSlider(Qt::Horizontal, playerWidget);
      mainLayout->addWidget(progressSlider, 2, 0, 1, 2);

      // Configura el rango de la barra de progreso (0 a 100)
      progressSlider->setRange(0, 100);

      // Conecta la señal valueChanged de la barra de progreso a un slot para controlar el avance o retroceso de la canción
      connect(progressSlider, &QSlider::valueChanged, this, &MainWindow::cambiarPosicionReproduccion);

      // Agrega una etiqueta para mostrar el tiempo actual de reproducción
      currentTimeLabel = new QLabel("0:00", playerWidget);
      mainLayout->addWidget(currentTimeLabel, 3, 0, 1, 1);
      currentTimeLabel->setText("hola");
      timer = new QTimer(this);
      connect(timer, &QTimer::timeout, this, &MainWindow::actualizarBarraProgreso);

}
void MainWindow::cambiarPosicionReproduccion(int value) {
      // Calcula el nuevo tiempo en segundos basado en el valor de la barra de progreso
      double nuevaPosicionSegundos = value * reproductorMP3.obtenerDuracion() / 100;
      tiempoActual = nuevaPosicionSegundos;
      qDebug() <<nuevaPosicionSegundos ;
      // Cambia la posición de reproducción en el reproductor
      //reproductorMP3.adelantar(tiempoActual);
}
void MainWindow::iniciarReproduccion() {
      reproductorMP3.reanudar();
      reproductorMP3.reproducir();

      // Configura un temporizador para actualizar la barra de progreso y el tiempo actual


      timer->start(1000);  // Actualiza cada segundo (puedes ajustar esto)



}

void MainWindow::actualizarBarraProgreso() {
      // Obtén el tiempo actual de reproducción y la duración total

      double duracionTotal = reproductorMP3.obtenerDuracion();
      //double duracionTotal = reproductorMP3.obtenerDuracionTotal();

      qDebug() <<  tiempoActual;
      tiempoActual ++;
      // Actualiza la etiqueta de tiempo actual
      QString tiempoTotalStr = QString::fromStdString(formatTiempo(duracionTotal));

        qDebug() << "Tiempo actual de la cancion" << duracionTotal;



          //Calcula el valor para la barra de progreso (0-100)
        int valorBarraProgreso = static_cast<int>((tiempoActual / duracionTotal) * 100);

        progressSlider->setValue(valorBarraProgreso);
        QString tiempoActualStr = QString::fromStdString(formatTiempo(tiempoActual));
        currentTimeLabel -> setText(tiempoActualStr);


}

std::string MainWindow::formatTiempo(double tiempoSegundos) {
      int minutos = static_cast<int>(tiempoSegundos / 60);
      int segundos = static_cast<int>(tiempoSegundos) % 60;
      QString tiempoStr = QString("%1:%2").arg(minutos, 2, 10, QChar('0')).arg(segundos, 2, 10, QChar('0'));

      return tiempoStr.toStdString();
}


void MainWindow::cargarDatosCSV() {
    QFile file("/home/ulisesmz/Descargas/fma_metadata/raw_tracks.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");

            if (fields.size() >= 6) {
                QString trackID = fields[0].trimmed();    // Columna "track_id"
                QString albumID = fields[2].trimmed();    // Columna "album_id"
                QString artist = fields[5].trimmed(); // Columna "album_title"

                // Verifica que los campos no estén vacíos antes de agregarlos al mapa
                if (!trackID.isEmpty() && !albumID.isEmpty() && !artist.isEmpty()) {
                    // Verifica si trackID es un número
                    bool isTrackIDNumeric;
                    int trackIDValue = trackID.toInt(&isTrackIDNumeric);

                    if (isTrackIDNumeric) {
                        // trackID es un número válido
                        // Crea un objeto que contenga "track_id", "album_id" y "album_title"
                        SongInfo songInfo;
                        songInfo.name = trackID;
                        songInfo.artist = artist;
                        songInfo.genre = albumID;

                        // Agrega este objeto al mapa usando "track_id" como clave
                        songData.insert(trackID, songInfo);
                    } else {
                        // trackID no es un número válido, puedes manejar esto según tus necesidades
                    }
                }
            }
        }
        file.close();
    }
}





QStringList MainWindow::getAllSongFiles(const QString &folderPath) {
    QDirIterator it(folderPath, QStringList() << "*.mp3", QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    QStringList songFiles;

    while (it.hasNext()) {
        it.next();
        songFiles.append(it.filePath());
    }

    return songFiles;
}

void MainWindow::cargarCancion() {
    QString filePath = QFileDialog::getOpenFileName(this, "Seleccionar Canción", QDir::homePath(), "Archivos de música (*.mp3)");

    if (!filePath.isEmpty()) {
        const char *cFilePath = filePath.toUtf8().constData();

        if (reproductorMP3.cargarCancion(cFilePath)) {
            // La canción se cargó con éxito, ahora la reproducimos

        }
    }
}
// Implementación de la función para actualizar selectedSongPath
void MainWindow::setSelectedSongPath(const QString &path) {
    selectedSongPath = path;
}

void MainWindow::cargarCancionSeleccionada() {
    if (!selectedSongPath.isEmpty()) {
        qDebug() << "Ruta holaaaaa: " << this->selectedSongPath;
        const char *cFilePath = selectedSongPath.toUtf8().constData();
        detenerReproduccion();
        tiempoActual =0;
        if (reproductorMP3.cargarCancion(cFilePath)) {
            // La canción se cargó con éxito, ahora la reproducimos

            iniciarReproduccion();
        }
    }
}


void MainWindow::detenerReproduccion()
{
    timer->stop();
    reproductorMP3.detener();

}




MainWindow::~MainWindow()
{

}
