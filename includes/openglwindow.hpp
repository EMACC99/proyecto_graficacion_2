#pragma once

#include "includes/scene.hpp"
#include <QMainWindow>
#include <QString>
#include <QSurfaceFormat>
#include <QTimer>
#include <QImage>
#include <QOpenGLFunctions>
#include <QOpenGLWindow>
#include <QOpenGLWidget>
#include <QtOpenGL>

#include<array>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <string>
#include <unordered_map>

#ifdef _WIN32
    #include <windows.h>
#endif
#ifdef __APPLE__
    #include <OpenGL/glu.h>
    constexpr float MOUSE_NORMALIZATION = 8.f;
#else
    #include <GL/glu.h>
    constexpr float MOUSE_NORMALIZATION = 100.f;
#endif

class Viewport : public QOpenGLWidget{
    Q_OBJECT

public:
    Viewport(QWidget *parent = 0);
    ~Viewport();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void initTextures();
    void initModels();
    
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    QOpenGLContext *context;

    std::string frag_shader, geom_shader, vert_shader;


private:
    QTimer timer;
    QOpenGLFunctions *gl;

    std::vector<Model> modelos;
    unsigned short model_index = 0;

    GLdouble eyez = 5;
    std::unordered_map<std::string ,GLuint> textureID;
    
    std::vector<GLuint> kirbyID;
    

    bool LightOn;

    GLuint program;

    Player *player;
};

