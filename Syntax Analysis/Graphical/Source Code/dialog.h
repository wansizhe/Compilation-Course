#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPaintDevice>
#include <QPaintEngine>
#include <QPainter>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void setTabColCount(int);
    void setTabItem(int row, int col, QString info);
    void addTabRow(int);
    void setTabColWidth(int,int);
    void paintEvent(QPaintEvent *event);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
