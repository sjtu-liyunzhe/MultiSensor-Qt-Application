/********************************************************************************
** Form generated from reading UI file 'parameters_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMETERS_DIALOG_H
#define UI_PARAMETERS_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_Parameters_dialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *parameters_num;
    QLineEdit *parameters_trainTime;
    QLabel *label_3;

    void setupUi(QDialog *Parameters_dialog)
    {
        if (Parameters_dialog->objectName().isEmpty())
            Parameters_dialog->setObjectName(QStringLiteral("Parameters_dialog"));
        Parameters_dialog->resize(320, 240);
        buttonBox = new QDialogButtonBox(Parameters_dialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(10, 200, 301, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label_2 = new QLabel(Parameters_dialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(70, 80, 54, 20));
        label = new QLabel(Parameters_dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(70, 40, 54, 20));
        parameters_num = new QLineEdit(Parameters_dialog);
        parameters_num->setObjectName(QStringLiteral("parameters_num"));
        parameters_num->setGeometry(QRect(130, 80, 50, 20));
        parameters_trainTime = new QLineEdit(Parameters_dialog);
        parameters_trainTime->setObjectName(QStringLiteral("parameters_trainTime"));
        parameters_trainTime->setGeometry(QRect(130, 40, 50, 20));
        label_3 = new QLabel(Parameters_dialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(190, 40, 54, 20));

        retranslateUi(Parameters_dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Parameters_dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Parameters_dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Parameters_dialog);
    } // setupUi

    void retranslateUi(QDialog *Parameters_dialog)
    {
        Parameters_dialog->setWindowTitle(QApplication::translate("Parameters_dialog", "Parameters", 0));
        label_2->setText(QApplication::translate("Parameters_dialog", "\351\207\215\345\244\215\346\254\241\346\225\260", 0));
        label->setText(QApplication::translate("Parameters_dialog", "\350\256\255\347\273\203\346\227\266\351\225\277", 0));
        label_3->setText(QApplication::translate("Parameters_dialog", "s", 0));
    } // retranslateUi

};

namespace Ui {
    class Parameters_dialog: public Ui_Parameters_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETERS_DIALOG_H
