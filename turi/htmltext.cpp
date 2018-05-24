#include "htmltext.h"
#include <QDebug>

HtmlText::HtmlText(QString _text) { text = _text; }

QString HtmlText::getText() { return text; }

void HtmlText::markErrors(QVector<TuriParserError *> & errors) {
    QStringList strList = text.split('\n');
    QString start = "<font style=\"background-color:" + ERROR_COLOR + "\">";
    QString end = "</font>";
    QString newLine = "<br/>";
    int errorIndex = 0;
    text.clear();
    for (int i = 0; i < strList.length(); i++) {
        if (errorIndex < errors.size() &&
            i == errors.at(errorIndex)->getLine() - 1) {
            text.append(start + strList.at(i) + end + newLine);
            errorIndex++;
        } else
            text.append(strList.at(i) + newLine);
    }
}
