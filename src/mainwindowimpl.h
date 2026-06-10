#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

#include <QMainWindow>
#include <QFileInfo>
#include <QList>
#include <QSettings>
#include <QDropEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QDirIterator>
#include "ui_mainwindow.h"
#include "converter.h"
#include "dialogquality.h"
#include "dialogconversionstatus.h"
#include "pixtreewidget.h"
#include "mylabelpreviewer.h"
#include "cachingsystem.h"
#include "magickdefine.h"

class QDropEvent;

class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
public:
    MainWindowImpl( QWidget * parent = 0);
    ~MainWindowImpl();
    void importListFromArgv(QString fileName);
    void printSupportedFormats();
    void loadTheme();

private:
    void createActions();
    void setupMenu();
    void createContextMenu();
    void addContextMenuSeparator();
    void loadDirectoryFiles(const QString &directory, const QStringList &readableFiltersList, const QDirIterator::IteratorFlag &flag);
    void loadFiles(QStringList fileNames);
    void setFileInList(QString fileName);
    bool checkDuplicates(QStringList fileNames, int e);
    void convert();
    void fillTreeView();
    void loadFormats();
    void loadOptions();
    void loadQuality();

    void deleteItems(int selectedItems);

    void loadPreview(QString fileName);
    void showPreviewAndInfos();   // quello che succede alla selezione dell'item.

    void resetDisplays();

    void checkVersion();

    void saveSettings();    // This method is called when the program is closed

    QString renameFileNameOnPrefixSuffix(QString oldFileName);
    QString renameFileNameOnProgressiveN(QString oldFileName);
    QString destinationPath();

    QList<ImageAttributes> *iAList;

    Converter *convertThread;
    DialogConversionStatus *dlgCStatus;

    int curr_index;             // Indice dell'immagine che si sta processando dalla lista iAList

    int m_jpgQuality;             // Imposta la qualità jpg anche nel file ini
    int m_pngQuality;             // Imposta la qualità png anche nel file ini

    bool m_isWebPLosslessCompression;
    int m_webPCompression;
    int m_webPQuality;
    bool m_iskWebPDithering;

    bool m_removeMetadata;

    int img_width,img_height;  /* vengono riempiti solo quando un'immagine è selezionata.
                                  Riempire con dati su ImageAttributes potrebbe essere lento e non conveniente! */
    int new_img_width,new_img_height; /* Vengono riempiti con numeri che rappresentano i pixel dato che non capisco come
                                         si passa la corretta percentuale in virgola mobile a geometry*/
    double m_xResolution, m_yResolution;

    bool abort_all;

    int m_progressiveNren;      // Incremented when the progressive number renaming is checked

    QString m_bgColor;

    IMFilterType m_resamplingFilter;

private slots:
    void dropped(QStringList fileNames, QStringList directories);
    void openFiles();
    void addFiles();
    void openImportDirectoryDialog();
    void importIcoFile();
    void importPdfFile();

    void openMultipageFile(QString fileName);   // !
    void elabora();
    void setCurrentDirectory();
    void openOutDirectory();
    void removeItems();
    void removeAllItems();
    void setQuality();
    void selectGeometryUnit(QString unit);
    void relativeSizeW();
    void relativeSizeH();

    void showImageInformations(int orig_w, int orig_h, double orig_dens_x, double orig_dens_y);

    void onItemSelection();
    void editSettings();

    void about();

    void enableRenameLine();
    void enableShowPreview();
    void enableProgressiveSpin();

    void showNewSizePreview();

    void onPushResetClick();

    void startConversion();
    void nextConversion(int conv_status);

    void overwriteDialog(QString baseName);

    void stopProcess();
    void errorMessage(QString err_status);

    void choseBackgroundColor();

    void setRelativeSizeCheckboxes(int state);
    void setOverwriteStatus();
};
#endif // MAINWINDOWIMPL_H
