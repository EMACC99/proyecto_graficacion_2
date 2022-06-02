#include "includes/gl_debug.hpp"
#include "includes/openglwindow.hpp"
#include "includes/utils.hpp"
using namespace std::chrono_literals;

Viewport::Viewport(QWidget *parent): QOpenGLWidget(parent){
    this -> setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setVersion(2,1);
    this -> setFormat(format);

    this -> create();

    LightOn = true;
    connect(&this -> timer, SIGNAL(timeout()), this , SLOT(update()));
    timer.start(100ms);

    vert_shader = "programs.vert";
    frag_shader = "programs.frag";
    geom_shader = "";

    context = new QOpenGLContext;
    context -> setFormat(format);
    context -> create();
}

Viewport::~Viewport(){
    makeCurrent();
    for (auto &[name, textID] : textureID)
        glDeleteTextures(1, &textID);
    doneCurrent();

}

void Viewport::initializeGL(){
    gl = context -> functions(); 
    qDebug() << context;
    qDebug() << context -> isValid();
    qDebug() << (const char*) gl -> glGetString(GL_VERSION);
    // qDebug() << (const char*) gl -> glGetString(GL_EXTENSIONS);
    qDebug() << (const char*) gl -> glGetString(GL_SHADING_LANGUAGE_VERSION);


    GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat LightDiffuse_blue[]= { 0.0f, 0.0f, 1.0f, 1.0f }; //azul
    GLfloat LightDiffuse_red[] = {2.f, 0.0f, 0.0f, 1.0f};
    GLfloat LightDiffuse_morado[] = {.49f, 0.f, 1.f, 1.f};
    /*Setting light values*/
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse_morado);

    glClearColor(0.0,0.0,0.0,0.0);
    glClearDepth(1.f);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LEQUAL);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    initTextures();
    initModels();

    program = LoadShaders(vert_shader, frag_shader, gl);

    resizeGL(this -> width(), this -> height());
    
}


void Viewport::resizeGL(int w, int h){
    glViewport(0,0,h,w);
    float aspect = (GLfloat)w / (GLfloat)h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(43, aspect, 0.01, 100.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,eyez, 0,0,0, 0,1,0);

}

void Viewport::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0,0,eyez, 0,0,0,  0,1,0);
    if (LightOn && !glIsEnabled(GL_LIGHT0)){
        glDisable(GL_LIGHT1);
        glEnable(GL_LIGHT0);
    }
    else if(!LightOn){
        glDisable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
    }


    if (std::abs(player -> x() - modelos[model_index].x()) < 1.f && std::abs(player -> y() - modelos[model_index].y()) < .1f){
        gl -> glUseProgram(program);
        std::cout << "\rboo!"; 
    }
    else{
        gl -> glUseProgram(0);
        std::cout << "\rno boo :x";
    }
    glEnable(GL_TEXTURE_2D);

    // glEnable(GL_TEXTURE_GEN_S);
    // glEnable(GL_TEXTURE_GEN_T);
    // glBindTexture(GL_TEXTURE_2D, textureID.at("texture"));
    // Scene::draw_teapot();
    // glDisable(GL_TEXTURE_GEN_T);
    // glDisable(GL_TEXTURE_GEN_T);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, textureID.at("fur"));
    modelos[model_index].Draw();
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_S);
   
   
    glBindTexture(GL_TEXTURE_2D, textureID.at("wall"));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    Scene::draw_room();
    // Scene::draw_donut();

    glBindTexture(GL_TEXTURE_2D, kirbyID.at(player -> current_sprite));
    player -> Draw();

    glDisable(GL_TEXTURE_2D);
}

void Viewport::wheelEvent(QWheelEvent *event){
    this -> eyez -= static_cast<GLfloat>(event -> angleDelta().y()) / (MOUSE_NORMALIZATION);

    update();
}

void Viewport::mouseMoveEvent(QMouseEvent *event){

}

void Viewport::keyPressEvent(QKeyEvent *event){
    switch (event -> key()){
    case Qt::Key_L:
        LightOn = !LightOn;
        break;

    case Qt::Key_1:
        model_index = 0;
        break;
    
    case Qt::Key_2:
        model_index = 1;
        break;
    
    case Qt::Key_3:
        model_index = 2;
        break;

    case Qt::Key_Up:
        player -> move_y(player -> POSTIVE);
        player -> current_sprite += 1;
        if (player -> current_sprite >= kirbyID.size())
            player -> current_sprite = 0;
        break;
    
    case Qt::Key_Down:
        player -> move_y(player -> NEGATIVE);
        player -> current_sprite += 1;
        if (player -> current_sprite >= kirbyID.size())
            player -> current_sprite = 0;
        break;

    case Qt::Key_Left:
        player -> move_x(player -> NEGATIVE);
        player -> current_sprite += 1;
        if (player -> current_sprite >= kirbyID.size())
            player -> current_sprite = 0;
        break;

    case Qt::Key_Right:
        player -> move_x(player -> POSTIVE);
        player -> current_sprite += 1;
        if (player -> current_sprite >= kirbyID.size())
            player -> current_sprite = 0;
        break;

    default:
        break;
    }
}

void Viewport::initTextures(){


    std::vector<std::string> files {"fur.png", "texture.bmp", "wall.png"};
    std::vector<std::string> texture_names = {"fur", "texture", "wall"};

    std::vector<std::string> kirby;
    std::vector<std::string> kirby_names;

    uint8_t count = 1;
    for (const auto &item : fs::directory_iterator("assets/Kirby/")){
        kirby.emplace_back("Kirby/" + item.path().filename().string()); 
        kirby_names.emplace_back("kirby_" + std::to_string(count));
        ++count;
    }

    unsigned int n = files.size();

    std::vector<GLuint> IDS(n);

    glGenTextures(n, IDS.data());
    for (unsigned int i = 0; i < n; ++i){
        textureID[texture_names[i]] = IDS[i];
        glBindTexture(GL_TEXTURE_2D, IDS[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        auto [textureData, width, height] = Texture::LoadTextureFile(files[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glBindTexture(GL_TEXTURE_2D, 0);
        Texture::FreeTextureData(textureData);        
    }


    n = kirby.size();

    // std::fill(IDS.begin(), IDS.end(), 0);

    IDS.clear();

    for(unsigned int i = 0; i < n; ++i)
        IDS.emplace_back(0);

    kirbyID.resize(n, 0);

    glGenTextures(n , IDS.data());

    for (unsigned int i = 0; i < n; ++i){
        kirbyID[i] = IDS[i];
        glBindTexture(GL_TEXTURE_2D, IDS[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        auto [textureData, width, height] = Texture::LoadTextureFile(kirby[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        Texture::FreeTextureData(textureData);
    }
}


void Viewport::initModels(){

    std::vector<std::string> files {"bunny.obj", "dragon.obj", "tyra.obj"};
    // std::vector<std::string> files {"bunny.obj"};

    for (unsigned int i = 0; i < files.size(); ++i){
        Model model(files[i]);
        modelos.emplace_back(model);
    }

    player = new Player("assets/");
}