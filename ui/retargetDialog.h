#ifndef RETARGETDIALOG_H
#define RETARGETDIALOG_H

#include <QDialog>
#include <QVector>
#include <QString>
#include <QGridLayout>
#include <QScrollArea>
#include <vector>

#include "glwidget.h"

QT_BEGIN_NAMESPACE
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
QT_END_NAMESPACE

class RetargetDialog : public QDialog
{
    Q_OBJECT

public:
    RetargetDialog(QWidget *parent, QPair<Skeleton*, Skeleton*> skeletons, GLWidget *glWidget);

private slots:
    //void refresh();
    void updateMap();

private:
    QGridLayout* setupMainLayout();
    QStringList createDestinationBoxItems(Skeleton* skeleton);
    void createComboBoxes(Skeleton* source, QGridLayout* mainLayout, QStringList destinationBoxItems);
    QScrollArea* createScrollArea();
    QWidget* createViewPort(QScrollArea* scroll);
    QGridLayout* createMainLayout(QWidget* viewport);

    QVector<QComboBox *> comboBoxes;
    QVector<QLabel *> labels;
    QVector<QCheckBox *> checkBoxes;
    GLWidget *glWidget;

};

#endif
