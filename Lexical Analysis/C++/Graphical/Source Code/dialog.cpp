#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}
