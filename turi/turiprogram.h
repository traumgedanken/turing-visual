#pragma once

#include <QVector>
#include <turicommand.h>

class TuriProgram {
    QVector<TuriCommand *> program;

  public:
    TuriProgram();

    ~TuriProgram();

    void addCommand(TuriCommand * newCommand);

    TuriCommand * getCommand(int index);

    int count();

    QVector<QString> getErrors();
};
