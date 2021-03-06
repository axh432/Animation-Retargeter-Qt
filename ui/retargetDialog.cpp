#include <QtWidgets>
#include <iostream>
#include "retargetdialog.h"

using std::cout;
using std::endl;

RetargetDialog::RetargetDialog(QWidget *parent, QPair<Skeleton*, Skeleton*> skeletons, GLWidget *glWidget)
    : QDialog(parent), glWidget(glWidget)
{

    QScrollArea *scroll = createScrollArea();
    QWidget * viewport = createViewPort(scroll);
    QGridLayout* mainLayout = createMainLayout(viewport);

    createComboBoxes(skeletons.first, mainLayout, createDestinationBoxItems(skeletons.second));

    QHBoxLayout * dialog_layout = new QHBoxLayout(this);
    this->setLayout(dialog_layout);
    this->layout()->addWidget(scroll);

    setWindowTitle(tr("Retarget"));
    setFixedHeight(sizeHint().height());
}

QScrollArea* RetargetDialog::createScrollArea(){
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    return scroll;
}

QWidget* RetargetDialog::createViewPort(QScrollArea* scroll){

    QWidget * viewport = new QWidget(this);
    scroll->setWidget(viewport);
    scroll->setWidgetResizable(true);

    return viewport;
}

void RetargetDialog::createComboBoxes(Skeleton* source, QGridLayout* mainLayout, QStringList destinationBoxItems){

    vector<Joint>& joints = source->getJoints();

    for(int i = 0; i < joints.size(); i++){

        QString sourceJointName = joints[i].name;

        QComboBox * cbox = new QComboBox;
        cbox->setParent(this);
        cbox->addItems(destinationBoxItems);
        comboBoxes.push_back(cbox);

        int potentialMatchIndex = cbox->findText(sourceJointName, Qt::MatchFixedString);

        if(potentialMatchIndex > -1){
            cbox->setCurrentIndex(potentialMatchIndex);
        }

        QLabel * label = new QLabel(sourceJointName);
        label->setParent(this);
        label->setBuddy(cbox);
        labels.push_back(label);

        QCheckBox * chkBox = new QCheckBox;
        chkBox->setParent(this);
        checkBoxes.push_back(chkBox);

        int row = i + 1;

        mainLayout->addWidget(label, row, 0);
        mainLayout->addWidget(cbox, row, 1);
        mainLayout->addWidget(chkBox, row, 2);

    }

    QPushButton *doneButton = new QPushButton("Done", this);
    mainLayout->addWidget(doneButton, joints.size() + 1, 0);

    QPushButton *clearButton = new QPushButton("Clear", this);
    mainLayout->addWidget(doneButton, joints.size() + 1, 1);


    connect(doneButton, SIGNAL(clicked()), this, SLOT(updateMap()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearWidgets()));
}

QStringList RetargetDialog::createDestinationBoxItems(Skeleton* skeleton){
    QStringList boxItems;
    vector<Joint>& joints = skeleton->getJoints();

    boxItems << "";

    for(int i = 0; i < joints.size(); i++){
        boxItems << joints[i].name;
    }

    return boxItems;
}

QGridLayout* RetargetDialog::createMainLayout(QWidget* viewport){

    setWindowModality(Qt::NonModal);

    QGridLayout *mainLayout = new QGridLayout(viewport);

    QLabel * sourceJointLabel = new QLabel(QString("Source Joint"));
    sourceJointLabel->setParent(this);

    QLabel * destinationJointLabel = new QLabel(QString("Destination Joint"));
    destinationJointLabel->setParent(this);

    QLabel * rootLabel = new QLabel(QString("Root"));
    rootLabel->setParent(this);

    mainLayout->addWidget(sourceJointLabel, 0, 0);
    mainLayout->addWidget(destinationJointLabel, 0, 1);
    mainLayout->addWidget(rootLabel, 0, 2);

    viewport->setLayout(mainLayout);

    return mainLayout;
}

void RetargetDialog::updateMap(){
    qDebug() << "updateMap";

    vector<int> jointMappings;

    for(int i = 0; i < labels.size(); i++){
        jointMappings.push_back(comboBoxes[i]->currentIndex() -1);
        qDebug() << labels[i]->text() << " " << comboBoxes[i]->currentText();
    }

    glWidget->retargetAnimation(jointMappings);

    done(1);
}

void RetargetDialog::clearWidgets(){

    for(int i = 0; i < labels.size(); i++){
        comboBoxes[i]->setCurrentIndex(0);
    }
}
