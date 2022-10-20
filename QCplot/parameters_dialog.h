#ifndef PARAMETERS_DIALOG_H
#define PARAMETERS_DIALOG_H

#include <QDialog>

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
};

#endif // PARAMETERS_DIALOG_H
