//
// Created by barth on 9/8/18.
//

#ifndef SCRIPTBOTS_VTKSPECIESVIEW_H
#define SCRIPTBOTS_VTKSPECIESVIEW_H

#include <map>
#include <vector>
#include "vtkNew.h"
#include "vtkSmartPointer.h"
#include "vtkContextView.h"
#include "vtkTable.h"
#include "NEAT/Species.h"

class VTKSpeciesView {
    vtkNew<vtkContextView> _view;
    std::map<int, vtkSmartPointer<vtkTable>> _data;
    std::map<int, std::vector<unsigned char>> _colors;
    long int _xCounter;

public:
    VTKSpeciesView();
    VTKSpeciesView(std::ifstream &inFile);

    void addSpeciesData(std::vector<NEAT::Species*> species);
    void startInteraction();
    void renderData();
    void saveToFile(std::ofstream &outFile);
};

extern VTKSpeciesView *VTKSPECIESVIEW;

#endif //SCRIPTBOTS_VTKSPECIESVIEW_H
