#include "dialog.h"
#include "ui_dialog.h"
#include <QString>
#include <QTableWidgetItem>

extern QString paintinfo;
extern int paintnum;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setTabColCount(int n)
{
    ui->outtab->setColumnCount(n);
}

void Dialog::setTabItem(int row, int col, QString info)
{
    ui->outtab->setItem(row,col,new QTableWidgetItem(info));
}

void Dialog::addTabRow(int n)
{
    ui->outtab->insertRow(n);
}

void Dialog::setTabColWidth(int c, int w)
{
    ui->outtab->setColumnWidth(c,w);
}

void Dialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 设置画笔颜色、宽度
    painter.setPen(QPen(QColor(0, 160, 230), 2));
    painter.drawLine(QPointF(950, 500), QPointF(1100, 500));
    painter.drawLine(QPointF(950, 500), QPointF(950, 500-30*paintnum));
    painter.drawLine(QPointF(1100, 500), QPointF(1100, 500-30*paintnum));

    for(int i=0;i<paintinfo.length();i++)
    {
        painter.drawLine(QPointF(950, 500-30*(1+i)), QPointF(1100, 500-30*(1+i)));
        QString temp = "";
        temp += paintinfo[i];
        painter.drawText(950, 500-30*(1+i), 150, 30,Qt::AlignCenter, temp);
    }

}
