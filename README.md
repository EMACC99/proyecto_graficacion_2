# Proyecto 2 de graficacion

Librerias Necesarias:

* OpenGL
* glu
* glew
* glm
* glut
* Qt
* qmake
* make

Para clonar el proyecto, hagalo con `git clone --recursive` para que los submodulos se inicialicen correctamente.

## Compliacion

```sh
qmake
make
./Proyecto
```

## Instrucciones de uso

Mover al Kirby:
<kbd> W A S D </kdb>
![move](Images/move.gif)

Cuando se acerca al objeto, se activa el shader
![shader](Images/shader.gif)

Para cambiar la iluminacion, presione la tecla
<kbd>L</kbd>
![luz](Images/Luz.gif)
Mueva la rueda del mouse para hacer zoom en la escena.
![zoom](Images/zoom.gif)
Cambia el modelo obj con las tecla:
<kbd>1, 2</kbd> o <kbd>3</kbd>
![cambio](Images/change_obj.gif)

## Ventajas y otras notas

Se penso en la disminucion de dependencias fuertes como lo es `QT` para que se pueda usar independientemente de la libreria que maneje el vewport.

Uso de header only libraries para minimizar instalaciones.
