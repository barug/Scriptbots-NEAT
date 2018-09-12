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
#include "vtkTableWriter.h"
#include "vtkTableReader.h"
#include "VTKSpeciesView.h"

using namespace NEAT;

VTKSpeciesView::VTKSpeciesView() :
    _xCounter(0)
{
    _view->GetRenderWindow()->SetSize(800, 400);
    _view->Render();
}

VTKSpeciesView::VTKSpeciesView(std::ifstream &inFile)
{
    vtkNew<vtkTableReader> reader;
    std::string wordBuff;
    char curline[4096];

    inFile >> wordBuff;
    if (wordBuff != "speciesViewBegin")
        throw std::runtime_error("bad format : speciesViewBegin");

    inFile >> _xCounter;

    int nbrOfTable;
    inFile >> nbrOfTable;

    reader->ReadFromInputStringOn();

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

        _colors.insert(std::pair<int, std::vector<unsigned char>>(id, color));

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

        cout << "tableSize : " << tableSize << endl;

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
            cout << "x : " << row->GetPointer(0)->ToLong() << " yMin : " << row->GetPointer(1)->ToInt() << " yMax : " << row->GetPointer(2)->ToInt() << endl;
            table->InsertNextRow(row.GetPointer());
        }

        _data.insert(std::pair<int, vtkSmartPointer<vtkTable>>(id, table));

        /*int tabletextSize;
        inFile >> tabletextSize;

        inFile.read(curline, tabletextSize);
        curline[tabletextSize] = 0;
        //cout << curline;
        reader->SetInputString(curline);
        reader->Update();

        vtkSmartPointer<vtkTable> newTable(reader->GetOutput());
        _data.insert(std::pair<int, vtkSmartPointer<vtkTable>>(id, newTable));
        newTable->Print(cout);*/
    }

    cout << "nbrofTable : " << nbrOfTable << " _data size : " << _data.size() << " colors size : " << _colors.size() << endl;

    inFile >> wordBuff;
    cout << "|" << wordBuff << "|" << endl;
    if (wordBuff != "speciesViewEnd")
        throw std::runtime_error("bad format : speciesViewEnd");
    cout << "finished loading species view"<< endl;

    _view->GetRenderWindow()->SetSize(800, 400);
    renderData();
}



void VTKSpeciesView::addSpeciesData(std::vector<Species*> all_species)
{
    //vtkNew<vtkChartXY> chart;
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
                //cout << "x : " << row->GetPointer(0)->ToLong() << " yMin : " << row->GetPointer(1)->ToInt() << " yMax : " << row->GetPointer(2)->ToInt() << endl;
                table->InsertNextRow(row.GetPointer());

                currentHeight = nextHeight;
            }
  /*          vtkNew<vtkPlotArea> area;
            area->SetInputData(table.GetPointer());
            area->SetInputArray(0, "x");
            area->SetInputArray(1, "yMinArray");
            area->SetInputArray(2, "yMaxArray");
            area->SetColor(color[0], color[1], color[2], 255);
            chart->AddPlot(area.GetPointer());
            chart->RecalculateBounds();
  */      }
    }

    ++_xCounter;
    //std::cout << "xCounter : " << _xCounter << endl;

    renderData();

    /*_view->GetRenderWindow()->SetMultiSamples(0);
    _view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);


    _view->GetScene()->AddItem(chart.GetPointer());
    _view->Render();
*/
}

void VTKSpeciesView::renderData()
{
    vtkNew<vtkChartXY> chart;

    _view->GetScene()->ClearItems();

    for (auto speciesTable: _data) {
        int id = speciesTable.first;
        vtkSmartPointer<vtkTable> table = speciesTable.second;
        std::vector<unsigned char> color = _colors.at(id);

        vtkNew<vtkPlotArea> area;
        area->SetInputData(table.GetPointer());
        area->SetInputArray(0, "x");
        area->SetInputArray(1, "yMinArray");
        area->SetInputArray(2, "yMaxArray");
        area->SetColor(color[0], color[1], color[2], 255);
        chart->AddPlot(area.GetPointer());
        chart->RecalculateBounds();
    }

    _view->GetRenderWindow()->SetMultiSamples(0);
    _view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);


    _view->GetScene()->AddItem(chart.GetPointer());
    _view->Render();
}

void VTKSpeciesView::startInteraction()
{
    _view->GetInteractor()->Start();
}

void VTKSpeciesView::saveToFile(std::ofstream &outFile)
{
    cout << "speciesViewBegin" << endl;
    outFile << "speciesViewBegin" << std::endl;
    outFile << _xCounter << " ";
    outFile << _data.size() << endl;
    for (auto pair: _data) {
        vtkNew<vtkTableWriter> writer;
        writer->WriteToOutputStringOn();

        std::vector<unsigned char> color = _colors.at(pair.first);
        outFile << pair.first << " ";
        outFile << std::to_string(color[0]) << " " << std::to_string(color[1]) << " " << std::to_string(color[2]) << std::endl;
        writer->SetInputData(pair.second.GetPointer());

        vtkSmartPointer<vtkTable> table = pair.second;

        outFile << table->GetNumberOfRows() << endl;

        cout << table->GetNumberOfRows() << endl;

        for (int i = 0; i < table->GetNumberOfRows(); ++i) {
            vtkVariantArray *row = table->GetRow(i);
            outFile << row->GetPointer(0)->ToLong() << " " << row->GetPointer(1)->ToInt() << " " << row->GetPointer(2)->ToInt() << " ";
            cout << row->GetPointer(0)->ToLong() << " " << row->GetPointer(1)->ToInt() << " " << row->GetPointer(2)->ToInt()<< " ";
        }
        outFile << endl;

        /*writer->Write();
        char *outStr = writer->GetOutputString();
        //outStr[writer->GetOutputStringLength()] = 0;
        outFile << writer->GetOutputStringLength() << endl;
        outFile << outStr;*/
    }
    cout << "speciesViewEnd" << endl;
    outFile << "speciesViewEnd" << std::endl;
}