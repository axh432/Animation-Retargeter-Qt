#include "material.h"

Material::Material(
            QString name,
            QString alias,
            QString type,
            std::unique_ptr<QOpenGLShaderProgram> shader,
            std::unique_ptr<QOpenGLTexture> diffuse,
            std::unique_ptr<QOpenGLTexture>local,
            std::unique_ptr<QOpenGLTexture>height,
            std::unique_ptr<QOpenGLTexture>specular
        ): name(name),
            alias(alias),
            type(type),
            shader(std::move(shader)),
            diffuse(std::move(diffuse)),
            local(std::move(local)),
            height(std::move(height)),
            specular(std::move(specular))
{

}
