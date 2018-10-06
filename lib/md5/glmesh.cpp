#include "glmesh.h"

GLMesh::GLMesh(Mesh* mesh, Material* material): mesh(mesh), material(material), numIndices(mesh->getTris().size()){
    vertices.create();
}

void GLMesh::ifVertexBufferNotCreatedCreateVertexBuffer(){

    if(!vertices.isCreated()){
        vertices.create();
        qDebug() << "vertexBuffer is recreated";
    }

}

GLMesh::~GLMesh(){
    vertices.destroy();
}

void GLMesh::update(Skeleton* skeleton){

    std::vector<float> verts = mesh->computeGLVertices(skeleton);

    ifVertexBufferNotCreatedCreateVertexBuffer();

    vertices.bind();
    vertices.allocate(verts.data(), verts.size() * sizeof(float));

    /*qDebug() << "Update - vertexBuffer is created: " << vertices.isCreated();
    qDebug() << "Update - vertexBuffer size: " << vertices.size();*/
}

void GLMesh::render(QMatrix4x4& mvp_matrix){

    //if we dont have a material dont draw
    if(!material){
        return;
    }

    QOpenGLTexture* diffuse = this->material->getDiffuse();
    QOpenGLShaderProgram* shader = this->material->getShader();
    QOpenGLBuffer* indexBuffer = this->getIndexBuffer();
    QOpenGLBuffer* texCoords = this->getTextureCoords();

    shader->bind();

    qDebug() << "is shader linked?: " << shader->isLinked();

    diffuse->bind();

    qDebug() << "is diffuse texture created?: " << diffuse->isCreated();
    qDebug() << "is diffuse texture bound?: " << diffuse->isBound();
    qDebug() << "is diffuse texture storage allocated?: " << diffuse->isStorageAllocated();

    shader->setUniformValue("mvp_matrix", mvp_matrix);

    shader->setUniformValue("texture", 0);

    // Tell OpenGL which VBOs to use

    indexBuffer->bind();

    qDebug() << "indexBuffer is created: " << indexBuffer->isCreated();
    qDebug() << "indexBuffer size: " << indexBuffer->size();

    qDebug() << "vertexBuffer is created: " << vertices.isCreated();
    qDebug() << "vertexBuffer size: " << vertices.size();

    qDebug() << "texCoords is created: " << texCoords->isCreated();
    qDebug() << "texCoords size: " << texCoords->size();

    // Tell OpenGL programmable pipeline how to locate vertex position data
    vertices.bind();
    int vertexLocation = shader->attributeLocation("a_position");
    shader->enableAttributeArray(vertexLocation);
    shader->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, 0);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    texCoords->bind();
    int texcoordLocation = shader->attributeLocation("a_texcoord");
    shader->enableAttributeArray(texcoordLocation);
    shader->setAttributeBuffer(texcoordLocation, GL_FLOAT, 0, 2, 0);

    qDebug() << "numIndices: " << numIndices;

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

    //indexBuffer->release();
    //vertices.release();
    //texCoords->release();

}


//GLModel
GLModel::GLModel(std::vector<GLMesh> meshes, Model* model): meshes(std::move(meshes)), model(model){/*empty*/}

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
