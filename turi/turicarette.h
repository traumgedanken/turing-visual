#ifndef TURICARETTE_H
#define TURICARETTE_H

#include <QString>
#include <turiprogram.h>


class TuriCarette
{
    QString word;
    int position;
    QString currentState;
public:
    TuriCarette(QString _word);
    int exec(TuriProgram * program);
    QString getResult();
private:
    void move(DIRECTION direction);
    QChar getSymbol();
    void setSymbol(QChar symbol);
    void setState(QString state);

};

#endif // TURICARETTE_H
