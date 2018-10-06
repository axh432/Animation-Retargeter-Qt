#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QHash>
#include <vector>
#include "material.h"
#include "lib/md5/md5.h"
#include "lib/md5/md5factory.h"

class ResourceManager
{
public:
    ResourceManager();

    Material* getMaterial( QString materialName );

    Anim* getAnim( QString animName );

    unique_ptr<GLModel> createGLModel( QString modelName );

    void storeModel( DataBuffer* buffer, QString name );

    void storeAnim( DataBuffer* buffer, QString name );

    void storeMaterial(DataBuffer& buffer);

private:
    QHash<QString, Material*> materials;
    std::vector<Material> materialStorage;

    QHash<QString, Model*> models;
    std::vector<Model> modelStorage;

    QHash<QString, Anim*> anims;
    std::vector<Anim> animStorage;

    unique_ptr<Md5Factory> md5Factory;

    unique_ptr<QOpenGLShaderProgram> createShaderProgram(QString vertexShaderPath, QString fragmentShaderPath);
    unique_ptr<QOpenGLTexture> loadTexture(QString texturePath);
};

#endif // RESOURCEMANAGER_H
