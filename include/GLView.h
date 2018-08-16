#ifndef GLVIEW_H
#define GLVIEW_H


#include "View.h"
#include "World.h"

class GLView;

extern GLView* GLVIEW;

void gl_processNormalKeys(unsigned char key, int x, int y);
void gl_processMouse(int button, int state, int x, int y);
void gl_processMouseActiveMotion(int x, int y);
void gl_changeSize(int w, int h);
void gl_handleIdle();
void gl_renderScene();

class GLView : public View
{

public:
    GLView(World* w);
    virtual ~GLView();
    
    virtual void drawAgent(const Agent &a);
    virtual void drawFood(int x, int y, float quantity);
    virtual void drawMisc();
    
    void setWorld(World* w);
    
    //GLUT functions
    void processNormalKeys(unsigned char key, int x, int y);
    void processMouse(int button, int state, int x, int y);
    void processMouseActiveMotion(int x, int y);
    void changeSize(int w, int h);
    void handleIdle();
    void renderScene();
    
private:
    
    World *world;
    bool paused;
    bool draw;
    int skipdraw;
    bool drawfood;
    char buf[100];
    char buf2[10];
    int modcounter;
    int lastUpdate;
    int frames;
    
    
    float scalemult;
    float xtranslate, ytranslate;
    int downb[3];
    int mousex, mousey;
    
    int following;
    
};

#endif // GLVIEW_H
