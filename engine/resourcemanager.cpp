#include "resourcemanager.h"

ResourceManager::ResourceManager()
{
    md5Factory.reset(new Md5Factory());
}

void ResourceManager::storeMaterial( QString name,
                    QString alias,
                    QString type,
                    std::unique_ptr<QOpenGLShaderProgram> shader,
                    std::unique_ptr<QOpenGLTexture> diffuse,
                    std::unique_ptr<QOpenGLTexture>local,
                    std::unique_ptr<QOpenGLTexture>height,
                    std::unique_ptr<QOpenGLTexture>specular ){

    materialStorage.emplace_back(name, alias, type, std::move(shader), std::move(diffuse), std::move(local), std::move(height), std::move(specular));
    Material* material = &materialStorage[materialStorage.size() - 1];

    materials.insert(material->getName(), material);

}

void ResourceManager::storeModel(DataBuffer* buffer, QString name){

    md5Factory->buildModel(buffer, name, modelStorage);

    Model* modelRef = &modelStorage[modelStorage.size() - 1];

    models.insert(modelRef->name, modelRef);
}

void ResourceManager::storeAnim( DataBuffer* buffer, QString name ){

    md5Factory->buildAnim(buffer, name, animStorage);

    Anim* animRef = &animStorage[animStorage.size() - 1];

    anims.insert(animRef->name, animRef);
}

unique_ptr<GLModel> ResourceManager::createGLModel( QString modelName ){

    Model* model = models.find(modelName).value();

    vector<GLMesh> glmeshes;
    vector<Mesh>& meshes = model->meshes;

    for(int i = 0; i < meshes.size(); i++){
        Mesh* mesh = &meshes[i];

        QString materialName = mesh->getMaterialName();

        Material* material = getMaterial(materialName);

        glmeshes.emplace_back(mesh, material);

        glmeshes[i].update(model->bindPose.get());
    }

    unique_ptr<GLModel> glModel(new GLModel(std::move(glmeshes), model));

    return std::move(glModel);
}

Material* ResourceManager::getMaterial( QString materialName ){
    return materials.find(materialName).value();
}


Anim* ResourceManager::getAnim(QString animName){
    return anims.find(animName).value();
}
