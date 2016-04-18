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
#include <QKeyEvent>
#include <QMouseEvent>

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
    QVector < glm::vec3 > mVertexData;
    QVector < glm::vec2 > mTexData;
    glm::vec3 mEye, mUp, mFront;
    GLuint mFragmentShader, mVertexShader, mShaderProgram, mBuffer[2], mModelID, mViewID, mProjID, mTexture, mSampler;
    glm::mat4 mView, mModel, mProjection;
    QPointF mMouseCoord;
    GLfloat mCameraSpeed, mYaw, mPitch;
    bool mFlag;
};

#endif // CUBE_H
