#pragma once

#include "common_global.h"
#include <QString>

// class to get text from exception
class COMMONSHARED_EXPORT ExceptionMessage : public std::exception {
    QString errorMessage;

  public:
    ExceptionMessage(QString _errorMessage);

    QString getErrorMessage();
};
