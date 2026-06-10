#ifndef PUSHCOLORCHOOSER_H
#define PUSHCOLORCHOOSER_H

#include <QPushButton>

class PushColorChooser : public QPushButton
{
    Q_OBJECT
public:
    explicit PushColorChooser(QWidget *parent = 0);
    void setColor(QColor color);

signals:

public slots:

};

#endif // PUSHCOLORCHOOSER_H
