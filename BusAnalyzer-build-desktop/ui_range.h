/********************************************************************************
** Form generated from reading UI file 'range.ui'
**
** Created: Wed Jun 8 17:26:16 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RANGE_H
#define UI_RANGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSlider>

QT_BEGIN_NAMESPACE

class Ui_Range
{
public:
    QDialogButtonBox *okBox;
    QGroupBox *minBox;
    QSlider *minSlider;
    QLabel *minMax;
    QLabel *minMin;
    QLabel *currentMinValue;
    QGroupBox *maxBox;
    QSlider *maxSlider;
    QLabel *maxMax;
    QLabel *maxMin;
    QLabel *currentMaxValue;
    QLabel *title;

    void setupUi(QDialog *Range)
    {
        if (Range->objectName().isEmpty())
            Range->setObjectName(QString::fromUtf8("Range"));
        Range->resize(657, 280);
        okBox = new QDialogButtonBox(Range);
        okBox->setObjectName(QString::fromUtf8("okBox"));
        okBox->setGeometry(QRect(250, 230, 161, 32));
        okBox->setOrientation(Qt::Horizontal);
        okBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        minBox = new QGroupBox(Range);
        minBox->setObjectName(QString::fromUtf8("minBox"));
        minBox->setGeometry(QRect(10, 40, 631, 80));
        minSlider = new QSlider(minBox);
        minSlider->setObjectName(QString::fromUtf8("minSlider"));
        minSlider->setGeometry(QRect(20, 30, 591, 20));
        minSlider->setOrientation(Qt::Horizontal);
        minMax = new QLabel(minBox);
        minMax->setObjectName(QString::fromUtf8("minMax"));
        minMax->setGeometry(QRect(565, 60, 61, 20));
        minMin = new QLabel(minBox);
        minMin->setObjectName(QString::fromUtf8("minMin"));
        minMin->setGeometry(QRect(10, 60, 46, 13));
        currentMinValue = new QLabel(minBox);
        currentMinValue->setObjectName(QString::fromUtf8("currentMinValue"));
        currentMinValue->setGeometry(QRect(565, 10, 61, 20));
        maxBox = new QGroupBox(Range);
        maxBox->setObjectName(QString::fromUtf8("maxBox"));
        maxBox->setGeometry(QRect(10, 140, 631, 80));
        maxSlider = new QSlider(maxBox);
        maxSlider->setObjectName(QString::fromUtf8("maxSlider"));
        maxSlider->setGeometry(QRect(20, 30, 591, 20));
        maxSlider->setOrientation(Qt::Horizontal);
        maxMax = new QLabel(maxBox);
        maxMax->setObjectName(QString::fromUtf8("maxMax"));
        maxMax->setGeometry(QRect(565, 60, 61, 20));
        maxMin = new QLabel(maxBox);
        maxMin->setObjectName(QString::fromUtf8("maxMin"));
        maxMin->setGeometry(QRect(10, 60, 46, 13));
        currentMaxValue = new QLabel(maxBox);
        currentMaxValue->setObjectName(QString::fromUtf8("currentMaxValue"));
        currentMaxValue->setGeometry(QRect(565, 10, 61, 20));
        title = new QLabel(Range);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(20, 10, 341, 16));

        retranslateUi(Range);
        QObject::connect(okBox, SIGNAL(accepted()), Range, SLOT(accept()));
        QObject::connect(okBox, SIGNAL(rejected()), Range, SLOT(reject()));

        QMetaObject::connectSlotsByName(Range);
    } // setupUi

    void retranslateUi(QDialog *Range)
    {
        Range->setWindowTitle(QApplication::translate("Range", "Dialog", 0, QApplication::UnicodeUTF8));
        minBox->setTitle(QApplication::translate("Range", "Select Min:", 0, QApplication::UnicodeUTF8));
        minMax->setText(QApplication::translate("Range", "64964368", 0, QApplication::UnicodeUTF8));
        minMin->setText(QApplication::translate("Range", "00001", 0, QApplication::UnicodeUTF8));
        currentMinValue->setText(QApplication::translate("Range", "64964368", 0, QApplication::UnicodeUTF8));
        maxBox->setTitle(QApplication::translate("Range", "Select Max:", 0, QApplication::UnicodeUTF8));
        maxMax->setText(QApplication::translate("Range", "64964368", 0, QApplication::UnicodeUTF8));
        maxMin->setText(QApplication::translate("Range", "00001", 0, QApplication::UnicodeUTF8));
        currentMaxValue->setText(QApplication::translate("Range", "64964368", 0, QApplication::UnicodeUTF8));
        title->setText(QApplication::translate("Range", "title", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Range: public Ui_Range {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RANGE_H
