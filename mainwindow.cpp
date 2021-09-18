#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QtGlobal>
#include <QDateTime>
#include <QTimer>
#include <QPaintEngine>
#include <QDebug>
#include <Windows.h>

static int isPressed=0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qsrand(QDateTime::currentMSecsSinceEpoch());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    if(isPressed){
        ui->widget->clearGraphs();
        isPressed = false;
    }

    std::random_device rd{};
    std::mt19937 engine(rd()); // Mersenne Twister
    std:: normal_distribution<double> distribution(0, 1);

    QVector<double> y;
    for(int i = 0; i < 100; ++i) y.push_back(i);

    QVector<double> norm_rasp(100);
    QVector<double> x(200);

    for(auto&i: norm_rasp)
        i = distribution(engine);

    double ro = ui->lineEdit->text().toDouble();

    double sigma = 1.0;
    x[0] = sigma * norm_rasp[0];
    for(int i = 1; i < 100; ++i)
        x[i] = ro * x[i - 1] + sigma * sqrt(1 - ro * ro) * norm_rasp[i]; //distribution(engine);

    ui->widget->addGraph();
    ui->widget->graph()->setPen(QPen(QColor("#FF1493")));
    ui->widget->graph()->addData(y, x);
    ui->widget->rescaleAxes();
    ui->widget->replot();
    isPressed = true;
}
