//
// Created by barth on 9/8/18.
//

#include "vtkRenderWindowInteractor.h"
#include "vtkVariantArray.h"
#include "vtkLongArray.h"
#include "vtkIntArray.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkContextScene.h"
#include "vtkChartXY.h"
#include "vtkPlotArea.h"
#include "VTKSpeciesView.h"

using namespace NEAT;

VTKSpeciesView::VTKSpeciesView() :
    _xCounter(0)
{
    _view->GetRenderWindow()->SetSize(800, 400);
    _view->Render();
}

void VTKSpeciesView::addSpeciesData(std::vector<Species*> all_species)
{
    vtkNew<vtkChartXY> chart;
    int currentHeight = 0;

    _view->GetScene()->ClearItems();

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
                _data.insert(std::pair<int, vtkSmartPointer<vtkTable>>(id, table));

                color.resize(3);
                color[0] = rand() % 255;
                color[1] = rand() % 255;
                color[2] = rand() % 255;


                _colors.insert(std::pair<const int, std::vector<unsigned char>>(id, color));
            }
            else {
                table = _data.at(id);
                color = _colors.at(id);
            }
            if (species->getNumberOfAgents() > 0) {
                int nextHeight = currentHeight + species->getNumberOfAgents();

                //cout << "currentHeight : " << currentHeight << " nextHeight : " << nextHeight << endl;
                vtkNew<vtkVariantArray> row;
                vtkVariant buf[3] = {vtkVariant(_xCounter), vtkVariant(currentHeight), vtkVariant(nextHeight)};
                row->SetArray(buf, 3, 1);
                cout << "x : " << row->GetPointer(0)->ToLong() << " yMin : " << row->GetPointer(1)->ToInt() << " yMax : " << row->GetPointer(2)->ToInt() << endl;
                table->InsertNextRow(row.GetPointer());

                currentHeight = nextHeight;
            }
            vtkNew<vtkPlotArea> area;
            area->SetInputData(table.GetPointer());
            area->SetInputArray(0, "x");
            area->SetInputArray(1, "yMinArray");
            area->SetInputArray(2, "yMaxArray");
            area->SetColor(color[0], color[1], color[2], 255);
            chart->AddPlot(area.GetPointer());
            chart->RecalculateBounds();
        }
    }

    ++_xCounter;
    std::cout << "xCounter : " << _xCounter << endl;

    _view->GetRenderWindow()->SetMultiSamples(0);
    _view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);


    _view->GetScene()->AddItem(chart.GetPointer());
    _view->Render();

}

void VTKSpeciesView::startInteraction()
{
    _view->GetInteractor()->Start();
}