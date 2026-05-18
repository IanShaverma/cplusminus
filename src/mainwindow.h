#indef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QUdpSocket>
#include <QVector>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void readPendingDatagrams();
  void updatePlot();

 private:
  Ui::MainWindow* ui;
  QUdpSocket* udpSocket;
  QTimer* timer;
  QVector<QVector<double>> dataHistory;
  int maxHistorySize = 10;
};
#endif