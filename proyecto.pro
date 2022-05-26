QT+= core gui opengl widgets

equals(QT_MAJOR_VERSION, 6){
        QT += openglwidgets
}

SOURCES += \
    src/main.cpp\
    src/openglwindow.cpp\
    src/utils.cpp \
    src/scene.cpp

HEADERS+=\
         includes/openglwindow.hpp\
         includes/gl_debug.hpp\
         includes/utils.hpp\
         includes/scene.hpp

QMAKE_CXXFLAGS+= -std=c++17 -g -Wno-deprecated-declarations -Wall -Wpedantic -Wextra -Wno-sign-compare



CONFIG += debug
#CONFIG += release

# make a mac debug build instead of an .app
CONFIG -= app_bundle
#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE += -Os
MOC_DIR = tmp/moc/
OBJECTS_DIR = tmp/obj/

macx {
	LIBS += -framework OpenGL -framework GLUT -framework Cocoa

}
else{
	LIBS += -lGL -lglut -lGLU -lGLEW
}

win32:QMAKE_CXXFLAGS -=  -Wno-deprecated-declarations

TARGET = Proyecto
