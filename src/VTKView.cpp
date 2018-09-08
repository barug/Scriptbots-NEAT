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
#include <vtkSimple2DLayoutStrategy.h>
#include <vtkClustering2DLayoutStrategy.h>
#include <vtkAttributeClustering2DLayoutStrategy.h>
#include <vtkForceDirectedLayoutStrategy.h>
#include <vtkPassThroughLayoutStrategy.h>
#include <vtkGraphLayout.h>
#include <vtkGraphToGlyphs.h>
#include <vtkArcParallelEdgeStrategy.h>
#include <vtkEdgeLayout.h>
#include <vtkGraphToPolyData.h>
#include <vtkProperty.h>
#include <vtkMutableDirectedGraph.h>
#include <vtkTree.h>
#include <vtkTreeLayoutStrategy.h>
#include <vtkCosmicTreeLayoutStrategy.h>
#include <vtkDataSetAttributes.h>
#include <vtkGraphMapper.h>
#include <vtkSpanTreeLayoutStrategy.h>
#include <vtkContextScene.h>

#include "NEAT/nnode.h"

#include <vtkChartBox.h>

#include <vtkTable.h>
#include <vtkComputeQuartiles.h>
#include <vtkStatisticsAlgorithm.h>
#include <vtkContextActor.h>
#include <vtkLookupTable.h>

#include "vtkSmartPointer.h"

VTKView::VTKView()
{
    //setup a strategy for laying out the graph
// NOTE: You can set additional options for each strategy, as desired


//strategy = vtkSimple2DLayoutStrategy()
//strategy = vtkCosmicTreeLayoutStrategy()
//strategy = vtkForceDirectedLayoutStrategy()
//strategy = vtkTreeLayoutStrategy()
    //vtkNew<vtkSimple2DLayoutStrategy> strategy;
// set the strategy on the layout
    //vtkNew<vtkGraphLayout> layout;

    //_strategy->SetRestDistance(0.1);
    _strategy->SetVertexAttribute("nodeType");
    _layout->SetLayoutStrategy(_strategy.GetPointer());
    //layout->SetInputConnection(source->GetOutputPort());
    _layout->SetInputData(_graph.GetPointer());
// create the renderer to help in sizing glyphs for the vertices


// Pipeline for displaying vertices - glyph -> mapper -> actor -> display
// mark each vertex with a circle glyph
    vtkNew<vtkGraphToGlyphs> vertex_glyphs;
    vertex_glyphs->SetInputConnection(_layout->GetOutputPort());
    vertex_glyphs->SetGlyphType(7);
    vertex_glyphs->FilledOn();
    vertex_glyphs->SetRenderer(_ren.GetPointer());

// create a mapper for vertex display
    vtkNew<vtkPolyDataMapper> vertex_mapper;
    vertex_mapper->SetInputConnection(vertex_glyphs->GetOutputPort());
    vertex_mapper->SetScalarRange(0,3);
    vertex_mapper->SetScalarModeToUsePointFieldData();
    vertex_mapper->SelectColorArray("nodeType");

    vtkNew<vtkActor> edge_actor;
    vtkNew<vtkActor> vertex_actor;


// create the actor for displaying vertices
    vertex_actor->SetMapper(vertex_mapper.GetPointer());
// Pipeline for displaying edges of the graph - layout -> lines -> mapper -> actor -> display
// NOTE: If no edge layout is performed, all edges will be rendered as
// line segments between vertices in the graph.
    vtkNew<vtkArcParallelEdgeStrategy> edge_strategy;
    vtkNew<vtkEdgeLayout> edge_layout;
    edge_layout->SetLayoutStrategy(edge_strategy.GetPointer());
    edge_layout->SetInputConnection(_layout->GetOutputPort());
    vtkNew<vtkGraphToPolyData> edge_geom;
    edge_geom->SetInputConnection(edge_layout->GetOutputPort());
// create a mapper for edge display
    vtkNew<vtkPolyDataMapper> edge_mapper;
    edge_mapper->SetInputConnection(edge_geom->GetOutputPort());
// create the actor for displaying theedges
    edge_actor->SetMapper(edge_mapper.GetPointer());
    edge_actor->GetProperty()->SetColor(1,1,1);
    edge_actor->GetProperty()->SetOpacity(0.25);


    _ren->AddActor(vertex_actor.GetPointer());
    _ren->AddActor(edge_actor.GetPointer());

    _camera = _ren->GetActiveCamera();

    _renWin->AddRenderer(_ren.GetPointer());
}

void VTKView::zoom(double factor) {
    if (_graph->GetNumberOfVertices() > 0) {
        _camera->Zoom(factor);
        _renWin->Render();
    }
}

void VTKView::move(double x, double y) {
    if (_graph->GetNumberOfVertices() > 0) {
        double lastScale = 2.0 * _camera->GetParallelScale() / _ren->GetSize()[1];
        double lastFocalPt[] = {0, 0, 0};
        _camera->GetFocalPoint(lastFocalPt);
        double lastPos[] = {0, 0, 0};
        _camera->GetPosition(lastPos);

        double delta[] = {0, 0, 0};
        delta[0] = -lastScale * (x);
        delta[1] = -lastScale * (y);
        delta[2] = 0;
        _camera->SetFocalPoint(lastFocalPt[0] + delta[0], lastFocalPt[1] + delta[1], lastFocalPt[2] + delta[2]);
        _camera->SetPosition(lastPos[0] + delta[0], lastPos[1] + delta[1], lastPos[2] + delta[2]);

        _renWin->Render();
    }
}


void VTKView::displayAgentInfo(const Agent *agent) {
    std::vector<NEAT::NNode*> &nodes = agent->brain->_net->all_nodes;


    _layout->SetLayoutStrategy(_strategy.GetPointer());
    _graph->Initialize();
    _graph->SetNumberOfVertices(nodes.size());
    _graph->GetVertexData()->Initialize();

    vtkNew<vtkIntArray> nodeTypeArray;
    nodeTypeArray->SetNumberOfComponents(1);
    nodeTypeArray->SetNumberOfTuples(nodes.size());
    nodeTypeArray->SetName("nodeType");

    for (NEAT::NNode* node: nodes) {
        nodeTypeArray->SetComponent(node->node_id - 1, 0, node->gen_node_label);

        for (auto link: node->incoming) {
            _graph->AddEdge(link->in_node->node_id - 1, node->node_id - 1);
        }
    }

    _graph->GetVertexData()->AddArray(nodeTypeArray.GetPointer());
    _layout->SetInputData(_graph.GetPointer());
    _strategy->SetGraph(_graph.GetPointer());
    _strategy->Initialize();
    _layout->Update();
    _ren->ResetCamera();

    //ren2->Render();
    _renWin->Render();
}