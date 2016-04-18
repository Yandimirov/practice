#include "cube.h"


Cube::Cube(QWidget *parent) : QGLWidget(parent),
    mTexture(0),
    mInstanceVBO(0),
    mCameraSpeed(0.3f),
    mYaw(-90.0f),
    mPitch(0.0f)
{
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    setFocus();
    setMouseTracking(true);

    connect(&timer, SIGNAL(timeout()), SLOT(updateGL()));
    timer.start(33);
    mVBO[0] = mVBO[1] = 0;
    mEBO[0] = mEBO[1] = 0;
    mFVBO[0] = mFVBO[1] = 0;
    mVAO[0] = mVAO[1] = 0;
    mVertexShader[0] = mVertexShader[1] = 0;
    mShaderProgram[0] = mShaderProgram[1] = 0;
    mFragmentShader[0] = mFragmentShader[1] = 0;

    mVertexData.push_back(glm::vec3( -1.0f, 1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, 1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, -1.0f, 1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, -1.0f, 1.0f ));

    mVertexData.push_back(glm::vec3( -1.0f, 1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, 1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( 1.0f, -1.0f, -1.0f ));
    mVertexData.push_back(glm::vec3( -1.0f, -1.0f, -1.0f ));

    mFloorData.push_back(glm::vec3(-50.0f, -1.0f, 50.0f));
    mFloorData.push_back(glm::vec3(50.0f, -1.0f, 50.0f));
    mFloorData.push_back(glm::vec3(50.0f, -1.0f, -50.0f));
    mFloorData.push_back(glm::vec3(-50.0f, -1.0f, -50.0f));

    mIndices = {
        0,1,2,2,3,0,

        5,4,7,7,6,5,

        4,0,3,3,7,4,

        1,5,6,6,2,1,

        4,5,1,1,0,4,

        3,2,6,6,7,3
    };


    int offset = 5;

    QVector <glm::vec3> allTranslations;
    for(int i = -48; i < 50; i += offset){
        for(int j = -48; j < 50; j += offset){
            allTranslations.push_back(glm::vec3(i,0,j));
        }
    }

    unsigned int n = qrand() % allTranslations.size();
    for(unsigned int i = 0; i < n; i++){
        int tmp = qrand() % allTranslations.size();
        mTranslations.push_back(allTranslations[tmp]);
        allTranslations.remove(tmp);
    }

    for(int i = 0; i < mTranslations.size(); i++){
        mColorData.push_back(glm::vec4((qrand() % 256) / 255.0f, (qrand() % 256) / 255.0f, (qrand() % 256) / 255.0f, 0.5f));
    }

    mFloorTexData.push_back(glm::vec2(0.0f, 1.0f));
    mFloorTexData.push_back(glm::vec2(1.0f, 1.0f));
    mFloorTexData.push_back(glm::vec2(1.0f, 0.0f));
    mFloorTexData.push_back(glm::vec2(0.0f, 0.0f));

    mFloorIndices = {
        0,1,2,2,3,0
    };

    mFlag = false;
    mEye = glm::vec3(0.0f, 10.0f, 0.0f);
    mUp = glm::vec3(0.0f, 1.0f, 0.0f);
    mFront = glm::vec3(0.0f, 0.0f, -1.0f);
}

void Cube::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

void Cube::initializeGL(){

    glewInit();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const GLchar* floorVertexShaderSource = "#version 420 core\n"
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

    const GLchar* floorFragmentShaderSource = "#version 420 core\n"
        "in vec2 TexCoord;\n"
        "out vec4 color;\n"
        "uniform sampler2D ourTexture;\n"
        "void main()\n"
        "{\n"
        "color = texture(ourTexture, TexCoord);\n"
        "}\0";

    const GLchar* cubeVertexShaderSource = "#version 420 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 offset;\n"
        "layout (location = 2) in vec4 Color;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "out vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        "gl_Position = projection * view * model * vec4(position + offset, 1.0f);\n"
        "ourColor = Color;\n"
        "}\0";

    const GLchar* cubeFragmentShaderSource = "#version 420 core\n"
        "in vec4 ourColor;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = ourColor;\n"
        "}\0";

    mVertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(mVertexShader[0], 1, &floorVertexShaderSource, NULL);
    glCompileShader(mVertexShader[0]);

    mFragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(mFragmentShader[0], 1, &floorFragmentShaderSource, NULL);
    glCompileShader(mFragmentShader[0]);

    mShaderProgram[0] = glCreateProgram();
    glAttachShader(mShaderProgram[0], mFragmentShader[0]);
    glAttachShader(mShaderProgram[0], mVertexShader[0]);
    glLinkProgram(mShaderProgram[0]);

    mVertexShader[1] = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(mVertexShader[1], 1, &cubeVertexShaderSource, NULL);
    glCompileShader(mVertexShader[1]);

    mFragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(mFragmentShader[1], 1, &cubeFragmentShaderSource, NULL);
    glCompileShader(mFragmentShader[1]);

    mShaderProgram[1] = glCreateProgram();
    glAttachShader(mShaderProgram[1], mFragmentShader[1]);
    glAttachShader(mShaderProgram[1], mVertexShader[1]);
    glLinkProgram(mShaderProgram[1]);

    glGenVertexArrays(2, mVAO);
    glGenBuffers(2, mVBO);
    glGenBuffers(2, mFVBO);
    glGenBuffers(2, mEBO);
    glGenBuffers(1, &mInstanceVBO);


    QImage img(":/img/wall.jpg");
    glGenTextures(1, &mTexture);

    mProjection = glm::perspective(45.0f, (float)width()/height(), 0.1f, 100.0f);
    mView = glm::lookAt(
        mEye,
        glm::vec3(0.0f,0.0f,0.0f),
        mUp
    );
    mModel = glm::mat4(1.0f);

    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, img.bits());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(mVAO[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mFloorIndices), mFloorIndices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mFVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, mFloorData.size() * sizeof(glm::vec3), mFloorData.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mFVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, mFloorTexData.size() * sizeof(glm::vec2), mFloorTexData.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glBindVertexArray(mVAO[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndices), mIndices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(glm::vec3), mVertexData.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mTranslations.size(), mTranslations.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, mColorData.size() * sizeof(glm::vec3), mColorData.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);
    glBindVertexArray(0);

}

void Cube::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(mShaderProgram[0]);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glUniformMatrix4fv(glGetUniformLocation(mShaderProgram[0], "model"), 1, GL_FALSE, glm::value_ptr(mModel));
    glUniformMatrix4fv(glGetUniformLocation(mShaderProgram[0], "projection"), 1, GL_FALSE, glm::value_ptr(mProjection));
    glUniformMatrix4fv(glGetUniformLocation(mShaderProgram[0], "view"), 1, GL_FALSE, glm::value_ptr(mView));
    glBindVertexArray(mVAO[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(mShaderProgram[1]);
    glUniformMatrix4fv(glGetUniformLocation(mShaderProgram[1], "model"), 1, GL_FALSE, glm::value_ptr(mModel));
    glUniformMatrix4fv(glGetUniformLocation(mShaderProgram[1], "projection"), 1, GL_FALSE, glm::value_ptr(mProjection));
    glUniformMatrix4fv(glGetUniformLocation(mShaderProgram[1], "view"), 1, GL_FALSE, glm::value_ptr(mView));
    glBindVertexArray(mVAO[1]);
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, mTranslations.size());
    glBindVertexArray(0);

    mView = glm::lookAt(
        mEye,
        mEye + mFront,
        mUp
    );
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
    mYaw += delta.x() * mCameraSpeed;
    mPitch -= delta.y() * mCameraSpeed;
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
}

Cube::~Cube(){
    glDeleteTextures(1, &mTexture);
    glDeleteBuffers(2, mVBO);
    glDeleteBuffers(2, mEBO);
    glDeleteBuffers(2, mFVBO);
    glDeleteBuffers(1, &mInstanceVBO);
    glDeleteProgram(mShaderProgram[0]);
    glDeleteProgram(mShaderProgram[1]);
    glDeleteVertexArrays(2, mVAO);
    glDeleteShader(mFragmentShader[0]);
    glDeleteShader(mFragmentShader[1]);
    glDeleteShader(mVertexShader[0]);
    glDeleteShader(mVertexShader[1]);
}
