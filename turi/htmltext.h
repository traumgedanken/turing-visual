#pragma once

#include <turiparsererror.h>

class HtmlText {
    QString text;
    QString ERROR_COLOR = "#E9967A";
    QString WARNING_COLOR = "#FFE4C4";
    QString LINE_COLOR = "#32CD32";

  public:
    HtmlText(QString _text);

    QString getText();

    void markErrors(QVector<TuriParserError *> & errors);

    void markLine(int index);
};
