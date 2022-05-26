#include "includes/utils.hpp"
#include "includes/gl_debug.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"



std::tuple<unsigned char *, int, int> Texture::LoadTextureFile(const std::string &filename){
    fs::path path = "assets/" + filename;
    path = fs::absolute(path);

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