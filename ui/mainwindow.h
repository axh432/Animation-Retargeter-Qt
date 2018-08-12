#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QElapsedTimer>
#include <memory>

using std::unique_ptr;

class GLWidget;
class Engine;

const int MILLISECONDS_PER_FRAME = 16; // ~ at 60 fps

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QApplication* newApp);
    ~MainWindow();
    void run();
    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onOpenToMesh();
    void onOpenFromMesh();
    void onOpenAnimation();
    void endSession();

private:
    unique_ptr<GLWidget> glWidget;
    unique_ptr<QElapsedTimer> timer;
    QApplication * app;
    bool running;
};

#endif
