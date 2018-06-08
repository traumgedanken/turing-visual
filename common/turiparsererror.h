#pragma once

#include "common_global.h"
#include "defines.h"
#include <QString>
#include <QVector>

class COMMONSHARED_EXPORT TuriParserError {
    // lines numeration starts from 0
    int line;
    QString errorText;
    ERROR_TYPE errorType;

  public:
    TuriParserError(ERROR_TYPE _errorType, int _line, QString _errorText);
    TuriParserError(ERROR_TYPE _errorType, QString _errorText);

    int getLine();

    QString getErrorText();

    ERROR_TYPE getErrorType();

    QString toQString();

    // free memory
    static void clearErrorsList(QVector<TuriParserError *> & errors);
};
