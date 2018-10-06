#include "resourcemanager.h"

ResourceManager::ResourceManager()
{
    md5Factory.reset(new Md5Factory());
}

std::unique_ptr<QOpenGLShaderProgram> ResourceManager::createShaderProgram(QString vertexShaderPath, QString fragmentShaderPath){

    std::unique_ptr<QOpenGLShaderProgram> program(new QOpenGLShaderProgram());

    // Compile vertex shader
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath));


    // Compile fragment shader
    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath));

    // Link shader pipeline
    if (!program->link());

    // Bind shader pipeline for use
    if (!program->bind());

    return std::move(program);
}

std::unique_ptr<QOpenGLTexture> ResourceManager::loadTexture(QString texturePath){

    std::unique_ptr<QOpenGLTexture> texture(new QOpenGLTexture(QImage(texturePath)));

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    return texture;
}

void ResourceManager::storeMaterial(DataBuffer& buffer){

    QString name = buffer.popString();
    QString alias = buffer.popString();
    QString type = buffer.popString();
    QString vertshader = buffer.popString();
    QString fragshader = buffer.popString();
    QString diffusePath = buffer.popString();
    QString localPath = buffer.popString();
    QString heightPath = buffer.popString();
    QString specularPath = buffer.popString();

    std::unique_ptr<QOpenGLShaderProgram> shader(nullptr);
    std::unique_ptr<QOpenGLTexture> diffuse(nullptr);
    std::unique_ptr<QOpenGLTexture> local(nullptr);
    std::unique_ptr<QOpenGLTexture> height(nullptr);
    std::unique_ptr<QOpenGLTexture> specular(nullptr);


    if(!vertshader.isEmpty() && !fragshader.isEmpty()){
        shader = std::move(createShaderProgram(vertshader, fragshader));
    }else{
        throw runtime_error("Error: material definition does not contain paths to shader source");
    }

    if(!diffusePath.isEmpty()){
        diffuse = std::move(loadTexture(diffusePath));
    }

    if(!localPath.isEmpty()){
        local = std::move(loadTexture(localPath));
    }

    if(!heightPath.isEmpty()){
        height = std::move(loadTexture(heightPath));
    }

    if(!specularPath.isEmpty()){
        specular = std::move(loadTexture(specularPath));
    }

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

    if(materials.contains(materialName)){
        return materials.find(materialName).value();
    }else{
        return nullptr;
    }
}


Anim* ResourceManager::getAnim(QString animName){

    if(anims.contains(animName)){
        return anims.find(animName).value();
    }else{
        return nullptr;
    }
}
