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
VTKPlotView * VTKPLOTVIEW = new VTKPlotView();
VTKSpeciesView *VTKSPECIESVIEW;


void load_conf(std::string path)
{
    char curline[4096];
    std::string wordBuff;
    std::ifstream inFile(path, std::ifstream::in);

    inFile.eof();
    //for (ifstream.getline(curline, 4096 && (inFile.rdstate & std::ifstream::eofbit)))
    while (!inFile.eof()) {
        inFile.getline(curline, 4096);
        std::stringstream lineStream(curline);

        lineStream >> wordBuff;
        if (wordBuff == "WIDTH") {
            lineStream >> conf::WIDTH;
        }
        if (wordBuff == "HEIGHT") {
            lineStream >> conf::HEIGHT;
        }
        if (wordBuff == "WWIDTH") {
            lineStream >> conf::WWIDTH;
        }
        if (wordBuff == "WHEIGHT") {
            lineStream >> conf::WHEIGHT;
        }
        if (wordBuff == "CZ") {
            lineStream >> conf::CZ;
        }
        if (wordBuff == "NUMBOTS") {
            lineStream >> conf::NUMBOTS;
        }
        if (wordBuff == "BOTRADIUS") {
            lineStream >> conf::BOTRADIUS;
        }
        if (wordBuff == "BOTSPEED") {
            lineStream >> conf::BOTSPEED;
        }
        if (wordBuff == "SPIKESPEED") {
            lineStream >> conf::SPIKESPEED;
        }
        if (wordBuff == "SPIKEMULT") {
            lineStream >> conf::SPIKEMULT;
        }
        if (wordBuff == "BABIES") {
            lineStream >> conf::BABIES;
        }
        if (wordBuff == "BOOSTSIZEMULT") {
            lineStream >> conf::WWIDTH;
        }
        if (wordBuff == "REPRATEH") {
            lineStream >> conf::REPRATEH;
        }
        if (wordBuff == "REPRATEC") {
            lineStream >> conf::REPRATEC;
        }
        if (wordBuff == "MATING_RADIUS") {
            lineStream >> conf::MATING_RADIUS;
        }
        if (wordBuff == "MATING_COMPATIBILITY_TRESHOLD") {
            lineStream >> conf::MATING_COMPATIBILITY_TRESHOLD;
        }
        if (wordBuff == "MATING_BABIES") {
            lineStream >> conf::MATING_BABIES;
        }
        if (wordBuff == "DIST") {
            lineStream >> conf::DIST;
        }
        if (wordBuff == "METAMUTRATE1") {
            lineStream >> conf::METAMUTRATE1;
        }
        if (wordBuff == "METAMUTRATE2") {
            lineStream >> conf::METAMUTRATE2;
        }
        if (wordBuff == "FOODINTAKE") {
            lineStream >> conf::FOODINTAKE;
        }
        if (wordBuff == "FOODWASTE") {
            lineStream >> conf::FOODWASTE;
        }
        if (wordBuff == "FOODMAX") {
            lineStream >> conf::FOODMAX;
        }
        if (wordBuff == "FOODADDFREQ") {
            lineStream >> conf::FOODADDFREQ;
        }
        if (wordBuff == "FOODTRANSFER") {
            lineStream >> conf::FOODTRANSFER;
        }
        if (wordBuff == "FOOD_SHARING_DISTANCE") {
            lineStream >> conf::FOOD_SHARING_DISTANCE;
        }
        if (wordBuff == "TEMPERATURE_DISCOMFORT") {
            lineStream >> conf::TEMPERATURE_DISCOMFORT;
        }
        if (wordBuff == "FOOD_DISTRIBUTION_RADIUS") {
            lineStream >> conf::FOOD_DISTRIBUTION_RADIUS;
        }
        if (wordBuff == "REPMULT") {
            lineStream >> conf::REPMULT;
        }
    }
    inFile.close();
}

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
                load_conf(optarg);
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
