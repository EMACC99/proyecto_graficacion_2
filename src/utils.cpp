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
        std::cerr << "Couldn't open " << ShaderPath << "Is it in the shaders directory?";
        exit(EXIT_FAILURE);
    }
}


void CompileShader(const char *shader_src_ptr, GLuint *ShaderID, int *InfoLogLength, GLint *Result){
    // int InfoLogLength;
    // GLint Result = GL_FALSE;
    glShaderSource(*ShaderID, 1, &shader_src_ptr , NULL);
	glCompileShader(*ShaderID);

	// Check Vertex Shader
	glGetShaderiv(*ShaderID, GL_COMPILE_STATUS, Result);
	glGetShaderiv(*ShaderID, GL_INFO_LOG_LENGTH, InfoLogLength);
	if ( *InfoLogLength > 0 ){
		std::vector<char> ShaderErrorMessage(*InfoLogLength+1);
		glGetShaderInfoLog(*ShaderID, *InfoLogLength, NULL, &ShaderErrorMessage[0]);
		printf("%s\n", &ShaderErrorMessage[0]);
	}
}

GLuint LoadShaders(const std::string &vertex_file, const std::string &fragment_file){
    fs::path vertex_path = "shaders/" + vertex_file;
    vertex_path = fs::absolute(vertex_path);
    fs::path fragment_path = "shaders/" + fragment_file;
    fragment_path = fs::absolute(fragment_path);

    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string VertexShaderCode;
    ReadCode(VertexShaderCode, vertex_path);

    std::string FragmentShaderCode;
    ReadCode(FragmentShaderCode, fragment_path);


    int InfoLogLength;
    GLint Result = GL_FALSE;

    std::cout << "Compiling shader: " << vertex_file << "\n";

    const char *vertex_src_ptr = VertexShaderCode.c_str();

    CompileShader(vertex_src_ptr, &VertexShaderID, &InfoLogLength, &Result);

    std::cout << "Compiling shader: " << fragment_file << "\n";

	const char *fragment_scr_ptr = FragmentShaderCode.c_str();

    CompileShader(fragment_scr_ptr, &FragmentShaderID, &InfoLogLength, &Result);


    printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;

}