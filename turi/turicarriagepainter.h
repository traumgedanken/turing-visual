#ifndef TURICARRIAGEPAINTER_H
#define TURICARRIAGEPAINTER_H

#include <QLabel>
#include <QString>

class TuriCarriagePainter
{
    static const int carriageWidth = 800;
    static const int cellNumber = 20;
    static const int cellWidth = carriageWidth / cellNumber;
    static const int cellHeigth = 50;
public:
    static void draw(QLabel * label, QString & word, QString & state, int position);
};

#endif // TURICARRIAGEPAINTER_H
