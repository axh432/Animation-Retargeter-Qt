#include "graphicscardmemorymanager.h"

GraphicsCardMemoryManager::GraphicsCardMemoryManager(size_t vertexBufferSize, size_t textureBufferSize, size_t indexBufferSize): vertexPtr(0), indexPtr(0), texturePtr(0), indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    indexBuffer.create();
    vertexBuffer.create();
    textureBuffer.create();

    indexBuffer.bind();
    indexBuffer.allocate(indexBufferSize);

    vertexBuffer.bind();
    vertexBuffer.allocate(vertexBufferSize);

    textureBuffer.bind();
    textureBuffer.allocate(textureBufferSize);
}

GraphicsCardMemoryManager::GraphicsCardMemoryManager(GraphicsCardSpace& space): vertexPtr(0), indexPtr(0), texturePtr(0), indexBuffer(QOpenGLBuffer::IndexBuffer){
    indexBuffer.create();
    vertexBuffer.create();
    textureBuffer.create();

    indexBuffer.bind();
    indexBuffer.allocate(space.indexSpaceInBytes);

    vertexBuffer.bind();
    vertexBuffer.allocate(space.vertexSpaceInBytes);

    textureBuffer.bind();
    textureBuffer.allocate(space.textureSpaceInBytes);
}

GraphicsCardMemoryManager::~GraphicsCardMemoryManager(){
    indexBuffer.destroy();
    vertexBuffer.destroy();
    textureBuffer.destroy();
}

unique_ptr<GLData> GraphicsCardMemoryManager::storeMeshOnGraphicsCard(Mesh* mesh, Skeleton* skeleton){

    unique_ptr<GLData> glData(new GLData);
    std::vector<GLfloat> vertices = mesh->computeGLVertices(skeleton);

    glData->indicesPtr = storeIndicesOnGraphicsCard(mesh->getTris());
    glData->texturePtr = storeTextureCoordsOnGraphicsCard(mesh->getTextureCoords());
    glData->verticesPtr = storeVertsOnGraphicsCard(vertices);
    glData->numIndices = mesh->getTris().size();
    glData->textureBuffer = &textureBuffer;
    glData->vertexBuffer = &vertexBuffer;
    glData->indexBuffer = &indexBuffer;

    return std::move(glData);
}

quintptr GraphicsCardMemoryManager::storeTextureCoordsOnGraphicsCard(vector<GLfloat> texCoords){

    int texCoordsSizeInBytes = texCoords.size() * sizeof(GLfloat);
    quintptr oldPtr = texturePtr;

    textureBuffer.bind();
    textureBuffer.write(texturePtr, texCoords.data(), texCoordsSizeInBytes);

    texturePtr += texCoordsSizeInBytes;

    return oldPtr;

}

quintptr GraphicsCardMemoryManager::storeIndicesOnGraphicsCard(vector<GLuint> indices){

    int indicesSizeInBytes = indices.size() * sizeof(GLuint);
    quintptr oldPtr = indexPtr;

    indexBuffer.bind();
    indexBuffer.write(indexPtr, indices.data(), indicesSizeInBytes);

    indexPtr += indicesSizeInBytes;

    return oldPtr;
}

quintptr GraphicsCardMemoryManager::storeVertsOnGraphicsCard(vector<GLfloat> verts){

    int verticesSizeInBytes = verts.size() * sizeof(GLfloat);
    quintptr oldPtr = vertexPtr;

    vertexBuffer.bind();
    vertexBuffer.write(vertexPtr, verts.data(), verticesSizeInBytes);

    vertexPtr += verticesSizeInBytes;

    return oldPtr;

}

unique_ptr<GLModel> GraphicsCardMemoryManager::createGLModel( QString modelName, ResourceManager* resourceManager ){

    Model* model = resourceManager->getModel(modelName);

    vector<GLMesh> glmeshes;
    vector<Mesh>& meshes = model->meshes;
    Skeleton* bindPose = model->bindPose.get();

    for(int i = 0; i < meshes.size(); i++){
        Mesh* mesh = &meshes[i];

        unique_ptr<GLData> glData = storeMeshOnGraphicsCard(mesh, bindPose);

        QString materialName = mesh->getMaterialName();

        Material* material = resourceManager->getMaterial(materialName);

        glmeshes.emplace_back(mesh, std::move(glData), material);
    }

    unique_ptr<GLModel> glModel(new GLModel(std::move(glmeshes)));

    return std::move(glModel);
}
