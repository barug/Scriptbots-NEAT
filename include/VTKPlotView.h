//
// Created by barth on 9/3/18.
//

#ifndef SCRIPTBOTS_VTKPLOTVIEW_H
#define SCRIPTBOTS_VTKPLOTVIEW_H

#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include "vtkChartXY.h"
#include "vtkStatisticsAlgorithm.h"
#include "vtkComputeQuartiles.h"
#include "vtkContextScene.h"
#include "vtkContextView.h"
#include "vtkIntArray.h"
#include "vtkLookupTable.h"
#include "vtkPlotBox.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkStringArray.h"
#include "vtkTable.h"
#include "vtkAxis.h"
#include "vtkTextProperty.h"
#include <vtkPlotLine.h>

class VTKPlotView;

extern VTKPlotView *VTKPLOTVIEW;

class VTKPlotView {
    vtkNew<vtkContextView> _view;
    vtkNew<vtkTable> _data;


public:
    VTKPlotView();
    VTKPlotView(std::ifstream &inFile);
    void addDataRow(int numHerb, int numCarn);
    void startInteraction();
    void saveToFile(std::ofstream &outFile);
};


#endif //SCRIPTBOTS_VTKPLOTVIEW_H
