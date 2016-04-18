#include "triangle.h"

Triangle::Triangle(QWidget *parent) :
    QGLWidget(parent),
    mShaderProgram(0),
    mVertexShader(0),
    mFragmentShader(0),
    mModelID(0),
    mProjID(0),
    mViewID(0),
    mR(0.05),
    mMouseCoord(0,0),
    mCurrIdx(-1)
{
    mVertexBuffer[0] = mVertexBuffer[1] = 0;
    mVertexData.push_back( glm::vec2(-0.5f, -0.5f ) );
    mVertexData.push_back( glm::vec2( 0.0f,  0.5f ) );
    mVertexData.push_back( glm::vec2( 0.5f, -0.5f ) );

    mColorData.push_back( glm::vec3( 1.0f, 0.0f, 0.0f ) );
    mColorData.push_back( glm::vec3( 0.0f, 1.0f, 0.0f ) );
    mColorData.push_back( glm::vec3( 0.0f, 0.0f, 1.0f ) );
}

void Triangle::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(mShaderProgram);
    glUniformMatrix4fv(mModelID, 1, GL_FALSE, glm::value_ptr(mModel));
    glUniformMatrix4fv(mProjID, 1, GL_FALSE, glm::value_ptr(mProjection));
    glUniformMatrix4fv(mViewID, 1, GL_FALSE, glm::value_ptr(mView));
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

Triangle::~Triangle()
{
    glDeleteBuffers(2, mVertexBuffer);
    glDeleteProgram(mShaderProgram);
    glDeleteShader(mFragmentShader);
    glDeleteShader(mVertexShader);
}

void Triangle::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Triangle::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && mCurrIdx != -1){
        glm::vec2 coords = transformCoords();
        qDebug() << coords[0] << " " << coords[1];
        mVertexData[mCurrIdx][0] = coords[0];
        mVertexData[mCurrIdx][1] = coords[1];
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[0]);
        glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(glm::vec2), mVertexData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    if ( event->buttons() == Qt::RightButton ){
        float deltaX = 2.0f * (float)((event->x() - mMouseCoord.x()) / width());
        float deltaY = -2.0f * (float)((event->y() - mMouseCoord.y()) / height());
        mView = glm::translate(mView, glm::vec3(-deltaX, -deltaY, 0.0f));
    }
    mMouseCoord = event->pos();
    updateGL();
}

glm::vec2 Triangle::transformCoords(){
    glm::vec2 tmpCoords = toNormalized(mMouseCoord);
    glm::vec4 transformedCoord = glm::inverse(mModel) * (glm::vec4(tmpCoords[0], tmpCoords[1], 0.0f, 1.0f) - mView * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    return glm::vec2(transformedCoord[0], transformedCoord[1]);
}

glm::vec2 Triangle::toNormalized( QPointF p )
{
    return glm::vec2( 2.0f * p.x() / width() - 1.0f, 1.0f - (2.0f * p.y()/ height()));
}

void Triangle::mousePressEvent(QMouseEvent *event)
{
    mMouseCoord = event->pos();
    if(event->button() == Qt::LeftButton){
        glm::vec2 coords = transformCoords();
        for(int i = 0; i < mVertexData.size(); i++){
            if( pow(mVertexData[i][0] - coords[0], 2) + pow(mVertexData[i][1] - coords[1], 2) <= pow(mR,2)){
                mCurrIdx = i;
                break;
            }
        }
    }
}

void Triangle::mouseReleaseEvent(QMouseEvent *event)
{
    mCurrIdx = -1;
}

void Triangle::wheelEvent(QWheelEvent *event)
{
    if ( event->delta() > 0 ) {
        mModel = glm::scale(mModel, glm::vec3(1.1f));
        glm::vec2 coords = transformCoords();
        mR = sqrt(pow(coords[0],2) + pow(coords[1], 2)) * 0.1;

    }
    else {
        mModel = glm::scale(mModel, glm::vec3(0.9f));
        glm::vec2 coords = transformCoords();
        mR = sqrt(pow(coords[0],2) + pow(coords[1], 2)) * 0.1;
    }
    updateGL();
}

void Triangle::initializeGL()
{
    glewInit();
    const GLchar* vertexShaderSource = "#version 420 core\n"
        "layout (location = 0) in vec2 position;\n"
        "layout (location = 1) in vec3 color;\n"
        "out vec3 ourColor;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "gl_Position = projection * view * model * vec4(position, 0.0f, 1.0f);\n"
        "ourColor = color;\n"
        "}\0";

    const GLchar* fragmentShaderSource = "#version 420 core\n"
        "in vec3 ourColor;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(ourColor, 1.0f);\n"
        "}\n\0";

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

    glGenBuffers(2, mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(glm::vec2), mVertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    mView = glm::mat4(1.0f);
    mModel = glm::mat4(1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, mColorData.size() * sizeof(glm::vec3), mColorData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mModelID = glGetUniformLocation(mShaderProgram, "model");
    mProjID = glGetUniformLocation(mShaderProgram, "projection");
    mViewID = glGetUniformLocation(mShaderProgram, "view");
}
