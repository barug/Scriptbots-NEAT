#include <unistd.h>
#include <sstream>

#include "GLView.h"
#include "VTKView.h"
#include "VTKPlotView.h"
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
VTKPlotView * VTKPLOTVIEW;
VTKSpeciesView *VTKSPECIESVIEW;

int main(int argc, char **argv) {
    conf::initialize();
    srand(time(0));
    
    printf("p= pause, d= toggle drawing (for faster computation), f= draw food too, += faster, -= slower\n");
    printf("Pan around by holding down right mouse button, and zoom by holding down middle button.\n");

    World* world = nullptr;


    int opt;
    while ((opt = getopt(argc, argv, "l:s:p:c:")) != -1) {
        switch (opt) {
            case 'l':
                world = new World(optarg);
                break;
            case 's':
                if (!world)
                    world = new World();
                world->setSaveFilePath(optarg);
                break;
            case 'p':
                if (!world)
                    world = new World();
                world->setPeriodicSave(std::stoi(optarg));
                break;
            case 'c':
                if (world) {
                    cout << "c option should be first" << endl;
                    return 0;
                }
                conf::load_conf(optarg);
                break;
            default: /* '?' */
                break;
        }
    }

    if (!world)
        world = new World();

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

    glutMainLoop();
    return 0;
}
