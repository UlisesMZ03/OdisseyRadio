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
#include "ReproductorMP3.h" // Asegúrate de incluir correctamente el encabezado de ReproductorMP3

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    setWindowTitle("OdisseyRadio");
    setGeometry(100, 100, 800, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

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

    connect(leftListViewFolders, &QListWidget::itemSelectionChanged, this, [=]() {
        rightListViewSongs->clear();
        QListWidgetItem *selectedFolderItem = leftListViewFolders->currentItem();
        if (selectedFolderItem) {
            QString folderPath = selectedFolderItem->text();

            QDir folderDir(folderPath);
            QStringList songFiles = folderDir.entryList(QStringList() << "*.mp3", QDir::Files);
            rightListViewSongs->addItems(songFiles);
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

    QPushButton *pauseButton = new QPushButton(playerWidget);
    pauseButton->setIcon(QIcon::fromTheme("media-playback-pause"));
    pauseButton->setIconSize(QSize(32, 32));
    pauseButton->setFixedSize(32, 32);
    pauseButton->setFlat(true);
    pauseButton->setCursor(Qt::PointingHandCursor);

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

    // Agregar un botón para cargar una canción
    QPushButton *loadSongButton = new QPushButton("Cargar Canción", centralWidget);
    mainLayout->addWidget(loadSongButton, 2, 0, 1, 2);

    // Conectar el botón para cargar una canción a una función
    connect(loadSongButton, &QPushButton::clicked, this, &MainWindow::cargarCancion);

}

void MainWindow::cargarCancion() {
    QString filePath = QFileDialog::getOpenFileName(this, "Seleccionar Canción", QDir::homePath(), "Archivos de música (*.mp3)");

    if (!filePath.isEmpty()) {
        const char *cFilePath = filePath.toUtf8().constData();

        if (reproductorMP3.cargarCancion(cFilePath)) {
            // La canción se cargó con éxito, ahora la reproducimos
            reproductorMP3.reproducir();
        }
    }
}


MainWindow::~MainWindow()
{

}
