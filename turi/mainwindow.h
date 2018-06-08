#pragma once

#include "client.h"
#include <QCloseEvent>
#include <QMainWindow>
#include <QTableWidget>
#include <graphwidget.h>
#include <turiprogram.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    Ui::MainWindow * ui;
    TuriProgram * program = nullptr;
    Client * client = nullptr;
    GraphWidget * graph = nullptr;
    bool fileIsSaved = false;
    bool codeEditedByUser = true;
    bool itemChangedByUser = true;
    bool askToSave = true;

    // to display errors
    QVector<TuriParserError *> errors;

    // to remember filename after opening a file
    QString fileName;

    // to determine if code text was edited
    QString originCodeText;

    // user id for using carriage on server
    int carriageIndex = -1;

  public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();

  private slots:
    // parse program every time code text was edited
    void on_codeEdit_textChanged();

    // working with files
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();

    void on_inputEdit_textChanged(const QString & arg1);

    void closeEvent(QCloseEvent * ev);
    void on_actionExit_triggered();

    void on_actionAbout_Turi_IDE_triggered();

    void on_setupBtn_clicked();
    void on_actionSetup_triggered();

    void on_runBtn_clicked();
    void on_actionRun_triggered();

    void on_prevBtn_clicked();
    void on_actionPrevious_step_triggered();

    void on_nextBtn_clicked();
    void on_actionNext_step_triggered();

    void on_resetBtn_clicked();
    void on_actionReset_triggered();

    void on_resetAndRunBtn_clicked();
    void on_actionReset_and_run_triggered();

    void on_moveLeftBtn_clicked();

    void on_moveRightBtn_clicked();

    void on_suckingDickPushBtn_clicked();

    // to edit text code according to new values in table
    void on_tableWidget_itemChanged(QTableWidgetItem * item);

  private:
    void printProgramTable();

    void printErrorsList();

    void validateSetupBtn();

    // ask user for save code if it is needed
    int onClose();

    void printProgram();

    // mark current line according to current carriage state
    void markCurrentLine(int line);

    void setLineCounter();

    void updateGraph();

    void exchangeLine(int row, QString newLine);
};
