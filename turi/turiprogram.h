#ifndef TURIPROGRAM_H
#define TURIPROGRAM_H

#include <QVector>
#include <turicommand.h>

class TuriProgram
{
    QVector<TuriCommand *> program;
public:
    TuriProgram();

    void addCommand(TuriCommand * newCommand);
    TuriCommand * getCommand(int index);
    int numberOfCommand();
    QVector<QString> getErrors();
};

#endif // TURIPROGRAM_H
