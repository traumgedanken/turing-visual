#include "turiparsererror.h"

TuriParserError::TuriParserError(ERROR_TYPE _errorType, int _line, QString _errorText) {
    errorType = _errorType;
    line = _line;
    errorText = _errorText;
}

TuriParserError::TuriParserError(ERROR_TYPE _errorType, QString _errorText) {
    errorType = _errorType;
    line = 0;
    errorText = _errorText;
}

int TuriParserError::getLine() { return line; }
QString TuriParserError::getErrorText() { return errorText; }
ERROR_TYPE TuriParserError::getErrorType() { return errorType; }
QString TuriParserError::toQString() {
    QString errorTypeStr = errorType == ERROR ? "ERROR" : "WARNING";
    QString errorQString = errorTypeStr;
    if (line != 0) {
         errorQString += " at line " + QString::number(line);
    }
    errorQString += " : " + errorText;
    return errorQString;
}

void TuriParserError::clearErrorsList(QVector<TuriParserError *> & errors) {
    for (TuriParserError * error : errors) {
        delete error;
    }
}
