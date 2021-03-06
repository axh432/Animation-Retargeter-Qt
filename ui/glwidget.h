#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include "commands.h"
#include "engine/engine.h"
#include <QTime>
#include <memory>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
using std::unique_ptr;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget();
    ~GLWidget();

    void updateWorld(qint64 delta);

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void handleKeyEvent(QKeyEvent *e);
    void loadEntities(std::shared_ptr<EntityResourcePaths> sourcePaths, std::shared_ptr<EntityResourcePaths> destinationPaths);
    void changeSourceAnimState(AnimState newState);
    void changeDestinationAnimState(AnimState newState);
    void changeSourceVisualState(VisualState newState);
    void changeDestinationVisualState(VisualState newState);
    QPair<Skeleton*, Skeleton*> getSkeletonsForRetargeting();
    void retargetAnimation(vector<int> jointMappings);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

private:
    unique_ptr<Engine> engine;
};

#endif
