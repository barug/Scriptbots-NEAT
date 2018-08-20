//
// Created by barth on 8/18/18.
//

#ifndef SCRIPTBOTS_VTKVIEW_H
#define SCRIPTBOTS_VTKVIEW_H

#include "vtkNew.h"
#include "vtkRenderWindow.h"
#include "vtkMutableDirectedGraph.h"
#include "vtkGraphLayout.h"
#include "Agent.h"

class VTKView;

extern VTKView *VTKVIEW;

void VTKDraw();
void VTKReshape(int width, int height);

class VTKView {

    vtkNew<vtkRenderWindow> _renWin;
    vtkNew<vtkMutableDirectedGraph> _graph;
    vtkNew<vtkGraphLayout> layout;

public:
    VTKView();

    void init();
    void reshape(int width, int height);
    void draw();
    void displayAgentInfo(const Agent *agent);
};


#endif //SCRIPTBOTS_VTKVIEW_H
