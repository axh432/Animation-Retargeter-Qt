#ifndef GRAPHICSCARDMEMORYMANAGER_H
#define GRAPHICSCARDMEMORYMANAGER_H

#include <QOpenGLBuffer>
#include <vector>
#include "gldata.h"
#include "lib/md5/md5.h"
#include "engine/resourcemanager.h"

using std::vector;

class GraphicsCardMemoryManager
{
public:
    GraphicsCardMemoryManager(size_t vertexBufferSize, size_t textureBufferSize, size_t indexBufferSize);
    GraphicsCardMemoryManager(GraphicsCardSpace& space);
    ~GraphicsCardMemoryManager();
    unique_ptr<GLData> storeMeshOnGraphicsCard(Mesh* mesh, Skeleton* skeleton);
    unique_ptr<GLModel> createGLModel(QString modelName, ResourceManager* resourceManager);

private:
    quintptr storeTextureCoordsOnGraphicsCard(vector<GLfloat> texCoords);
    quintptr storeIndicesOnGraphicsCard(vector<GLuint> indices);
    quintptr storeVertsOnGraphicsCard(vector<GLfloat> verts);

    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer indexBuffer;
    QOpenGLBuffer textureBuffer;

    quintptr indexPtr;
    quintptr vertexPtr;
    quintptr texturePtr;

};

#endif // GRAPHICSCARDMEMORYMANAGER_H
