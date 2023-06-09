#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <stdlib.h>
#include <time.h>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

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

//Funkcja do wyświetlania błędów
void MainWindow::error(QString message) {
    QMessageBox messageBox;
    messageBox.critical(0,"Błąd",message);
    messageBox.setFixedSize(500,200);
}

QString encrypt(QString content, QString pin) {
    for(int i = 0; i<content.size(); i++) {
        int sum = 1 * pin[0].digitValue() + 2 * pin[1].digitValue() + 3 * pin[2].digitValue() + 4 *pin[3].digitValue();
        char a = content[i].unicode() + sum+1;
        content[i] = a;
    }
    return "^"+content;
}

QString decrypt(QString content, QString pin) {
    content.remove(0,1);
    for(int i = 0; i<content.size(); i++) {
        int sum = 1 * pin[0].digitValue() + 2 * pin[1].digitValue() + 3 * pin[2].digitValue() + 4 *pin[3].digitValue();
        char a = content[i].unicode() - sum-1;
        content[i] = a;
    }
    return content;
}


//Czyszczenie listy haseł
void MainWindow::clearOutput() {
    ui->outputList->clear();
}

//Sprawdzenie czy użytkownik wybrał jakikolwiek zakres znaków do generowania haseł
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


//Losowanie pojedynczego znaku
char MainWindow::generateChar() {
    //Sprawdzenie wybranych zakresów znaków
    bool uppercase = ui->uppercaseLetters->isChecked();
    bool lowercase = ui->lowercaseLetters->isChecked();
    bool numbers = ui->numbers->isChecked();
    bool chars = ui->specialChars->isChecked();

    //Tablica z czterema miejscami na cztery potencjalne znaki z różnych zakresów
    char randomChars[4];

    //Licznik wypełnionych miejsc
    int filledElements = 0;

    //Dodawanie kolejno wylosowanych znaków z różnych zakresów do kolejnych elementów
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

    //Wylosowanie jednego ze znaków z tablicy, która zawiera wylosowane znaki z od jednego do czterech zakresów
    char random = randomChars[(rand() % filledElements)];

    return random;
}

//Generowanie n liczby haseł o długości length
void MainWindow::generatePasswords(int n, int length) {
    //Pętla generująca n haseł
    for (int i = 0; i<n; i++) {
        QString pass = "";
        pass.resize(length,' ');
        //Losowanie kolejnych znaków pojedycznego hasła
        for(int j = 0; j<length; j++) {
            pass[j] = generateChar();
        }

        //Dodanie hasła jako kolejnego elementu listy
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(pass);
        ui->outputList->insertItem(i, newItem);
    }
}

//Wywołanie funkcji generującej hasła po kliknięciu przycisku
void MainWindow::on_genButton_clicked()
{
    //Sprawdzenie czy użytkownik wybrał jakikolwiek zakres
    if(checkIfAnythingChecked()) {
        //Czyszczenie listy znaków zgodnie z wyborem użytkownika
        if(ui->clearList->isChecked()) {
            clearOutput();
        }
        //Pobranie liczby i długości haseł oraz wywołanie funkcji generującej
        int n = ui->noOfPassToGen->value();
        int length = ui->passLength->value();
        generatePasswords(n, length);
    }
}

//Funkcja zapisująca hasła do pliku po kliknięciu przycisku
void MainWindow::on_saveToFile_clicked()
{
    //Zliczenie elementów listy z hasłami
    int passCount = ui->outputList->count();

    //Błąd jeżeli lista jest pusta
    if(passCount == 0) {
        error("Lista haseł jest pusta!");
        return;
    }

    //Zrzucenie całej listy do pojedynczego ciągu znaków, gdzie hasła są oddzielone końcem linii
    QString passString = "";
    for(int i = 0; i<passCount; i++) {
        passString += ui->outputList->item(i)->text() + "\n";
    }

    //Wyświetlenie okna dialogowego zapisu, z którego pobierana jest ścieżka
    QString filename = QFileDialog::getSaveFileName(this,tr("Zapisz hasła do pliku"), "",tr("Pliki tekstowe (*.txt);;All Files (*)"));
    if (filename.isEmpty()) return;

    //Błąd jeżeli nie można otworzyć pliku
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        error("Nie można otworzyć pliku");
        return;
    }

    //Szyfrowanie pliku zgodnie z wolą użytkownika
    if(ui->encrypt->isChecked()) {
        QString pin;
        do{
        bool ok;
        pin = QInputDialog::getText(this, tr("Podaj PIN"),
                                             tr("Czterocyfrowy PIN:"), QLineEdit::Normal,
                                             "0000", &ok);
        if (ok && !pin.isEmpty())
        passString = encrypt(passString, pin);
        }while(pin.length() != 4);
    }

    //Zapisanie ciągu znaków z hasłami do pliku
    QTextStream out(&file);
    out << passString;
}

//Wywołanie funkcji czyszczącej listę po kliknięciu przycisku
void MainWindow::on_clearOutput_clicked()
{
    clearOutput();
}


void MainWindow::on_loadFromFile_clicked()
{
    //Wyświetlenie okna dialogowego wczytywania, z którego pobierana jest ścieżka
    QString filename = QFileDialog::getOpenFileName(this,tr("Wczytaj hasła z pliku"), "",tr("Pliki tekstowe (*.txt);;All Files (*)"));
    if (filename.isEmpty()) return;

    //Błąd jeżeli nie można otworzyć pliku
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        error("Nie można otworzyć pliku");
        return;
    }

    //Czyszczenie listy znaków zgodnie z wyborem użytkownika
    if(ui->clearList->isChecked()) {
        clearOutput();
    }

    //Wczytanie pliku do zmiennej
    QTextStream in(&file);
    QString passString = in.readAll();

    //Sprawdzenie czy plik jest szyfrowany, jeśli tak to odszyfrować
    if(passString[0] == '^') {
        bool ok;
        QString pin = QInputDialog::getText(this, tr("Podaj PIN"),
                                            tr("PIN:"), QLineEdit::Normal,
                                            "0000", &ok);
        if (ok && !pin.isEmpty())
        passString = decrypt(passString, pin);
    }

    //Wczytywanie kolejnych linijek ciągu znaków jako elementy listy haseł
    QTextStream in2(&passString);
    int i = 0;
    while (!in2.atEnd())
    {
        QString line = in2.readLine();
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(line);
        ui->outputList->insertItem(i, newItem);
        i++;
    }

}

