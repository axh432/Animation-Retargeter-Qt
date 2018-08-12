#include "engine.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <math.h>
#include <memory>
#include <iostream>

#include "lib/TextProcessor/databuffer.h"
#include "lib/TextProcessor/textprocessor.h"
#include "lib/md5/md5factory.h"
#include "lib/md5/md5.h"
#include "engine/material.h"

using std::unique_ptr;

Engine::Engine() :
    geometries(0),
    texture(0),
    angularSpeed(30.0f),
    camera(new Camera(QVector3D(0,0,0)))
{
    rotationAxis.setX(1);
    rotationAxis.setY(0);
    rotationAxis.setZ(0);
    initializeGL();
}

void Engine::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    initShaders();
    initMaterials();
    initTextures();

    
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    geometries = new GeometryEngine;

    createEntity();
}

void Engine::createEntity(){
    entity.reset(new Entity(loadModel(), loadAnim()));
    geometries->initMd5Geometry(entity->getModel());
}

unique_ptr<Model> Engine::loadModel(){
    QFile infile(":/imp.md5mesh");
    QFile schemaFile(":/md5mesh.schema");

    if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Unable to open file: " << infile.fileName();
    }
    qDebug() << "Opened file: " << infile.fileName();

    if(!schemaFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Unable to open file: " << schemaFile.fileName();
    }
    qDebug() << "Opened file: " << schemaFile.fileName();

    unique_ptr<QTextStream> in(new QTextStream(&infile));
    unique_ptr<QTextStream> schema(new QTextStream(&schemaFile));
    DataBuffer buffer;

    unique_ptr<TextProcessor> processor(new TextProcessor(std::move(in), std::move(schema), &buffer));
    qDebug("Created Processor");

    processor->parse();
    qDebug("finished parsing");

    qDebug() << "Parse Result: \n\tInt Buffer size - " << buffer.getIntBufferSize()
             << " \n\tFloat Buffer size - " << buffer.getFloatBufferSize()
             << " \n\tString Buffer size - " << buffer.getStringBufferSize();

    unique_ptr<Md5Factory> factory(new Md5Factory());

    return factory->buildModel(&buffer);
}

unique_ptr<Anim> Engine::loadAnim(){

    QFile infile(":/evade_left.md5anim");
    QFile schemaFile(":/md5anim.schema");

    if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Unable to open file: " << infile.fileName();
    }
    qDebug() << "Opened file: " << infile.fileName();

    if(!schemaFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Unable to open file: " << schemaFile.fileName();
    }
    qDebug() << "Opened file: " << schemaFile.fileName();

    unique_ptr<QTextStream> in(new QTextStream(&infile));
    unique_ptr<QTextStream> schema(new QTextStream(&schemaFile));
    DataBuffer buffer;

    unique_ptr<TextProcessor> processor(new TextProcessor(std::move(in), std::move(schema), &buffer));
    qDebug("Created Processor");

    processor->parse();
    qDebug("finished parsing");

    qDebug() << "Parse Result: \n\tInt Buffer size - " << buffer.getIntBufferSize()
             << " \n\tFloat Buffer size - " << buffer.getFloatBufferSize()
             << " \n\tString Buffer size - " << buffer.getStringBufferSize();

    unique_ptr<Md5Factory> factory(new Md5Factory());

    return factory->buildAnim(&buffer);
}

void Engine::updateEntity(double delta){

    entity->update(delta);

    vector<float> verts = entity->computeOpenGLVerts();

    geometries->updateMd5Geometry(verts);

}

void Engine::testSimpleModel(){
    QFile infile(":/hellknight.md5mesh");
    QFile schemaFile(":/md5mesh.schema");

    if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Unable to open file: " << infile.fileName();
    }
    qDebug() << "Opened file: " << infile.fileName();

    if(!schemaFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Unable to open file: " << schemaFile.fileName();
    }
    qDebug() << "Opened file: " << schemaFile.fileName();

    unique_ptr<QTextStream> in(new QTextStream(&infile));
    unique_ptr<QTextStream> schema(new QTextStream(&schemaFile));
    DataBuffer buffer;

    unique_ptr<TextProcessor> processor(new TextProcessor(std::move(in), std::move(schema), &buffer));
    qDebug("Created Processor");

    processor->parse();
    qDebug("finished parsing");

    qDebug() << "Parse Result: \n\tInt Buffer size - " << buffer.getIntBufferSize()
             << " \n\tFloat Buffer size - " << buffer.getFloatBufferSize()
             << " \n\tString Buffer size - " << buffer.getStringBufferSize();

    qDebug() << "MD5Version " << buffer.popInt();
    qDebug() << "numJoints " << buffer.popInt();
    qDebug() << "numMeshes " << buffer.popInt();
    qDebug() << "";
    qDebug() << "joints {";
    qDebug() << "origin " << buffer.popInt() << "( " << buffer.popFloat() << " " << buffer.popFloat() << " " << buffer.popFloat()
             << " ) ( " << buffer.popFloat() << " " << buffer.popFloat() << " " << buffer.popFloat() << ")";
}

void Engine::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"));
        //throw an exception

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"));
        //throw an exception

    // Link shader pipeline
    if (!program.link());
        //throw an exception

    // Bind shader pipeline for use
    if (!program.bind());
        //throw an exception
}

void Engine::initMaterials(){

    QFile infile(":/imp.mtr");
    QFile schemaFile(":/mtr.schema");

    if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Unable to open file: " << infile.fileName();
    }
    qDebug() << "Opened file: " << infile.fileName();

    if(!schemaFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Unable to open file: " << schemaFile.fileName();
    }
    qDebug() << "Opened file: " << schemaFile.fileName();

    unique_ptr<QTextStream> in(new QTextStream(&infile));
    unique_ptr<QTextStream> schema(new QTextStream(&schemaFile));
    DataBuffer buffer;

    unique_ptr<TextProcessor> processor(new TextProcessor(std::move(in), std::move(schema), &buffer));
    qDebug("Created Processor");

    processor->parse();
    qDebug("finished parsing");

    createMaterial(buffer);

    /*for(int i = 0; i < buffer.getStringBufferSize(); i++){
        qDebug() << buffer.popString();
    }*/

}

std::unique_ptr<QOpenGLShaderProgram> Engine::createShaderProgram(QString vertexShaderPath, QString fragmentShaderPath){

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

std::unique_ptr<QOpenGLTexture> Engine::loadTexture(QString texturePath){

    std::unique_ptr<QOpenGLTexture> texture(new QOpenGLTexture(QImage(texturePath)));

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    return texture;
}

void Engine::createMaterial(DataBuffer& buffer){

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

void Engine::initTextures()
{
    // Load cube.png image
    texture = new QOpenGLTexture(QImage(":/cube.png").mirrored());
    imp = new QOpenGLTexture(QImage(":/imp_d.tga"));

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    imp->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    imp->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Engine::handleKeyEvent(QKeyEvent *e){

    switch(e->key()){
        case Qt::Key_W:
            camera->moveForward();
            //qDebug() << "W key pressed";
            break;

        case Qt::Key_S:
            camera->moveBackward();
            //qDebug() << "S key pressed";
            break;

        case Qt::Key_A:
            camera->moveLeft();
            //qDebug() << "A key pressed";
            break;

        case Qt::Key_D:
            camera->moveRight();
            //qDebug() << "D key pressed";
            break;

        case Qt::Key_R:
            camera->moveUp();
            //qDebug() << "R key pressed";
            break;

        case Qt::Key_F:
            camera->moveDown();
            //qDebug() << "F key pressed";
            break;

        case Qt::Key_Up:
            camera->lookUp();
            //qDebug() << "up key pressed";
            break;

        case Qt::Key_Down:
            camera->lookDown();
            //qDebug() << "down key pressed";
            break;

        case Qt::Key_Left:
            camera->lookLeft();
            //qDebug() << "left key pressed";
            break;

        case Qt::Key_Right:
            camera->lookRight();
            //qDebug() << "right key pressed";
            break;
    }


}

Engine::~Engine()
{
    delete texture;
    delete imp;
    delete geometries;
    delete camera;
    std::cout << "Deleting engine" << std::endl;
}

void Engine::updateWorld(qint64 delta){

    qDebug() << "qint64 delta: " << delta;

    double deltaAsDouble = (double) delta;

    updateEntity(deltaAsDouble);
}


void Engine::resize(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.1, zFar = 1000.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void Engine::render()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind();

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0f, -35.0f, -150.0f);
    matrix.rotate(-90.0f, QVector3D(1.0, 0.0, 0.0));

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * camera->getViewMatrix() * matrix);

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    geometries->drawCubeGeometry(&program);



    Mesh& mesh = entity->getModel()->meshes[0];

    QString materialName = mesh.getMaterialName();

    Material* material = materials.find(materialName).value();

    material->getDiffuse()->bind();

    QOpenGLShaderProgram* impShader = material->getShader();

    // Set modelview-projection matrix
    impShader->setUniformValue("mvp_matrix", projection * camera->getViewMatrix() * matrix);

    impShader->setUniformValue("texture", 0);

    //imp->bind();

    geometries->drawMd5Geometry(impShader);
}
