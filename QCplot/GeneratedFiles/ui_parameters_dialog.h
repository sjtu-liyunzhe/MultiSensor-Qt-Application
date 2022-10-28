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
#include <QtWidgets/QPushButton>

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
    QLabel *label_4;
    QLineEdit *parameters_dirName;
    QPushButton *parameters_getDirName;

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
        label_2->setGeometry(QRect(10, 100, 54, 20));
        label = new QLabel(Parameters_dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 60, 54, 20));
        parameters_num = new QLineEdit(Parameters_dialog);
        parameters_num->setObjectName(QStringLiteral("parameters_num"));
        parameters_num->setGeometry(QRect(70, 100, 50, 20));
        parameters_trainTime = new QLineEdit(Parameters_dialog);
        parameters_trainTime->setObjectName(QStringLiteral("parameters_trainTime"));
        parameters_trainTime->setGeometry(QRect(70, 60, 50, 20));
        label_3 = new QLabel(Parameters_dialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(130, 60, 54, 20));
        label_4 = new QLabel(Parameters_dialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 20, 54, 20));
        parameters_dirName = new QLineEdit(Parameters_dialog);
        parameters_dirName->setObjectName(QStringLiteral("parameters_dirName"));
        parameters_dirName->setGeometry(QRect(70, 20, 151, 21));
        parameters_getDirName = new QPushButton(Parameters_dialog);
        parameters_getDirName->setObjectName(QStringLiteral("parameters_getDirName"));
        parameters_getDirName->setGeometry(QRect(230, 20, 75, 23));

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
        label_4->setText(QApplication::translate("Parameters_dialog", "\346\226\207\344\273\266\347\256\241\347\220\206", 0));
        parameters_getDirName->setText(QApplication::translate("Parameters_dialog", "\346\265\217\350\247\210\346\226\207\344\273\266\345\244\271", 0));
    } // retranslateUi

};

namespace Ui {
    class Parameters_dialog: public Ui_Parameters_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETERS_DIALOG_H
