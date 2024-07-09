#include "mainwindow.h"
#include <QTime>
#include <QPixmap>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), totalRestSeconds(0), totalWorkSeconds(0), isResting(false)
{
    setWindowTitle("Main Window");
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background-color: #404040;");

    startTime = QTime(8, 0, 0);  // 工作开始时间 8:00
    endTime = QTime(16, 0, 0);  // 工作结束时间 16:00

    // 状态标签
    QLabel *labelStatus = new QLabel("Work", this);
    labelStatus->setAlignment(Qt::AlignCenter);
    labelStatus->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");

    // 红色感叹号图标
    QLabel *labelIcon = new QLabel(this);
    QPixmap pixmap(100, 100);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::red);
    painter.drawEllipse(0, 0, 100, 100);

    QPolygon triangle;
    triangle << QPoint(50, 20) << QPoint(80, 80) << QPoint(20, 80);

    painter.setBrush(Qt::white);
    painter.drawPolygon(triangle);

    painter.setBrush(Qt::red);
    painter.drawEllipse(47, 65, 6, 6);

    QPen pen(Qt::red, 4);
    painter.setPen(pen);
    painter.drawLine(50, 36, 50, 59);

    labelIcon->setPixmap(pixmap);
    labelIcon->setAlignment(Qt::AlignCenter);

    // 时钟标签
    labelClock = new QLabel(QTime::currentTime().toString("hh:mm"), this);
    labelClock->setAlignment(Qt::AlignCenter);
    labelClock->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateClock);
    timer->start(1000);

    // 工作剩余时间标签
    labelWorkRemain = new QLabel("Work Remain: 00:00:00", this);
    labelWorkRemain->setStyleSheet("background-color: blue; color: white; font-size: 18px;");

    // 当前休息时间标签
    labelCurrentRest = new QLabel("Current Rest: 00:00:00", this);
    labelCurrentRest->setStyleSheet("background-color: red; color: white; font-size: 18px;");

    // 下次休息时间标签
    labelNextRestTime = new QLabel("Next Rest Time: 00:00", this);
    labelNextRestTime->setStyleSheet("background-color: blue; color: white; font-size: 18px;");

    // 总工作时间标签
    labelTotalWork = new QLabel("Total Work: 00:00:00", this);
    labelTotalWork->setStyleSheet("background-color: blue; color: white; font-size: 18px;");

    // 总休息时间标签
    labelTotalRest = new QLabel("Total Rest: 00:00:00", this);
    labelTotalRest->setStyleSheet("background-color: blue; color: white; font-size: 18px;");

    // 最后休息时间标签
    labelLastRestTime = new QLabel("Last Rest Time: 00:00", this);
    labelLastRestTime->setStyleSheet("background-color: blue; color: white; font-size: 18px;");

    // SOS按钮
    QPushButton *buttonSOS = new QPushButton("SOS", this);
    buttonSOS->setStyleSheet("background-color: red; color: white; font-size: 18px;");

    // 休息按钮
    buttonRest = new QPushButton("Rest", this);
    buttonRest->setStyleSheet("background-color: green; color: white; font-size: 18px;");
    connect(buttonRest, &QPushButton::clicked, this, &MainWindow::toggleRest);

    // 布局
    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addWidget(labelStatus);
    headerLayout->addWidget(labelIcon);
    headerLayout->addWidget(labelClock);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(labelWorkRemain, 0, 0);
    gridLayout->addWidget(labelCurrentRest, 0, 1);
    gridLayout->addWidget(labelNextRestTime, 0, 2);
    gridLayout->addWidget(labelTotalWork, 1, 0);
    gridLayout->addWidget(labelTotalRest, 1, 1);
    gridLayout->addWidget(labelLastRestTime, 1, 2);

    QHBoxLayout *footerLayout = new QHBoxLayout;
    footerLayout->addWidget(buttonRest);
    footerLayout->addWidget(buttonSOS);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(gridLayout);
    mainLayout->addLayout(footerLayout);

    connect(buttonSOS, &QPushButton::clicked, this, &MainWindow::sendSOS);

    restTimer = new QTimer(this);
    connect(restTimer, &QTimer::timeout, [=](){
        int restSeconds = restStartTime.secsTo(QTime::currentTime());
        int hours = restSeconds / 3600;
        int minutes = (restSeconds % 3600) / 60;
        int seconds = restSeconds % 60;
        QString timeText = QString("Current Rest: %1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
        labelCurrentRest->setText(timeText);

        if (restSeconds < 3600) {
            labelCurrentRest->setStyleSheet("background-color: red; color: white; font-size: 18px;");
        } else {
            labelCurrentRest->setStyleSheet("background-color: blue; color: white; font-size: 18px;");
        }
    });
}

void MainWindow::sendSOS()
{
    QMessageBox::warning(this, "SOS", "SOS signal sent!");
}

void MainWindow::updateClock()
{
    QTime currentTime = QTime::currentTime();
    labelClock->setText(currentTime.toString("hh:mm"));

    // 计算距离工作结束还有多长时间
    int secondsToEnd = currentTime.secsTo(endTime);
    if (secondsToEnd < 0) {
        labelWorkRemain->setText("Work Remain: 00:00:00");
    } else {
        int hours = secondsToEnd / 3600;
        int minutes = (secondsToEnd % 3600) / 60;
        int seconds = secondsToEnd % 60;
        QString timeText = QString("Work Remain: %1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
        labelWorkRemain->setText(timeText);
    }

    // 更新总工作时间
    int totalSeconds = startTime.secsTo(currentTime) - totalRestSeconds;
    int workHours = totalSeconds / 3600;
    int workMinutes = (totalSeconds % 3600) / 60;
    int workSeconds = totalSeconds % 60;
    QString workTimeText = QString("Total Work: %1:%2:%3").arg(workHours, 2, 10, QChar('0')).arg(workMinutes, 2, 10, QChar('0')).arg(workSeconds, 2, 10, QChar('0'));
    labelTotalWork->setText(workTimeText);

    // 更新总休息时间
    int restHours = totalRestSeconds / 3600;
    int restMinutes = (totalRestSeconds % 3600) / 60;
    int restSeconds = totalRestSeconds % 60;
    QString restTimeText = QString("Total Rest: %1:%2:%3").arg(restHours, 2, 10, QChar('0')).arg(restMinutes, 2, 10, QChar('0')).arg(restSeconds, 2, 10, QChar('0'));
    labelTotalRest->setText(restTimeText);
}

void MainWindow::toggleRest()
{
    if (isResting) {
        // 停止休息
        restTimer->stop();
        lastRestEndTime = QTime::currentTime();
        totalRestSeconds += restStartTime.secsTo(lastRestEndTime);
    labelLastRestTime->setText("Last Rest Time: " + lastRestEndTime.toString("hh:mm"));
        buttonRest->setText("Rest");
        isResting = false;
        // 设置下次休息时间
        QTime nextRestTime = lastRestEndTime.addSecs(2 * 3600);
        labelNextRestTime->setText("Next Rest Time: " + nextRestTime.toString("hh:mm"));
    } else {
        // 开始休息
        restStartTime = QTime::currentTime();
        restTimer->start(1000);
        buttonRest->setText("Stop Rest");
        isResting = true;
    }
}
