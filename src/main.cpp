#include "GLView.h"
#include "VTKView.h"
#include "World.h"

#include "config.h"
#ifdef LOCAL_GLUT32
    #include "glut.h"
#else
    #include <GL/glut.h>
#endif

#include <stdio.h>
#include <vtkNew.h>

GLView* GLVIEW = new GLView(0);
VTKView* VTKVIEW = new VTKView();

int main(int argc, char **argv) {
    srand(time(0));
    if (conf::WIDTH%conf::CZ!=0 || conf::HEIGHT%conf::CZ!=0) printf("CAREFUL! The cell size variable conf::CZ should divide evenly into  both conf::WIDTH and conf::HEIGHT! It doesn't right now!");
    
    
    printf("p= pause, d= toggle drawing (for faster computation), f= draw food too, += faster, -= slower\n");
    printf("Pan around by holding down right mouse button, and zoom by holding down middle button.\n");
    
    World* world = new World("save");
    GLVIEW->setWorld(world);

    //GLUT SETUP
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(30,30);
    glutInitWindowSize(conf::WWIDTH,conf::WHEIGHT);
    glutCreateWindow("Scriptbots");
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glutDisplayFunc(gl_renderScene);
    glutIdleFunc(gl_handleIdle);
    glutReshapeFunc(gl_changeSize);

    glutKeyboardFunc(gl_processNormalKeys);
    glutMouseFunc(gl_processMouse);
    glutMotionFunc(gl_processMouseActiveMotion);

    VTKVIEW->init();
    glutMainLoop();
    return 0;
}
