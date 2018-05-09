#include <bits/exception.h>

#include <QString>

class ExceptionMessage : public std::exception {
    QString errorMessage;

  public:
    ExceptionMessage(QString _errorMessage);

    QString getErrorMessage();
};
