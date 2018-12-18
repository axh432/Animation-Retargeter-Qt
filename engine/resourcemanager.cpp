#include "resourcemanager.h"

ResourceManager::ResourceManager()
{
    md5Factory.reset(new Md5Factory());
}

std::unique_ptr<QOpenGLShaderProgram> ResourceManager::createShaderProgram(QString vertexShaderPath, QString fragmentShaderPath){

    std::unique_ptr<QOpenGLShaderProgram> program(new QOpenGLShaderProgram());

    // Compile vertex shader
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath)){
            throw runtime_error("Error: Vertex shader compilation failed - " + program->log().toStdString());
        }


    // Compile fragment shader
    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath)){
            throw runtime_error("Error: Fragment shader compilation failed - " + program->log().toStdString());
    }

    // Link shader pipeline
    if (!program->link()){
            throw runtime_error("Error: Shader linking failed - " + program->log().toStdString());
    }

    // Bind shader pipeline for use
    if (!program->bind()){
        throw runtime_error("Error: Shader binding failed - " + program->log().toStdString());
    }

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

    size_t materialIndex = materialStorage.size() - 1;

    materials.insert(name, materialIndex);
}


void ResourceManager::storeModel(DataBuffer* buffer, QString name){

    md5Factory->buildModel(buffer, name, modelStorage);

    size_t modelIndex = modelStorage.size() - 1;

    models.insert(name, modelIndex);
}

void ResourceManager::storeAnim( DataBuffer* buffer, QString name ){

    md5Factory->buildAnim(buffer, name, animStorage);

    size_t animIndex = animStorage.size() - 1;

    anims.insert(name, animIndex);
}

Material* ResourceManager::getMaterial( QString materialName ){

    if(materials.contains(materialName)){
        size_t index = materials.find(materialName).value();
        return &materialStorage[index];
    }else{
        return nullptr;
    }
}

Model* ResourceManager::getModel(QString modelName){

    if(models.contains(modelName)){
        size_t index = models.find(modelName).value();
        return &modelStorage[index];
    }else{
        return nullptr;
    }
}

Anim* ResourceManager::getAnim(QString animName){

    if(anims.contains(animName)){
        size_t index = anims.find(animName).value();
        return &animStorage[index];
    }else{
        return nullptr;
    }
}

GraphicsCardSpace ResourceManager::computeGraphicsCardSpaceForModel( QString modelName ){

    Model* model = getModel(modelName);

    vector<Mesh>& meshes = model->meshes;
    Skeleton* bindPose = model->bindPose.get();

    GraphicsCardSpace space = {0, 0, 0};

    for(int i = 0; i < meshes.size(); i++){
        Mesh* mesh = &meshes[i];

        std::vector<GLfloat> vertices = mesh->computeGLVertices(bindPose);
        std::vector<GLuint> indices = mesh->getTris();
        std::vector<GLfloat> texCoords = mesh->getTextureCoords();

        space.vertexSpaceInBytes += vertices.size() * sizeof(GLfloat);
        space.textureSpaceInBytes += texCoords.size() * sizeof(GLfloat);
        space.indexSpaceInBytes += indices.size() * sizeof(GLuint);
    }

    return space;
}
