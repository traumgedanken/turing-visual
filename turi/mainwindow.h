#include <QMainWindow>
#include <turiparsererror.h>
#include <turiprogram.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    TuriProgram * program = nullptr;
    QVector<TuriParserError *> errors;

  public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();

  private slots:
    void on_codeEdit_textChanged();

    void on_actionOpen_triggered();

    void on_actionLight_triggered();

    void on_runBtn_clicked();

    void on_inputEdit_textChanged(const QString & arg1);

  private:
    Ui::MainWindow * ui;

    void printProgramTable();

    void printErrorsList();

    void validateRunBtn();
};
