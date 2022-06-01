#pragma once
#include <QtOpenGL>
#include <QImage>


#include <string>
#include <filesystem>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

constexpr GLfloat violet [] = { 0.5f, 0, 1.f};
constexpr GLfloat white[] = {1.f, 1.f, 1.f};
constexpr GLfloat grass_color[] = {.77f, 1.f, .42f};

#ifdef _WIN32
    #include <windows.h>
#endif
#ifdef __APPLE__
    #include <OpenGL/glu.h>
#else
    #include <GL/glu.h>
#endif
#include <glm/glm.hpp>

const std::string SHADER_PATH = "shaders/";

class Texture{

public:
    static std::tuple<unsigned char *, int, int> LoadTextureFile(const std::string &filename);
    static void FreeTextureData(unsigned char *data);
};

bool CompileShader(std::string nvert,std::string ngeom, std::string nfrag,GLuint *prog);