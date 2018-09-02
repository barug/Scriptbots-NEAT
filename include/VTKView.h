//
// Created by barth on 8/18/18.
//

#ifndef SCRIPTBOTS_VTKVIEW_H
#define SCRIPTBOTS_VTKVIEW_H

#include "vtkNew.h"
#include "vtkRenderWindow.h"
#include "vtkMutableDirectedGraph.h"
#include "vtkGraphLayout.h"
#include "vtkCamera.h"
#include "Agent.h"
#include <vtkAttributeClustering2DLayoutStrategy.h>
#include <vtkActor.h>

class VTKView;

extern VTKView *VTKVIEW;

void VTKDraw();
void VTKReshape(int width, int height);

class VTKView {

    vtkNew<vtkRenderWindow> _renWin;


    vtkNew<vtkMutableDirectedGraph> _graph;
    vtkNew<vtkGraphLayout> _layout;
    vtkNew<vtkAttributeClustering2DLayoutStrategy> _strategy;
    vtkNew<vtkRenderer> _ren;
    vtkCamera *_camera;
    vtkNew<vtkActor> edge_actor;
    vtkNew<vtkActor> vertex_actor;



public:
    VTKView();

    void init();
    void reshape(int width, int height);
    void draw();
    void zoom(double factor);
    void move(double x, double y);
    void displayAgentInfo(const Agent *agent);
};


#endif //SCRIPTBOTS_VTKVIEW_H
