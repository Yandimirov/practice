#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <GL/glew.h>
#include <QGLWidget>
#include <QVector>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

class Triangle : public QGLWidget
{
    Q_OBJECT
public:
    Triangle(QWidget *parent = 0);
    virtual ~Triangle();
    glm::vec2 toNormalized( QPointF p);
    glm::vec2 transformCoords();
protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    GLuint mVertexBuffer[2], mShaderProgram, mVertexShader, mFragmentShader, mModelID, mProjID, mViewID;
    QVector < glm::vec3 > mColorData;
    QVector < glm::vec2 > mVertexData;
    GLfloat mR;
    QPointF mMouseCoord;
    glm::mat4 mProjection, mView, mModel;
    GLint mCurrIdx;
};

#endif // TRIANGLE_H
