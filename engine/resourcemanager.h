#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QHash>
#include <vector>
#include "material.h"
#include "lib/md5/md5.h"
#include "lib/md5/md5factory.h"

class GraphicsCardSpace {

public:
    size_t vertexSpaceInBytes;
    size_t indexSpaceInBytes;
    size_t textureSpaceInBytes;

    inline GraphicsCardSpace add(GraphicsCardSpace& other){
        GraphicsCardSpace sum;

        sum.indexSpaceInBytes = this->indexSpaceInBytes + other.indexSpaceInBytes;
        sum.textureSpaceInBytes = this->textureSpaceInBytes + other.textureSpaceInBytes;
        sum.vertexSpaceInBytes = this->vertexSpaceInBytes + other.vertexSpaceInBytes;

        return sum;
    }
};

class ResourceManager
{
public:
    ResourceManager();

    Material* getMaterial( QString materialName );

    Anim* getAnim( QString animName );

    Model* getModel( QString modelName );

   // unique_ptr<GLModel> createGLModel( QString modelName );

    void storeModel( DataBuffer* buffer, QString name );

    void storeAnim( DataBuffer* buffer, QString name );

    void storeMaterial(DataBuffer& buffer, QString materialPath);

    GraphicsCardSpace computeGraphicsCardSpaceForModel( QString modelName );

private:
    QHash<QString, size_t> materials;
    std::vector<Material> materialStorage;

    QHash<QString, size_t> models;
    std::vector<Model> modelStorage;

    QHash<QString, size_t> anims;
    std::vector<Anim> animStorage;

    unique_ptr<Md5Factory> md5Factory;

    unique_ptr<QOpenGLShaderProgram> createShaderProgram(QString vertexShaderPath, QString fragmentShaderPath);
    unique_ptr<QOpenGLTexture> loadTexture(QString texturePath);
};

#endif // RESOURCEMANAGER_H
