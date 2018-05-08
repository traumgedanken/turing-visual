#ifndef EXCEPTIONMESSAGE_H
#define EXCEPTIONMESSAGE_H

#include <bits/exception.h>

#include <QString>



class ExceptionMessage : public std::exception
{
    QString errorMessage;
public:
    ExceptionMessage(QString _errorMessage);
    QString getErrorMessage();
};

#endif // EXCEPTIONMESSAGE_H
