#include "shaderprogram.h"

ShaderProgram::ShaderProgram(QString vertexShaderPath, QString fragmentShaderPath)
{
    mVertPath = vertexShaderPath;
    mFragPath = fragmentShaderPath;
}

void ShaderProgram::create(){
    QFile vertexFile(mVertPath), fragmentFile(mFragPath);
    vertexFile.open(QIODevice::ReadOnly);
    fragmentFile.open(QIODevice::ReadOnly);
    QString vertexShaderSource, fragmentShaderSource;
    vertexShaderSource = vertexFile.readAll();
    fragmentShaderSource = fragmentFile.readAll();
    vertexFile.close();
    fragmentFile.close();

    const GLchar* vert = vertexShaderSource.toStdString().c_str();
    const GLchar* frag = fragmentShaderSource.toStdString().c_str();

    mVert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(mVert, 1, &vert, NULL);
    glCompileShader(mVert);

    mFrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(mFrag, 1, &frag, NULL);
    glCompileShader(mFrag);

    mProgram = glCreateProgram();
    glAttachShader(mProgram, mFrag);
    glAttachShader(mProgram, mVert);
    glLinkProgram(mProgram);
}

void ShaderProgram::use(){
    glUseProgram(mProgram);
}

GLuint ShaderProgram::get(){
    return mProgram;
}

ShaderProgram::~ShaderProgram(){
    glDeleteProgram(mProgram);
}
