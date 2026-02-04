#include "SimulationContext.h"
#include "World.h"
#include "GLView.h"

#ifdef HAVE_VTK
#include "VTKView.h"
#include "VTKPlotView.h"
#include "VTKSpeciesView.h"
#endif

SimulationContext& SimulationContext::instance() {
    static SimulationContext instance;
    return instance;
}

SimulationContext::SimulationContext()
    : initialized_(false)
    , world_(nullptr)
    , glView_(nullptr)
#ifdef HAVE_VTK
    , vtkView_(nullptr)
    , vtkPlotView_(nullptr)
    , vtkSpeciesView_(nullptr)
#endif
{
}

SimulationContext::~SimulationContext() {
    shutdown();
}

void SimulationContext::initialize(World* world) {
    if (initialized_) {
        return;  // Already initialized
    }
    
    world_ = world;
    
    // Create GLView (it needs to exist for GLUT callbacks)
    glView_ = new GLView(world_);
    
#ifdef HAVE_VTK
    vtkView_ = new VTKView();
    vtkPlotView_ = new VTKPlotView();
    // Note: vtkSpeciesView_ is created by World during construction
    // because it may need to load from a save file
#endif
    
    initialized_ = true;
}

void SimulationContext::shutdown() {
    if (!initialized_) {
        return;
    }
    
    // Note: GLView and World are managed elsewhere (main.cpp cleanup)
    // We don't delete them here to avoid double-free issues
    // In a more thorough refactor, SimulationContext would own these objects
    
    initialized_ = false;
}
