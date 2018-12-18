#ifndef GLDATA_H
#define GLDATA_H

#include <QOpenGLBuffer>

struct GLData {

public:
    int numIndices;
    quintptr indicesPtr;
    quintptr verticesPtr;
    quintptr texturePtr;
    QOpenGLBuffer* indexBuffer;
    QOpenGLBuffer* textureBuffer;
    QOpenGLBuffer* vertexBuffer;
};

#endif // GLDATA_H
