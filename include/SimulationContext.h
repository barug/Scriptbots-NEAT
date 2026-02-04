#ifndef SIMULATION_CONTEXT_H
#define SIMULATION_CONTEXT_H

#include "config.h"

// Forward declarations
class World;
class GLView;

#ifdef HAVE_VTK
class VTKDashboard;
#endif

/**
 * SimulationContext singleton that manages the shared state of the simulation.
 * 
 * This centralizes what were previously scattered global variables,
 * providing a single point of access for GLUT callbacks and other
 * components that need shared state.
 * 
 * Manages:
 *   - World: the simulation world
 *   - GLView: the OpenGL view for rendering
 *   - VTK views (when VTK is enabled): additional visualization views
 * 
 * Usage:
 *   SimulationContext& ctx = SimulationContext::instance();
 *   ctx.glView()->renderScene();
 */
class SimulationContext {
public:
    /**
     * Get the singleton instance.
     */
    static SimulationContext& instance();
    
    // Delete copy/move constructors and assignment operators
    SimulationContext(const SimulationContext&) = delete;
    SimulationContext& operator=(const SimulationContext&) = delete;
    SimulationContext(SimulationContext&&) = delete;
    SimulationContext& operator=(SimulationContext&&) = delete;
    
    /**
     * Initialize the application with a world.
     * Must be called before using other methods.
     */
    void initialize(World* world);
    
    /**
     * Clean up resources.
     */
    void shutdown();
    
    // Accessors for main components
    World* world() { return world_; }
    const World* world() const { return world_; }
    
    GLView* glView() { return glView_; }
    const GLView* glView() const { return glView_; }
    
#ifdef HAVE_VTK
    VTKDashboard* vtkDashboard() { return vtkDashboard_; }
    const VTKDashboard* vtkDashboard() const { return vtkDashboard_; }
    
    void setVtkDashboard(VTKDashboard* dashboard) { vtkDashboard_ = dashboard; }
    
    bool hasVtk() const { return true; }
#else
    bool hasVtk() const { return false; }
#endif
    
    /**
     * Check if application has been initialized.
     */
    bool isInitialized() const { return initialized_; }
    
private:
    SimulationContext();
    ~SimulationContext();
    
    bool initialized_;
    World* world_;
    GLView* glView_;
    
#ifdef HAVE_VTK
    VTKDashboard* vtkDashboard_;
#endif
};

// Convenience macro for accessing the simulation context
#define Sim SimulationContext::instance()

#endif // SIMULATION_CONTEXT_H
