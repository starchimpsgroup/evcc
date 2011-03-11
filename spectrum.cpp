#include "spectrum.h"

Spectrum::Spectrum(QByteArray * byteArray, QWidget *parent) :
    QWidget(parent)
{
    _byteArray = byteArray;
}

void Spectrum::paintEvent(QPaintEvent *)
{
    QPainter painter( this );
    painter.setBrush( Qt::black );
    painter.setPen(Qt::NoPen);
    painter.drawRect( 0, 0, width(), height() );
    painter.setPen(Qt::green);
    painter.drawLine(0, height()/2, width(), height()/2);
}
