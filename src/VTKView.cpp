//
// Created by barth on 8/18/18.
//

#include "VTKView.h"

#ifdef LOCAL_GLUT32
#include "glut.h"
#else
#include <GL/glut.h>
#endif


#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkNew.h"

#include <vtkRandomGraphSource.h>
#include <vtkGraphLayoutView.h>
#include <vtkViewTheme.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkFast2DLayoutStrategy.h>
#include <vtkGraphLayout.h>
#include <vtkGraphToGlyphs.h>
#include <vtkArcParallelEdgeStrategy.h>
#include <vtkEdgeLayout.h>
#include <vtkGraphToPolyData.h>
#include <vtkProperty.h>
#include <vtkMutableDirectedGraph.h>

#include "NEAT/nnode.h"

void VTKDraw()
{
    VTKVIEW->draw();
    glutSwapBuffers();
}

void VTKReshape(int width, int height)
{
    VTKVIEW->reshape(width, height);
}

VTKView::VTKView()
{
    //setup a strategy for laying out the graph
// NOTE: You can set additional options for each strategy, as desired
    vtkNew<vtkFast2DLayoutStrategy> strategy;
//strategy = vtkSimple2DLayoutStrategy()
//strategy = vtkCosmicTreeLayoutStrategy()
//strategy = vtkForceDirectedLayoutStrategy()
//strategy = vtkTreeLayoutStrategy()
// set the strategy on the layout
    //vtkNew<vtkGraphLayout> layout;
    layout->SetLayoutStrategy(strategy.GetPointer());
    //layout->SetInputConnection(source->GetOutputPort());
    layout->SetInputData(_graph.GetPointer());
// create the renderer to help in sizing glyphs for the vertices
    vtkNew<vtkRenderer> ren;

// Pipeline for displaying vertices - glyph -> mapper -> actor -> display
// mark each vertex with a circle glyph
    vtkNew<vtkGraphToGlyphs> vertex_glyphs;
    vertex_glyphs->SetInputConnection(layout->GetOutputPort());
    vertex_glyphs->SetGlyphType(7);
    vertex_glyphs->FilledOn();
    vertex_glyphs->SetRenderer(ren.GetPointer());

// create a mapper for vertex display
    vtkNew<vtkPolyDataMapper> vertex_mapper;
    vertex_mapper->SetInputConnection(vertex_glyphs->GetOutputPort());
    vertex_mapper->SetScalarRange(0,100);
    vertex_mapper->SetScalarModeToUsePointFieldData();
    vertex_mapper->SelectColorArray("vertex id");

// create the actor for displaying vertices
    vtkNew<vtkActor> vertex_actor;
    vertex_actor->SetMapper(vertex_mapper.GetPointer());
// Pipeline for displaying edges of the graph - layout -> lines -> mapper -> actor -> display
// NOTE: If no edge layout is performed, all edges will be rendered as
// line segments between vertices in the graph.
    vtkNew<vtkArcParallelEdgeStrategy> edge_strategy;
    vtkNew<vtkEdgeLayout> edge_layout;
    edge_layout->SetLayoutStrategy(edge_strategy.GetPointer());
    edge_layout->SetInputConnection(layout->GetOutputPort());
    vtkNew<vtkGraphToPolyData> edge_geom;
    edge_geom->SetInputConnection(edge_layout->GetOutputPort());
// create a mapper for edge display
    vtkNew<vtkPolyDataMapper> edge_mapper;
    edge_mapper->SetInputConnection(edge_geom->GetOutputPort());
// create the actor for displaying theedges
    vtkNew<vtkActor> edge_actor;
    edge_actor->SetMapper(edge_mapper.GetPointer());
    edge_actor->GetProperty()->SetColor(1,1,1);
    edge_actor->GetProperty()->SetOpacity(0.25);

    ren->AddActor(vertex_actor.GetPointer());
    ren->AddActor(edge_actor.GetPointer());
    _renWin->AddRenderer(ren.GetPointer());
}

void VTKView::init()
{
    _renWin->Start();
}

void VTKView::reshape(int width, int height) {
    _renWin->SetSize( width, height );
}
void VTKView::draw() {
    _renWin->Render();
}

void VTKView::displayAgentInfo(const Agent *agent) {
    std::vector<NEAT::NNode*> &nodes = agent->brain._net->all_nodes;

    _graph->Initialize();
    _graph->SetNumberOfVertices(nodes.size());
    cout << "total :" << _graph->GetNumberOfVertices() << endl;
    for (auto *node: nodes) {
        cout << "node_id : " << node->node_id - 1 << endl;
        for (auto link: node->incoming) {
            cout << "link from : " << link->in_node->node_id - 1 << " to : " << node->node_id - 1 << endl;
            _graph->AddEdge(link->in_node->node_id - 1, node->node_id - 1);
        }
    }
    //layout->SetInputData(_graph.GetPointer());
    layout->Update();
    _renWin->Render();
    //_renWin->Start();

    //glutPostRedisplay();
}