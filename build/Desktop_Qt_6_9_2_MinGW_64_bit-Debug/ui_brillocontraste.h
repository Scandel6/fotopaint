/********************************************************************************
** Form generated from reading UI file 'brillocontraste.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BRILLOCONTRASTE_H
#define UI_BRILLOCONTRASTE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_brillocontraste
{
public:
    QDialogButtonBox *buttonBox;
    QSlider *horizontalSlider;
    QLabel *label;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QSlider *horizontalSlider_2;
    QLabel *label_2;
    QCheckBox *checkBox;

    void setupUi(QDialog *brillocontraste)
    {
        if (brillocontraste->objectName().isEmpty())
            brillocontraste->setObjectName("brillocontraste");
        brillocontraste->setWindowModality(Qt::WindowModality::WindowModal);
        brillocontraste->resize(380, 150);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(brillocontraste->sizePolicy().hasHeightForWidth());
        brillocontraste->setSizePolicy(sizePolicy);
        brillocontraste->setMinimumSize(QSize(380, 150));
        brillocontraste->setMaximumSize(QSize(380, 150));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imagenes/icono.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        brillocontraste->setWindowIcon(icon);
        buttonBox = new QDialogButtonBox(brillocontraste);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(10, 110, 161, 32));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);
        horizontalSlider = new QSlider(brillocontraste);
        horizontalSlider->setObjectName("horizontalSlider");
        horizontalSlider->setGeometry(QRect(10, 30, 271, 21));
        horizontalSlider->setMinimum(-256);
        horizontalSlider->setMaximum(256);
        horizontalSlider->setPageStep(16);
        horizontalSlider->setOrientation(Qt::Orientation::Horizontal);
        horizontalSlider->setTickPosition(QSlider::TickPosition::TicksBelow);
        horizontalSlider->setTickInterval(32);
        label = new QLabel(brillocontraste);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 10, 271, 16));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        spinBox = new QSpinBox(brillocontraste);
        spinBox->setObjectName("spinBox");
        spinBox->setGeometry(QRect(290, 10, 81, 22));
        spinBox->setMinimum(-256);
        spinBox->setMaximum(256);
        spinBox_2 = new QSpinBox(brillocontraste);
        spinBox_2->setObjectName("spinBox_2");
        spinBox_2->setGeometry(QRect(290, 60, 81, 22));
        spinBox_2->setMinimum(-100);
        spinBox_2->setMaximum(100);
        horizontalSlider_2 = new QSlider(brillocontraste);
        horizontalSlider_2->setObjectName("horizontalSlider_2");
        horizontalSlider_2->setGeometry(QRect(10, 80, 271, 21));
        horizontalSlider_2->setMinimum(-100);
        horizontalSlider_2->setMaximum(100);
        horizontalSlider_2->setOrientation(Qt::Orientation::Horizontal);
        horizontalSlider_2->setTickPosition(QSlider::TickPosition::TicksBelow);
        horizontalSlider_2->setTickInterval(10);
        label_2 = new QLabel(brillocontraste);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 60, 271, 16));
        label_2->setStyleSheet(QString::fromUtf8("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);"));
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        checkBox = new QCheckBox(brillocontraste);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(190, 118, 101, 17));
        checkBox->setChecked(true);

        retranslateUi(brillocontraste);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, brillocontraste, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, brillocontraste, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(brillocontraste);
    } // setupUi

    void retranslateUi(QDialog *brillocontraste)
    {
        brillocontraste->setWindowTitle(QCoreApplication::translate("brillocontraste", "Brillo/contraste...", nullptr));
        label->setText(QCoreApplication::translate("brillocontraste", "Brillo", nullptr));
        label_2->setText(QCoreApplication::translate("brillocontraste", "Contraste", nullptr));
        checkBox->setText(QCoreApplication::translate("brillocontraste", "Previsualizar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class brillocontraste: public Ui_brillocontraste {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BRILLOCONTRASTE_H
