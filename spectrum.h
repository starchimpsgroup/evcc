#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QWidget>
#include <QPainter>

class Spectrum : public QWidget
{
    Q_OBJECT
public:
    explicit Spectrum(QByteArray * byteArray, QWidget *parent = 0);

private:
        virtual void paintEvent(QPaintEvent*);
        QByteArray * _byteArray;

signals:

public slots:

};

#endif // SPECTRUM_H
