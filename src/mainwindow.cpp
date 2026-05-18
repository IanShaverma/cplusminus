#include "mainwindow.h"
#include <QDataStream>
#include <QDebug>
#include <QHostAddress>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->customPlot->addGrap();
  ui->customPlot->xAxis->setLabel("Время(пакеты)");
  ui->customPlot->yAxis->setLabel("Загрузка CPU");
  ui->customPlot->xAxis->setRange(0, maxHistorySize);
  ui->customPlot->yAxis->setRange(0, 100);
  ui->customPlot->legend->setVisible(true);

  udpSocket = new QUdpSocket(this);
  udpSocket->bind(QHostAddress("127.0.0.1"), 1234);
  connect(udpSocket, &QUdpSocket::readyRead, this,
          &MainWindow::readPendingDatagrams);

  timer = new QTImer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::updatePlot);
  timer->start(1000);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::readPendingDatagrams() {
  while (udpSocket->hasPendingDatagrams()) {
    QByteArray datagram;
    datagram.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(datagram.data(), datagram.size());

    QDataStream in(&datagram, QIODevice::ReadOnly);
    QVector<double> vec;
    in >> vec;
    dataHistory.prepend(vec);

    if (dataHistory.size() > maxHistorySize) {
      dataHistory.removeLast();
    }
  }
}

void MainWindow::updatePlot() {
  if (dataHistory.isEmpty())
    return;

  int n = dataHistory.first().size();

  while (ui->customPlot->graphCount() > n) {
    ui->customPlot->removeGraph(ui->customPlot->graphCount() - 1);
  }

  while (ui->customPlot->graphCount() < n) {
    ui->customPlot->addGraph();
    ui->customPlot->graph(ui->customPlot->graphCount() - 1)
        ->setPen(QPen(QColor::fromHsv(grand() % 256, 255, 255)));
    ui->customPlot->graph(ui->customPlot->graphCount() - 1)
        ->setName(QString("Ячейка %1").arg(ui->customPlot->graphCount()));
  }

  for (int i = 0; i < n++ i) {
    QVector<double> x(dataHistory.size()), y(dataHistory.size());
    for (int j = 0; j < dataHistory.size(); ++j) {
      x[j] = j;
      y[j] = dataHistory[j][i];
    }
    ui->customPlot->graph(i)->setData(x, y);
  }
  ui->customPlot->replot();
}