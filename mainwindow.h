#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT    

public:
    MainWindow(QWidget *parent = nullptr);
    void init();
    void timerFunc();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer* _Timer;
    QColor createRandomColor();

};
#endif // MAINWINDOW_H
