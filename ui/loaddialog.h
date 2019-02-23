#ifndef LOADDIALOG_H
#define LOADDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QFileDialog>
#include "ui/glwidget.h"
#include "ui/commands.h"
#include "engine/entityresourcepaths.h"

class LoadDialog : public QDialog {

    Q_OBJECT

public:
    LoadDialog(QWidget *parent, GLWidget *glWidget);
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

private slots:
    inline void openSourceModelFile(){ onOpenFile(tr("Open Model"), tr("MD5 mesh files (*.md5mesh)"), SourceMeshLine); }
    inline void openSourceMaterialFile(){ onOpenFile(tr("Open Material"), tr("MTR files (*.mtr)"), SourceMatLine); }
    inline void openSourceAnimFile(){ onOpenFile(tr("Open Animation"), tr("MD5 anim files (*.md5anim)"), SourceAnimLine); }
    inline void openDestModelFile(){ onOpenFile(tr("Open Model"), tr("MD5 mesh files (*.md5mesh)"), DestMeshLine); }
    inline void openDestMaterialFile(){ onOpenFile(tr("Open Material"), tr("MTR files (*.mtr)"), DestMatLine); }

    void populateWithDefaultValues();

    void onDone();

private:
    void createRow(QLabel* label, QLineEdit* line, QPushButton* findButton, int rowNumber);
    void onOpenFile(QString title, QString fileType, QLineEdit * line);
    QLineEdit * SourceAnimLine;
    QLineEdit * SourceMeshLine;
    QLineEdit * SourceMatLine;

    QLineEdit * DestMeshLine;
    QLineEdit * DestMatLine;

    QGridLayout *mainLayout;
    GLWidget  * widget;

    void createDialog(QString title);

};

#endif // LOADDIALOG_H
