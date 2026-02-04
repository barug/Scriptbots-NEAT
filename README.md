# Scriptbots-NEAT

An artificial life simulation where agents evolve neural networks using the NEAT (NeuroEvolution of Augmenting Topologies) algorithm. Agents learn to survive by eating food, avoiding predators, and reproducing.

## Overview

This project is based on [Scriptbots](https://github.com/karpathy/scriptsbots) originally created by Andrej Karpathy.

## Features Added to Original Scriptbots

The following features were implemented on top of the original Scriptbots simulation:

### NEAT Algorithm Integration
- **Replaced the original brain** with NEAT (NeuroEvolution of Augmenting Topologies)
- Neural network **topology evolves** alongside weights - networks can grow new nodes and connections
- Based on the [NEAT paper](http://nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf) by Stanley & Miikkulainen
- Implemented using a modified version of the [NEAT C++ library](https://github.com/FernandoTorres/NEAT)

### Speciation System
- Agents are **automatically grouped into species** based on genetic compatibility
- Species protect evolutionary innovation by allowing new structures time to optimize
- Compatible agents can mate; incompatible agents cannot
- Species tracking over time for population analysis

### Sexual Reproduction & Mating
- Agents can now **sexually reproduce** when two compatible agents meet
- Mating requires:
  - Physical proximity (`MATING_RADIUS`)
  - Genetic compatibility below threshold (`MATING_COMPATIBILITY_TRESHOLD`)
  - Both agents being healthy enough
- Offspring inherit genes from both parents via crossover
- Configurable parameters: `BODY_COMPAT_MULT`, `BRAIN_COMPAT_MULT`

### Save/Load System
- **Save complete world state** to file (agents, genomes, food, species, innovations)
- **Resume simulations** from saved state
- **Periodic auto-save** option for long-running simulations
- Configuration persistence

### Configuration System
- **External configuration files** to customize simulation parameters
- Load settings from file with `-c` option
- All major simulation parameters are configurable
- Settings are saved/loaded with world state

### VTK Dashboard (Optional)
- **Unified dashboard window** with three visualization panels:
  - **Neural network graph** (top-left): Visualize the brain structure of selected agents
  - **Population plot** (top-right): Herbivore vs carnivore population over time
  - **Species chart** (bottom): Stacked area chart showing species populations
- **Automatic window layout**: Dashboard on left, simulation on right (half-screen each)
- Interactive pan/zoom for network visualization

---

## Original Scriptbots Features

The following features come from the original Scriptbots and its earlier modifications:

- Herbivore/Carnivore dynamics with stomach specialization
- Multi-eye vision system with color and proximity detection
- Sound generation and hearing
- Food sharing between agents
- Blood sensor (detect injured agents)
- Internal clocks for temporal behavior
- Temperature preferences
- Boost ability for speed bursts
- Spike attacks for carnivores

## Dependencies

### Required
- CMake (>= 3.10)
- OpenGL
- GLUT (freeglut on Linux, built-in on macOS)
- C++11 compatible compiler

### Optional
- **VTK** - For advanced visualization features (neural network graphs, species charts)
- **OpenMP** - For parallel processing (improves performance)

## Building

```bash
# Clone the repository
git clone https://github.com/yourusername/Scriptbots-NEAT.git
cd Scriptbots-NEAT

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make -j4

# Run
./scriptbots
```

### Platform-specific Notes

**macOS**: GLUT is provided by the system. OpenGL/GLUT APIs are deprecated but still functional.

**Linux**: Install freeglut development package:
```bash
# Ubuntu/Debian
sudo apt-get install freeglut3-dev

# Fedora
sudo dnf install freeglut-devel
```

**Windows**: GLUT libraries are included in the repository.

### Building with VTK (Optional)

If you have VTK installed, CMake will automatically detect it and enable VTK features:
```bash
cmake -DVTK_DIR=/path/to/vtk ..
make -j4
```

## Usage

### Command Line Options

```bash
./scriptbots [options]

Options:
  -c <config_file>    Load configuration from file (must be first option)
  -l <save_file>      Load world state from file
  -s <save_path>      Set save file path
  -p <period>         Enable periodic auto-save every N epochs
```

### Examples

```bash
# Start a new simulation
./scriptbots

# Load a saved world
./scriptbots -l mysave

# Use custom config and enable auto-save every 10 epochs
./scriptbots -c myconfig.txt -s worldsave -p 10
```

## Controls

### Simulation Window - Keyboard

| Key | Action |
|-----|--------|
| `p` | Pause/Resume simulation |
| `f` | Toggle rendering (faster simulation when off) |
| `g` | Toggle food rendering |
| `+` | Increase simulation speed (skip frames) |
| `-` | Decrease simulation speed |
| `i` | Zoom in |
| `k` | Zoom out |
| `r` | Reset world |
| `m` | Save world to file |
| `c` | Toggle closed environment (no random bot spawning) |
| `x` | Follow selected agent |
| `o` | Follow oldest agent |
| `Esc` | Exit |

### Simulation Window - Mouse

- **Left click**: Select an agent (displays neural network in VTK dashboard)
- **Left drag** or **Right drag**: Pan the view
- **Middle drag**: Zoom in/out

### VTK Dashboard - Neural Network Controls

These keyboard shortcuts control the neural network graph view in the VTK dashboard:

| Key | Action |
|-----|--------|
| `a` | Zoom in (neural network graph) |
| `e` | Zoom out (neural network graph) |
| `z/s/q/d` | Pan neural network graph (up/down/left/right) |

> **Note**: The VTK Dashboard window is display-only. Clicking on it has no effect.

## Configuration

Create a configuration file to customize simulation parameters:

```
WIDTH 8000
HEIGHT 4000
NUMBOTS 70
BOTRADIUS 10
BOTSPEED 0.3
REPRATEH 7
REPRATEC 7
MATING_COMPATIBILITY_TRESHOLD 4.0
FOODMAX 0.5
FOODADDFREQ 15
```

See `include/settings.h` for all available parameters.

## How It Works

### Agent Senses (Inputs)
- 4 eyes with color detection (RGB) and proximity
- Food sensor (ground food at current position)
- Sound and smell sensors
- Health level
- Internal clocks
- Temperature preference
- Blood sensor (detects injured agents)

### Agent Actions (Outputs)
- Left/Right wheel speed (movement)
- RGB color output (for camouflage/signaling)
- Spike attack
- Speed boost
- Sound generation
- Food sharing

### Evolution (NEAT-based)
The evolution system uses NEAT, which differs from traditional neuroevolution:

1. **Topology Evolution**: Networks start minimal and grow over time by adding nodes and connections
2. **Historical Markings**: Each gene has an innovation number to track its origin, enabling meaningful crossover
3. **Speciation**: Agents are grouped by genetic similarity, protecting new innovations
4. **Two Reproduction Modes**:
   - **Sexual**: Two compatible agents mate, combining their genomes
   - **Asexual**: Single agent reproduces with mutations
5. **Mutation Types**:
   - Weight perturbation
   - Add new connection
   - Add new node (splits existing connection)
   - Enable/disable connections

## References

- [Original Scriptbots](https://github.com/karpathy/scriptsbots) by Andrej Karpathy
- [NEAT Paper](http://nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf) by Kenneth O. Stanley and Risto Miikkulainen
- [NEAT C++ Implementation](https://github.com/FernandoTorres/NEAT)
- [VTK - Visualization Toolkit](https://www.vtk.org/)

## License

This project inherits from the original Scriptbots project. Please refer to the original repository for license information.
