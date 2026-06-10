#include <QKeyEvent>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDir>
#include <QFileInfo>
#include <QMimeData>
#include <QUrl>
#include "pixtreewidget.h"
#include "formats.h"
#include "sizeutil.h"
#include "cachingsystem.h"

PixTreeWidget::PixTreeWidget(QWidget *parent):QTreeWidget(parent)
{
    QStringList headers;
    headers.append(tr("To convert"));
    headers.append(tr("Status"));
    headers.append(tr("File name"));
    headers.append(tr("Image type"));
    headers.append(tr("Image size"));
    headers.append(tr("Resolution"));
    headers.append(tr("File size"));
    headers.append(tr("File path"));

    setRootIsDecorated(false);
    setAlternatingRowColors(true);

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setHeaderLabels(headers);
    setUniformRowHeights(true);
    /*setSortingEnabled(true);
    sortItems(0, Qt::AscendingOrder);*/
    setAcceptDrops(true);
}

void PixTreeWidget::addItems(QList<ImageAttributes> *iAList)
{
    int cnt = iAList->count();

    for (int i = 0; i < cnt; i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setCheckState(0, Qt::Checked);
        item->setText(2, iAList->at(i).fileName);
        item->setText(3, iAList->at(i).suffix);
        item->setText(4, iAList->at(i).imgSize);
        item->setText(5, iAList->at(i).imgRes);
        item->setText(6, SizeUtil::simplifyFileSize(iAList->at(i).size));
        item->setText(7, QDir::toNativeSeparators(iAList->at(i).path));
    }
}

void PixTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void PixTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void PixTreeWidget::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    QStringList fileNames;
    QStringList directories;
    QFileInfo fi;

    for (int i = 0; i < urls.count(); i++) {
        QString localFile = urls.at(i).toLocalFile();

        fi.setFile(localFile);

        if (fi.isDir()) {
            directories << urls.at(i).toLocalFile().toUtf8();
        }
        if (fi.isFile()) {
            QStringList readableFomats = Formats::readableFilters();

            if (readableFomats.contains(fi.suffix(), Qt::CaseInsensitive))
                fileNames << urls.at(i).toLocalFile().toUtf8();
        }
    }

    if ((!fileNames.isEmpty()) || (!directories.isEmpty()))
        emit dropped(fileNames, directories);
}

void PixTreeWidget::checkItems()
{
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if ((*it)->isSelected())
            (*it)->setCheckState (0, Qt::Checked);
        ++it;
    }
}

void PixTreeWidget::checkAllItems()
{
    QTreeWidgetItemIterator it(this);
    while (*it) {
        (*it)->setCheckState (0, Qt::Checked);
        ++it;
    }
}

void PixTreeWidget::uncheckItems()
{
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if ((*it)->isSelected())
            (*it)->setCheckState (0, Qt::Unchecked);
        ++it;
    }
}

void PixTreeWidget::uncheckAllItems()
{
    QTreeWidgetItemIterator it(this);
    while (*it) {
        (*it)->setCheckState (0, Qt::Unchecked);
        ++it;
    }
}

void PixTreeWidget::invertCheckAllItems()
{
    QTreeWidgetItemIterator it(this);
    while (*it) {
        Qt::CheckState currentState = (*it)->checkState(0);
        if (currentState == Qt::Checked)
            (*it)->setCheckState(0, Qt::Unchecked);
        else if (currentState == Qt::Unchecked)
            (*it)->setCheckState(0, Qt::Checked);
        // PartiallyChecked rimane invariato
        ++it;
    }
}

void PixTreeWidget::removeItems(QList<ImageAttributes> *iAList)
{
    for (int i = this->topLevelItemCount() - 1; i >= 0; i--)
        if ((this->topLevelItem(i))->isSelected()) {
            QString s = iAList->at(i).completeFileName;

            this->takeTopLevelItem(i);
            iAList->removeAt(i);
            CachingSystem::remove(s);
        }
}

bool PixTreeWidget::thereAreItemsChecked()
{
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if ((*it)->checkState(0) == Qt::Checked)
            return true;
        ++it;
    }

    return false;
}

int PixTreeWidget::countChecked()
{
    int cnt = 0;

    QTreeWidgetItemIterator it(this);
    while (*it) {
        if ((*it)->checkState(0) == Qt::Checked)
            cnt++;
        ++it;
    }

    return cnt;
}
