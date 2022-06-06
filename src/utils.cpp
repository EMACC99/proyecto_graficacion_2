#include "includes/utils.hpp"
#include "includes/gl_debug.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

std::tuple<unsigned char *, int, int> Texture::LoadTextureFile(const std::string &filename){
    fs::path path = "assets/" + filename;
    path = fs::absolute(path);
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 3);
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

void ReadCode(std::string &ShaderCode, const fs::path &ShaderPath){
    std::cout << ShaderPath.c_str() << "\n";
    std::ifstream ShaderStream(ShaderPath.c_str(), std::ios::in);
    if (ShaderStream.is_open()){
        std::stringstream sstr;
        sstr << ShaderStream.rdbuf();
        ShaderCode = sstr.str();
        ShaderStream.close();
    }
    else{
        std::cerr << "Couldn't open " << ShaderPath << "Is it in the shaders directory?\n";
        exit(EXIT_FAILURE);
    }
}


void CompileShader(const char *shader_src_ptr, GLuint *ShaderID, int *InfoLogLength, GLint *Result, QOpenGLFunctions *gl){
    // int InfoLogLength;
    // GLint Result = GL_FALSE;
    gl -> glShaderSource(*ShaderID, 1, &shader_src_ptr , NULL);
	gl -> glCompileShader(*ShaderID);

	// Check Shader
	gl -> glGetShaderiv(*ShaderID, GL_COMPILE_STATUS, Result);
	gl -> glGetShaderiv(*ShaderID, GL_INFO_LOG_LENGTH, InfoLogLength);
	if ( *InfoLogLength > 0 ){
		std::vector<char> ShaderErrorMessage(*InfoLogLength+1);
		gl -> glGetShaderInfoLog(*ShaderID, *InfoLogLength, NULL, &ShaderErrorMessage[0]);
		qDebug() << &ShaderErrorMessage[0];
	}
}

GLuint LoadShaders(const std::string &vertex_file, const std::string &fragment_file, QOpenGLFunctions *gl){
    fs::path vertex_path = "shaders/" + vertex_file;
    vertex_path = fs::absolute(vertex_path);
    fs::path fragment_path = "shaders/" + fragment_file;
    fragment_path = fs::absolute(fragment_path);

    GLuint VertexShaderID = gl -> glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = gl -> glCreateShader(GL_FRAGMENT_SHADER);

    std::string VertexShaderCode;
    ReadCode(VertexShaderCode, vertex_path);

    std::string FragmentShaderCode;
    ReadCode(FragmentShaderCode, fragment_path);


    int InfoLogLength;
    GLint Result = GL_FALSE;

    std::cout << "Compiling shader: " << vertex_file << "\n";

    const char *vertex_src_ptr = VertexShaderCode.c_str();

    CompileShader(vertex_src_ptr, &VertexShaderID, &InfoLogLength, &Result, gl);

    std::cout << "Compiling shader: " << fragment_file << "\n";

	const char *fragment_scr_ptr = FragmentShaderCode.c_str();

    CompileShader(fragment_scr_ptr, &FragmentShaderID, &InfoLogLength, &Result, gl);


    qDebug() << "Linking program";
	GLuint ProgramID = gl -> glCreateProgram();
	gl -> glAttachShader(ProgramID, VertexShaderID);
	gl -> glAttachShader(ProgramID, FragmentShaderID);
	gl -> glLinkProgram(ProgramID);

	// Check the program
	gl -> glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	gl -> glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		gl -> glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		qDebug() <<  &ProgramErrorMessage[0];
	}
	
	gl -> glDetachShader(ProgramID, VertexShaderID);
	gl -> glDetachShader(ProgramID, FragmentShaderID);
	
	gl -> glDeleteShader(VertexShaderID);
	gl -> glDeleteShader(FragmentShaderID);

	return ProgramID;

}