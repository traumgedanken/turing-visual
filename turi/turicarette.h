#include <QString>
#include <turiprogram.h>
#include <QLabel>

class TuriCarette {
    QString word;
    int position;
    QString currentState;
    QLabel * label = nullptr;

  public:
    TuriCarette(QString _word, QLabel *_label);

    int exec(TuriProgram * program);

    QString getResult();

    void drawTable();

  private:
    void move(DIRECTION direction);

    QChar getSymbol();

    void setSymbol(QChar symbol);

    void setState(QString state);

    void drawWord();
};
