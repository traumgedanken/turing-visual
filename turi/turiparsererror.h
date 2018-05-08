#ifndef PARSERERROR_H
#define PARSERERROR_H

#include <QString>

class TuriParserError
{
    int line;
    QString errorText;
public:
    TuriParserError(int _line, QString _errorText);
    int getLine();
    QString getErrorText();
};

#endif // PARSERERROR_H
