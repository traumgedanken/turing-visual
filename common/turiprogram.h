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

    // remove all commands, free memory
    void removeCommands();

    TuriCommand * getCommand(int index);

    // return number of commands
    int count();

    QVector<TuriParserError *> getErrors();

    void setErrorsList(QVector<TuriParserError *> & _errors);

    // return first command with sprecified current state
    TuriCommand * getCommandWithCurrentState(QString _currentState);
};
