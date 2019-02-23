#include "glmesh.h"

GLMesh::GLMesh(Mesh* mesh, unique_ptr<GLData> glData, Material* material): mesh(mesh), glData(std::move(glData)), material(material){

    initializeOpenGLFunctions();

}

/*void GLMesh::ifBuffersNotCreatedCreateBuffers(){

    QOpenGLBuffer* indices = getIndexBuffer();
    QOpenGLBuffer* textureCoords = getTextureCoords();

    if(!indices->isCreated()){
        indices->create();

        vector<GLuint>& tris = mesh->getTris();

        indices->bind();
        indices->allocate(tris.data(), tris.size() * sizeof(GLuint));
        qDebug() << "indexBuffer is recreated";
    }

    if(!textureCoords->isCreated()){
        textureCoords->create();

        vector<float>& coords = mesh->getTextureCoords();

        textureCoords->bind();
        textureCoords->allocate(coords.data(), coords.size() * sizeof(float));
        qDebug() << "textureBuffer is recreated";
    }

    if(!vertices.isCreated()){
        vertices.create();
        qDebug() << "vertexBuffer is recreated";
    }

}*/


void GLMesh::update(Skeleton* skeleton){

    std::vector<GLfloat> verts = mesh->computeGLVertices(skeleton);

    QOpenGLBuffer* vertexBuffer = glData->vertexBuffer;
    quintptr offset = glData->verticesPtr;

    vertexBuffer->bind();
    vertexBuffer->write(offset, verts.data(), verts.size() * sizeof(GLfloat));

}

/*void GLMesh::checkGLValidity(){

    qDebug() << this->mesh->getMaterialName();

    //if we dont have a material dont draw
    if(!material){
        qDebug() << "no material";
    }else{

        QOpenGLTexture* diffuse = this->material->getDiffuse();
        QOpenGLShaderProgram* shader = this->material->getShader();

        shader->bind();

        qDebug() << "is shader linked?: " << shader->isLinked();

        diffuse->bind();

        qDebug() << "is diffuse texture created?: " << diffuse->isCreated();
        qDebug() << "is diffuse texture bound?: " << diffuse->isBound();
        qDebug() << "is diffuse texture storage allocated?: " << diffuse->isStorageAllocated();
    }

    QOpenGLBuffer* indexBuffer = this->getIndexBuffer();
    QOpenGLBuffer* texCoords = this->getTextureCoords();


    // Tell OpenGL which VBOs to use
    indexBuffer->bind();
    vertices.bind();
    texCoords->bind();

    qDebug() << "indexBuffer is created: " << indexBuffer->isCreated();
    qDebug() << "indexBuffer size: " << indexBuffer->size();

    qDebug() << "vertexBuffer is created: " << vertices.isCreated();
    qDebug() << "vertexBuffer size: " << vertices.size();

    qDebug() << "texCoords is created: " << texCoords->isCreated();
    qDebug() << "texCoords size: " << texCoords->size();


    qDebug() << "numIndices: " << numIndices;

    indexBuffer->release();
    vertices.release();
    texCoords->release();

    qDebug() << "";

}*/

void GLMesh::render(QMatrix4x4& mvp_matrix){

    //if we dont have a material dont draw
    if(!material){
        return;
    }

    QOpenGLTexture* diffuse = material->getDiffuse();
    QOpenGLShaderProgram* shader = material->getShader();
    QOpenGLBuffer* indexBuffer = glData->indexBuffer;
    QOpenGLBuffer* textureBuffer = glData->textureBuffer;
    QOpenGLBuffer* vertexBuffer = glData->vertexBuffer;

    shader->bind();

    //qDebug() << "is shader linked?: " << shader->isLinked();

    diffuse->bind();

    //qDebug() << "is diffuse texture created?: " << diffuse->isCreated();
    //qDebug() << "is diffuse texture bound?: " << diffuse->isBound();
    //qDebug() << "is diffuse texture storage allocated?: " << diffuse->isStorageAllocated();

    shader->setUniformValue("mvp_matrix", mvp_matrix);

    shader->setUniformValue("texture", 0);

    // Tell OpenGL which VBOs to use
    indexBuffer->bind();

    //qDebug() << "indexBuffer is created: " << indexBuffer->isCreated();
    //qDebug() << "indexBuffer size: " << indexBuffer->size();

    //qDebug() << "vertexBuffer is created: " << vertexBuffer->isCreated();
    //qDebug() << "vertexBuffer size: " << vertexBuffer->size();

    //qDebug() << "textureBuffer is created: " << textureBuffer->isCreated();
    //qDebug() << "textureBuffer size: " << textureBuffer->size();

    // Tell OpenGL programmable pipeline how to locate vertex position data
    vertexBuffer->bind();
    int vertexLocation = shader->attributeLocation("a_position");
    shader->enableAttributeArray(vertexLocation);
    shader->setAttributeBuffer(vertexLocation, GL_FLOAT, glData->verticesPtr, 3, 0);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    textureBuffer->bind();
    int texcoordLocation = shader->attributeLocation("a_texcoord");
    shader->enableAttributeArray(texcoordLocation);
    shader->setAttributeBuffer(texcoordLocation, GL_FLOAT, glData->texturePtr, 2, 0);

    //qDebug() << "numIndices: " << glData->numIndices;

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, glData->numIndices, GL_UNSIGNED_INT, (void*) glData->indicesPtr);

    indexBuffer->release();
    textureBuffer->release();
    vertexBuffer->release();
    shader->release();
    diffuse->release();

}


//GLModel
GLModel::GLModel(std::vector<GLMesh> meshes, Skeleton* bindPose):
    meshes(std::move(meshes)),
    bindPose(bindPose)
{/*empty*/}

/*void GLModel::checkGLValidity(){

    for(int i = 0; i < meshes.size(); i++){
        meshes[i].checkGLValidity();
    }

}*/

void GLModel::render(QMatrix4x4& mvp_matrix){

    for(int i = 0; i < meshes.size(); i++){
        meshes[i].render(mvp_matrix);
    }

}

void GLModel::update(Skeleton* skeleton){

    for(int i = 0; i < meshes.size(); i++){
        meshes[i].update(skeleton);
    }

}
