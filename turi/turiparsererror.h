#pragma once

#include <QString>
#include <QVector>
#include <defines.h>

class TuriParserError {
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

    static void clearErrorsList(QVector<TuriParserError *> & errors);
};
