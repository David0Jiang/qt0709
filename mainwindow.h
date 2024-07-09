#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QDateTime>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void sendSOS();
    void updateClock();
    void toggleRest();

private:
    QLabel *labelWorkRemain;
    QLabel *labelCurrentRest;
    QLabel *labelNextRestTime;
    QLabel *labelTotalWork;
    QLabel *labelTotalRest;
    QLabel *labelLastRestTime;
    QLabel *labelClock;
    QTimer *timer;
    QTimer *restTimer;
    QPushButton *buttonRest;

    QTime startTime;
    QTime endTime;
    QTime restStartTime;
    QTime lastRestEndTime;
    int totalRestSeconds;
    int totalWorkSeconds;
    bool isResting;
};

#endif // MAINWINDOW_H
