#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_genButton_clicked();
    void clearOutput();
    bool checkIfAnythingChecked();
    char generateChar();
    void generatePasswords(int, int);
    void error(QString);

    void on_saveToFile_clicked();

    void on_clearOutput_clicked();

    void on_loadFromFile_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
