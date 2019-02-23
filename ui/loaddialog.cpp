#include "loaddialog.h"
#include "commands.h"
#include "ui/glwidget.h"
#include <iostream>

LoadDialog::LoadDialog(QWidget *parent, GLWidget *glWidget):
    QDialog(parent),
    widget(glWidget),
    SourceAnimLine(new QLineEdit(this)),
    SourceMatLine(new QLineEdit(this)),
    SourceMeshLine(new QLineEdit(this)),
    DestMatLine(new QLineEdit(this)),
    DestMeshLine(new QLineEdit(this)),
    mainLayout(new QGridLayout(this))
{

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowModality(Qt::ApplicationModal);

    setLayout(mainLayout);

    createDialog("Load Source Model and Animation");

    SourceMeshLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    SourceMatLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    SourceAnimLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    DestMeshLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    DestMatLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    SourceMeshLine->clear();
    SourceMatLine->clear();
    SourceAnimLine->clear();

    DestMeshLine->clear();
    DestMatLine->clear();

    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QPushButton *defaultValuesButton = new QPushButton("Default Values", this);

    QPushButton *doneButton = new QPushButton("Done", this);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(defaultValuesButton, SIGNAL(clicked()), this, SLOT(populateWithDefaultValues()));
    connect(doneButton, SIGNAL(clicked()), this, SLOT(onDone()));

    //done/cancel
    mainLayout->addWidget(cancelButton, 5, 0);
    mainLayout->addWidget(defaultValuesButton, 5, 1);
    mainLayout->addWidget(doneButton, 5, 2);

    //resize(700, 300);
    setFixedHeight(400);
    setFixedWidth(400);

}

void LoadDialog::createDialog(QString title){

    QLabel * SourceMeshLabel = new QLabel("Source Mesh:", this);
    QLabel * SourceMatLabel = new QLabel("Source Material:", this);
    QLabel * SourceAnimLabel = new QLabel("Source Animation:", this);

    QLabel * DestMeshLabel = new QLabel("Destination Mesh:", this);
    QLabel * DestMatLabel = new QLabel("Destination Material:", this);

    SourceMeshLine = new QLineEdit(this);
    SourceMatLine = new QLineEdit(this);
    SourceAnimLine = new QLineEdit(this);

    DestMeshLine = new QLineEdit(this);
    DestMatLine = new QLineEdit(this);

    QPushButton * SourceMeshFind = new QPushButton("Find", this);
    QPushButton * SourceMatFind = new QPushButton("Find", this);
    QPushButton * SourceAnimFind = new QPushButton("Find", this);

    QPushButton * DestMeshFind = new QPushButton("Find", this);
    QPushButton * DestMatFind = new QPushButton("Find", this);

    connect(SourceMeshFind, SIGNAL(clicked()), this, SLOT(openSourceModelFile()));
    connect(SourceMatFind, SIGNAL(clicked()), this, SLOT(openSourceMaterialFile()));
    connect(SourceAnimFind, SIGNAL(clicked()), this, SLOT(openSourceAnimFile()));

    connect(DestMeshFind, SIGNAL(clicked()), this, SLOT(openDestModelFile()));
    connect(DestMatFind, SIGNAL(clicked()), this, SLOT(openDestMaterialFile()));

    //Source Paths
    createRow(SourceMeshLabel, SourceMeshLine, SourceMeshFind, 0);
    createRow(SourceMatLabel, SourceMatLine, SourceMatFind, 1);
    createRow(SourceAnimLabel, SourceAnimLine, SourceAnimFind, 2);

    //Destination Paths
    createRow(DestMeshLabel, DestMeshLine, DestMeshFind, 3);
    createRow(DestMatLabel, DestMatLine, DestMatFind, 4);

    setWindowTitle(title);
}

void LoadDialog::createRow(QLabel* label, QLineEdit* line, QPushButton* findButton, int rowNumber){

    mainLayout->addWidget(label, rowNumber, 0);
    mainLayout->addWidget(line, rowNumber, 1, 1, 1);
    mainLayout->addWidget(findButton, rowNumber, 2);

}

QSize LoadDialog::minimumSizeHint() const{
    return QSize(400, 400);
}

QSize LoadDialog::sizeHint() const{
    return QSize(400, 400);
}

void LoadDialog::onOpenFile(QString title, QString fileType, QLineEdit * line){
    QString filename = QFileDialog::getOpenFileName(this, title, QDir::currentPath(), fileType);

    if(!filename.isEmpty()){
        line->clear();
        line->setText(filename);
    }
}

void LoadDialog::populateWithDefaultValues(){
    SourceMeshLine->setText("/Users/Alex/Documents/QtProjects/RenderWindow/resources/fatty.md5mesh");
    SourceMatLine->setText("/Users/Alex/Documents/QtProjects/RenderWindow/resources/fatty.mtr");
    SourceAnimLine->setText("/Users/Alex/Documents/QtProjects/RenderWindow/resources/walk1.md5anim");
    DestMeshLine->setText("/Users/Alex/Documents/QtProjects/RenderWindow/resources/imp.md5mesh");
    DestMatLine->setText("/Users/Alex/Documents/QtProjects/RenderWindow/resources/imp.mtr");
}

void LoadDialog::onDone(){

    std::shared_ptr<EntityResourcePaths> sourceResourcePaths(new EntityResourcePaths());
    std::shared_ptr<EntityResourcePaths> destinationResourcePaths(new EntityResourcePaths());

    //get file paths
    sourceResourcePaths->modelPath = SourceMeshLine->text();
    sourceResourcePaths->materialPath =  SourceMatLine->text();
    sourceResourcePaths->animationPath = SourceAnimLine->text();

    destinationResourcePaths->modelPath = DestMeshLine->text();
    destinationResourcePaths->materialPath =  DestMatLine->text();

    //no mesh file path?
    if(sourceResourcePaths->modelPath.isEmpty()){
        QMessageBox::warning(this, tr("Problem loading Source Model: "), "You have not provided a path for an .md5model file", QMessageBox::Ok);
        return;
    }

    if(sourceResourcePaths->materialPath.isEmpty()){
        QMessageBox::information(this, tr("Problem loading Source Model: "), "You have not provided a path for an .mtr file. Using the default material.", QMessageBox::Ok);
    }

    //if we're loading the source model we need an anim path
    if(sourceResourcePaths->animationPath.isEmpty()){
        QMessageBox::warning(this, tr("Problem loading Source Model: "), "You have not provided a path for an .md5anim file", QMessageBox::Ok);
        return;
    }

    if(destinationResourcePaths->modelPath.isEmpty()){
        QMessageBox::warning(this, tr("Problem loading Destination Model: "), "You have not provided a path for an .md5model file", QMessageBox::Ok);
        return;
    }

    if(destinationResourcePaths->materialPath.isEmpty()){
        QMessageBox::information(this, tr("Problem loading Destination Model: "), "You have not provided a path for an .mtr file. Using the default material.", QMessageBox::Ok);
    }

    //load these entities
    try{
        widget->loadEntities(sourceResourcePaths, destinationResourcePaths);
    }catch(const std::exception& ex){
      QMessageBox::warning(this, tr("Problem loading Scene: "), ex.what(), QMessageBox::Ok);
      return;
    }

    accept();
}
