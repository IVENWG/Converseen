#include <QFileInfo>
#include <QFileIconProvider>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include "dialogmultipageeditor.h"
#include "ui_dialogmultipageeditor.h"

DialogMultipageEditor::DialogMultipageEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMultipageEditor)
{
    ui->setupUi(this);

    toolBar = new QToolBar(this);
    this->layout()->setMenuBar(toolBar);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolBar->setVisible(true);
    toolBar->addAction(ui->actionCheck);
    toolBar->addAction(ui->actionCheckAll);
    toolBar->addSeparator();
    toolBar->addAction(ui->actionUncheck);
    toolBar->addAction(ui->actionUncheckAll);
    toolBar->addSeparator();
    toolBar->addAction(ui->actionHelp);

    ui->treeWidget->setUniformRowHeights(true);
    ui->treeWidget->setAlternatingRowColors(true);

    connect(ui->pushOk, SIGNAL(clicked()), this, SLOT(acceptDialog()));
    connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(reject()));

#if defined(Q_OS_WIN)
    checkGsWinInstalled();
#endif
}

DialogMultipageEditor::~DialogMultipageEditor()
{
    delete ui;
}

void DialogMultipageEditor::readFile(QString fileName)
{
    analyzeMultipageFile(fileName);
}

QStringList DialogMultipageEditor::fileNames()
{
    return m_fileNames;
}

void DialogMultipageEditor::analyzeMultipageFile(QString fileName)
{
    m_currentFileName = fileName;

    MultipageConverter *mpc_aaa = new MultipageConverter();
    mpc_aaa->readFile(fileName);

    QFileInfo fi(fileName);
    QString baseFileName = fi.fileName();

    QList<MultipageItem> pagesList = mpc_aaa->pages();

    for (int i = 0; i < pagesList.count(); i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setCheckState(0, Qt::Checked);

        QFileIconProvider qficon;
        QIcon icon = qficon.icon(fi);

        item->setIcon(0, icon);

        QString numberedFileName = QString("%1[%2]")
                .arg(baseFileName)
                .arg(QString::number(i));
        item->setText(0, numberedFileName); // Filename + num

        item->setText(1, QString::number(i + 1));

        QString pxSize = QString("%1x%2 px")
                .arg(pagesList.at(i).w)
                .arg(pagesList.at(i).h);
        item->setText(2, pxSize);           // pixel size

        QString density = QString("x = %1 y = %2")
                        .arg(pagesList.at(i).xres)
                        .arg(pagesList.at(i).yres);
        item->setText(3, density);

        QString depth = QString("%1 bit")
                        .arg(pagesList.at(i).depth);
        item->setText(4, depth);            // depth in bit
    }
}

void DialogMultipageEditor::checkGsWinInstalled()
{
    if (isGhostscriptInstalled()) {
        return;
    }

    showGhostscriptInstallationDialog();
}

bool DialogMultipageEditor::isGhostscriptInstalled()
{
    const QStringList commands = {"gswin64c", "gswin32c"};
    const QStringList arguments = {"--version"};

    QProcess process;
    process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());

    for (const QString& command : commands) {
        process.start(command, arguments);

        if (!process.waitForStarted()) {
            continue;
        }

        if (process.exitCode() == 0) {
            const QByteArray output = process.readAllStandardOutput();

            return true;
        }
    }

    return false;
}

void DialogMultipageEditor::showGhostscriptInstallationDialog()
{
    const QString message = tr(
            "In order to perform the conversion of <b>PDF</b> files to images, <b>Ghostscript for Windows</b> must be installed on your system.<br><br> \
            Please install the correct version of <b>Ghostscript</b> and ensure it is available on your system PATH."
        );

    QMessageBox::warning(
        this,
        QApplication::applicationName(),
        message,
        QMessageBox::Ok
        );
}

void DialogMultipageEditor::acceptDialog()
{
    QTreeWidgetItemIterator it(ui->treeWidget);
    int i = 0;

    m_fileNames.clear();

    while (*it) {
        if ((*it)->checkState(0) == Qt::Checked) {
            QString s = QString("%1[%2]")
                    .arg(m_currentFileName)
                    .arg(QString::number(i));  // Complete filename + num

            m_fileNames.append(s);
        }
        ++it;
        i++;
    }

    accept();
}

/* Select all and select (true), unselect (false) */
void DialogMultipageEditor::selector(bool select_all, bool action_select)
{
    QTreeWidgetItemIterator it(ui->treeWidget);
    while (*it) {
        if (((*it)->isSelected()) || (select_all)) {
            if (action_select)
                (*it)->setCheckState(0, Qt::Checked);
            else
                (*it)->setCheckState(0, Qt::Unchecked);
        }
        ++it;
    }
}

void DialogMultipageEditor::on_actionCheck_triggered()
{
    selector(false, true);
}

void DialogMultipageEditor::on_actionCheckAll_triggered()
{
    selector(true, true);
}

void DialogMultipageEditor::on_actionUncheck_triggered()
{
    selector(false, false);
}

void DialogMultipageEditor::on_actionUncheckAll_triggered()
{
    selector(true, false);
}

void DialogMultipageEditor::on_actionHelp_triggered()
{
}

