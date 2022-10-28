#include "parameters_dialog.h"
#include "ui_parameters_dialog.h"

Parameters_dialog::Parameters_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Parameters_dialog)
{
    ui->setupUi(this);
    ui->parameters_num->setValidator(new QIntValidator(1, 1000, this));
    ui->parameters_trainTime->setValidator(new QIntValidator(1, 100, this));
    ui->parameters_dirName->setCursorPosition(0);
}

Parameters_dialog::~Parameters_dialog()
{
    delete ui;
}

void Parameters_dialog::on_buttonBox_accepted()
{
    holdTime = ui->parameters_trainTime->text().toInt();
    trialNum = ui->parameters_num->text().toInt();
    dirName = ui->parameters_dirName->text();
    qDebug() << "\n" << holdTime;
    qDebug() << trialNum;
    qDebug() << dirName;
}

void Parameters_dialog::on_buttonBox_rejected()
{
    qDebug() << "\n" << holdTime;
    qDebug() << trialNum;
    qDebug() << dirName;
    ui->parameters_trainTime->setText(QString::number(holdTime));
    ui->parameters_num->setText(QString::number(trialNum));
    ui->parameters_dirName->setText(dirName);
}

void Parameters_dialog::on_parameters_getDirName_clicked()
{
    QString str = QFileDialog::getExistingDirectory(this);
    ui->parameters_dirName->setText(str);
}
