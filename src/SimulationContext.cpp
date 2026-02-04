#include "SimulationContext.h"
#include "World.h"
#include "GLView.h"

#ifdef HAVE_VTK
#include "VTKDashboard.h"

// VTK auto-initialization for OpenGL2 rendering backend
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingContextOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)
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
    , vtkDashboard_(nullptr)
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
    // VTKDashboard is created by World during construction
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
