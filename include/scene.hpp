#pragma once

#include <vector>
#include <string>
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
private:
    std::vector<GLfloat> vertexData;
    GLuint VAO, VBO, EBO; //vertex array obj, vertex buffer obj, 
    GLuint texture;
    float rotation = 0 ;
};
