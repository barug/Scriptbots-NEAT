//
// VTKDashboard.cpp - Unified VTK visualization dashboard
//

#include "VTKDashboard.h"

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkGraphToGlyphs.h>
#include <vtkGraphToPolyData.h>
#include <vtkEdgeLayout.h>
#include <vtkArcParallelEdgeStrategy.h>
#include <vtkProperty.h>
#include <vtkIntArray.h>
#include <vtkDataSetAttributes.h>
#include <vtkVariantArray.h>
#include <vtkAxis.h>
#include <vtkTextProperty.h>

#include "NEAT/nnode.h"

using namespace std;

VTKDashboard::VTKDashboard()
    : isVisible_(false)
    , graphCamera_(nullptr)
    , speciesXCounter_(0)
{
    setupWindow();
    setupGraphView();
    setupPopulationPlot();
    setupSpeciesView();
}

VTKDashboard::VTKDashboard(std::ifstream &inFile)
    : isVisible_(false)
    , graphCamera_(nullptr)
    , speciesXCounter_(0)
{
    setupWindow();
    setupGraphView();
    setupPopulationPlot();
    setupSpeciesView();

    // Load species data from file
    std::string wordBuff;

    inFile >> wordBuff;
    if (wordBuff != "speciesViewBegin")
        throw std::runtime_error("bad format : speciesViewBegin");

    inFile >> speciesXCounter_;

    int nbrOfTable;
    inFile >> nbrOfTable;

    for (int i = 0; i < nbrOfTable; ++i) {
        int id;
        std::vector<unsigned char> color;
        color.resize(3);

        int buff;

        inFile >> id;
        inFile >> buff;
        color[0] = (unsigned char) buff;
        inFile >> buff;
        color[1] = (unsigned char) buff;
        inFile >> buff;
        color[2] = (unsigned char) buff;

        speciesColors_.insert(std::pair<int, std::vector<unsigned char>>(id, color));

        int tableSize;
        inFile >> tableSize;

        vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();
        vtkNew<vtkLongArray> xArray;
        vtkNew<vtkIntArray> yMinArray;
        vtkNew<vtkIntArray> yMaxArray;
        xArray->SetName("x");
        yMinArray->SetName("yMinArray");
        yMaxArray->SetName("yMaxArray");
        table->AddColumn(xArray.GetPointer());
        table->AddColumn(yMinArray.GetPointer());
        table->AddColumn(yMaxArray.GetPointer());

        for (int j = 0; j < tableSize; ++j) {
            long int x;
            int yMin;
            int yMax;

            inFile >> x;
            inFile >> yMin;
            inFile >> yMax;

            vtkNew<vtkVariantArray> row;
            vtkVariant buf[3] = {vtkVariant(x), vtkVariant(yMin), vtkVariant(yMax)};
            row->SetArray(buf, 3, 1);
            table->InsertNextRow(row.GetPointer());
        }

        speciesData_.insert(std::pair<int, vtkSmartPointer<vtkTable>>(id, table));
    }

    inFile >> wordBuff;
    if (wordBuff != "speciesViewEnd")
        throw std::runtime_error("bad format : speciesViewEnd");

    cout << "Finished loading VTK dashboard" << endl;
}

VTKDashboard::~VTKDashboard() {
}

void VTKDashboard::setupWindow() {
    renderWindow_->SetSize(1200, 800);
    renderWindow_->SetWindowName("Scriptbots Dashboard");
    renderWindow_->SetShowWindow(false);  // Start hidden

    interactor_->SetRenderWindow(renderWindow_.GetPointer());
}

void VTKDashboard::setupGraphView() {
    // Top-left viewport (0.0-0.5 x, 0.5-1.0 y)
    graphRenderer_->SetViewport(0.0, 0.5, 0.5, 1.0);
    graphRenderer_->SetBackground(0.1, 0.1, 0.2);
    renderWindow_->AddRenderer(graphRenderer_.GetPointer());

    // Setup graph layout strategy
    graphStrategy_->SetVertexAttribute("nodeType");
    graphLayout_->SetLayoutStrategy(graphStrategy_.GetPointer());
    graphLayout_->SetInputData(graph_.GetPointer());

    graphCamera_ = graphRenderer_->GetActiveCamera();
}

void VTKDashboard::setupPopulationPlot() {
    // Top-right viewport (0.5-1.0 x, 0.5-1.0 y)
    popRenderer_->SetViewport(0.5, 0.5, 1.0, 1.0);
    popRenderer_->SetBackground(1.0, 1.0, 1.0);
    renderWindow_->AddRenderer(popRenderer_.GetPointer());

    // Setup data table
    vtkNew<vtkIntArray> index;
    vtkNew<vtkIntArray> numHerb;
    vtkNew<vtkIntArray> numCarn;
    index->SetName("x");
    numHerb->SetName("numHerb");
    numCarn->SetName("numCarn");
    popData_->AddColumn(index.GetPointer());
    popData_->AddColumn(numHerb.GetPointer());
    popData_->AddColumn(numCarn.GetPointer());

    // Setup context scene for 2D chart
    popScene_->AddItem(popChart_.GetPointer());
    popContextActor_->SetScene(popScene_.GetPointer());
    popRenderer_->AddActor(popContextActor_.GetPointer());

    // Configure chart
    popChart_->SetTitle("Population");
    popChart_->GetAxis(vtkAxis::LEFT)->SetTitle("Count");
    popChart_->GetAxis(vtkAxis::BOTTOM)->SetTitle("Time");
}

void VTKDashboard::setupSpeciesView() {
    // Bottom viewport (0.0-1.0 x, 0.0-0.5 y)
    speciesRenderer_->SetViewport(0.0, 0.0, 1.0, 0.5);
    speciesRenderer_->SetBackground(1.0, 1.0, 1.0);
    renderWindow_->AddRenderer(speciesRenderer_.GetPointer());

    // Setup context scene for 2D chart
    speciesScene_->AddItem(speciesChart_.GetPointer());
    speciesContextActor_->SetScene(speciesScene_.GetPointer());
    speciesRenderer_->AddActor(speciesContextActor_.GetPointer());

    // Configure chart
    speciesChart_->SetTitle("Species Distribution");
    speciesChart_->GetAxis(vtkAxis::LEFT)->SetTitle("Population");
    speciesChart_->GetAxis(vtkAxis::BOTTOM)->SetTitle("Time");
}

void VTKDashboard::displayAgentInfo(const Agent *agent) {
    if (!agent || !agent->brain || !agent->brain->_net) return;

    std::vector<NEAT::NNode*> &nodes = agent->brain->_net->all_nodes;

    graphLayout_->SetLayoutStrategy(graphStrategy_.GetPointer());
    graph_->Initialize();
    graph_->SetNumberOfVertices(nodes.size());
    graph_->GetVertexData()->Initialize();

    vtkNew<vtkIntArray> nodeTypeArray;
    nodeTypeArray->SetNumberOfComponents(1);
    nodeTypeArray->SetNumberOfTuples(nodes.size());
    nodeTypeArray->SetName("nodeType");

    for (NEAT::NNode* node: nodes) {
        nodeTypeArray->SetComponent(node->node_id - 1, 0, node->gen_node_label);

        for (auto link: node->incoming) {
            graph_->AddEdge(link->in_node->node_id - 1, node->node_id - 1);
        }
    }

    graph_->GetVertexData()->AddArray(nodeTypeArray.GetPointer());
    graphLayout_->SetInputData(graph_.GetPointer());
    graphStrategy_->SetGraph(graph_.GetPointer());
    graphStrategy_->Initialize();
    graphLayout_->Update();

    // Rebuild visualization pipeline
    graphRenderer_->RemoveAllViewProps();

    vtkNew<vtkGraphToGlyphs> vertex_glyphs;
    vertex_glyphs->SetInputConnection(graphLayout_->GetOutputPort());
    vertex_glyphs->SetGlyphType(7);
    vertex_glyphs->FilledOn();
    vertex_glyphs->SetRenderer(graphRenderer_.GetPointer());

    vtkNew<vtkPolyDataMapper> vertex_mapper;
    vertex_mapper->SetInputConnection(vertex_glyphs->GetOutputPort());
    vertex_mapper->SetScalarRange(0, 3);
    vertex_mapper->SetScalarModeToUsePointFieldData();
    vertex_mapper->SelectColorArray("nodeType");

    vtkNew<vtkActor> vertex_actor;
    vertex_actor->SetMapper(vertex_mapper.GetPointer());

    vtkNew<vtkArcParallelEdgeStrategy> edge_strategy;
    vtkNew<vtkEdgeLayout> edge_layout;
    edge_layout->SetLayoutStrategy(edge_strategy.GetPointer());
    edge_layout->SetInputConnection(graphLayout_->GetOutputPort());

    vtkNew<vtkGraphToPolyData> edge_geom;
    edge_geom->SetInputConnection(edge_layout->GetOutputPort());

    vtkNew<vtkPolyDataMapper> edge_mapper;
    edge_mapper->SetInputConnection(edge_geom->GetOutputPort());

    vtkNew<vtkActor> edge_actor;
    edge_actor->SetMapper(edge_mapper.GetPointer());
    edge_actor->GetProperty()->SetColor(1, 1, 1);
    edge_actor->GetProperty()->SetOpacity(0.25);

    graphRenderer_->AddActor(vertex_actor.GetPointer());
    graphRenderer_->AddActor(edge_actor.GetPointer());
    graphRenderer_->ResetCamera();

    if (isVisible_) {
        renderWindow_->Render();
    }
}

void VTKDashboard::zoomGraph(double factor) {
    if (graph_->GetNumberOfVertices() > 0 && graphCamera_) {
        graphCamera_->Zoom(factor);
        if (isVisible_) {
            renderWindow_->Render();
        }
    }
}

void VTKDashboard::moveGraph(double x, double y) {
    if (graph_->GetNumberOfVertices() > 0 && graphCamera_) {
        double lastScale = 2.0 * graphCamera_->GetParallelScale() / graphRenderer_->GetSize()[1];
        double lastFocalPt[] = {0, 0, 0};
        graphCamera_->GetFocalPoint(lastFocalPt);
        double lastPos[] = {0, 0, 0};
        graphCamera_->GetPosition(lastPos);

        double delta[] = {-lastScale * x, -lastScale * y, 0};
        graphCamera_->SetFocalPoint(lastFocalPt[0] + delta[0], lastFocalPt[1] + delta[1], lastFocalPt[2] + delta[2]);
        graphCamera_->SetPosition(lastPos[0] + delta[0], lastPos[1] + delta[1], lastPos[2] + delta[2]);

        if (isVisible_) {
            renderWindow_->Render();
        }
    }
}

void VTKDashboard::addPopulationData(int numHerb, int numCarn) {
    // Clear and rebuild chart
    popChart_->ClearPlots();

    vtkNew<vtkVariantArray> row;
    vtkVariant buf[3] = {vtkVariant((int)popData_->GetNumberOfRows()), vtkVariant(numHerb), vtkVariant(numCarn)};
    row->SetArray(buf, 3, 1);
    popData_->InsertNextRow(row.GetPointer());

    vtkNew<vtkPlotLine> lineHerb;
    lineHerb->SetInputData(popData_.GetPointer(), 0, 1);
    lineHerb->SetColor(0, 255, 0, 255);
    lineHerb->SetWidth(2.0);

    vtkNew<vtkPlotLine> lineCarn;
    lineCarn->SetInputData(popData_.GetPointer(), 0, 2);
    lineCarn->SetColor(255, 0, 0, 255);
    lineCarn->SetWidth(2.0);

    popChart_->AddPlot(lineHerb.GetPointer());
    popChart_->AddPlot(lineCarn.GetPointer());

    if (isVisible_) {
        renderWindow_->Render();
    }
}

void VTKDashboard::addSpeciesData(std::vector<NEAT::Species*> all_species) {
    int currentHeight = 0;

    for (auto species: all_species) {
        if (species->isDrawn() || species->getNumberOfAgents() >= 2) {
            vtkSmartPointer<vtkTable> table;
            std::vector<unsigned char> color;
            int id = species->getId();

            if (!species->isDrawn()) {
                species->setDrawn(true);
                table = vtkSmartPointer<vtkTable>::New();
                vtkNew<vtkLongArray> xArray;
                vtkNew<vtkIntArray> yMinArray;
                vtkNew<vtkIntArray> yMaxArray;
                xArray->SetName("x");
                yMinArray->SetName("yMinArray");
                yMaxArray->SetName("yMaxArray");
                table->AddColumn(xArray.GetPointer());
                table->AddColumn(yMinArray.GetPointer());
                table->AddColumn(yMaxArray.GetPointer());
                speciesData_.insert(std::pair<int, vtkSmartPointer<vtkTable>>(id, table));

                color.resize(3);
                color[0] = rand() % 255;
                color[1] = rand() % 255;
                color[2] = rand() % 255;
                speciesColors_.insert(std::pair<const int, std::vector<unsigned char>>(id, color));
            } else {
                table = speciesData_.at(id);
                color = speciesColors_.at(id);
            }

            if (species->getNumberOfAgents() > 0) {
                int nextHeight = currentHeight + species->getNumberOfAgents();

                vtkNew<vtkVariantArray> row;
                vtkVariant buf[3] = {vtkVariant(speciesXCounter_), vtkVariant(currentHeight), vtkVariant(nextHeight)};
                row->SetArray(buf, 3, 1);
                table->InsertNextRow(row.GetPointer());

                currentHeight = nextHeight;
            }
        }
    }

    ++speciesXCounter_;
    renderSpeciesChart();
}

void VTKDashboard::renderSpeciesChart() {
    speciesChart_->ClearPlots();

    for (auto& speciesTable: speciesData_) {
        int id = speciesTable.first;
        vtkSmartPointer<vtkTable> table = speciesTable.second;
        std::vector<unsigned char> color = speciesColors_.at(id);

        vtkNew<vtkPlotArea> area;
        area->SetInputData(table.GetPointer());
        area->SetInputArray(0, "x");
        area->SetInputArray(1, "yMinArray");
        area->SetInputArray(2, "yMaxArray");
        area->SetColor(color[0], color[1], color[2], 255);
        speciesChart_->AddPlot(area.GetPointer());
    }

    speciesChart_->RecalculateBounds();

    if (isVisible_) {
        renderWindow_->Render();
    }
}

void VTKDashboard::show() {
    isVisible_ = true;
    renderWindow_->SetShowWindow(true);
    renderWindow_->Render();
}

void VTKDashboard::hide() {
    isVisible_ = false;
    renderWindow_->SetShowWindow(false);
}

void VTKDashboard::render() {
    if (isVisible_) {
        renderWindow_->Render();
    }
}

void VTKDashboard::startInteraction() {
    show();
    interactor_->Start();
}

void VTKDashboard::saveToFile(std::ofstream &outFile) {
    outFile << "speciesViewBegin" << std::endl;
    outFile << speciesXCounter_ << " ";
    outFile << speciesData_.size() << endl;

    for (auto& pair: speciesData_) {
        std::vector<unsigned char> color = speciesColors_.at(pair.first);
        outFile << pair.first << " ";
        outFile << std::to_string(color[0]) << " " << std::to_string(color[1]) << " " << std::to_string(color[2]) << std::endl;

        vtkSmartPointer<vtkTable> table = pair.second;
        outFile << table->GetNumberOfRows() << endl;

        for (int i = 0; i < table->GetNumberOfRows(); ++i) {
            vtkVariantArray *row = table->GetRow(i);
            outFile << row->GetPointer(0)->ToLong() << " " << row->GetPointer(1)->ToInt() << " " << row->GetPointer(2)->ToInt() << " ";
        }
        outFile << endl;
    }

    outFile << "speciesViewEnd" << std::endl;
}
