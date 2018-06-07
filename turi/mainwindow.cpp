#include "mainwindow.h"
#include "defines.h"
#include "htmltext.h"
#include "turicarriagepainter.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QTextCodec>
#include <QTextStream>

#define PROCESS_FALSE_RESPONSE                                                 \
    if (res.status == NETWORK_FALSE) {                                         \
        QMessageBox::information(this, "ERROR", res.word);                     \
        return;                                                                \
    }

#define PROCESS_ERROR_RESPONSE                                                 \
    if (res.status == NETWORK_ERROR_CODE) {                                    \
        QMessageBox::critical(this, "ERROR", res.word);                        \
        close();                                                               \
    }

#define TRY_GET_RESPONSE(REQUEST)                                              \
    Response res;                                                              \
    try {                                                                      \
        client = new Client(REQUEST, this);                                    \
        res = client->getResponse();                                           \
        delete client;                                                         \
    } catch (std::exception) {                                                 \
        QMessageBox::critical(this, "ERROR", "Error connecting to server");    \
        close();                                                               \
        return;                                                                \
    }                                                                          \
    PROCESS_ERROR_RESPONSE

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
    QString name = "Graph";
    ui->tabWidget->addTab(graph, name);
    setLineCounter();
    show();
    Request req(FN_NONE);
    TRY_GET_RESPONSE(req);
}

MainWindow::~MainWindow() {
    delete ui;
    delete graph;
}

void MainWindow::printProgram() {
    codeEditedByUser = false;
    HtmlText textEditor(ui->codeEdit->toPlainText());
    textEditor.markErrors(errors);
    QTextCursor cursor = ui->codeEdit->textCursor();
    int position = cursor.position();
    ui->codeEdit->setHtml(textEditor.getText());
    cursor.setPosition(position);
    ui->codeEdit->setTextCursor(cursor);
    cursor = ui->codeEdit->textCursor();
    codeEditedByUser = true;
}

void MainWindow::on_codeEdit_textChanged() {
    fileIsSaved = false;
    if (!codeEditedByUser) return;
    QString codeText = ui->codeEdit->toPlainText();
    if (codeText.isEmpty()) {
        ui->errorsList->clear();
        ui->tabWidget->setTabText(1, "Errors");
        return;
    }
    if (!codeText.endsWith('\n')) codeText.append('\n');
    Request req(FN_PARSE_PROGRAM, -1, codeText);
    TRY_GET_RESPONSE(req)
    if (program != nullptr) delete program;
    program = res.program;
    errors = program->getErrors();
    printErrorsList();
    printProgram();
    validateSetupBtn();
    if (errors.isEmpty()) {
        printProgramTable();
        ui->tabWidget->setTabText(1, "Errors");
        ui->tabWidget->removeTab(2);
        QString name = "Graph";
        graph = new GraphWidget(this, program);
        ui->tabWidget->addTab(graph, name);
    } else {
        QString errorTabTitle =
            "Errors (" + QString::number(errors.length()) + ")";
        ui->tabWidget->setTabText(1, errorTabTitle);
    }
}

void MainWindow::printErrorsList() {
    ui->errorsList->clear();
    for (auto error : errors) {
        ui->errorsList->addItem(error->toQString());
    }
}

void MainWindow::printProgramTable() {
    ui->tableWidget->setRowCount(program->count());
    for (int i = 0; i < program->count(); i++) {
        TuriCommand * currentCommand = program->getCommand(i);
        ui->tableWidget->setItem(
            i, 0, new QTableWidgetItem(currentCommand->getCurrentState()));
        ui->tableWidget->setItem(
            i, 1, new QTableWidgetItem(currentCommand->getCurrentSymbol()));
        ui->tableWidget->setItem(
            i, 2, new QTableWidgetItem(currentCommand->getNextSymbol()));
        QString direction =
            TuriCommand::directionToQString(currentCommand->getDirection());
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(direction));
        ui->tableWidget->setItem(
            i, 4, new QTableWidgetItem(currentCommand->getNextState()));
    }
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::on_actionOpen_triggered() {
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("TURI files (*.turi)"));
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        fileName = fileNames.at(0);

        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QByteArray a = file.readAll();
        QString codeStr = QTextCodec::codecForMib(106)->toUnicode(a);
        file.close();

        originCodeText = codeStr;
        ui->codeEdit->setPlainText(codeStr);
        fileIsSaved = true;
    }
}

void MainWindow::on_setupBtn_clicked() {
    Request req(FN_CARRIAGE_CREATE, carriageIndex, ui->inputEdit->text(), program);
    TRY_GET_RESPONSE(req)
    PROCESS_FALSE_RESPONSE
    carriageIndex = res.id;
    TuriCarriagePainter::draw(ui->outputResult, res.word, res.state, 5);
    markCurrentLine(res.line);
    ui->nextBtn->setEnabled(true);
    ui->runBtn->setEnabled(true);
    ui->resetBtn->setEnabled(true);
    ui->resetAndRunBtn->setEnabled(true);
    ui->moveLeftBtn->setEnabled(true);
    ui->moveRightBtn->setEnabled(true);
}

void MainWindow::on_inputEdit_textChanged(const QString & arg1) {
    validateSetupBtn();
}

void MainWindow::validateSetupBtn() {
    int errorNumber = errors.length();
    bool validity = !ui->inputEdit->text().isEmpty() &&
                    !ui->codeEdit->toPlainText().isEmpty() && errorNumber == 0;
    ui->setupBtn->setEnabled(validity);
    if (!validity) {
        ui->prevBtn->setEnabled(validity);
        ui->nextBtn->setEnabled(validity);
        ui->runBtn->setEnabled(validity);
        ui->resetBtn->setEnabled(validity);
        ui->resetAndRunBtn->setEnabled(validity);
    }
}

void MainWindow::on_actionSave_triggered() {
    if (fileName.isEmpty()) {
        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setDefaultSuffix("turi");
        QStringList fileNames;
        if (dialog.exec()) {
            fileNames = dialog.selectedFiles();
            fileName = fileNames.at(0);
            fileIsSaved = true;
        }
    }

    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << ui->codeEdit->toPlainText();
    file.close();
    fileIsSaved = true;
}

void MainWindow::on_actionSave_as_triggered() {
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("turi");
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        fileName = fileNames.at(0);
        fileIsSaved = true;
    }

    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << ui->codeEdit->toPlainText();
    file.close();
    fileIsSaved = true;
}

int MainWindow::onClose() {
    if (fileIsSaved || ui->codeEdit->toPlainText() == originCodeText ||
        (ui->codeEdit->toPlainText().isEmpty() && fileName.isEmpty())) {
        return 0;
    }
    QMessageBox::StandardButton res = QMessageBox::question(
        this, "On close", "Code was edited.\nDo you want to save it?");
    if (res == QMessageBox::StandardButton::Yes) {
        on_actionSave_triggered();
    } else
        return 0;
    if (fileIsSaved) {
        return 0;
    } else
        return 1;
}

void MainWindow::closeEvent(QCloseEvent * ev) {
    if (onClose() == 0) {
        ev->accept();
    } else
        ev->ignore();
    if (carriageIndex != -1) {
        Request req(FN_CARRIAGE_DELETE, carriageIndex);
        try {
            client = new Client(req, this);
            delete client;
        } catch(std::exception) {}
    }
}

void MainWindow::on_actionExit_triggered() {
    if (onClose() == 0) exit(0);
}

void MainWindow::on_actionAbout_Turi_IDE_triggered() {
    QString info = "Turi IDE v1.0\n"
                   "Developer: Ihor Bulaevsky\n"
                   "Course work at KPI\n"
                   "Teacher: Hadyniak, R.A.\n\n\n"
                   "Donate: 5167 9856 9006 2179";
    QMessageBox::information(this, "About", info);
}

void MainWindow::markCurrentLine(int line) {
    HtmlText textEditor(ui->codeEdit->toPlainText());
    textEditor.markLine(line);
    codeEditedByUser = false;
    ui->codeEdit->setHtml(textEditor.getText());
    codeEditedByUser = true;
}

void MainWindow::on_nextBtn_clicked() {
    Request req(FN_CARRIAGE_NEXT, carriageIndex, "");
    TRY_GET_RESPONSE(req)
    bool succes = res.status == NETWORK_TRUE;
    TuriCarriagePainter::draw(ui->outputResult, res.word, res.state, res.id);
    if (!succes) {
        codeEditedByUser = false;
        ui->codeEdit->setText(ui->codeEdit->toPlainText());
        codeEditedByUser = true;
        ui->nextBtn->setEnabled(false);
        ui->runBtn->setEnabled(false);
    } else {
        markCurrentLine(res.line);
        ui->prevBtn->setEnabled(true);
    }
}

void MainWindow::on_prevBtn_clicked() {
    Request req(FN_CARRIAGE_PREV, carriageIndex, "");
    TRY_GET_RESPONSE(req)
    bool succes = res.status == NETWORK_TRUE;
    TuriCarriagePainter::draw(ui->outputResult, res.word, res.state, res.id);
    if (!succes) {
        ui->prevBtn->setEnabled(false);
    } else {
        ui->nextBtn->setEnabled(true);
        ui->runBtn->setEnabled(true);
    }
    markCurrentLine(res.line);
}

void MainWindow::on_runBtn_clicked() {
    Request req(FN_CARRIAGE_RUN, carriageIndex, "");
    TRY_GET_RESPONSE(req);
    TuriCarriagePainter::draw(ui->outputResult, res.word, res.state, res.id);
    ui->runBtn->setEnabled(false);
    ui->nextBtn->setEnabled(false);
    ui->prevBtn->setEnabled(true);
    ui->codeEdit->setText(ui->codeEdit->toPlainText());
}

void MainWindow::on_resetBtn_clicked() {
    Request req(FN_CARRIAGE_WORD, carriageIndex, "");
    TRY_GET_RESPONSE(req)
    QString newWord = res.word;
    req = Request(FN_CARRIAGE_CREATE, carriageIndex, newWord, program);
    client = new Client(req, this);
    res = client->getResponse();
    delete client;
    PROCESS_FALSE_RESPONSE
    carriageIndex = res.id;
    TuriCarriagePainter::draw(ui->outputResult, res.word, res.state, 5);
    markCurrentLine(res.line);
    ui->prevBtn->setEnabled(false);
    ui->nextBtn->setEnabled(true);
    ui->runBtn->setEnabled(true);
    ui->resetBtn->setEnabled(true);
    ui->resetAndRunBtn->setEnabled(true);
}

void MainWindow::on_resetAndRunBtn_clicked() {
    on_resetBtn_clicked();
    on_runBtn_clicked();
}

void MainWindow::on_moveLeftBtn_clicked() {
    Request req(FN_CARRIAGE_LEFT, carriageIndex, "");
    TRY_GET_RESPONSE(req)
    TuriCarriagePainter::draw(ui->outputResult, res.word, res.state, res.id);
}

void MainWindow::on_moveRightBtn_clicked() {
    Request req(FN_CARRIAGE_RIGHT, carriageIndex, "");
    TRY_GET_RESPONSE(req)
    TuriCarriagePainter::draw(ui->outputResult, res.word, res.state, res.id);
}

void MainWindow::setLineCounter() {
    QString res;
    int lineNumber = 30;
    for (int i = 1; i < lineNumber; i++) {
        if (i < 10) res += " ";
        res += QString::number(i) + "\n";
    }
    ui->lineCountLabel->setText(res);
}

void MainWindow::on_actionSetup_triggered() {
    if (ui->setupBtn->isEnabled())
        on_setupBtn_clicked();
}

void MainWindow::on_actionPrevious_step_triggered() {
    if (ui->prevBtn->isEnabled())
        on_prevBtn_clicked();
}

void MainWindow::on_actionNext_step_triggered() {
    if (ui->nextBtn->isEnabled())
        on_nextBtn_clicked();
}

void MainWindow::on_actionRun_triggered() {
    if (ui->runBtn->isEnabled())
        on_runBtn_clicked();
}

void MainWindow::on_actionReset_triggered() {
    if (ui->resetBtn->isEnabled())
        on_resetBtn_clicked();
}

void MainWindow::on_actionReset_and_run_triggered() {
    if (ui->resetAndRunBtn->isEnabled())
        on_resetAndRunBtn_clicked();
}
