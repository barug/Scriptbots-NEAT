//
// Created by barth on 9/3/18.
//

#include "VTKPlotView.h"
#include <vtkVariantArray.h>

VTKPlotView::VTKPlotView()
{
    vtkNew<vtkIntArray> index;
    vtkNew<vtkIntArray> numHerb;
    vtkNew<vtkIntArray> numCarn;
    numHerb->SetName("numHerb");
    numCarn->SetName("numCarn");
    index->SetName("x");
    _data->AddColumn(index.GetPointer());
    _data->AddColumn(numHerb.GetPointer());
    _data->AddColumn(numCarn.GetPointer());

    _view->GetRenderWindow()->SetSize(800, 400);
    _view->Render();
}

void VTKPlotView::addDataRow(int numHerb, int numCarn)
{
    _view->GetScene()->ClearItems();


    vtkNew<vtkChartXY> chart;
    vtkNew<vtkPlotLine> lineHerb;
    vtkNew<vtkPlotLine> lineCarn;

    vtkNew<vtkVariantArray> row;
    vtkVariant buf[3] = {vtkVariant(_data->GetNumberOfRows()), vtkVariant(numHerb), vtkVariant(numCarn)};
    row->SetArray(buf, 3, 1);
    _data->InsertNextRow(row.GetPointer());

    /*std::cout << "nRows : " << _data->GetNumberOfRows() << std::endl;
    for (int i = 0; i < _data->GetNumberOfRows(); ++i) {
        vtkVariantArray *arr = _data->GetRow(i);
        std::cout << arr->GetPointer(0)->ToInt()<< "  " << arr->GetPointer(1)->ToInt() << std::endl;
    }*/
    lineHerb->SetInputData(_data.GetPointer(), 0, 1);
    lineHerb->SetColor(0, 255, 0 , 255);
    lineHerb->SetWidth(1.0);
    lineCarn->SetInputData(_data.GetPointer(), 0, 2);
    lineCarn->SetColor(255, 0, 0, 255);
    lineCarn->SetWidth(1.0);
    chart->AddPlot(lineHerb.GetPointer());
    chart->AddPlot(lineCarn.GetPointer());



    // Render the scene
    _view->GetRenderWindow()->SetMultiSamples(0);
    _view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);


    _view->GetScene()->AddItem(chart.GetPointer());
    _view->Render();
}

void VTKPlotView::startInteraction()
{
    _view->GetInteractor()->Start();
}