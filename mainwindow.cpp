#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <stdlib.h>
#include <time.h>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::error(QString message) {
    QMessageBox messageBox;
    messageBox.critical(0,"Błąd",message);
    messageBox.setFixedSize(500,200);
}

void MainWindow::clearOutput() {
    if(ui->clearList->isChecked()) ui->outputList->clear();
}

bool MainWindow::checkIfAnythingChecked() {
    bool a = ui->uppercaseLetters->isChecked();
    bool b = ui->lowercaseLetters->isChecked();
    bool c = ui->numbers->isChecked();
    bool d = ui->specialChars->isChecked();
    if(!(a | b | c | d)) {
        error("Nie wybrano żadnych znaków do generowania!");
        return false;
    }
    return true;
}


char MainWindow::generateChar() {
    bool uppercase = ui->uppercaseLetters->isChecked();
    bool lowercase = ui->lowercaseLetters->isChecked();
    bool numbers = ui->numbers->isChecked();
    bool chars = ui->specialChars->isChecked();

    //char allChars = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    char randomChars[4];

    int filledElements = 0;

    if(uppercase) {
        randomChars[filledElements] = rand() % 26 + 65;
        filledElements++;
    }
    if(lowercase) {
        randomChars[filledElements] = rand() % 26 + 97;
        filledElements++;
    }
    if(numbers) {
        randomChars[filledElements] = rand() % 10 + 48;
        filledElements++;
    }
    if(chars) {
        randomChars[filledElements] = rand() % 15 + 33;
        filledElements++;
    }

    char random = randomChars[(rand() % filledElements)];



    return random;
    //return 65;
}

void MainWindow::generatePasswords(int n, int length) {
    for (int i = 0; i<n; i++) {
        QString pass = "";
        pass.resize(length,' ');
        for(int j = 0; j<length; j++) {
            pass[j] = generateChar();
        }

        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(pass);
        ui->outputList->insertItem(i, newItem);
    }
}

void MainWindow::on_genButton_clicked()
{
    if(checkIfAnythingChecked()) {
        clearOutput();
        int n = ui->noOfPassToGen->value();
        int length = ui->passLength->value();
        generatePasswords(n, length);
    }
}

