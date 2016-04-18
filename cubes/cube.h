#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>
#include <QString>
#include <QGLWidget>
#include <QVector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <QImage>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include "QTime"
#include <QFile>
#include <iostream>
using namespace std;


class Cube : public QGLWidget
{
    Q_OBJECT
public:
    explicit Cube(QWidget *parent = 0);
    virtual ~Cube();
protected:
    void resizeGL(int w, int h);
    void paintGL();
    void initializeGL();
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QVector <glm::vec3> mTranslations;
    QVector <glm::vec4> mColorData;
    QVector <glm::vec3 > mVertexData;
    int mIndices[36];
    QVector <glm::vec2 > mTexData;
    QVector <glm::vec3 > mFloorData;
    QVector <glm::vec2> mFloorTexData;
    int mFloorIndices[6];
    glm::vec3 mEye, mUp, mFront;
    GLuint mFragmentShader[2], mVertexShader[2], mShaderProgram[2], mTexture, mVBO[2], mEBO[2], mFVBO[2], mVAO[2], mInstanceVBO;
    glm::mat4 mView, mModel, mProjection;
    QPointF mMouseCoord;
    GLfloat mCameraSpeed, mYaw, mPitch;
    bool mFlag;
    QTimer timer;
};

#endif // CUBE_H
