#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
signals:
    void temperatureChanged(qreal temperature);
private slots:
    void onSliderMoved(int position);
};

#endif // MAINWINDOW_H
