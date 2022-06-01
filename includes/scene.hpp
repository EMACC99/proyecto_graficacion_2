#pragma once

#include <vector>
#include <string>
#include <array>
#ifdef _WIN32
    #include <windows.h>
#endif
#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#endif

#include <QTimer>
#include <glm/glm.hpp>

constexpr float rotating_factor = 0.5f;
class Scene{

public: 
    static void draw_room();
    static void draw_teapot(const GLdouble &size = 0.5f);
    static void draw_donut(const GLdouble &innerRadius = 0.2f,
                           const GLdouble &outerRadius = 0.4f,
                           const GLint &sides = 10,
                           const GLint &rings = 50);
};

class Model{
public:
    Model(const std::string &filename);
    ~Model();
    void Draw();

    float x();
    float y();
    float z();

private:
    std::vector<GLfloat> vertexData;
    GLuint VAO, VBO, EBO; //vertex array obj, vertex buffer obj, 
    GLuint texture;
    float rotation = 0;
    std::array<float, 3> position;
};


class Player{
public:
    Player(const std::string &sprites);
    ~Player(); 

    enum direction_sign{
        POSTIVE = 1,
        NEGATIVE = -1
    };

    void Draw();

    void move_x(const direction_sign &dir);
    void move_y(const direction_sign &dir);

    float x();
    float y();

    unsigned short int current_sprite;


private:
    GLuint texutre;
    std::array<float, 3> position;
    float move_rate = 0.1f;
};