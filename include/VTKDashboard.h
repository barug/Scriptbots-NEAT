//
// VTKDashboard.h - Unified VTK visualization dashboard
// Consolidates neural network view, population plot, and species view
//

#ifndef SCRIPTBOTS_VTKDASHBOARD_H
#define SCRIPTBOTS_VTKDASHBOARD_H

#include <map>
#include <vector>
#include <fstream>

#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>

// For neural network graph
#include <vtkMutableDirectedGraph.h>
#include <vtkGraphLayout.h>
#include <vtkAttributeClustering2DLayoutStrategy.h>

// For 2D charts
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkContextActor.h>
#include <vtkChartXY.h>
#include <vtkPlotLine.h>
#include <vtkPlotArea.h>
#include <vtkTable.h>
#include <vtkIntArray.h>
#include <vtkLongArray.h>

#include "Agent.h"
#include "NEAT/Species.h"

class VTKDashboard {
public:
    VTKDashboard();
    VTKDashboard(std::ifstream &inFile);
    ~VTKDashboard();

    // Neural network view (from VTKView)
    void displayAgentInfo(const Agent *agent);
    void zoomGraph(double factor);
    void moveGraph(double x, double y);

    // Population plot (from VTKPlotView)
    void addPopulationData(int numHerb, int numCarn);

    // Species view (from VTKSpeciesView)
    void addSpeciesData(std::vector<NEAT::Species*> all_species);

    // Window control
    void show();
    void hide();
    void render();
    void startInteraction();
    void setWindowGeometry(int x, int y, int width, int height);

    // Serialization
    void saveToFile(std::ofstream &outFile);

private:
    void setupWindow();
    void setupGraphView();
    void setupPopulationPlot();
    void setupSpeciesView();
    void renderSpeciesChart();

    // Main window
    vtkNew<vtkRenderWindow> renderWindow_;
    vtkNew<vtkRenderWindowInteractor> interactor_;
    bool isVisible_;

    // Neural network graph (top-left viewport)
    vtkNew<vtkRenderer> graphRenderer_;
    vtkNew<vtkMutableDirectedGraph> graph_;
    vtkNew<vtkGraphLayout> graphLayout_;
    vtkNew<vtkAttributeClustering2DLayoutStrategy> graphStrategy_;
    vtkCamera* graphCamera_;

    // Population plot (top-right viewport)
    vtkNew<vtkRenderer> popRenderer_;
    vtkNew<vtkContextActor> popContextActor_;
    vtkNew<vtkContextScene> popScene_;
    vtkNew<vtkChartXY> popChart_;
    vtkNew<vtkTable> popData_;

    // Species view (bottom viewport)
    vtkNew<vtkRenderer> speciesRenderer_;
    vtkNew<vtkContextActor> speciesContextActor_;
    vtkNew<vtkContextScene> speciesScene_;
    vtkNew<vtkChartXY> speciesChart_;
    std::map<int, vtkSmartPointer<vtkTable>> speciesData_;
    std::map<int, std::vector<unsigned char>> speciesColors_;
    long int speciesXCounter_;
    
    // Stored window geometry (for applying after window is shown)
    int windowX_, windowY_, windowW_, windowH_;
};

#endif //SCRIPTBOTS_VTKDASHBOARD_H
