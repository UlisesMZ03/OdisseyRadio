#include "mainwindow.h"
#include "qapplication.h"
#include <QDir>
#include <QDirIterator>
#include <QStandardItemModel>
#include <QListView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>
#include <sys/resource.h>
#include <QProcess>
#include <QTimer>
#include <QCheckBox>
#include <QLabel>
#include <QScrollBar>
#include "ReproductorMP3.h" // Asegúrate de que el nombre del archivo sea correcto
#include <QCoreApplication>
#include <QDebug>
#include <fileref.h>
#include <tag.h>
#include <QMessageBox>
#include <QFileDialog>
MainWindow::~MainWindow()
{
    // Liberar la memoria de las canciones cargadas
    for (QStringList* songs : loadedSongs) {
        free(songs); // Usar free en lugar de delete
    }



}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)

{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Cambia el layout principal a un QGridLayout
    QGridLayout *mainLayout = new QGridLayout(centralWidget);









    // Obtener una lista de las canciones en el directorio




    // Crear el modelo de datos
    model = new QStandardItemModel(this);
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QModelIndex index = model->index(row, col);
            QStandardItem* item = model->itemFromIndex(index);
            if (item) {
                // Deshabilitar la edición para este elemento
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }
        }
    }






        // Configurar el QListView
        listView = new QListView(this);
        listView->setModel(model);
        listView->setSelectionMode(QAbstractItemView::SingleSelection); // Permitir selecciones individuales
        connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::handleSelectionChanged);


        // Crear un nuevo QListView para los artistas
        QListView *artistListView = new QListView(this);
        // Establecer hoja de estilo para eliminar el borde
        artistListView->setStyleSheet("QListView { border: none; }");

        // Crear un QVBoxLayout para organizar el QListView y el QLabel
        QVBoxLayout *listViewLayout = new QVBoxLayout();
        listViewLayout->addWidget(listView);

        // Crear un QHBoxLayout para organizar los botones "prev" y "next"
        QPushButton *infoButton = new QPushButton("Info", this);
        connect(infoButton, &QPushButton::clicked, this, &MainWindow::mostrarInfoCancion);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *prevPageButton = new QPushButton("<<", this);
        prevPageButton->setMaximumSize(40, 40);
        QPushButton *nextPageButton = new QPushButton(">>", this);
        nextPageButton->setMaximumSize(40, 40);

        // Agregar los botones al layout horizontal
        buttonLayout->addWidget(prevPageButton);
        connect(prevPageButton, &QPushButton::clicked, this, &MainWindow::loadPreviousPage);
        buttonLayout->addWidget(infoButton);
        buttonLayout->addWidget(nextPageButton);
         connect(prevPageButton, &QPushButton::clicked, this, &MainWindow::loadNextPage);

        // Agregar el layout horizontal de botones al layout vertical del QListView
        listViewLayout->addLayout(buttonLayout);

        // Agregar el layout del QListView al layout principal
        mainLayout->addLayout(listViewLayout, 0, 1, 1, 1);

    artistModel = new QStandardItemModel(this);
    // Configura el modelo para el artistListView
    artistListView->setModel(artistModel);

    connect(artistListView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::handleArtistSelectionChanged);

    // Crear un QLabel con el texto "Artistas"
    QLabel *artistsLabel = new QLabel("Artistas", this);
    artistsLabel->setAlignment(Qt::AlignCenter); // Alinea el texto al centro

    // Crear un QVBoxLayout para organizar el QListView y el QLabel
    QVBoxLayout *artistLayout = new QVBoxLayout();
    artistLayout->addWidget(artistsLabel);
    artistLayout->addWidget(artistListView);

    // Agregar el QVBoxLayout al layout principal

    mainLayout->addLayout(artistLayout, 0, 0, 2, 1); // El layout de artistas ocupa dos filas


    QCheckBox *paginationCheckBox = new QCheckBox("Paginación", this);
                                      paginationCheckBox->setChecked(true);
    connect(paginationCheckBox, &QCheckBox::clicked, this, &MainWindow::togglePaginationMode);



        // Inicializa el temporizador para verificar el uso de memoria cada X milisegundos
        memoryUsageTimer = new QTimer(this);
        connect(memoryUsageTimer, &QTimer::timeout, this, &MainWindow::printMemoryUsageWithQProcess);
        memoryUsageTimer->start(1000);

        // Agregar widgets al layout principal

// Agrega el botón en la misma fila que el botón "Siguiente Página"

        mainLayout->addWidget(paginationCheckBox, 2, 1, 1, 1); // Puedes ajustar las posiciones y el tamaño según tus necesidades
        // Crear un QSlider para controlar la reproducción
        progressSlider= new QSlider(Qt::Horizontal, this);
        mainLayout->addWidget(progressSlider, 3, 0, 1, 2); // Ocupa dos columnas
        // Configura el rango de la barra de progreso (0 a 100)
        progressSlider->setRange(0, 100);

        // Conecta la señal valueChanged de la barra de progreso a un slot para controlar el avance o retroceso de la canción
        connect(progressSlider, &QSlider::valueChanged, this, &MainWindow::cambiarPosicionReproduccion);
        // Crear botones para reproducir, pausar, atrasar y adelantar
        // Crear un widget para los botones de reproducción
        QWidget *playerWidget = new QWidget(centralWidget);
        mainLayout->addWidget(playerWidget, 4, 0, 1, 2); // Ocupa dos columnas

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
        // Conectar los botones "next" y "prev"


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

        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::actualizarBarraProgreso);
        // Conectar el botón "next" a la función loadNextItem
        connect(nextButton, &QPushButton::clicked, this, &MainWindow::loadNextItem);

        // Conectar el botón "prev" a la función loadPrevItem
        connect(prevButton, &QPushButton::clicked, this, &MainWindow::loadPrevItem);
        // Crear un botón "info" para mostrar la información de la canción

        QPushButton *openFolderButton = new QPushButton("Open Folder", this);
        connect(openFolderButton, &QPushButton::clicked, this, &MainWindow::openFolder);

        // Agregar el botón al layout de artistas
        artistLayout->addWidget(openFolderButton);

}

void MainWindow::openFolder() {
        songsDirectory = QFileDialog::getExistingDirectory(this, "Select Folder", songsDirectory);

        if (!songsDirectory.isEmpty()) {

qDebug()<<songsDirectory;
        // Limpiar el modelo de datos actual y liberar la memoria de las canciones cargadas
        model->removeRows(0, model->rowCount());
        for (QStringList* songs : loadedSongs) {
            free(songs); // Usar free en lugar de delete
        }
        loadedSongs.clear();

        // Invocar al recolector de basura para liberar la memoria inmediatamente
        QCoreApplication::processEvents();

        // Volver a cargar las canciones desde la nueva carpeta
        QDirIterator it(songsDirectory, QStringList() << "*.mp3", QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            songList.append(it.filePath());
        }
        // Cargar las primeras 3 páginas en memoria



        int totalSongs = songList.count();
        songsPerPage = 5; // Canciones por página
        currentPage = 0; // Página actual
        const int maxSongsPerPage = 5;

        totalPages = (totalSongs + maxSongsPerPage - 1) / maxSongsPerPage;
        loadPagesToModel();
        cargarDatosCSV(songsDirectory);
        }
}

void MainWindow::loadNextItem() {
        timer->stop();
        detenerReproduccion();
        int rowCount = model->rowCount();
        if (rowCount > 0) {
        currentIndex = (currentIndex + 1) % rowCount;
        QModelIndex nextIndex = model->index(currentIndex, 0);
        listView->setCurrentIndex(nextIndex);

        cargarCancionSeleccionada();
        }
}

void MainWindow::loadPrevItem() {
        timer->stop();
        detenerReproduccion();
        int rowCount = model->rowCount();
        if (rowCount > 0) {
        currentIndex = (currentIndex - 1 + rowCount) % rowCount;
        QModelIndex prevIndex = model->index(currentIndex, 0);
        listView->setCurrentIndex(prevIndex);

        cargarCancionSeleccionada();
        }
}

void MainWindow::handleScrollValueChanged(int value)
{
        int maxScrollValue = listView->verticalScrollBar()->maximum();
        int minScrollValue = listView->verticalScrollBar()->minimum();

        if (value == maxScrollValue) {
        // El usuario ha llegado al final (desplazamiento hacia abajo)
        loadNextPage();
        } else if (value == minScrollValue) {
        // El usuario ha llegado al principio (desplazamiento hacia arriba)
        loadPreviousPage();
        }
}void MainWindow::handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
        Q_UNUSED(deselected);

        QModelIndexList selectedIndexes = selected.indexes();

        if (!selectedIndexes.isEmpty()) {
       selectedIndex = selectedIndexes.first();
        QString selectedData = selectedIndex.data(Qt::DisplayRole).toString();


        // Obtener la ruta de la canción seleccionada desde el modelo de datos
        QModelIndex index = selectedIndex.sibling(selectedIndex.row(), 0);
        songPath = model->data(index, Qt::DisplayRole).toString();

        // Convertir el QString a const char* utilizando toUtf8() o toLocal8Bit()


        // Deselecciona todos los elementos del grupo actualmente seleccionado
        int groupIndex = selectedIndex.row() / songsPerPage;
        int firstIndexInGroup = groupIndex * songsPerPage;
        int lastIndexInGroup = firstIndexInGroup + songsPerPage - 1;

        for (int i = firstIndexInGroup; i <= lastIndexInGroup; ++i) {
            if (i != selectedIndex.row()) {
                QModelIndex indexToDeselect = model->index(i, 0);
                listView->selectionModel()->select(indexToDeselect, QItemSelectionModel::Deselect);
            }
        }
        }
}
void MainWindow::cargarCancionSeleccionada() {
        qDebug() << "Ruta holaaaaa: " << this->songPath;
        if (!songPath.isEmpty()) {
        qDebug() << "Ruta holaaaaa: " << this->songPath;
        const char *cFilePath = songPath.toUtf8().constData();
        detenerReproduccion();
        tiempoActual =0;
        if (reproductorMP3.cargarCancion(cFilePath)) {
            // La canción se cargó con éxito, ahora la reproducimos

            iniciarReproduccion();
        }
        }
}
void MainWindow::handleArtistSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
        Q_UNUSED(deselected);

        QModelIndexList selectedIndexes = selected.indexes();

        if (!selectedIndexes.isEmpty()) {
        QModelIndex selectedIndex = selectedIndexes.first();
        QString selectedData = selectedIndex.data(Qt::DisplayRole).toString();
        qDebug() << "Artista seleccionado en el artistListView: " << selectedData;

        // Almacena el nombre del artista seleccionado
        selectedArtist = selectedData;

        // Carga las canciones del artista en el listView
        loadArtistSongsToModel(selectedArtist);
        }
}
void MainWindow::handleSongSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
        Q_UNUSED(deselected);

        QModelIndexList selectedIndexes = selected.indexes();

        if (!selectedIndexes.isEmpty()) {
        QModelIndex selectedIndex = selectedIndexes.first();
        QString selectedData = selectedIndex.data(Qt::DisplayRole).toString();
        qDebug() << "Elemento seleccionado: " << selectedData;

        // Obtener la ruta de la canción seleccionada desde el modelo de datos
        QModelIndex index = selectedIndex.sibling(selectedIndex.row(), 0);
        QString songPath = model->data(index, Qt::DisplayRole).toString();

        // Acceder a los metadatos de la canción usando TagLib
        TagLib::FileRef file(songPath.toUtf8().constData());

        if (!file.isNull()) {
            // Acceder a los metadatos de la canción
            TagLib::Tag *tag = file.tag();

            // Obtener el nombre real de la canción desde los metadatos
            QString songTitle = QString::fromStdString(tag->title().to8Bit(true));

            // Mostrar el nombre real de la canción en la consola
            qDebug() << "Nombre de la canción:" << songTitle;
        } else {
            qDebug() << "No se pudo abrir el archivo MP3.";
        }
        }
}

QString MainWindow::obtenerSongName(const QString &songPath)
{
        QString songTitle;

        // Acceder a los metadatos de la canción usando TagLib
        TagLib::FileRef file(songPath.toUtf8().constData());

        if (!file.isNull()) {
        // Acceder a los metadatos de la canción
        TagLib::Tag *tag = file.tag();

        // Obtener el nombre real de la canción desde los metadatos
        songTitle = QString::fromStdString(tag->title().to8Bit(true));

        // Mostrar el nombre real de la canción en la consola
        qDebug() << "Nombre de la canción:" << songTitle;
        } else {
        qDebug() << "No se pudo abrir el archivo MP3.";
        }

        return songTitle;
}

void MainWindow::cambiarPosicionReproduccion(int value) {
        // Calcula el nuevo tiempo en segundos basado en el valor de la barra de progreso
        double nuevaPosicionSegundos = value * reproductorMP3.obtenerDuracion() / 100;
        tiempoActual = nuevaPosicionSegundos;
        qDebug() <<nuevaPosicionSegundos ;
        // Cambia la posición de reproducción en el reproductor
        //reproductorMP3.adelantar(tiempoActual);
}

std::string MainWindow::formatTiempo(double tiempoSegundos) {
        int minutos = static_cast<int>(tiempoSegundos / 60);
        int segundos = static_cast<int>(tiempoSegundos) % 60;
        QString tiempoStr = QString("%1:%2").arg(minutos, 2, 10, QChar('0')).arg(segundos, 2, 10, QChar('0'));

        return tiempoStr.toStdString();
}
void MainWindow::actualizarBarraProgreso() {
        // Obtén el tiempo actual de reproducción y la duración total

        double duracionTotal = reproductorMP3.obtenerDuracion();
        //double duracionTotal = reproductorMP3.obtenerDuracionTotal();

        qDebug() <<  tiempoActual;
        tiempoActual ++;
        // Actualiza la etiqueta de tiempo actual
        if (tiempoActual>=duracionTotal){
        tiempoActual=duracionTotal;
        timer->stop();
        }
        QString tiempoTotalStr = QString::fromStdString(formatTiempo(duracionTotal));

        qDebug() << "Tiempo actual de la cancion" << duracionTotal;



        //Calcula el valor para la barra de progreso (0-100)
        int valorBarraProgreso = static_cast<int>((tiempoActual / duracionTotal) * 100);

        progressSlider->setValue(valorBarraProgreso);
        QString tiempoActualStr = QString::fromStdString(formatTiempo(tiempoActual));
        //currentTimeLabel -> setText(tiempoActualStr);






}
void MainWindow::iniciarReproduccion() {

        if (reproductorMP3.cargarCancion(songPath.toUtf8().constData())) {
        //iniciarReproduccion();
        } else {
        // Manejar el error, por ejemplo, mostrar un mensaje al usuario
        }
        reproductorMP3.reanudar();
        reproductorMP3.reproducir();

        // Configura un temporizador para actualizar la barra de progreso y el tiempo actual


        timer->start(1000);  // Actualiza cada segundo (puedes ajustar esto)



}
void MainWindow::detenerReproduccion()
{
        reproductorMP3.pausar();
        timer->stop();
        reproductorMP3.detener();

}
void MainWindow::togglePaginationMode()
{
    // Liberar la memoria de las canciones cargadas en el modo anterior
    for (QStringList* songs : loadedSongs) {
        free(songs); // Usar free en lugar de delete
    }
    loadedSongs.clear(); // Limpiar la lista

    if (paginationMode) {
        paginationMode = false;
    } else {
        paginationMode = true;
    }

    // Vuelve a cargar las canciones según el nuevo modo
    loadPagesToModel();
}

void MainWindow::loadPreviousPage()
{
    if (currentPage > 0) {  // Verifica que no estés en la primera página
        currentPage--;
        loadPagesToModel();
    }
}

void MainWindow::loadNextPage()
{
    if (currentPage < totalPages) {  // Verifica que no se haya alcanzado la última página
        currentPage++;
        loadPagesToModel();
    }
}
void MainWindow::printMemoryUsageWithQProcess()
{
    QProcess process;
    process.start("ps", QStringList() << "-p" << QString::number(QCoreApplication::applicationPid()) << "-o" << "rss=");
    if (process.waitForFinished()) {
        QString output = process.readAll();
        bool ok;
        int memoryUsageKB = output.trimmed().toInt(&ok);
        if (ok) {
            // Convertir KB a MB
            double memoryUsageMB = memoryUsageKB / 1024.0;
            std::cout << "Memoria en uso (MB): " << memoryUsageMB << std::endl;
        } else {
            std::cerr << "Error al obtener el uso de memoria." << std::endl;
        }
    } else {
        std::cerr << "Error al ejecutar el comando ps." << std::endl;
    }
}

void MainWindow::loadPagesToModel()
{
    // Limpia el modelo de datos actual
    model->removeRows(0, model->rowCount());

    // Liberar la memoria de las canciones cargadas previamente
    for (QStringList* songs : loadedSongs) {
        free(songs); // Usar free en lugar de delete
    }
    loadedSongs.clear();

    // Invocar al recolector de basura para liberar la memoria inmediatamente
    QCoreApplication::processEvents();

    if (paginationMode) {
        // Carga las páginas siguientes en memoria
        for (int i = 0; i < 2; ++i) {
            int pageToLoad = currentPage + i;
            if (pageToLoad < totalPages) {
                QStringList* currentPageSongs = (QStringList*)malloc(sizeof(QStringList));
                new(currentPageSongs) QStringList(
                    songList.mid(pageToLoad * songsPerPage, songsPerPage));

                // Agregar una entrada por cada canción en la página
                for (const QString& song : *currentPageSongs) {
                    QStandardItem* songItem = new QStandardItem(song);
                    model->appendRow(songItem);
                }

                // Almacena la lista de canciones cargadas en memoria
                loadedSongs.append(currentPageSongs);
            }
        }
    } else {
        // Modo sin paginación: Carga todas las canciones en memoria
        QStringList allSongs;
        for (int i = 0; i < songList.count(); ++i) {
            allSongs.append(songList.at(i));
        }

        // Agrega todas las canciones al modelo de datos
        for (const QString& song : allSongs) {
            QStandardItem* songItem = new QStandardItem(song);
            model->appendRow(songItem);
        }

        // Almacena la lista de todas las canciones cargadas en memoria
        QStringList* songsCopy = (QStringList*)malloc(sizeof(QStringList));
        new(songsCopy) QStringList(allSongs);
        loadedSongs.append(songsCopy);
        QCoreApplication::processEvents();
    }
}
void MainWindow::mostrarInfoCancion() {
    if (!selectedIndex.isValid()) {
        // No se ha seleccionado ninguna canción
        return;
    }

    // Obtener la ruta de la canción seleccionada desde el modelo de datos
    QModelIndex index = selectedIndex.sibling(selectedIndex.row(), 0);
    QString songPath = model->data(index, Qt::DisplayRole).toString();

    // Acceder a los metadatos de la canción usando TagLib
    TagLib::FileRef file(songPath.toUtf8().constData());

    if (!file.isNull()) {
        // Acceder a los metadatos de la canción
        TagLib::Tag *tag = file.tag();

        // Obtener la información de la canción
        QString songTitle = QString::fromStdString(tag->title().to8Bit(true));
        QString artist = QString::fromStdString(tag->artist().to8Bit(true));
        QString album = QString::fromStdString(tag->album().to8Bit(true));
        QString year = QString::number(tag->year());
        QString genre = QString::fromStdString(tag->genre().to8Bit(true));
        QString duration = QString::fromStdString(formatTiempo(file.audioProperties()->length()));

        // Crear el mensaje de información
        QString infoText = "Título: " + songTitle + "\n"
                            + "Artista: " + artist + "\n"
                            + "Álbum: " + album + "\n"
                            + "Año: " + year + "\n"
                            + "Género: " + genre + "\n"
                            + "Duración: " + duration;

        // Mostrar la información en un cuadro de diálogo
        QMessageBox::information(this, "Información de la Canción", infoText);
    } else {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo MP3.");
    }
}


void MainWindow::cargarDatosCSV(const QString &folderPath) {
    qDebug() << "Si entro";
    QFile file("/home/ulisesmz/Descargas/fma_metadata/raw_tracks.csv");

    songInfoList; // Lista para almacenar detalles de canciones
    // Mantener un conjunto de artistas ya agregados
    QSet<QString> artistSet;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");

            if (fields.size() >= 6) {
                QString trackID = fields[0].trimmed();
                QString albumID = fields[2].trimmed();
                QString artist = fields[5].trimmed();

                if (!trackID.isEmpty() && !albumID.isEmpty() && !artist.isEmpty()) {
                    bool isTrackIDNumeric;
                    int trackIDValue = trackID.toInt(&isTrackIDNumeric);

                    if (isTrackIDNumeric) {
                        QString trackFileName;
                        // Obtén el nombre del archivo (sin la ruta) desde el trackID
                        if (trackID.length()==3){
                            trackFileName = folderPath + "/" +"000"+"/"+"000"+ trackID + ".mp3";
                        }
                        else if(trackID.length()==4){
                            trackFileName = folderPath + "/" +"00"+trackID.left(1)+"/"+"00"+trackID + ".mp3";
                        }
                        else if(trackID.length()==5){
                            trackFileName = folderPath + "/" +"0"+trackID.left(2)+"/"+"0"+trackID + ".mp3";
                        }
                        else{
                            trackFileName = folderPath + "/"+trackID.left(3)+"/"+trackID + ".mp3";
                        }

                        // Verifica si el trackFileName está en las canciones cargadas
                        if (songList.contains(trackFileName)) {
                            // Crea un objeto SongInfo y llénalo con los datos
                            SongInfo songInfo;
                            songInfo.trackID = trackID;
                            songInfo.albumID = albumID;
                            songInfo.artist = artist;
                            songInfo.trackFileName = trackFileName;


                            // Agrega songInfo a la lista
                            songInfoList.append(songInfo);

                            // Verifica si el artista ya se agregó previamente
                            if (!artistSet.contains(artist)) {
                                // Agrega el artista al artistModel
                                QStandardItem *artistItem = new QStandardItem(artist);
                                artistModel->appendRow(artistItem);

                                // Agrega el artista al conjunto de artistas ya agregados
                                artistSet.insert(artist);
                            }
                        }
                    }
                }
            }
        }
        file.close();
    } else {
        qDebug() << "No se pudo abrir el archivo CSV.";
    }

    // Ahora, songInfoList contiene detalles de las canciones
    // Puedes almacenar esta lista como una variable miembro de tu clase para acceder a ella en otros métodos.
}
void MainWindow::loadArtistSongsToModel(const QString& artistName) {
    // Limpia el modelo de datos actual
    model->removeRows(0, model->rowCount());
    for (QStringList* songs : loadedSongs) {
        free(songs); // Usar free en lugar de delete
    }
    loadedSongs.clear();

    // Invocar al recolector de basura para liberar la memoria inmediatamente
    QCoreApplication::processEvents();
    // Itera a través de la lista de songInfoList para encontrar canciones del artista seleccionado
    for (const SongInfo& songInfo : songInfoList) {
        if (songInfo.artist == artistName) {
            // Agrega la canción al modelo de datos
            QStandardItem* songItem = new QStandardItem(songInfo.trackFileName);
            model->appendRow(songItem);
        }
    }
}


