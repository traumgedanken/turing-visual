#include "turiparsererror.h"

TuriParserError::TuriParserError(int _line, QString _errorText) {
    line = _line;
    errorText = _errorText;
}

int TuriParserError::getLine() { return line; }
QString TuriParserError::getErrorText() { return errorText; }
