#pragma once

#include <turiparsererror.h>

class HtmlText {
    QString text;
    const QString ERROR_COLOR = "#E9967A";
    const QString WARNING_COLOR = "#FFE4C4";
    const QString LINE_COLOR = "#32CD32";

  public:
    HtmlText(QString _text);

    QString getText();

    void markErrors(QVector<TuriParserError *> & errors);

    // mark line with LINE_COLOR
    void markLine(int index);
};
