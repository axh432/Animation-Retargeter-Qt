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
    void storeMaterial( QString name,
                        QString alias,
                        QString type,
                        std::unique_ptr<QOpenGLShaderProgram> shader,
                        std::unique_ptr<QOpenGLTexture> diffuse,
                        std::unique_ptr<QOpenGLTexture>local,
                        std::unique_ptr<QOpenGLTexture>height,
                        std::unique_ptr<QOpenGLTexture>specular );

    Material* getMaterial( QString materialName );

    Anim* getAnim( QString animName );

    unique_ptr<GLModel> createGLModel( QString modelName );

    void storeModel( DataBuffer* buffer, QString name );

    void storeAnim( DataBuffer* buffer, QString name );

private:
    QHash<QString, Material*> materials;
    std::vector<Material> materialStorage;

    QHash<QString, Model*> models;
    std::vector<Model> modelStorage;

    QHash<QString, Anim*> anims;
    std::vector<Anim> animStorage;

    unique_ptr<Md5Factory> md5Factory;
};

#endif // RESOURCEMANAGER_H
