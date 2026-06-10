#ifndef PIXTREEWIDGET_H
#define PIXTREEWIDGET_H

#include <QTreeWidget>
#include <QList>

class QKeyEvent;
class QDropEvent;
class QDragEnterEvent;
class QDragMoveEvent;

class ImageAttributes
{
public:
    QString completeFileName, fileName, suffix, path, format, imgSize, imgRes;
    qint64 size;
};

class PixTreeWidget : public QTreeWidget {

    Q_OBJECT

    public:
        PixTreeWidget(QWidget *parent = 0);
        void addItems(QList<ImageAttributes> *iAList);
        void dropEvent(QDropEvent *event);
        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        void setListItems(QList<ImageAttributes> *iAList);
        void removeItems(QList<ImageAttributes> *iAList);
        bool thereAreItemsChecked();
        int countChecked();

    signals:
        void dropped(QStringList, QStringList);

    public slots:
        void checkItems();
        void checkAllItems();
        void uncheckItems();
        void uncheckAllItems();
        void invertCheckAllItems();
};

#endif // PIXTREEWIDGET_H
