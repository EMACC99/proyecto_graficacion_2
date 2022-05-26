#include "includes/scene.hpp"
#include "includes/utils.hpp"
#include "OBJ-Loader/Source/OBJ_Loader.h"
float teapot_rotation = 0.f;
float torus_rotation = 0.f;
void Scene::draw_room(){
    
    glPushMatrix();
        glTranslatef(0.f,0,-.2f);
        // glRotatef(20, 0,1 ,0);
        glScalef(1.7f, 1.7f, 1.7f);
        // glutWireCube(4);

        glBegin(GL_QUADS);
            //top
            glColor3f(  1.f,  1.f,  0.f);
            // glMaterialfv( GL_FRONT, GL_DIFFUSE, grass_color);

            glTexCoord2f(0.0f,0.0f);
            glVertex3f( 1.0f, 1.0f,-1.0f);
            glTexCoord2f(1.0f,0.0f);
            glVertex3f(-1.0f, 1.0f,-1.0f);
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);
            glTexCoord2f(0.0f,1.0f);
            glVertex3f( 1.0f, 1.0f, 1.0f);

            //back

            glColor3f(  .5f,  1.f,  0.f);
            // glMaterialfv( GL_FRONT, GL_DIFFUSE, grass_color);
            glTexCoord2f(1.0f,0.0f);
            glVertex3f( 1.f,  1.0f, -1.f);
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(-1.f,  1.0f, -1.f);
            glTexCoord2f(0.0f,1.0f);
            glVertex3f(-1.f, -1.0f, -1.f);
            glTexCoord2f(0.0f,0.0f);
            glVertex3f( 1.f, -1.0f, -1.f);

            //floor
            glColor3f(  1.f,  0.5f,  0.f);
            // glMaterialfv( GL_FRONT, GL_DIFFUSE, grass_color);
            glTexCoord2f(0.0f,0.0f);
            glVertex3f( 1.0f,-1.0f, 1.0f);
            glTexCoord2f(1.0f,0.0f);
            glVertex3f(-1.0f,-1.0f, 1.0f);
            glTexCoord2f(1.f,1.0f);
            glVertex3f(-1.0f,-1.0f,-1.0f);
            glTexCoord2f(0.f,1.f);
            glVertex3f( 1.0f,-1.0f,-1.0f);
            
            //left
            glColor3f(  .5f,  1.f, 1.f);
            // glMaterialfv( GL_FRONT, GL_DIFFUSE, grass_color);
            glTexCoord2f(0.0f,0.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);
            glTexCoord2f(1.0f,0.0f);
            glVertex3f(-1.0f, 1.0f,-1.0f);
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(-1.0f,-1.0f,-1.0f);
            glTexCoord2f(0.0f,1.0f);
            glVertex3f(-1.0f,-1.0f, 1.0f);

            //right
            glColor3f(  0.f,  0.5f, 1.f);
            // glMaterialfv( GL_FRONT, GL_DIFFUSE, grass_color);
            glTexCoord2f(0.0f,0.0f);
            glVertex3f( 1.0f, 1.0f,-1.0f);
            glTexCoord2f(1.0f,0.0f);
            glVertex3f( 1.0f, 1.0f, 1.0f);
            glTexCoord2f(1.0f,1.0f);
            glVertex3f( 1.0f,-1.0f, 1.0f);
            glTexCoord2f(0.0f,1.0f);
            glVertex3f( 1.0f,-1.0f,-1.0f);

        glEnd();


    glPopMatrix();
}


void Scene::draw_teapot(const GLdouble &size){
    teapot_rotation += rotating_factor;
    glPushMatrix();
        // glMaterialfv(GL_FRONT, GL_DIFFUSE,violet);
        glTranslatef(0.7f, 0.7f, 0.f);
        glColor3f(1.f, .6f, 0.f);
        glRotatef(teapot_rotation, 1, 1, 1);
        glutSolidTeapot(size);
    glPopMatrix();
}


void Scene::draw_donut(const GLdouble &innerRadius, const GLdouble &outerRadius, const GLint &sides, const GLint &rings){
    torus_rotation += rotating_factor;
    glPushMatrix();
        glTranslatef(1.1f, -1.f, -0.3f);
        glRotatef(torus_rotation, 3, 0, 0);
        glColor3f(2.f, .2f, 1.98f);
        glutSolidTorus(innerRadius, outerRadius, sides, rings);
    glPopMatrix();
}

Model::Model(const std::string &filename){
    fs::path path = "assets/" + filename;
    path = fs::absolute(path);

    objl::Loader loader;
    loader.LoadFile(path.string());
    objl::Mesh mesh = loader.LoadedMeshes.back();

    for (unsigned long vertex = 0; vertex < mesh.Vertices.size(); ++vertex){
        vertexData.push_back(mesh.Vertices[vertex].Position.X);
        vertexData.push_back(mesh.Vertices[vertex].Position.Y);
        vertexData.push_back(mesh.Vertices[vertex].Position.Z);
        vertexData.push_back(mesh.Vertices[vertex].Normal.X);
        vertexData.push_back(mesh.Vertices[vertex].Normal.Y);
        vertexData.push_back(mesh.Vertices[vertex].Normal.Z);
    }

    
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);

    // glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER,VBO);
    // glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    // glEnableVertexAttribArray(0);
}

Model::~Model(){
    if (texture != 0)
        glDeleteTextures(1, &texture);
}

void Model::Draw(){
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glNormalPointer(GL_FLOAT, 6 * sizeof(GLfloat), vertexData.data() + 3);
    glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), vertexData.data());
    
    glPushMatrix();
        glScalef(.3f, .3f, .3f);
        glTranslatef(2.f, 0, 0);
        glColor3f(1,1,1);
        glRotatef(rotation, 1,1,1);
        glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 6);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    rotation -= rotating_factor;
    
}


Player::Player(const std::string &sprites){
    position = {-0.5f, -0.5f, 0.f};
}

Player::~Player(){   
}


float Player::x(){
    return position[0];
}

float Player::y(){
    return position[1];
}


void Player::move_x(const Player::direction_sign &dir){
    position[0] += dir*move_rate;
}

void Player::move_y(const Player::direction_sign &dir){
    position[1] += dir*move_rate;
}

void Player::Draw(){
    auto [x,y,z]  = position;
    glPushMatrix();
        // glPointSize(20.0f);
        // glScalef(0.1f, 0.1f, 0.1f);
        // glBegin(GL_POINTS);
        //     glColor3f(1.0f,0.0f,1.0f);
        //     glVertex3f(x,y,z);
        // glEnd();
        glTranslatef(x,y,z);
        glBegin(GL_POLYGON);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f( 1.f,  1.0f, 0.f);
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(-1.f,  1.0f, 0.f);
            glTexCoord2f(0.0f,1.0f);
            glVertex3f(-1.f, -1.0f, 0.f);
            glTexCoord2f(0.0f,0.0f);
            glVertex3f( 1.f, -1.0f, 0.f);
        glEnd();
    glPopMatrix();
}