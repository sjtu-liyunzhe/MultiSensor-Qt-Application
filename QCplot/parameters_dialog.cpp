#include "parameters_dialog.h"
#include "ui_parameters_dialog.h"

Parameters_dialog::Parameters_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Parameters_dialog)
{
    ui->setupUi(this);
    ui->parameters_num->setText("5");
}

Parameters_dialog::~Parameters_dialog()
{
    delete ui;
}
