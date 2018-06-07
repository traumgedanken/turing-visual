#pragma once

#include "common_global.h"
#include "turicommand.h"
#include "turiparsererror.h"

class COMMONSHARED_EXPORT TuriProgram {
    QVector<TuriCommand *> program;
    QVector<TuriParserError *> errors;

  public:
    TuriProgram();

    ~TuriProgram();

    void addCommand(TuriCommand * newCommand);

    void removeCommands();

    TuriCommand * getCommand(int index);

    int count();

    QVector<TuriParserError *> getErrors();

    void setErrorsList(QVector<TuriParserError *> & _errors);

    TuriCommand * getCommandWithCurrentState(QString _currentState);
};
