/********************************************************************************
** Form generated from reading UI file 'qcplot.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCPLOT_H
#define UI_QCPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_QCplotClass
{
public:
    QAction *actionParameter;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_6;
    QWidget *widget;
    QVBoxLayout *wholeLayout_3;
    QWidget *widget1;
    QHBoxLayout *predictLayout_2;
    QWidget *imageWidget_3;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *imageLayout_3;
    QHBoxLayout *motion_image_mode1;
    QLabel *predictGesture_3;
    QWidget *channel_layer;
    QHBoxLayout *signalLayout_2;
    QCustomPlot *customPlot_mode1;
    QVBoxLayout *keyLayout_2;
    QLineEdit *remind_mode1;
    QSpacerItem *verticalSpacer_4;
    QPushButton *mode1_start;
    QSpacerItem *verticalSpacer_5;
    QPushButton *mode1_train;
    QSpacerItem *verticalSpacer_6;
    QPushButton *mode1_predict;
    QSpacerItem *verticalSpacer_8;
    QComboBox *channelBox_2;
    QSpacerItem *verticalSpacer_10;
    QProgressBar *train_progressBar_mode1;
    QWidget *tab_2;
    QGridLayout *gridLayout_7;
    QVBoxLayout *wholeLayout_5;
    QHBoxLayout *predictLayout_4;
    QWidget *imageWidget_5;
    QHBoxLayout *horizontalLayout_13;
    QLabel *wrist_motion;
    QVBoxLayout *imageLayout_5;
    QHBoxLayout *motion_image_mode2;
    QWidget *widget2;
    QHBoxLayout *signalLayout_4;
    QCustomPlot *customPlot_mode2;
    QVBoxLayout *keyLayout_4;
    QLineEdit *remind_mode2;
    QSpacerItem *verticalSpacer_14;
    QPushButton *mode2_start;
    QSpacerItem *verticalSpacer_15;
    QPushButton *mode2_train;
    QSpacerItem *verticalSpacer_16;
    QPushButton *mode2_predict;
    QSpacerItem *verticalSpacer_17;
    QComboBox *channelBox_4;
    QSpacerItem *verticalSpacer_18;
    QProgressBar *train_progressBar_mode2;
    QWidget *SPC;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout;
    QWidget *widget3;
    QGridLayout *gridLayout_5;
    QCustomPlot *customPlot_angle;
    QVBoxLayout *verticalLayout_2;
    QWidget *gif_layer;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_6;
    QLabel *hand_close_gif;
    QSpacerItem *horizontalSpacer_5;
    QLabel *wrist_rotation_gif;
    QSpacerItem *horizontalSpacer_4;
    QLabel *simultaneous_gif;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_7;
    QProgressBar *train_progressBar;
    QWidget *bottom_layer;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout;
    QToolButton *toolButton_3;
    QRadioButton *EthernetMode;
    QToolButton *toolButton_2;
    QRadioButton *WifiMode;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout_3;
    QPushButton *Xsens_Connect;
    QPushButton *wifi_connect;
    QPushButton *TAC_Connect;
    QPushButton *Delsys_On;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *motion_prompt;
    QLabel *trialno;
    QGridLayout *gridLayout_2;
    QPushButton *start_ectype;
    QPushButton *train_ectype;
    QPushButton *Run_matlab;
    QPushButton *Train_Model;
    QPushButton *predict_ectype;
    QPushButton *save_ectype;
    QWidget *Amode;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *wholeLayout;
    QHBoxLayout *imageLayout;
    QWidget *imageWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *vertialLayout;
    QHBoxLayout *imageIn_2;
    QSlider *horizontalSlider;
    QLabel *predictGesture;
    QHBoxLayout *imageButtonLayout;
    QGridLayout *showLayout;
    QCustomPlot *customPlot;
    QCustomPlot *customPlot1;
    QCustomPlot *customPlot2;
    QCustomPlot *customPlot3;
    QVBoxLayout *buttonLayout;
    QPushButton *start;
    QPushButton *stop;
    QPushButton *train;
    QPushButton *predict;
    QPushButton *accuracy;
    QPushButton *capture;
    QPushButton *calibration;
    QCheckBox *save;
    QWidget *SetSerialPort;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_10;
    QLabel *serialPort_label;
    QComboBox *serialPort_comboBox;
    QHBoxLayout *horizontalLayout_11;
    QLabel *baud_label;
    QComboBox *baud_comboBox;
    QHBoxLayout *horizontalLayout_15;
    QLabel *stopBit_label;
    QComboBox *stopBit_comboBox;
    QHBoxLayout *horizontalLayout_12;
    QLabel *dataBits_label;
    QComboBox *dataBits_comboBox;
    QHBoxLayout *horizontalLayout_14;
    QLabel *verify_label;
    QComboBox *verify_comboBox;
    QHBoxLayout *horizontalLayout_16;
    QPushButton *start_serialPort_button;
    QPushButton *stop_serialPort_button;
    QCheckBox *hex_checkBox;
    QPushButton *clear_button;
    QGroupBox *groupBox_2;
    QTextBrowser *receiveWindow;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_17;
    QLineEdit *sendWindow;
    QPushButton *serialSend_button;
    QWidget *EMGPort;
    QGroupBox *groupBox_5;
    QWidget *layoutWidget_6;
    QVBoxLayout *verticalLayout_13;
    QHBoxLayout *horizontalLayout_36;
    QLabel *serialPort_label_3;
    QComboBox *emg_serialPort_comboBox;
    QHBoxLayout *horizontalLayout_37;
    QLabel *baud_label_3;
    QComboBox *emg_baud_comboBox;
    QHBoxLayout *horizontalLayout_38;
    QLabel *stopBit_label_3;
    QComboBox *emg_stopBit_comboBox;
    QHBoxLayout *horizontalLayout_39;
    QLabel *dataBits_label_3;
    QComboBox *emg_dataBits_comboBox;
    QHBoxLayout *horizontalLayout_40;
    QLabel *verify_label_3;
    QComboBox *emg_verify_comboBox;
    QHBoxLayout *horizontalLayout_41;
    QPushButton *emg_start_serialPort_button;
    QPushButton *emg_stop_serialPort_button;
    QCheckBox *emg_hex_checkBox;
    QPushButton *emg_clear_button;
    QPushButton *clearData_button;
    QPushButton *saveData_button;
    QGroupBox *groupBox_6;
    QTextBrowser *emg_receiveWindow;
    QWidget *layoutWidget_7;
    QHBoxLayout *horizontalLayout_42;
    QLineEdit *emg_sendWindow;
    QPushButton *emg_serialSend_button;
    QWidget *MutiData;
    QFrame *line;
    QFrame *line_2;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_18;
    QVBoxLayout *verticalLayout_5;
    QCustomPlot *Amode_1_plot;
    QCustomPlot *Amode_2_plot;
    QCustomPlot *Amode_3_plot;
    QCustomPlot *Amode_4_plot;
    QVBoxLayout *verticalLayout_6;
    QCustomPlot *EMG_1_plot;
    QCustomPlot *EMG_3_plot;
    QCustomPlot *EMG_2_plot;
    QCustomPlot *EMG_4_plot;
    QVBoxLayout *verticalLayout_7;
    QCustomPlot *IMU_1_plot;
    QCustomPlot *IMU_2_plot;
    QCustomPlot *IMU_3_plot;
    QCustomPlot *IMU_4_plot;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_8;
    QLabel *Amode_label;
    QLabel *EMG_label;
    QLabel *IMU_label;
    QWidget *Discrete;
    QGroupBox *groupBox_3;
    QWidget *disGeature_widget_1;
    QWidget *disGeature_widget_2;
    QWidget *disGeature_widget_3;
    QWidget *disGeature_widget_4;
    QWidget *disGeature_widget_5;
    QWidget *disGeature_widget_6;
    QGroupBox *groupBox_4;
    QLabel *label;
    QWidget *disGeature_widget_result;
    QTextBrowser *textBrowser;
    QGroupBox *groupBox_7;
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QComboBox *comboBox_3;
    QCustomPlot *disGesture_Amode_plot;
    QCustomPlot *disGesture_IMU_plot;
    QCustomPlot *disGesture_EMG_plot;
    QPushButton *disGesture_start_button;
    QPushButton *disGesture_stop_button;
    QPushButton *disGesture_train_button;
    QPushButton *disGesture_predict_button;
    QPushButton *disGesture_clear_button;
    QPushButton *disGesture_save_button;
    QMenuBar *menuBar;
    QMenu *menuSetting;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QCplotClass)
    {
        if (QCplotClass->objectName().isEmpty())
            QCplotClass->setObjectName(QStringLiteral("QCplotClass"));
        QCplotClass->resize(1130, 928);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QCplotClass->sizePolicy().hasHeightForWidth());
        QCplotClass->setSizePolicy(sizePolicy);
        actionParameter = new QAction(QCplotClass);
        actionParameter->setObjectName(QStringLiteral("actionParameter"));
        centralWidget = new QWidget(QCplotClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 1, -1, 0);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QFont font;
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        tabWidget->setFont(font);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_6 = new QGridLayout(tab);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        widget = new QWidget(tab);
        widget->setObjectName(QStringLiteral("widget"));
        wholeLayout_3 = new QVBoxLayout(widget);
        wholeLayout_3->setSpacing(6);
        wholeLayout_3->setContentsMargins(11, 11, 11, 11);
        wholeLayout_3->setObjectName(QStringLiteral("wholeLayout_3"));
        widget1 = new QWidget(widget);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setMaximumSize(QSize(16777215, 800));
        predictLayout_2 = new QHBoxLayout(widget1);
        predictLayout_2->setSpacing(6);
        predictLayout_2->setContentsMargins(11, 11, 11, 11);
        predictLayout_2->setObjectName(QStringLiteral("predictLayout_2"));
        predictLayout_2->setContentsMargins(-1, -1, 10, -1);
        imageWidget_3 = new QWidget(widget1);
        imageWidget_3->setObjectName(QStringLiteral("imageWidget_3"));
        horizontalLayout_9 = new QHBoxLayout(imageWidget_3);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(10, 5, 10, 5);
        imageLayout_3 = new QVBoxLayout();
        imageLayout_3->setSpacing(6);
        imageLayout_3->setObjectName(QStringLiteral("imageLayout_3"));
        imageLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        motion_image_mode1 = new QHBoxLayout();
        motion_image_mode1->setSpacing(6);
        motion_image_mode1->setObjectName(QStringLiteral("motion_image_mode1"));

        imageLayout_3->addLayout(motion_image_mode1);

        imageLayout_3->setStretch(0, 7);

        horizontalLayout_9->addLayout(imageLayout_3);


        predictLayout_2->addWidget(imageWidget_3);

        predictGesture_3 = new QLabel(widget1);
        predictGesture_3->setObjectName(QStringLiteral("predictGesture_3"));

        predictLayout_2->addWidget(predictGesture_3);

        predictLayout_2->setStretch(0, 6);
        predictLayout_2->setStretch(1, 1);

        wholeLayout_3->addWidget(widget1);

        channel_layer = new QWidget(widget);
        channel_layer->setObjectName(QStringLiteral("channel_layer"));
        channel_layer->setMaximumSize(QSize(16777215, 16777215));
        signalLayout_2 = new QHBoxLayout(channel_layer);
        signalLayout_2->setSpacing(6);
        signalLayout_2->setContentsMargins(11, 11, 11, 11);
        signalLayout_2->setObjectName(QStringLiteral("signalLayout_2"));
        signalLayout_2->setContentsMargins(1, -1, -1, -1);
        customPlot_mode1 = new QCustomPlot(channel_layer);
        customPlot_mode1->setObjectName(QStringLiteral("customPlot_mode1"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(customPlot_mode1->sizePolicy().hasHeightForWidth());
        customPlot_mode1->setSizePolicy(sizePolicy1);

        signalLayout_2->addWidget(customPlot_mode1);

        keyLayout_2 = new QVBoxLayout();
        keyLayout_2->setSpacing(6);
        keyLayout_2->setObjectName(QStringLiteral("keyLayout_2"));
        keyLayout_2->setContentsMargins(40, -1, 40, -1);
        remind_mode1 = new QLineEdit(channel_layer);
        remind_mode1->setObjectName(QStringLiteral("remind_mode1"));
        sizePolicy1.setHeightForWidth(remind_mode1->sizePolicy().hasHeightForWidth());
        remind_mode1->setSizePolicy(sizePolicy1);
        remind_mode1->setMinimumSize(QSize(70, 50));
        QFont font1;
        font1.setFamily(QStringLiteral("Times New Roman"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        remind_mode1->setFont(font1);
        remind_mode1->setStyleSheet(QLatin1String("color: rgb(255, 0, 0);\n"
"background-color: rgb(226, 226, 226);"));
        remind_mode1->setAlignment(Qt::AlignCenter);

        keyLayout_2->addWidget(remind_mode1);

        verticalSpacer_4 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        keyLayout_2->addItem(verticalSpacer_4);

        mode1_start = new QPushButton(channel_layer);
        mode1_start->setObjectName(QStringLiteral("mode1_start"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(mode1_start->sizePolicy().hasHeightForWidth());
        mode1_start->setSizePolicy(sizePolicy2);

        keyLayout_2->addWidget(mode1_start);

        verticalSpacer_5 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        keyLayout_2->addItem(verticalSpacer_5);

        mode1_train = new QPushButton(channel_layer);
        mode1_train->setObjectName(QStringLiteral("mode1_train"));
        mode1_train->setEnabled(false);
        sizePolicy2.setHeightForWidth(mode1_train->sizePolicy().hasHeightForWidth());
        mode1_train->setSizePolicy(sizePolicy2);

        keyLayout_2->addWidget(mode1_train);

        verticalSpacer_6 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        keyLayout_2->addItem(verticalSpacer_6);

        mode1_predict = new QPushButton(channel_layer);
        mode1_predict->setObjectName(QStringLiteral("mode1_predict"));
        mode1_predict->setEnabled(false);
        sizePolicy2.setHeightForWidth(mode1_predict->sizePolicy().hasHeightForWidth());
        mode1_predict->setSizePolicy(sizePolicy2);
        mode1_predict->setLayoutDirection(Qt::LeftToRight);

        keyLayout_2->addWidget(mode1_predict);

        verticalSpacer_8 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        keyLayout_2->addItem(verticalSpacer_8);

        channelBox_2 = new QComboBox(channel_layer);
        channelBox_2->setObjectName(QStringLiteral("channelBox_2"));
        channelBox_2->setEnabled(true);
        sizePolicy2.setHeightForWidth(channelBox_2->sizePolicy().hasHeightForWidth());
        channelBox_2->setSizePolicy(sizePolicy2);
        channelBox_2->setLayoutDirection(Qt::LeftToRight);
        channelBox_2->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        keyLayout_2->addWidget(channelBox_2);

        verticalSpacer_10 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        keyLayout_2->addItem(verticalSpacer_10);


        signalLayout_2->addLayout(keyLayout_2);

        signalLayout_2->setStretch(0, 5);
        signalLayout_2->setStretch(1, 1);

        wholeLayout_3->addWidget(channel_layer);

        train_progressBar_mode1 = new QProgressBar(widget);
        train_progressBar_mode1->setObjectName(QStringLiteral("train_progressBar_mode1"));
        train_progressBar_mode1->setValue(24);

        wholeLayout_3->addWidget(train_progressBar_mode1);

        wholeLayout_3->setStretch(0, 3);
        wholeLayout_3->setStretch(1, 3);
        wholeLayout_3->setStretch(2, 1);

        gridLayout_6->addWidget(widget, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_7 = new QGridLayout(tab_2);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        wholeLayout_5 = new QVBoxLayout();
        wholeLayout_5->setSpacing(6);
        wholeLayout_5->setObjectName(QStringLiteral("wholeLayout_5"));
        predictLayout_4 = new QHBoxLayout();
        predictLayout_4->setSpacing(6);
        predictLayout_4->setObjectName(QStringLiteral("predictLayout_4"));
        predictLayout_4->setContentsMargins(-1, -1, 10, -1);
        imageWidget_5 = new QWidget(tab_2);
        imageWidget_5->setObjectName(QStringLiteral("imageWidget_5"));
        horizontalLayout_13 = new QHBoxLayout(imageWidget_5);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(10, 5, 10, 5);
        wrist_motion = new QLabel(imageWidget_5);
        wrist_motion->setObjectName(QStringLiteral("wrist_motion"));
        wrist_motion->setMaximumSize(QSize(350, 450));
        wrist_motion->setPixmap(QPixmap(QString::fromUtf8("Resources/WristRotation.gif")));
        wrist_motion->setScaledContents(true);

        horizontalLayout_13->addWidget(wrist_motion);

        imageLayout_5 = new QVBoxLayout();
        imageLayout_5->setSpacing(6);
        imageLayout_5->setObjectName(QStringLiteral("imageLayout_5"));
        imageLayout_5->setSizeConstraint(QLayout::SetFixedSize);
        motion_image_mode2 = new QHBoxLayout();
        motion_image_mode2->setSpacing(6);
        motion_image_mode2->setObjectName(QStringLiteral("motion_image_mode2"));

        imageLayout_5->addLayout(motion_image_mode2);


        horizontalLayout_13->addLayout(imageLayout_5);


        predictLayout_4->addWidget(imageWidget_5);


        wholeLayout_5->addLayout(predictLayout_4);

        widget2 = new QWidget(tab_2);
        widget2->setObjectName(QStringLiteral("widget2"));
        widget2->setMinimumSize(QSize(0, 300));
        widget2->setMaximumSize(QSize(16777215, 500));
        signalLayout_4 = new QHBoxLayout(widget2);
        signalLayout_4->setSpacing(6);
        signalLayout_4->setContentsMargins(11, 11, 11, 11);
        signalLayout_4->setObjectName(QStringLiteral("signalLayout_4"));
        signalLayout_4->setContentsMargins(1, -1, -1, -1);
        customPlot_mode2 = new QCustomPlot(widget2);
        customPlot_mode2->setObjectName(QStringLiteral("customPlot_mode2"));
        sizePolicy1.setHeightForWidth(customPlot_mode2->sizePolicy().hasHeightForWidth());
        customPlot_mode2->setSizePolicy(sizePolicy1);

        signalLayout_4->addWidget(customPlot_mode2);

        keyLayout_4 = new QVBoxLayout();
        keyLayout_4->setSpacing(6);
        keyLayout_4->setObjectName(QStringLiteral("keyLayout_4"));
        keyLayout_4->setContentsMargins(40, -1, 40, -1);
        remind_mode2 = new QLineEdit(widget2);
        remind_mode2->setObjectName(QStringLiteral("remind_mode2"));
        sizePolicy1.setHeightForWidth(remind_mode2->sizePolicy().hasHeightForWidth());
        remind_mode2->setSizePolicy(sizePolicy1);
        remind_mode2->setMinimumSize(QSize(70, 50));
        remind_mode2->setFont(font1);
        remind_mode2->setStyleSheet(QLatin1String("color: rgb(255, 0, 0);\n"
"background-color: rgb(226, 226, 226);"));
        remind_mode2->setAlignment(Qt::AlignCenter);

        keyLayout_4->addWidget(remind_mode2);

        verticalSpacer_14 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        keyLayout_4->addItem(verticalSpacer_14);

        mode2_start = new QPushButton(widget2);
        mode2_start->setObjectName(QStringLiteral("mode2_start"));
        sizePolicy2.setHeightForWidth(mode2_start->sizePolicy().hasHeightForWidth());
        mode2_start->setSizePolicy(sizePolicy2);

        keyLayout_4->addWidget(mode2_start);

        verticalSpacer_15 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        keyLayout_4->addItem(verticalSpacer_15);

        mode2_train = new QPushButton(widget2);
        mode2_train->setObjectName(QStringLiteral("mode2_train"));
        mode2_train->setEnabled(false);
        sizePolicy2.setHeightForWidth(mode2_train->sizePolicy().hasHeightForWidth());
        mode2_train->setSizePolicy(sizePolicy2);

        keyLayout_4->addWidget(mode2_train);

        verticalSpacer_16 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        keyLayout_4->addItem(verticalSpacer_16);

        mode2_predict = new QPushButton(widget2);
        mode2_predict->setObjectName(QStringLiteral("mode2_predict"));
        mode2_predict->setEnabled(false);
        sizePolicy2.setHeightForWidth(mode2_predict->sizePolicy().hasHeightForWidth());
        mode2_predict->setSizePolicy(sizePolicy2);
        mode2_predict->setLayoutDirection(Qt::LeftToRight);

        keyLayout_4->addWidget(mode2_predict);

        verticalSpacer_17 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        keyLayout_4->addItem(verticalSpacer_17);

        channelBox_4 = new QComboBox(widget2);
        channelBox_4->setObjectName(QStringLiteral("channelBox_4"));
        channelBox_4->setEnabled(true);
        sizePolicy2.setHeightForWidth(channelBox_4->sizePolicy().hasHeightForWidth());
        channelBox_4->setSizePolicy(sizePolicy2);
        channelBox_4->setLayoutDirection(Qt::LeftToRight);
        channelBox_4->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        keyLayout_4->addWidget(channelBox_4);

        verticalSpacer_18 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        keyLayout_4->addItem(verticalSpacer_18);


        signalLayout_4->addLayout(keyLayout_4);

        signalLayout_4->setStretch(0, 5);
        signalLayout_4->setStretch(1, 1);

        wholeLayout_5->addWidget(widget2);

        train_progressBar_mode2 = new QProgressBar(tab_2);
        train_progressBar_mode2->setObjectName(QStringLiteral("train_progressBar_mode2"));
        train_progressBar_mode2->setValue(24);

        wholeLayout_5->addWidget(train_progressBar_mode2);

        wholeLayout_5->setStretch(0, 3);

        gridLayout_7->addLayout(wholeLayout_5, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        SPC = new QWidget();
        SPC->setObjectName(QStringLiteral("SPC"));
        horizontalLayout_5 = new QHBoxLayout(SPC);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(9, 10, -1, 10);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(12);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, -1, 0);
        widget3 = new QWidget(SPC);
        widget3->setObjectName(QStringLiteral("widget3"));
        widget3->setMinimumSize(QSize(1080, 500));
        gridLayout_5 = new QGridLayout(widget3);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        customPlot_angle = new QCustomPlot(widget3);
        customPlot_angle->setObjectName(QStringLiteral("customPlot_angle"));
        sizePolicy1.setHeightForWidth(customPlot_angle->sizePolicy().hasHeightForWidth());
        customPlot_angle->setSizePolicy(sizePolicy1);
        customPlot_angle->setMinimumSize(QSize(0, 200));

        gridLayout_5->addWidget(customPlot_angle, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gif_layer = new QWidget(widget3);
        gif_layer->setObjectName(QStringLiteral("gif_layer"));
        gridLayout_4 = new QGridLayout(gif_layer);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);

        hand_close_gif = new QLabel(gif_layer);
        hand_close_gif->setObjectName(QStringLiteral("hand_close_gif"));
        hand_close_gif->setMinimumSize(QSize(220, 250));
        hand_close_gif->setMaximumSize(QSize(350, 400));
        hand_close_gif->setPixmap(QPixmap(QString::fromUtf8("Resources/HandClosing.gif")));
        hand_close_gif->setScaledContents(true);

        horizontalLayout_6->addWidget(hand_close_gif);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);

        wrist_rotation_gif = new QLabel(gif_layer);
        wrist_rotation_gif->setObjectName(QStringLiteral("wrist_rotation_gif"));
        wrist_rotation_gif->setMinimumSize(QSize(220, 250));
        wrist_rotation_gif->setMaximumSize(QSize(350, 400));
        wrist_rotation_gif->setPixmap(QPixmap(QString::fromUtf8("Resources/WristRotation.gif")));
        wrist_rotation_gif->setScaledContents(true);

        horizontalLayout_6->addWidget(wrist_rotation_gif);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);

        simultaneous_gif = new QLabel(gif_layer);
        simultaneous_gif->setObjectName(QStringLiteral("simultaneous_gif"));
        simultaneous_gif->setMinimumSize(QSize(220, 250));
        simultaneous_gif->setMaximumSize(QSize(350, 400));
        simultaneous_gif->setPixmap(QPixmap(QString::fromUtf8("Resources/simultaneous.gif")));
        simultaneous_gif->setScaledContents(true);

        horizontalLayout_6->addWidget(simultaneous_gif);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);


        gridLayout_4->addLayout(horizontalLayout_6, 0, 0, 1, 1);


        verticalLayout_2->addWidget(gif_layer);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        train_progressBar = new QProgressBar(widget3);
        train_progressBar->setObjectName(QStringLiteral("train_progressBar"));
        train_progressBar->setMinimumSize(QSize(930, 24));
        train_progressBar->setMaximumSize(QSize(115200, 24));
        train_progressBar->setValue(24);

        horizontalLayout_7->addWidget(train_progressBar);


        verticalLayout_2->addLayout(horizontalLayout_7);


        gridLayout_5->addLayout(verticalLayout_2, 1, 0, 1, 1);


        verticalLayout->addWidget(widget3);

        bottom_layer = new QWidget(SPC);
        bottom_layer->setObjectName(QStringLiteral("bottom_layer"));
        bottom_layer->setMaximumSize(QSize(16777215, 100));
        horizontalLayout_4 = new QHBoxLayout(bottom_layer);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        toolButton_3 = new QToolButton(bottom_layer);
        toolButton_3->setObjectName(QStringLiteral("toolButton_3"));
        toolButton_3->setStyleSheet(QStringLiteral("border-image: url(:/icon/Resources/timg.jpg);"));
        QIcon icon;
        icon.addFile(QStringLiteral("Resources/timg.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_3->setIcon(icon);

        gridLayout->addWidget(toolButton_3, 0, 0, 1, 1);

        EthernetMode = new QRadioButton(bottom_layer);
        EthernetMode->setObjectName(QStringLiteral("EthernetMode"));
        EthernetMode->setChecked(true);

        gridLayout->addWidget(EthernetMode, 0, 1, 1, 1);

        toolButton_2 = new QToolButton(bottom_layer);
        toolButton_2->setObjectName(QStringLiteral("toolButton_2"));
        toolButton_2->setStyleSheet(QStringLiteral("border-image: url(:/icon/Resources/wifi.jpg);"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("Resources/wifi.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_2->setIcon(icon1);

        gridLayout->addWidget(toolButton_2, 1, 0, 1, 1);

        WifiMode = new QRadioButton(bottom_layer);
        WifiMode->setObjectName(QStringLiteral("WifiMode"));
        WifiMode->setCheckable(true);
        WifiMode->setChecked(false);

        gridLayout->addWidget(WifiMode, 1, 1, 1, 1);


        horizontalLayout_4->addLayout(gridLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        Xsens_Connect = new QPushButton(bottom_layer);
        Xsens_Connect->setObjectName(QStringLiteral("Xsens_Connect"));

        gridLayout_3->addWidget(Xsens_Connect, 1, 1, 1, 1);

        wifi_connect = new QPushButton(bottom_layer);
        wifi_connect->setObjectName(QStringLiteral("wifi_connect"));
        wifi_connect->setEnabled(false);

        gridLayout_3->addWidget(wifi_connect, 1, 0, 1, 1);

        TAC_Connect = new QPushButton(bottom_layer);
        TAC_Connect->setObjectName(QStringLiteral("TAC_Connect"));

        gridLayout_3->addWidget(TAC_Connect, 0, 0, 1, 1);

        Delsys_On = new QPushButton(bottom_layer);
        Delsys_On->setObjectName(QStringLiteral("Delsys_On"));

        gridLayout_3->addWidget(Delsys_On, 0, 1, 1, 1);


        horizontalLayout_4->addLayout(gridLayout_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        motion_prompt = new QLineEdit(bottom_layer);
        motion_prompt->setObjectName(QStringLiteral("motion_prompt"));
        sizePolicy1.setHeightForWidth(motion_prompt->sizePolicy().hasHeightForWidth());
        motion_prompt->setSizePolicy(sizePolicy1);
        motion_prompt->setFont(font1);
        motion_prompt->setStyleSheet(QLatin1String("color: rgb(255, 0, 0);\n"
"background-color: rgb(226, 226, 226);"));
        motion_prompt->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(motion_prompt);

        trialno = new QLabel(bottom_layer);
        trialno->setObjectName(QStringLiteral("trialno"));
        QFont font2;
        font2.setFamily(QStringLiteral("Times New Roman"));
        trialno->setFont(font2);

        horizontalLayout_4->addWidget(trialno);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        start_ectype = new QPushButton(bottom_layer);
        start_ectype->setObjectName(QStringLiteral("start_ectype"));
        sizePolicy2.setHeightForWidth(start_ectype->sizePolicy().hasHeightForWidth());
        start_ectype->setSizePolicy(sizePolicy2);
        QFont font3;
        font3.setFamily(QStringLiteral("Times New Roman"));
        font3.setPointSize(12);
        start_ectype->setFont(font3);

        gridLayout_2->addWidget(start_ectype, 0, 1, 1, 1);

        train_ectype = new QPushButton(bottom_layer);
        train_ectype->setObjectName(QStringLiteral("train_ectype"));
        train_ectype->setEnabled(false);
        train_ectype->setFont(font3);

        gridLayout_2->addWidget(train_ectype, 0, 2, 1, 1);

        Run_matlab = new QPushButton(bottom_layer);
        Run_matlab->setObjectName(QStringLiteral("Run_matlab"));

        gridLayout_2->addWidget(Run_matlab, 0, 0, 1, 1);

        Train_Model = new QPushButton(bottom_layer);
        Train_Model->setObjectName(QStringLiteral("Train_Model"));
        Train_Model->setEnabled(false);

        gridLayout_2->addWidget(Train_Model, 1, 0, 1, 1);

        predict_ectype = new QPushButton(bottom_layer);
        predict_ectype->setObjectName(QStringLiteral("predict_ectype"));
        predict_ectype->setEnabled(false);
        predict_ectype->setFont(font3);

        gridLayout_2->addWidget(predict_ectype, 1, 1, 1, 1);

        save_ectype = new QPushButton(bottom_layer);
        save_ectype->setObjectName(QStringLiteral("save_ectype"));
        sizePolicy2.setHeightForWidth(save_ectype->sizePolicy().hasHeightForWidth());
        save_ectype->setSizePolicy(sizePolicy2);
        save_ectype->setFont(font3);
        save_ectype->setStyleSheet(QStringLiteral(""));
        save_ectype->setAutoDefault(false);
        save_ectype->setDefault(false);
        save_ectype->setFlat(false);

        gridLayout_2->addWidget(save_ectype, 1, 2, 1, 1);


        horizontalLayout_4->addLayout(gridLayout_2);


        verticalLayout->addWidget(bottom_layer);


        horizontalLayout_5->addLayout(verticalLayout);

        tabWidget->addTab(SPC, QString());
        Amode = new QWidget();
        Amode->setObjectName(QStringLiteral("Amode"));
        horizontalLayout_3 = new QHBoxLayout(Amode);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        wholeLayout = new QVBoxLayout();
        wholeLayout->setSpacing(6);
        wholeLayout->setObjectName(QStringLiteral("wholeLayout"));
        imageLayout = new QHBoxLayout();
        imageLayout->setSpacing(21);
        imageLayout->setObjectName(QStringLiteral("imageLayout"));
        imageLayout->setContentsMargins(10, -1, 20, -1);
        imageWidget = new QWidget(Amode);
        imageWidget->setObjectName(QStringLiteral("imageWidget"));
        horizontalLayout = new QHBoxLayout(imageWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        vertialLayout = new QVBoxLayout();
        vertialLayout->setSpacing(6);
        vertialLayout->setObjectName(QStringLiteral("vertialLayout"));
        imageIn_2 = new QHBoxLayout();
        imageIn_2->setSpacing(6);
        imageIn_2->setObjectName(QStringLiteral("imageIn_2"));

        vertialLayout->addLayout(imageIn_2);

        horizontalSlider = new QSlider(imageWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        vertialLayout->addWidget(horizontalSlider);


        horizontalLayout->addLayout(vertialLayout);


        imageLayout->addWidget(imageWidget);

        predictGesture = new QLabel(Amode);
        predictGesture->setObjectName(QStringLiteral("predictGesture"));

        imageLayout->addWidget(predictGesture);

        imageLayout->setStretch(0, 3);
        imageLayout->setStretch(1, 1);

        wholeLayout->addLayout(imageLayout);

        imageButtonLayout = new QHBoxLayout();
        imageButtonLayout->setSpacing(6);
        imageButtonLayout->setObjectName(QStringLiteral("imageButtonLayout"));
        imageButtonLayout->setContentsMargins(20, -1, 20, -1);
        showLayout = new QGridLayout();
        showLayout->setSpacing(6);
        showLayout->setObjectName(QStringLiteral("showLayout"));
        customPlot = new QCustomPlot(Amode);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        sizePolicy1.setHeightForWidth(customPlot->sizePolicy().hasHeightForWidth());
        customPlot->setSizePolicy(sizePolicy1);

        showLayout->addWidget(customPlot, 0, 0, 1, 1);

        customPlot1 = new QCustomPlot(Amode);
        customPlot1->setObjectName(QStringLiteral("customPlot1"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(customPlot1->sizePolicy().hasHeightForWidth());
        customPlot1->setSizePolicy(sizePolicy3);

        showLayout->addWidget(customPlot1, 0, 1, 1, 1);

        customPlot2 = new QCustomPlot(Amode);
        customPlot2->setObjectName(QStringLiteral("customPlot2"));
        sizePolicy1.setHeightForWidth(customPlot2->sizePolicy().hasHeightForWidth());
        customPlot2->setSizePolicy(sizePolicy1);

        showLayout->addWidget(customPlot2, 1, 0, 1, 1);

        customPlot3 = new QCustomPlot(Amode);
        customPlot3->setObjectName(QStringLiteral("customPlot3"));
        sizePolicy1.setHeightForWidth(customPlot3->sizePolicy().hasHeightForWidth());
        customPlot3->setSizePolicy(sizePolicy1);

        showLayout->addWidget(customPlot3, 1, 1, 1, 1);


        imageButtonLayout->addLayout(showLayout);

        buttonLayout = new QVBoxLayout();
        buttonLayout->setSpacing(6);
        buttonLayout->setObjectName(QStringLiteral("buttonLayout"));
        buttonLayout->setContentsMargins(10, -1, 10, -1);
        start = new QPushButton(Amode);
        start->setObjectName(QStringLiteral("start"));
        sizePolicy2.setHeightForWidth(start->sizePolicy().hasHeightForWidth());
        start->setSizePolicy(sizePolicy2);

        buttonLayout->addWidget(start);

        stop = new QPushButton(Amode);
        stop->setObjectName(QStringLiteral("stop"));
        sizePolicy2.setHeightForWidth(stop->sizePolicy().hasHeightForWidth());
        stop->setSizePolicy(sizePolicy2);

        buttonLayout->addWidget(stop);

        train = new QPushButton(Amode);
        train->setObjectName(QStringLiteral("train"));
        sizePolicy2.setHeightForWidth(train->sizePolicy().hasHeightForWidth());
        train->setSizePolicy(sizePolicy2);

        buttonLayout->addWidget(train);

        predict = new QPushButton(Amode);
        predict->setObjectName(QStringLiteral("predict"));
        sizePolicy2.setHeightForWidth(predict->sizePolicy().hasHeightForWidth());
        predict->setSizePolicy(sizePolicy2);

        buttonLayout->addWidget(predict);

        accuracy = new QPushButton(Amode);
        accuracy->setObjectName(QStringLiteral("accuracy"));
        sizePolicy2.setHeightForWidth(accuracy->sizePolicy().hasHeightForWidth());
        accuracy->setSizePolicy(sizePolicy2);

        buttonLayout->addWidget(accuracy);

        capture = new QPushButton(Amode);
        capture->setObjectName(QStringLiteral("capture"));
        sizePolicy2.setHeightForWidth(capture->sizePolicy().hasHeightForWidth());
        capture->setSizePolicy(sizePolicy2);

        buttonLayout->addWidget(capture);

        calibration = new QPushButton(Amode);
        calibration->setObjectName(QStringLiteral("calibration"));
        sizePolicy2.setHeightForWidth(calibration->sizePolicy().hasHeightForWidth());
        calibration->setSizePolicy(sizePolicy2);

        buttonLayout->addWidget(calibration);

        save = new QCheckBox(Amode);
        save->setObjectName(QStringLiteral("save"));
        save->setEnabled(true);
        QSizePolicy sizePolicy4(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(save->sizePolicy().hasHeightForWidth());
        save->setSizePolicy(sizePolicy4);
        save->setContextMenuPolicy(Qt::DefaultContextMenu);
        save->setAcceptDrops(false);
        save->setCheckable(true);
        save->setChecked(false);

        buttonLayout->addWidget(save);

        buttonLayout->setStretch(0, 1);
        buttonLayout->setStretch(1, 1);
        buttonLayout->setStretch(2, 1);
        buttonLayout->setStretch(3, 1);
        buttonLayout->setStretch(4, 1);
        buttonLayout->setStretch(7, 1);

        imageButtonLayout->addLayout(buttonLayout);

        imageButtonLayout->setStretch(0, 8);
        imageButtonLayout->setStretch(1, 1);

        wholeLayout->addLayout(imageButtonLayout);

        wholeLayout->setStretch(0, 2);
        wholeLayout->setStretch(1, 5);

        horizontalLayout_3->addLayout(wholeLayout);

        tabWidget->addTab(Amode, QString());
        SetSerialPort = new QWidget();
        SetSerialPort->setObjectName(QStringLiteral("SetSerialPort"));
        groupBox = new QGroupBox(SetSerialPort);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(780, 10, 321, 811));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 221, 301));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        serialPort_label = new QLabel(layoutWidget);
        serialPort_label->setObjectName(QStringLiteral("serialPort_label"));

        horizontalLayout_10->addWidget(serialPort_label);

        serialPort_comboBox = new QComboBox(layoutWidget);
        serialPort_comboBox->setObjectName(QStringLiteral("serialPort_comboBox"));
        serialPort_comboBox->setEnabled(true);

        horizontalLayout_10->addWidget(serialPort_comboBox);


        verticalLayout_3->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        baud_label = new QLabel(layoutWidget);
        baud_label->setObjectName(QStringLiteral("baud_label"));

        horizontalLayout_11->addWidget(baud_label);

        baud_comboBox = new QComboBox(layoutWidget);
        baud_comboBox->setObjectName(QStringLiteral("baud_comboBox"));

        horizontalLayout_11->addWidget(baud_comboBox);


        verticalLayout_3->addLayout(horizontalLayout_11);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        stopBit_label = new QLabel(layoutWidget);
        stopBit_label->setObjectName(QStringLiteral("stopBit_label"));

        horizontalLayout_15->addWidget(stopBit_label);

        stopBit_comboBox = new QComboBox(layoutWidget);
        stopBit_comboBox->setObjectName(QStringLiteral("stopBit_comboBox"));

        horizontalLayout_15->addWidget(stopBit_comboBox);


        verticalLayout_3->addLayout(horizontalLayout_15);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        dataBits_label = new QLabel(layoutWidget);
        dataBits_label->setObjectName(QStringLiteral("dataBits_label"));

        horizontalLayout_12->addWidget(dataBits_label);

        dataBits_comboBox = new QComboBox(layoutWidget);
        dataBits_comboBox->setObjectName(QStringLiteral("dataBits_comboBox"));

        horizontalLayout_12->addWidget(dataBits_comboBox);


        verticalLayout_3->addLayout(horizontalLayout_12);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        verify_label = new QLabel(layoutWidget);
        verify_label->setObjectName(QStringLiteral("verify_label"));

        horizontalLayout_14->addWidget(verify_label);

        verify_comboBox = new QComboBox(layoutWidget);
        verify_comboBox->setObjectName(QStringLiteral("verify_comboBox"));

        horizontalLayout_14->addWidget(verify_comboBox);


        verticalLayout_3->addLayout(horizontalLayout_14);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        start_serialPort_button = new QPushButton(layoutWidget);
        start_serialPort_button->setObjectName(QStringLiteral("start_serialPort_button"));

        horizontalLayout_16->addWidget(start_serialPort_button);

        stop_serialPort_button = new QPushButton(layoutWidget);
        stop_serialPort_button->setObjectName(QStringLiteral("stop_serialPort_button"));

        horizontalLayout_16->addWidget(stop_serialPort_button);


        verticalLayout_3->addLayout(horizontalLayout_16);

        hex_checkBox = new QCheckBox(layoutWidget);
        hex_checkBox->setObjectName(QStringLiteral("hex_checkBox"));

        verticalLayout_3->addWidget(hex_checkBox);

        clear_button = new QPushButton(layoutWidget);
        clear_button->setObjectName(QStringLiteral("clear_button"));

        verticalLayout_3->addWidget(clear_button);

        groupBox_2 = new QGroupBox(SetSerialPort);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(0, 13, 751, 811));
        receiveWindow = new QTextBrowser(groupBox_2);
        receiveWindow->setObjectName(QStringLiteral("receiveWindow"));
        receiveWindow->setGeometry(QRect(10, 61, 731, 741));
        layoutWidget1 = new QWidget(groupBox_2);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(13, 13, 731, 31));
        horizontalLayout_17 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        horizontalLayout_17->setContentsMargins(0, 0, 0, 0);
        sendWindow = new QLineEdit(layoutWidget1);
        sendWindow->setObjectName(QStringLiteral("sendWindow"));

        horizontalLayout_17->addWidget(sendWindow);

        serialSend_button = new QPushButton(layoutWidget1);
        serialSend_button->setObjectName(QStringLiteral("serialSend_button"));

        horizontalLayout_17->addWidget(serialSend_button);

        tabWidget->addTab(SetSerialPort, QString());
        EMGPort = new QWidget();
        EMGPort->setObjectName(QStringLiteral("EMGPort"));
        groupBox_5 = new QGroupBox(EMGPort);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(780, 10, 321, 811));
        layoutWidget_6 = new QWidget(groupBox_5);
        layoutWidget_6->setObjectName(QStringLiteral("layoutWidget_6"));
        layoutWidget_6->setGeometry(QRect(10, 20, 221, 301));
        verticalLayout_13 = new QVBoxLayout(layoutWidget_6);
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        verticalLayout_13->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_36 = new QHBoxLayout();
        horizontalLayout_36->setSpacing(6);
        horizontalLayout_36->setObjectName(QStringLiteral("horizontalLayout_36"));
        serialPort_label_3 = new QLabel(layoutWidget_6);
        serialPort_label_3->setObjectName(QStringLiteral("serialPort_label_3"));

        horizontalLayout_36->addWidget(serialPort_label_3);

        emg_serialPort_comboBox = new QComboBox(layoutWidget_6);
        emg_serialPort_comboBox->setObjectName(QStringLiteral("emg_serialPort_comboBox"));
        emg_serialPort_comboBox->setEnabled(true);

        horizontalLayout_36->addWidget(emg_serialPort_comboBox);


        verticalLayout_13->addLayout(horizontalLayout_36);

        horizontalLayout_37 = new QHBoxLayout();
        horizontalLayout_37->setSpacing(6);
        horizontalLayout_37->setObjectName(QStringLiteral("horizontalLayout_37"));
        baud_label_3 = new QLabel(layoutWidget_6);
        baud_label_3->setObjectName(QStringLiteral("baud_label_3"));

        horizontalLayout_37->addWidget(baud_label_3);

        emg_baud_comboBox = new QComboBox(layoutWidget_6);
        emg_baud_comboBox->setObjectName(QStringLiteral("emg_baud_comboBox"));

        horizontalLayout_37->addWidget(emg_baud_comboBox);


        verticalLayout_13->addLayout(horizontalLayout_37);

        horizontalLayout_38 = new QHBoxLayout();
        horizontalLayout_38->setSpacing(6);
        horizontalLayout_38->setObjectName(QStringLiteral("horizontalLayout_38"));
        stopBit_label_3 = new QLabel(layoutWidget_6);
        stopBit_label_3->setObjectName(QStringLiteral("stopBit_label_3"));

        horizontalLayout_38->addWidget(stopBit_label_3);

        emg_stopBit_comboBox = new QComboBox(layoutWidget_6);
        emg_stopBit_comboBox->setObjectName(QStringLiteral("emg_stopBit_comboBox"));

        horizontalLayout_38->addWidget(emg_stopBit_comboBox);


        verticalLayout_13->addLayout(horizontalLayout_38);

        horizontalLayout_39 = new QHBoxLayout();
        horizontalLayout_39->setSpacing(6);
        horizontalLayout_39->setObjectName(QStringLiteral("horizontalLayout_39"));
        dataBits_label_3 = new QLabel(layoutWidget_6);
        dataBits_label_3->setObjectName(QStringLiteral("dataBits_label_3"));

        horizontalLayout_39->addWidget(dataBits_label_3);

        emg_dataBits_comboBox = new QComboBox(layoutWidget_6);
        emg_dataBits_comboBox->setObjectName(QStringLiteral("emg_dataBits_comboBox"));

        horizontalLayout_39->addWidget(emg_dataBits_comboBox);


        verticalLayout_13->addLayout(horizontalLayout_39);

        horizontalLayout_40 = new QHBoxLayout();
        horizontalLayout_40->setSpacing(6);
        horizontalLayout_40->setObjectName(QStringLiteral("horizontalLayout_40"));
        verify_label_3 = new QLabel(layoutWidget_6);
        verify_label_3->setObjectName(QStringLiteral("verify_label_3"));

        horizontalLayout_40->addWidget(verify_label_3);

        emg_verify_comboBox = new QComboBox(layoutWidget_6);
        emg_verify_comboBox->setObjectName(QStringLiteral("emg_verify_comboBox"));

        horizontalLayout_40->addWidget(emg_verify_comboBox);


        verticalLayout_13->addLayout(horizontalLayout_40);

        horizontalLayout_41 = new QHBoxLayout();
        horizontalLayout_41->setSpacing(6);
        horizontalLayout_41->setObjectName(QStringLiteral("horizontalLayout_41"));
        emg_start_serialPort_button = new QPushButton(layoutWidget_6);
        emg_start_serialPort_button->setObjectName(QStringLiteral("emg_start_serialPort_button"));

        horizontalLayout_41->addWidget(emg_start_serialPort_button);

        emg_stop_serialPort_button = new QPushButton(layoutWidget_6);
        emg_stop_serialPort_button->setObjectName(QStringLiteral("emg_stop_serialPort_button"));

        horizontalLayout_41->addWidget(emg_stop_serialPort_button);


        verticalLayout_13->addLayout(horizontalLayout_41);

        emg_hex_checkBox = new QCheckBox(layoutWidget_6);
        emg_hex_checkBox->setObjectName(QStringLiteral("emg_hex_checkBox"));

        verticalLayout_13->addWidget(emg_hex_checkBox);

        emg_clear_button = new QPushButton(layoutWidget_6);
        emg_clear_button->setObjectName(QStringLiteral("emg_clear_button"));

        verticalLayout_13->addWidget(emg_clear_button);

        clearData_button = new QPushButton(groupBox_5);
        clearData_button->setObjectName(QStringLiteral("clearData_button"));
        clearData_button->setGeometry(QRect(10, 340, 93, 28));
        saveData_button = new QPushButton(groupBox_5);
        saveData_button->setObjectName(QStringLiteral("saveData_button"));
        saveData_button->setGeometry(QRect(130, 340, 93, 28));
        groupBox_6 = new QGroupBox(EMGPort);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(0, 13, 751, 811));
        emg_receiveWindow = new QTextBrowser(groupBox_6);
        emg_receiveWindow->setObjectName(QStringLiteral("emg_receiveWindow"));
        emg_receiveWindow->setGeometry(QRect(10, 61, 731, 741));
        layoutWidget_7 = new QWidget(groupBox_6);
        layoutWidget_7->setObjectName(QStringLiteral("layoutWidget_7"));
        layoutWidget_7->setGeometry(QRect(13, 13, 731, 31));
        horizontalLayout_42 = new QHBoxLayout(layoutWidget_7);
        horizontalLayout_42->setSpacing(6);
        horizontalLayout_42->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_42->setObjectName(QStringLiteral("horizontalLayout_42"));
        horizontalLayout_42->setContentsMargins(0, 0, 0, 0);
        emg_sendWindow = new QLineEdit(layoutWidget_7);
        emg_sendWindow->setObjectName(QStringLiteral("emg_sendWindow"));

        horizontalLayout_42->addWidget(emg_sendWindow);

        emg_serialSend_button = new QPushButton(layoutWidget_7);
        emg_serialSend_button->setObjectName(QStringLiteral("emg_serialSend_button"));

        horizontalLayout_42->addWidget(emg_serialSend_button);

        tabWidget->addTab(EMGPort, QString());
        MutiData = new QWidget();
        MutiData->setObjectName(QStringLiteral("MutiData"));
        line = new QFrame(MutiData);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(366, 0, 3, 825));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(MutiData);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(734, 0, 3, 825));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        layoutWidget2 = new QWidget(MutiData);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(0, 40, 1101, 781));
        horizontalLayout_18 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        horizontalLayout_18->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        Amode_1_plot = new QCustomPlot(layoutWidget2);
        Amode_1_plot->setObjectName(QStringLiteral("Amode_1_plot"));

        verticalLayout_5->addWidget(Amode_1_plot);

        Amode_2_plot = new QCustomPlot(layoutWidget2);
        Amode_2_plot->setObjectName(QStringLiteral("Amode_2_plot"));

        verticalLayout_5->addWidget(Amode_2_plot);

        Amode_3_plot = new QCustomPlot(layoutWidget2);
        Amode_3_plot->setObjectName(QStringLiteral("Amode_3_plot"));

        verticalLayout_5->addWidget(Amode_3_plot);

        Amode_4_plot = new QCustomPlot(layoutWidget2);
        Amode_4_plot->setObjectName(QStringLiteral("Amode_4_plot"));

        verticalLayout_5->addWidget(Amode_4_plot);


        horizontalLayout_18->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        EMG_1_plot = new QCustomPlot(layoutWidget2);
        EMG_1_plot->setObjectName(QStringLiteral("EMG_1_plot"));

        verticalLayout_6->addWidget(EMG_1_plot);

        EMG_3_plot = new QCustomPlot(layoutWidget2);
        EMG_3_plot->setObjectName(QStringLiteral("EMG_3_plot"));

        verticalLayout_6->addWidget(EMG_3_plot);

        EMG_2_plot = new QCustomPlot(layoutWidget2);
        EMG_2_plot->setObjectName(QStringLiteral("EMG_2_plot"));

        verticalLayout_6->addWidget(EMG_2_plot);

        EMG_4_plot = new QCustomPlot(layoutWidget2);
        EMG_4_plot->setObjectName(QStringLiteral("EMG_4_plot"));

        verticalLayout_6->addWidget(EMG_4_plot);


        horizontalLayout_18->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        IMU_1_plot = new QCustomPlot(layoutWidget2);
        IMU_1_plot->setObjectName(QStringLiteral("IMU_1_plot"));

        verticalLayout_7->addWidget(IMU_1_plot);

        IMU_2_plot = new QCustomPlot(layoutWidget2);
        IMU_2_plot->setObjectName(QStringLiteral("IMU_2_plot"));

        verticalLayout_7->addWidget(IMU_2_plot);

        IMU_3_plot = new QCustomPlot(layoutWidget2);
        IMU_3_plot->setObjectName(QStringLiteral("IMU_3_plot"));

        verticalLayout_7->addWidget(IMU_3_plot);

        IMU_4_plot = new QCustomPlot(layoutWidget2);
        IMU_4_plot->setObjectName(QStringLiteral("IMU_4_plot"));

        verticalLayout_7->addWidget(IMU_4_plot);


        horizontalLayout_18->addLayout(verticalLayout_7);

        layoutWidget3 = new QWidget(MutiData);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(10, 10, 1091, 31));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        Amode_label = new QLabel(layoutWidget3);
        Amode_label->setObjectName(QStringLiteral("Amode_label"));
        Amode_label->setLayoutDirection(Qt::LeftToRight);
        Amode_label->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(Amode_label);

        EMG_label = new QLabel(layoutWidget3);
        EMG_label->setObjectName(QStringLiteral("EMG_label"));
        EMG_label->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(EMG_label);

        IMU_label = new QLabel(layoutWidget3);
        IMU_label->setObjectName(QStringLiteral("IMU_label"));
        IMU_label->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(IMU_label);

        tabWidget->addTab(MutiData, QString());
        Discrete = new QWidget();
        Discrete->setObjectName(QStringLiteral("Discrete"));
        groupBox_3 = new QGroupBox(Discrete);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(11, 11, 1030, 321));
        disGeature_widget_1 = new QWidget(groupBox_3);
        disGeature_widget_1->setObjectName(QStringLiteral("disGeature_widget_1"));
        disGeature_widget_1->setGeometry(QRect(10, 50, 150, 200));
        disGeature_widget_2 = new QWidget(groupBox_3);
        disGeature_widget_2->setObjectName(QStringLiteral("disGeature_widget_2"));
        disGeature_widget_2->setGeometry(QRect(180, 50, 150, 200));
        disGeature_widget_3 = new QWidget(groupBox_3);
        disGeature_widget_3->setObjectName(QStringLiteral("disGeature_widget_3"));
        disGeature_widget_3->setGeometry(QRect(350, 50, 150, 200));
        disGeature_widget_4 = new QWidget(groupBox_3);
        disGeature_widget_4->setObjectName(QStringLiteral("disGeature_widget_4"));
        disGeature_widget_4->setGeometry(QRect(520, 50, 150, 200));
        disGeature_widget_5 = new QWidget(groupBox_3);
        disGeature_widget_5->setObjectName(QStringLiteral("disGeature_widget_5"));
        disGeature_widget_5->setGeometry(QRect(690, 50, 150, 200));
        disGeature_widget_6 = new QWidget(groupBox_3);
        disGeature_widget_6->setObjectName(QStringLiteral("disGeature_widget_6"));
        disGeature_widget_6->setGeometry(QRect(860, 50, 150, 200));
        groupBox_4 = new QGroupBox(Discrete);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 360, 170, 441));
        label = new QLabel(groupBox_4);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 20, 72, 15));
        disGeature_widget_result = new QWidget(groupBox_4);
        disGeature_widget_result->setObjectName(QStringLiteral("disGeature_widget_result"));
        disGeature_widget_result->setGeometry(QRect(10, 80, 150, 200));
        textBrowser = new QTextBrowser(groupBox_4);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(10, 330, 151, 81));
        groupBox_7 = new QGroupBox(Discrete);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(199, 359, 751, 441));
        comboBox = new QComboBox(groupBox_7);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(470, 230, 181, 22));
        comboBox->setInsertPolicy(QComboBox::InsertAtCurrent);
        comboBox_2 = new QComboBox(groupBox_7);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));
        comboBox_2->setGeometry(QRect(470, 305, 181, 22));
        comboBox_2->setLayoutDirection(Qt::LeftToRight);
        comboBox_2->setEditable(false);
        comboBox_2->setInsertPolicy(QComboBox::InsertAtCurrent);
        comboBox_3 = new QComboBox(groupBox_7);
        comboBox_3->setObjectName(QStringLiteral("comboBox_3"));
        comboBox_3->setGeometry(QRect(470, 380, 181, 22));
        comboBox_3->setInsertPolicy(QComboBox::InsertAtCurrent);
        disGesture_Amode_plot = new QCustomPlot(groupBox_7);
        disGesture_Amode_plot->setObjectName(QStringLiteral("disGesture_Amode_plot"));
        disGesture_Amode_plot->setGeometry(QRect(10, 10, 360, 189));
        disGesture_IMU_plot = new QCustomPlot(groupBox_7);
        disGesture_IMU_plot->setObjectName(QStringLiteral("disGesture_IMU_plot"));
        disGesture_IMU_plot->setGeometry(QRect(10, 220, 360, 189));
        disGesture_EMG_plot = new QCustomPlot(groupBox_7);
        disGesture_EMG_plot->setObjectName(QStringLiteral("disGesture_EMG_plot"));
        disGesture_EMG_plot->setGeometry(QRect(380, 10, 360, 189));
        disGesture_start_button = new QPushButton(Discrete);
        disGesture_start_button->setObjectName(QStringLiteral("disGesture_start_button"));
        disGesture_start_button->setGeometry(QRect(980, 370, 93, 28));
        disGesture_stop_button = new QPushButton(Discrete);
        disGesture_stop_button->setObjectName(QStringLiteral("disGesture_stop_button"));
        disGesture_stop_button->setEnabled(false);
        disGesture_stop_button->setGeometry(QRect(980, 580, 93, 28));
        disGesture_train_button = new QPushButton(Discrete);
        disGesture_train_button->setObjectName(QStringLiteral("disGesture_train_button"));
        disGesture_train_button->setEnabled(false);
        disGesture_train_button->setGeometry(QRect(980, 440, 93, 28));
        disGesture_predict_button = new QPushButton(Discrete);
        disGesture_predict_button->setObjectName(QStringLiteral("disGesture_predict_button"));
        disGesture_predict_button->setEnabled(false);
        disGesture_predict_button->setGeometry(QRect(980, 510, 93, 28));
        disGesture_clear_button = new QPushButton(Discrete);
        disGesture_clear_button->setObjectName(QStringLiteral("disGesture_clear_button"));
        disGesture_clear_button->setEnabled(true);
        disGesture_clear_button->setGeometry(QRect(980, 650, 93, 28));
        disGesture_save_button = new QPushButton(Discrete);
        disGesture_save_button->setObjectName(QStringLiteral("disGesture_save_button"));
        disGesture_save_button->setEnabled(true);
        disGesture_save_button->setGeometry(QRect(980, 720, 93, 28));
        tabWidget->addTab(Discrete, QString());

        horizontalLayout_2->addWidget(tabWidget);

        QCplotClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QCplotClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1130, 26));
        menuSetting = new QMenu(menuBar);
        menuSetting->setObjectName(QStringLiteral("menuSetting"));
        menuSetting->setStyleSheet(QStringLiteral("font: 12pt \"Times New Roman\";"));
        QCplotClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QCplotClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QCplotClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QCplotClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QCplotClass->setStatusBar(statusBar);

        menuBar->addAction(menuSetting->menuAction());
        menuSetting->addAction(actionParameter);

        retranslateUi(QCplotClass);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(QCplotClass);
    } // setupUi

    void retranslateUi(QMainWindow *QCplotClass)
    {
        QCplotClass->setWindowTitle(QApplication::translate("QCplotClass", "UltraAnalytics", 0));
        actionParameter->setText(QApplication::translate("QCplotClass", "Parameter", 0));
        predictGesture_3->setText(QString());
        remind_mode1->setText(QString());
        mode1_start->setText(QApplication::translate("QCplotClass", "start", 0));
        mode1_train->setText(QApplication::translate("QCplotClass", "train", 0));
        mode1_predict->setText(QApplication::translate("QCplotClass", "predict", 0));
        channelBox_2->clear();
        channelBox_2->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "channel1", 0)
         << QApplication::translate("QCplotClass", "channel2", 0)
         << QApplication::translate("QCplotClass", "channel3", 0)
         << QApplication::translate("QCplotClass", "channel4", 0)
         << QApplication::translate("QCplotClass", "channel5", 0)
         << QApplication::translate("QCplotClass", "channel6", 0)
         << QApplication::translate("QCplotClass", "channel7", 0)
         << QApplication::translate("QCplotClass", "channel8", 0)
        );
        channelBox_2->setCurrentText(QApplication::translate("QCplotClass", "channel1", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("QCplotClass", "Mode-1", 0));
        wrist_motion->setText(QString());
        remind_mode2->setText(QString());
        mode2_start->setText(QApplication::translate("QCplotClass", "start", 0));
        mode2_train->setText(QApplication::translate("QCplotClass", "train", 0));
        mode2_predict->setText(QApplication::translate("QCplotClass", "predict", 0));
        channelBox_4->clear();
        channelBox_4->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "channel1", 0)
         << QApplication::translate("QCplotClass", "channel2", 0)
         << QApplication::translate("QCplotClass", "channel3", 0)
         << QApplication::translate("QCplotClass", "channel4", 0)
         << QApplication::translate("QCplotClass", "channel5", 0)
         << QApplication::translate("QCplotClass", "channel6", 0)
         << QApplication::translate("QCplotClass", "channel7", 0)
         << QApplication::translate("QCplotClass", "channel8", 0)
        );
        channelBox_4->setCurrentText(QApplication::translate("QCplotClass", "channel1", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("QCplotClass", "Mode-2", 0));
        hand_close_gif->setText(QString());
        wrist_rotation_gif->setText(QString());
        simultaneous_gif->setText(QString());
        toolButton_3->setText(QString());
        EthernetMode->setText(QApplication::translate("QCplotClass", "Ethernet Mode", 0));
        toolButton_2->setText(QString());
        WifiMode->setText(QApplication::translate("QCplotClass", "Wifi Mode", 0));
        Xsens_Connect->setText(QApplication::translate("QCplotClass", "Xsens_On", 0));
        wifi_connect->setText(QApplication::translate("QCplotClass", "Wifi_On", 0));
        wifi_connect->setShortcut(QApplication::translate("QCplotClass", "Return", 0));
        TAC_Connect->setText(QApplication::translate("QCplotClass", "TAC_On", 0));
        Delsys_On->setText(QApplication::translate("QCplotClass", "Delsys_On", 0));
        motion_prompt->setText(QApplication::translate("QCplotClass", "Hand Closing", 0));
        trialno->setText(QApplication::translate("QCplotClass", "Predict Trial #1", 0));
        start_ectype->setText(QApplication::translate("QCplotClass", "Start", 0));
        train_ectype->setText(QApplication::translate("QCplotClass", "Train", 0));
        Run_matlab->setText(QApplication::translate("QCplotClass", "Run_Matlab", 0));
        Train_Model->setText(QApplication::translate("QCplotClass", "Train_Model", 0));
        predict_ectype->setText(QApplication::translate("QCplotClass", "Predict", 0));
        save_ectype->setText(QApplication::translate("QCplotClass", "Save", 0));
        tabWidget->setTabText(tabWidget->indexOf(SPC), QApplication::translate("QCplotClass", "SPC", 0));
        predictGesture->setText(QString());
        start->setText(QApplication::translate("QCplotClass", "Start", 0));
        stop->setText(QApplication::translate("QCplotClass", "Stop", 0));
        train->setText(QApplication::translate("QCplotClass", "Train", 0));
        predict->setText(QApplication::translate("QCplotClass", "Predict", 0));
        accuracy->setText(QApplication::translate("QCplotClass", "Acc", 0));
        capture->setText(QApplication::translate("QCplotClass", "Capture", 0));
        calibration->setText(QApplication::translate("QCplotClass", "Cali", 0));
        save->setText(QApplication::translate("QCplotClass", "Save", 0));
        tabWidget->setTabText(tabWidget->indexOf(Amode), QApplication::translate("QCplotClass", "PR", 0));
        groupBox->setTitle(QString());
        serialPort_label->setText(QApplication::translate("QCplotClass", "\344\270\262\345\217\243\345\217\267", 0));
        serialPort_comboBox->clear();
        serialPort_comboBox->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "COM1", 0)
         << QApplication::translate("QCplotClass", "COM2", 0)
         << QApplication::translate("QCplotClass", "COM3", 0)
         << QApplication::translate("QCplotClass", "COM4", 0)
         << QApplication::translate("QCplotClass", "COM5", 0)
         << QApplication::translate("QCplotClass", "COM6", 0)
         << QApplication::translate("QCplotClass", "COM7", 0)
         << QApplication::translate("QCplotClass", "COM8", 0)
         << QApplication::translate("QCplotClass", "COM9", 0)
         << QApplication::translate("QCplotClass", "COM10", 0)
         << QApplication::translate("QCplotClass", "COM11", 0)
         << QApplication::translate("QCplotClass", "COM12", 0)
        );
        baud_label->setText(QApplication::translate("QCplotClass", "\346\263\242\347\211\271\347\216\207", 0));
        baud_comboBox->clear();
        baud_comboBox->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "1200", 0)
         << QApplication::translate("QCplotClass", "2400", 0)
         << QApplication::translate("QCplotClass", "4800", 0)
         << QApplication::translate("QCplotClass", "9600", 0)
         << QApplication::translate("QCplotClass", "19200", 0)
         << QApplication::translate("QCplotClass", "38400", 0)
         << QApplication::translate("QCplotClass", "57600", 0)
         << QApplication::translate("QCplotClass", "115200", 0)
        );
        stopBit_label->setText(QApplication::translate("QCplotClass", "\345\201\234\346\255\242\344\275\215", 0));
        stopBit_comboBox->clear();
        stopBit_comboBox->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "1", 0)
         << QApplication::translate("QCplotClass", "1.5", 0)
         << QApplication::translate("QCplotClass", "2", 0)
        );
        dataBits_label->setText(QApplication::translate("QCplotClass", "\346\225\260\346\215\256\344\275\215", 0));
        dataBits_comboBox->clear();
        dataBits_comboBox->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "8", 0)
         << QApplication::translate("QCplotClass", "7", 0)
         << QApplication::translate("QCplotClass", "6", 0)
         << QApplication::translate("QCplotClass", "5", 0)
        );
        verify_label->setText(QApplication::translate("QCplotClass", "\346\240\241\351\252\214\344\275\215", 0));
        verify_comboBox->clear();
        verify_comboBox->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "None", 0)
         << QApplication::translate("QCplotClass", "Odd", 0)
         << QApplication::translate("QCplotClass", "Even", 0)
        );
        start_serialPort_button->setText(QApplication::translate("QCplotClass", "\346\211\223\345\274\200\344\270\262\345\217\243", 0));
        stop_serialPort_button->setText(QApplication::translate("QCplotClass", "\345\205\263\351\227\255\344\270\262\345\217\243", 0));
        hex_checkBox->setText(QApplication::translate("QCplotClass", "HEX", 0));
        clear_button->setText(QApplication::translate("QCplotClass", "\346\270\205\351\231\244\346\216\245\346\224\266", 0));
        groupBox_2->setTitle(QString());
        serialSend_button->setText(QApplication::translate("QCplotClass", "\345\217\221\351\200\201", 0));
        tabWidget->setTabText(tabWidget->indexOf(SetSerialPort), QApplication::translate("QCplotClass", "SerialPort", 0));
#ifndef QT_NO_WHATSTHIS
        EMGPort->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_ACCESSIBILITY
        EMGPort->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        groupBox_5->setTitle(QString());
        serialPort_label_3->setText(QApplication::translate("QCplotClass", "\344\270\262\345\217\243\345\217\267", 0));
        emg_serialPort_comboBox->clear();
        emg_serialPort_comboBox->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "COM1", 0)
         << QApplication::translate("QCplotClass", "COM2", 0)
         << QApplication::translate("QCplotClass", "COM3", 0)
         << QApplication::translate("QCplotClass", "COM4", 0)
         << QApplication::translate("QCplotClass", "COM5", 0)
         << QApplication::translate("QCplotClass", "COM6", 0)
         << QApplication::translate("QCplotClass", "COM7", 0)
         << QApplication::translate("QCplotClass", "COM8", 0)
         << QApplication::translate("QCplotClass", "COM9", 0)
         << QApplication::translate("QCplotClass", "COM10", 0)
         << QApplication::translate("QCplotClass", "COM11", 0)
         << QApplication::translate("QCplotClass", "COM12", 0)
        );
        baud_label_3->setText(QApplication::translate("QCplotClass", "\346\263\242\347\211\271\347\216\207", 0));
        emg_baud_comboBox->clear();
        emg_baud_comboBox->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "1200", 0)
         << QApplication::translate("QCplotClass", "2400", 0)
         << QApplication::translate("QCplotClass", "4800", 0)
         << QApplication::translate("QCplotClass", "9600", 0)
         << QApplication::translate("QCplotClass", "19200", 0)
         << QApplication::translate("QCplotClass", "38400", 0)
         << QApplication::translate("QCplotClass", "57600", 0)
         << QApplication::translate("QCplotClass", "115200", 0)
         << QApplication::translate("QCplotClass", "921600", 0)
        );
        stopBit_label_3->setText(QApplication::translate("QCplotClass", "\345\201\234\346\255\242\344\275\215", 0));
        emg_stopBit_comboBox->clear();
        emg_stopBit_comboBox->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "1", 0)
         << QApplication::translate("QCplotClass", "1.5", 0)
         << QApplication::translate("QCplotClass", "2", 0)
        );
        dataBits_label_3->setText(QApplication::translate("QCplotClass", "\346\225\260\346\215\256\344\275\215", 0));
        emg_dataBits_comboBox->clear();
        emg_dataBits_comboBox->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "8", 0)
         << QApplication::translate("QCplotClass", "7", 0)
         << QApplication::translate("QCplotClass", "6", 0)
         << QApplication::translate("QCplotClass", "5", 0)
        );
        verify_label_3->setText(QApplication::translate("QCplotClass", "\346\240\241\351\252\214\344\275\215", 0));
        emg_verify_comboBox->clear();
        emg_verify_comboBox->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "None", 0)
         << QApplication::translate("QCplotClass", "Odd", 0)
         << QApplication::translate("QCplotClass", "Even", 0)
        );
        emg_start_serialPort_button->setText(QApplication::translate("QCplotClass", "\346\211\223\345\274\200\344\270\262\345\217\243", 0));
        emg_stop_serialPort_button->setText(QApplication::translate("QCplotClass", "\345\205\263\351\227\255\344\270\262\345\217\243", 0));
        emg_hex_checkBox->setText(QApplication::translate("QCplotClass", "HEX", 0));
        emg_clear_button->setText(QApplication::translate("QCplotClass", "\346\270\205\351\231\244\346\216\245\346\224\266", 0));
        clearData_button->setText(QApplication::translate("QCplotClass", "Claer", 0));
        saveData_button->setText(QApplication::translate("QCplotClass", "Save", 0));
        groupBox_6->setTitle(QString());
        emg_serialSend_button->setText(QApplication::translate("QCplotClass", "\345\217\221\351\200\201", 0));
        tabWidget->setTabText(tabWidget->indexOf(EMGPort), QApplication::translate("QCplotClass", "EMGPort", 0));
        Amode_label->setText(QApplication::translate("QCplotClass", "A\350\266\205\344\277\241\345\217\267", 0));
        EMG_label->setText(QApplication::translate("QCplotClass", "EMG\344\277\241\345\217\267", 0));
        IMU_label->setText(QApplication::translate("QCplotClass", "IMU\344\277\241\345\217\267", 0));
        tabWidget->setTabText(tabWidget->indexOf(MutiData), QApplication::translate("QCplotClass", "Data", 0));
        groupBox_3->setTitle(QString());
        groupBox_4->setTitle(QString());
        label->setText(QApplication::translate("QCplotClass", "Result", 0));
        groupBox_7->setTitle(QString());
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "A-mode Channel 1", 0)
         << QApplication::translate("QCplotClass", "A-mode Channel 2", 0)
         << QApplication::translate("QCplotClass", "A-mode Channel 3", 0)
         << QApplication::translate("QCplotClass", "A-mode Channel 4", 0)
        );
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "EMG Channel 1", 0)
         << QApplication::translate("QCplotClass", "EMG Channel 2", 0)
         << QApplication::translate("QCplotClass", "EMG Channel 3", 0)
         << QApplication::translate("QCplotClass", "EMG Channel 4", 0)
        );
        comboBox_3->clear();
        comboBox_3->insertItems(0, QStringList()
         << QApplication::translate("QCplotClass", "IMU Channel 1", 0)
         << QApplication::translate("QCplotClass", "IMU Channel 2", 0)
         << QApplication::translate("QCplotClass", "IMU Channel 3", 0)
         << QApplication::translate("QCplotClass", "IMU Channel 4", 0)
        );
        disGesture_start_button->setText(QApplication::translate("QCplotClass", "Start", 0));
        disGesture_stop_button->setText(QApplication::translate("QCplotClass", "Stop", 0));
        disGesture_train_button->setText(QApplication::translate("QCplotClass", "Train", 0));
        disGesture_predict_button->setText(QApplication::translate("QCplotClass", "Predict", 0));
        disGesture_clear_button->setText(QApplication::translate("QCplotClass", "Clear", 0));
        disGesture_save_button->setText(QApplication::translate("QCplotClass", "Save", 0));
        tabWidget->setTabText(tabWidget->indexOf(Discrete), QApplication::translate("QCplotClass", "DisGesture", 0));
        menuSetting->setTitle(QApplication::translate("QCplotClass", "Setting", 0));
    } // retranslateUi

};

namespace Ui {
    class QCplotClass: public Ui_QCplotClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCPLOT_H
