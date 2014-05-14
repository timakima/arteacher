#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class QLineEdit;
class QLabel;
class BoltzmannInfoWidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    BoltzmannInfoWidget *b;
    QLineEdit *temp;
    QLabel *vel;
public slots:
    void updateTemp();
};

#endif // WIDGET_H
