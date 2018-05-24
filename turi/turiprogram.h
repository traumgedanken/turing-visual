#pragma once

#include "turiparsererror.h"

#include <QVector>
#include <turicommand.h>

class TuriProgram {
    QVector<TuriCommand *> program;
    QVector<TuriParserError *> errors;

  public:
    TuriProgram();

    ~TuriProgram();

    void addCommand(TuriCommand * newCommand);

    TuriCommand * getCommand(int index);

    int count();

    QVector<QString> getErrors();

    QString toJson();

    void setErrorsList(QVector<TuriParserError *> & _errors);

    TuriCommand * getCommandWithCurrentState(QString _currentState);
};
