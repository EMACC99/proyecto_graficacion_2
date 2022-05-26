#pragma once
#include <iostream>
#ifdef _WIN32
    #include <windows.h>
#endif
#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

inline void gl_clear_errors()
{
    while(glGetError()!= GL_NO_ERROR);
}

inline void gl_log_errors()
{
    while(auto err = glGetError())
        std::cout << "\tError code 0x" << std::hex << err << std::dec << std::endl;
}

#define GL_DEBUG(FUNC) gl_clear_errors();\
    FUNC;\
    std::cout << "Debugging " << #FUNC << " at line: " << __LINE__ << std::endl;\
    gl_log_errors()
