#include <unistd.h>
#include <sstream>

#include "GLView.h"
#include "World.h"
#include "SimulationContext.h"

#include "config.h"

#ifdef LOCAL_GLUT32
    #include "glut.h"
#elif defined(__APPLE__)
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#ifdef HAVE_VTK
    #include "VTKDashboard.h"
#endif

#include <stdio.h>

int main(int argc, char **argv) {
    conf::initialize();
    srand(time(0));
    
    printf("p= pause, d= toggle drawing (for faster computation), f= draw food too, += faster, -= slower\n");
    printf("Pan: drag with left or right mouse button. Zoom: scroll wheel, or i/k keys, or drag with middle button.\n");
    printf("Click on an agent to select it and view its neural network.\n");

    // Initialize GLUT first (before VTK which may also use GLUT)
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    
    // Get screen dimensions
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
    
    // Calculate window sizes (each takes half the screen)
    int halfWidth = screenWidth / 2;
    int winHeight = screenHeight - 45; // Leave space for menu bar

    // Now create the world (which creates VTKDashboard)
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
                    std::cout << "c option should be first" << std::endl;
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

    // Initialize the simulation context singleton with the world
    Sim.initialize(world);
    
    // Create GLUT window on RIGHT half (VTK will default to left)
    // GLUT: Y=0 is top of screen
    glutInitWindowPosition(halfWidth, 45);
    glutInitWindowSize(halfWidth, winHeight);
    glutCreateWindow("Scriptbots - Simulation");
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glutDisplayFunc(gl_renderScene);
    glutIdleFunc(gl_handleIdle);
    glutReshapeFunc(gl_changeSize);

    glutKeyboardFunc(gl_processNormalKeys);
    glutMouseFunc(gl_processMouse);
    glutMotionFunc(gl_processMouseActiveMotion);
#ifdef __APPLE__
    // On macOS/freeglut, use glutMouseWheelFunc if available
    // Note: Apple's GLUT doesn't have glutMouseWheelFunc, scroll is handled via buttons 3/4 in glutMouseFunc
#else
    glutMouseWheelFunc(gl_mouseWheel);
#endif

#ifdef HAVE_VTK
    // Show VTK window on LEFT half (default position)
    if (Sim.vtkDashboard()) {
        Sim.vtkDashboard()->setWindowGeometry(0, 0, halfWidth, winHeight);
        Sim.vtkDashboard()->show();
    }
#endif

    glutMainLoop();
    return 0;
}
