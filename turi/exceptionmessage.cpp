#include "exceptionmessage.h"

ExceptionMessage::ExceptionMessage(QString _errorMessage)
{
    errorMessage = _errorMessage;
}

QString ExceptionMessage::getErrorMessage() { return errorMessage; }
