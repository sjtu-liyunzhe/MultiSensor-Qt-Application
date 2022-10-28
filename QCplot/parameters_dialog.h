#ifndef PARAMETERS_DIALOG_H
#define PARAMETERS_DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>

namespace Ui {
class Parameters_dialog;
}

class Parameters_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Parameters_dialog(QWidget *parent = 0);
    ~Parameters_dialog();

public:
    Ui::Parameters_dialog *ui;
public:
    int holdTime;
    int trialNum;
    QString dirName;
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_parameters_getDirName_clicked();
};

#endif // PARAMETERS_DIALOG_H
