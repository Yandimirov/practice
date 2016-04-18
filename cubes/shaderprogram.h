#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <QGLWidget>
#include <QString>
#include <QFile>
#include <QDebug>

class ShaderProgram
{
public:
    ShaderProgram(QString vertexShaderPath, QString fragmentShaderPath);
    void use();
    void create();
    virtual ~ShaderProgram();
    GLuint get();
private:
    GLuint mProgram, mFrag, mVert;
    QString mVertPath, mFragPath;
};

#endif // SHADERPROGRAM_H
