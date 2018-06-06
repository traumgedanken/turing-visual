#include "turicarriagepainter.h"

#include <QPainter>
#include <QPicture>

void TuriCarriagePainter::draw(QLabel * label, QString & word, QString & state, int position) {
    label->clear();
    QPicture pi;
    QPainter p(&pi);

    p.setRenderHint(QPainter::Antialiasing);

    p.setPen(Qt::NoPen);
    if (state == "!") {
        p.setBrush(QColor(255, 150, 140));
    } else
        p.setBrush(QColor(120, 180, 150));
    p.drawRect((position - 1) * cellWidth, 0, cellWidth, cellHeigth);

    p.setPen(QPen(Qt::darkGray, 2));
    p.drawLine(0, 0, carriageWidth, 0);
    p.drawLine(0, cellHeigth, carriageWidth, cellHeigth);
    p.drawRect((position - 1) * cellWidth, cellHeigth, cellWidth, 25);
    for (int i = cellWidth; i < carriageWidth; i += cellWidth) {
        p.drawLine(i, 0, i, cellHeigth);
    }

    p.setPen(QPen(Qt::black));
    QFont font = p.font();
    font.setPointSize(25);
    p.setFont(font);
    for (int i = 0; i < cellNumber; i++) {
        QString ch = word.at(i);
        p.drawText(i * cellWidth, 0, cellWidth, cellHeigth, Qt::AlignCenter,
                   ch);
    }
    font.setPointSize(10);
    p.setFont(font);
    p.drawText((position - 1) * cellWidth, cellHeigth, cellWidth, 25,
               Qt::AlignCenter, state);
    p.end();
    label->setPicture(pi);
}

