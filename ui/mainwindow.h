#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QElapsedTimer>
#include <memory>
#include "commands.h"

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
    void retargetAnim();
    void endSession();

    void onNewScene();

    void receiveAnimCommandPlaySource();
    void receiveAnimCommandPlayDest();
    void receiveAnimCommandBindSource();
    void receiveAnimCommandBindDest();
    void receiveAnimCommandPauseSource();
    void receiveAnimCommandPauseDest();

    void receiveViewCommandHideSource();
    void receiveViewCommandHideDest();
    void receiveViewCommandShowSource();
    void receiveViewCommandShowDest();

private:
    void receiveAnimCommand(AnimCommands cmd);

    unique_ptr<GLWidget> glWidget;
    unique_ptr<QElapsedTimer> timer;
    QApplication * app;
    bool running;
};

#endif
