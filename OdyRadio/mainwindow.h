#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ReproductorMP3.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStringList>
#include <QListView>

class MainWindow : public QMainWindow
{
    Q_OBJECT

struct SongInfo {
    QString trackID;
    QString albumID;
    QString artist;
    QString trackFileName;
    QString songName;
};

private:
    ReproductorMP3 reproductorMP3;

    QStringList songList;
    int currentPage;
    int songsPerPage;
    int totalSongs;
    int totalPages;
    QStandardItemModel *model;
    QListView *listView;
    bool paginationMode;
    void togglePaginationMode();
    void loadNextPage();
    void loadPreviousPage();
    void handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void handleArtistSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void loadPagesToModel();
    QList<QStringList*> loadedSongs;
    void printMemoryUsageWithQProcess();
    QTimer *memoryUsageTimer;
    qint64 memoryUsage = -1;
    void scrollPages(int delta);
    void handleScrollValueChanged(int value);
    void iniciarReproduccion();
    void detenerReproduccion();
    void actualizarBarraProgreso();
    void cambiarPosicionReproduccion(int value) ;
    std::string formatTiempo(double tiempoSegundos);
    QTimer* timer ;
    QSlider *progressSlider ;
    double tiempoActual;
    void cargarDatosCSV(const QString &folderPath);
    QListView *artistListView;
    QStandardItemModel *artistModel;
    QString trackFileName;
    QString selectedArtist;
    void loadArtistSongsToModel(const QString& artistName) ;
    QList<SongInfo>  songInfoList;
    void handleSongSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
        void cargarCancionSeleccionada();
    QString songPath;
        QModelIndex selectedIndex;
    int currentIndex;
        void loadNextItem();
    void loadPrevItem();
        QString obtenerSongName(const QString &songPath);
    void mostrarInfoCancion();

        void openFolder();
    QString songsDirectory;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
