#include "cube.h"

Cube::Cube(QWidget *parent) : QGLWidget(parent),
    mYaw(-90.0f),
    mPitch(0.0f),
    mCameraSpeed(0.1f)
{
    setFocus();
    setMouseTracking(true);

    mVertexData.push_back(glm::vec3( -1.0f, 1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, 1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, -1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, -1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, -1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, 1.0f, 1.0f ));

    mVertexData.push_back(glm::vec3( -1.0f, 1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, 1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, 1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, 1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, 1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, 1.0f, -1.0f ));

    mVertexData.push_back(glm::vec3( -1.0f , 1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, 1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, -1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, -1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, -1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, 1.0f, -1.0f ));

    mVertexData.push_back(glm::vec3( 1.0f, 1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, 1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, -1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, -1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, -1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, 1.0f, -1.0f ));

    mVertexData.push_back(glm::vec3( 1.0f, 1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, 1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, -1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, -1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, -1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, 1.0f, 1.0f ));

    mVertexData.push_back(glm::vec3( -1.0f, -1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, -1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, -1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, -1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, -1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, -1.0f, 1.0f ));

    for(int i = 0; i < 6; i++){
        mTexData.push_back(glm::vec2 ( 0.0f, 1.0f ));
        mTexData.push_back(glm::vec2 ( 1.0f, 1.0f ));
        mTexData.push_back(glm::vec2 ( 1.0f, 0.0f ));
        mTexData.push_back(glm::vec2 ( 1.0f, 0.0f ));
        mTexData.push_back(glm::vec2 ( 0.0f, 0.0f ));
        mTexData.push_back(glm::vec2 ( 0.0f, 1.0f ));
    }
    mFlag = false;
    mEye = glm::vec3(0.0f, 0.0f, 5.0f);
    mUp = glm::vec3(0.0f, 1.0f, 0.0f);
    mFront = glm::vec3(0.0f, 0.0f, -1.0f);
}

void Cube::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

void Cube::initializeGL(){

    glewInit();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    const GLchar* vertexShaderSource = "#version 420 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec2 texCoord;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "gl_Position = projection * view * model * vec4(position, 1.0f);\n"
        "TexCoord = texCoord;\n"
        "}\0";

    const GLchar* fragmentShaderSource = "#version 420 core\n"
        "in vec2 TexCoord;\n"
        "out vec4 color;\n"
        "uniform sampler2D ourTexture;\n"
        "void main()\n"
        "{\n"
        "color = texture(ourTexture, TexCoord);\n"
        "}\0";

    mVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(mVertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(mVertexShader);

    mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(mFragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(mFragmentShader);

    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, mFragmentShader);
    glAttachShader(mShaderProgram, mVertexShader);
    glLinkProgram(mShaderProgram);

    glGenVertexArrays(2, mBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(glm::vec3), mVertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mProjection = glm::perspective(45.0f, (float)width()/height(), 0.1f, 100.0f);
    mView = glm::lookAt(
        mEye,
        glm::vec3(0.0f,0.0f,0.0f),
        mUp
    );
    mModel = glm::mat4(1.0f);
    QImage img(":/img/texture8.jpg");
    QGLWidget::convertToGLFormat(img);

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE,img.bits());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);


    glBindBuffer(GL_ARRAY_BUFFER, mBuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, mTexData.size() * sizeof(glm::vec2), mTexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mModelID = glGetUniformLocation(mShaderProgram, "model");
    mProjID = glGetUniformLocation(mShaderProgram, "projection");
    mViewID = glGetUniformLocation(mShaderProgram, "view");
}

void Cube::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(mShaderProgram);
    mView = glm::lookAt(
        mEye,
        mEye + mFront,
        mUp
    );
    glUniformMatrix4fv(mModelID, 1, GL_FALSE, glm::value_ptr(mModel));
    glUniformMatrix4fv(mProjID, 1, GL_FALSE, glm::value_ptr(mProjection));
    glUniformMatrix4fv(mViewID, 1, GL_FALSE, glm::value_ptr(mView));
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glBindBuffer(GL_ARRAY_BUFFER, mBuffer[0]);    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mBuffer[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT,GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Cube::keyPressEvent(QKeyEvent *event){

    switch(event->key()){
        case Qt::Key_Up:
            mEye += mFront * mCameraSpeed;
            break;
        case Qt::Key_Down:
            mEye -= mFront * mCameraSpeed;
            break;
        case Qt::Key_Left:
            mEye += glm::normalize(glm::cross(mUp, mFront)) * mCameraSpeed;
            break;
        case Qt::Key_Right:
            mEye -= glm::normalize(glm::cross(mUp, mFront)) * mCameraSpeed;
            break;
    }
    updateGL();
}

void Cube::mouseMoveEvent(QMouseEvent *event){
    if(!mFlag) {
        mFlag = true;
        mMouseCoord.setX(event->x());
        mMouseCoord.setY(event->y());
    }
    QPointF delta;
    delta.setX(event->x() - mMouseCoord.x());
    delta.setY(event->y() - mMouseCoord.y());
    mMouseCoord.setX(event->x());
    mMouseCoord.setY(event->y());
    mYaw += delta.x() * 0.1f;
    mPitch -= delta.y() * 0.1f;
    if (mPitch > 89.0f)
        mPitch = 89.0f;
    if (mPitch < -89.0f)
        mPitch = -89.0f;
    glm::vec3 front;
    front[0] = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front[1] = sin(glm::radians(mPitch));
    front[2] = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(front);
    if(mMouseCoord.x() >= width() || mMouseCoord.x() <= 0 || mMouseCoord.y() >= height() || mMouseCoord.y() <= 0){
        mFlag = false;
    }
    updateGL();
}

Cube::~Cube(){
    glDeleteTextures(1, &mTexture);
    glDeleteBuffers(2, mBuffer);
    glDeleteProgram(mShaderProgram);
}
