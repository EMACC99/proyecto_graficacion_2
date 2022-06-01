#include "includes/utils.hpp"
#include "includes/gl_debug.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

std::tuple<unsigned char *, int, int> Texture::LoadTextureFile(const std::string &filename){
    fs::path path = "assets/" + filename;
    path = fs::absolute(path);
    // stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data == NULL){
        std::cerr << "Failed to load texture" << std::endl;
        stbi_image_free(data);
        exit(EXIT_FAILURE);
    }
    
    return std::tuple<unsigned char *, int, int> {data, width, height};
}

void Texture::FreeTextureData(unsigned char *data){
    stbi_image_free(data);
}


bool CompileShader(std::string nvert,std::string ngeom, std::string nfrag,GLuint *prog){

    if (nvert != "")
        nvert  = SHADER_PATH + nvert;
    if(ngeom != "")
        ngeom  = SHADER_PATH + ngeom;
    if(nfrag != "")
        nfrag  = SHADER_PATH + nfrag;

    int compiledv = 0,
        compiledg = 0,
        compiledf = 0;
    
    bool f_vert = false,
         f_geom = false,
         f_frag = false;
    
    GLint length;

    GLchar *progSrc = 0;

    GLuint vertexShader = 0,
           geomShader = 0,
           fragmentShader = 0;

    std::ifstream inputFile;

    std::cout << "Shader Version:"<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    if(nvert.size() > 0){
        f_vert = true;
     // nvert="../../../"+nvert;
    }
    if(ngeom.size() > 0){
        if(GL_EXT_geometry_shader4){
            f_geom = true;
       // ngeom ="../../../"+ngeom;
        }
        else
            std::cout << "Geometry Shaders not Supported" << std::endl;
    }

    if(nfrag.size() > 0){
        f_frag = true;
        //nfrag="../../../"+nfrag;
    }

    /*
    *
    * Reading and compiling the vertex shader
    *
    */
    if(f_vert == true){
        inputFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
        try{
            std::cout << nvert.c_str() << std::endl;
            inputFile.open(nvert.c_str(), std::ios::in | std::ios::binary);
           }catch(std::ifstream::failure e){
            std::cout << "An exception occurred while reading Vertex Shader. Exception Nr. " << e.what() << std::endl;
           }
        if(inputFile.fail() == true){
           std::cout<<"I/O Error in the Vertex Program"<<std::endl;
          }
        else{
           inputFile.seekg (0, std::ios_base::end);
           length = inputFile.tellg();
           if(length > 0){
                progSrc = new GLchar[length]();
                progSrc = new char[length]();
                inputFile.seekg (0, std::ios_base::beg);
                inputFile.read(progSrc, length);
                inputFile.close();
                vertexShader = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertexShader, 1,(const GLchar **) &progSrc,&length);
                //std::cout<<"Vertex Program:"<<std::endl<<progSrc<<std::endl;
                delete[] progSrc;

                glCompileShader(vertexShader);
                // Checking if the compilation was a success
                glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&compiledv);
                if(compiledv == 0){
                    int infologLength = 0;

                    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infologLength);

                    if(infologLength > 0){
                        int charsWritten  = 0;
                        char *infoLog = new char[infologLength]();
                        glGetShaderInfoLog(vertexShader, infologLength, &charsWritten, infoLog);
                        std::cout<<"Vertex Shader Log Info: "<<infoLog<<std::endl;
                        delete[]infoLog;
                    }
                }
            }
        }
    }
 /*
  *
  * Reading and compiling the geometry shader
  *
  */
    if(f_geom == true){
        inputFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
        try{
           inputFile.open(ngeom.c_str(), std::ios::in | std::ios::binary);
        }
        catch(std::ifstream::failure e){
            std::cout << "An exception occurred while reading Geometry Shader. Exception Nr. " << e.what() << std::endl;
        }
        if(inputFile.fail() == true)
            std::cout<<"I/O Error in the Geometry Program"<<std::endl;

        else{
            inputFile.seekg (0, std::ios_base::end);
            length = inputFile.tellg();
            if(length > 0){
                progSrc = new GLchar[length]();
                const GLchar* pSrc=progSrc;
                inputFile.seekg (0, std::ios_base::beg);
                inputFile.read(progSrc, length);
                inputFile.close();
                geomShader = glCreateShader(GL_GEOMETRY_SHADER_EXT);
                glShaderSource(geomShader, 1, &pSrc,NULL);
                delete[] progSrc;

                glProgramParameteriEXT(*prog, GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES);
                glProgramParameteriEXT(*prog, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLES);
                glProgramParameteriEXT(*prog, GL_GEOMETRY_VERTICES_OUT_EXT, 3);

                glCompileShader(geomShader);
                // Checking if the compilation was a success
                glGetShaderiv(geomShader,GL_COMPILE_STATUS,&compiledv);
                if(compiledv == 0){
                    int infologLength = 0;

                glGetShaderiv(geomShader, GL_INFO_LOG_LENGTH, &infologLength);
                if(infologLength > 0){
                    int charsWritten  = 0;
                    char *infoLog = new char[infologLength]();
                    glGetShaderInfoLog(geomShader, infologLength, &charsWritten, infoLog);
                    std::cout<<"Log Info Geometry Shader: "<<infoLog<<std::endl;
                    delete[]infoLog;
                  }
               }
            }
         }
      }
     /*
      *
      * Reading and compiling the fragment shader
      *
      */
    if(f_frag == true){
        std::cout<<nfrag.c_str()<<std::endl;
        try{
            inputFile.open(nfrag.c_str(), std::ios::in | std::ios::binary);
        }
        catch(std::ifstream::failure e){
            std::cout << "An exception occurred while reading Fragment Shader. Exception Nr. " << e.what() << std::endl;
        }
        if(inputFile.fail() == true)
           std::cout<<"I/O Error in the Fragment Program"<<std::endl;

        else{
            inputFile.seekg (0, std::ios_base::end);
            length = inputFile.tellg();
            if(length > 0){
                progSrc = new char[length]();
                const GLchar* pSrc=progSrc;
                inputFile.seekg (0, std::ios_base::beg);
                inputFile.read((char*)progSrc, length*sizeof(char)-1);
                inputFile.close();

                fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
                //std::cout<<"Fragment Program:"<<std::endl<<progSrc<<std::endl;
                glShaderSource(fragmentShader, 1, &pSrc,NULL);
                delete[] progSrc;

                glCompileShader(fragmentShader);
              // Checking if the compilation was a success
                glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&compiledf);
                if(compiledf == 0){
                    int infologLength = 0;

                    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infologLength);
                    if(infologLength > 0){
                        int charsWritten  = 0;
                        GLchar *infoLog = new GLchar[infologLength]();
                        glGetShaderInfoLog(fragmentShader, infologLength, &charsWritten, infoLog);
                        std::cout<<"Fragment Shader Log Info: "<<infoLog<<std::endl;
                        delete[]infoLog;
                    }
                }
            }
        }
    }
    /*
    *
    * Linking the shader program
    *
    */
    GLuint tprog = glCreateProgram();
    if(tprog == 0)
        return false;

    if((f_vert == true) && (compiledv == 1))
        glAttachShader(tprog,vertexShader);

    if((f_geom == true) && (compiledg == 1))
        glAttachShader(tprog,geomShader);

    if((f_frag == true) && (compiledf == 1))
        glAttachShader(tprog,fragmentShader);

    if((f_vert == true) || (f_geom == true) || (f_frag == true)){
        int linked = 0;

        glLinkProgram(tprog);

        glGetProgramiv(tprog,GL_LINK_STATUS,&linked);
        if(linked == 0){
            int infologLength = 0;

            glGetProgramiv(tprog, GL_INFO_LOG_LENGTH,&infologLength);
            if(infologLength > 0){
                int charsWritten  = 0;
                char *infoLog = new char[infologLength]();
                glGetProgramInfoLog(tprog, infologLength, &charsWritten, infoLog);
                std::cout<<"Shader Program Log Info: "<<infoLog<<std::endl;
                delete[]infoLog;
            }
        }
        *prog = tprog;
    }
    return true;
}