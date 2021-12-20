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
#include <QtWidgets/QGridLayout>
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
    QMenuBar *menuBar;
    QMenu *menuSetting;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QCplotClass)
    {
        if (QCplotClass->objectName().isEmpty())
            QCplotClass->setObjectName(QStringLiteral("QCplotClass"));
        QCplotClass->resize(1124, 698);
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

        horizontalLayout_2->addWidget(tabWidget);

        QCplotClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QCplotClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1124, 23));
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

        tabWidget->setCurrentIndex(2);


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
        menuSetting->setTitle(QApplication::translate("QCplotClass", "Setting", 0));
    } // retranslateUi

};

namespace Ui {
    class QCplotClass: public Ui_QCplotClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCPLOT_H
