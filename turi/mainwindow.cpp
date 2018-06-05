#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPicture>
#include <QTabWidget>
#include <QTextCodec>
#include <QTextStream>
#include <QThread>
#include <htmltext.h>
#include <iostream>
#include <turicarriage.h>
#include <turiparser.h>

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
    QString name = "Graph";
    ui->tabWidget->addTab(graph, name);
    carriage = TuriCarriage(ui->outputResult);
    setLineCounter();
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
    Request req(FN_PARSE_PROGRAM, codeText);
    client = new Client(req, this);
    Response res = client->getResponse();
    delete client;
    if (program != nullptr) delete program;
    program = res.program;
    errors = program->getErrors();
    printErrorsList();
    printProgram();
    if (errors.isEmpty()) {
        printProgramTable();
        validateSetupBtn();
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
    try {
        QString firstState = program->getCommand(0)->getCurrentState();
        carriage = TuriCarriage(ui->inputEdit->text(), ui->outputResult,
                                firstState, program);
        markCurrentLine();
        ui->nextBtn->setEnabled(true);
        ui->runBtn->setEnabled(true);
        ui->resetBtn->setEnabled(true);
        ui->resetAndRunBtn->setEnabled(true);
        ui->moveLeftBtn->setEnabled(true);
        ui->moveRightBtn->setEnabled(true);
    } catch (std::exception) {
        QMessageBox msgBox(this);
        msgBox.setText("Error at setup");
        msgBox.exec();
    }
}

void MainWindow::on_inputEdit_textChanged(const QString & arg1) {
    validateSetupBtn();
}

void MainWindow::validateSetupBtn() {
    int errorNumber = errors.length();
    ui->setupBtn->setEnabled(!ui->inputEdit->text().isEmpty() &&
                             !ui->codeEdit->toPlainText().isEmpty() &&
                             errorNumber == 0);
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
}

void MainWindow::on_actionExit_triggered() {
    if (onClose() == 0) exit(0);
}

void MainWindow::on_actionAbout_Turi_IDE_triggered() {
    //
}

void MainWindow::markCurrentLine() {
    HtmlText textEditor(ui->codeEdit->toPlainText());
    int line = carriage.getLine();
    textEditor.markLine(line);
    codeEditedByUser = false;
    ui->codeEdit->setHtml(textEditor.getText());
    codeEditedByUser = true;
}

void MainWindow::on_nextBtn_clicked() {
    bool succes = carriage.next();
    if (!succes) {
        codeEditedByUser = false;
        ui->codeEdit->setText(ui->codeEdit->toPlainText());
        codeEditedByUser = true;
        ui->nextBtn->setEnabled(false);
        ui->runBtn->setEnabled(false);
    } else {
        markCurrentLine();
        ui->prevBtn->setEnabled(true);
    }
}

void MainWindow::on_prevBtn_clicked() {
    bool succes = carriage.prev();
    if (!succes) {
        ui->prevBtn->setEnabled(false);
    } else {
        ui->nextBtn->setEnabled(true);
        ui->runBtn->setEnabled(true);
    }

    markCurrentLine();
}

void MainWindow::on_runBtn_clicked() {
    while (carriage.next()) {}
    ui->runBtn->setEnabled(false);
    ui->nextBtn->setEnabled(false);
    ui->prevBtn->setEnabled(true);
    ui->codeEdit->setText(ui->codeEdit->toPlainText());
}

void MainWindow::on_resetBtn_clicked() {
    QString firstState = program->getCommand(0)->getCurrentState();
    carriage = TuriCarriage(carriage.getResult(), ui->outputResult, firstState,
                            program);
    markCurrentLine();
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

void MainWindow::on_moveLeftBtn_clicked() { carriage.moveView(LEFT); }

void MainWindow::on_moveRightBtn_clicked() { carriage.moveView(RIGHT); }

void MainWindow::setLineCounter() {
    QString res;
    int lineNumber = 30;
    for (int i = 1; i <= lineNumber; i++) {
        if (i < 10) res += " ";
        res += QString::number(i) + "\n";
    }
    ui->lineCountLabel->setText(res);
}
