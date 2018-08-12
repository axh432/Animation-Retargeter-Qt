#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <memory>

class Material
{
public:
    Material(
            QString name,
            QString alias,
            QString type,
            std::unique_ptr<QOpenGLShaderProgram> shader,
            std::unique_ptr<QOpenGLTexture> diffuse,
            std::unique_ptr<QOpenGLTexture> local,
            std::unique_ptr<QOpenGLTexture> height,
            std::unique_ptr<QOpenGLTexture> specular
            );

    inline QString getName(){ return name; }
    inline QString getAlias(){ return alias; }
    inline QString getType(){ return type; }

    inline QOpenGLShaderProgram* getShader(){ return shader.get(); }
    inline QOpenGLTexture* getDiffuse(){ return diffuse.get(); }
    inline QOpenGLTexture* getLocal(){ return local.get(); }
    inline QOpenGLTexture* getHeight(){ return height.get(); }
    inline QOpenGLTexture* getSpecular(){ return specular.get(); }

private:
    QString name;
    QString alias;
    QString type;

    std::unique_ptr<QOpenGLShaderProgram> shader;
    std::unique_ptr<QOpenGLTexture> diffuse;
    std::unique_ptr<QOpenGLTexture> local;
    std::unique_ptr<QOpenGLTexture> height;
    std::unique_ptr<QOpenGLTexture> specular;
};

#endif // MATERIAL_H
