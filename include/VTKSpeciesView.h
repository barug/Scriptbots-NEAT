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
    void addSpeciesData(std::vector<NEAT::Species*> species);
    void startInteraction();
};

extern VTKSpeciesView *VTKSPECIESVIEW;

#endif //SCRIPTBOTS_VTKSPECIESVIEW_H
