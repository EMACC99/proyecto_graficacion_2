#include "includes/openglwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    // glutInit(&argc, argv);
    Viewport ui;
    ui.setWindowTitle("Proyecto OpenGL");
    ui.show();

    return app.exec();
}
