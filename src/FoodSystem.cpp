#include "FoodSystem.h"
#include "View.h"
#include "helpers.h"
#include <algorithm>

FoodSystem::FoodSystem(int worldWidth, int worldHeight, int cellSize)
    : gridWidth_(worldWidth / cellSize),
      gridHeight_(worldHeight / cellSize),
      cellSize_(cellSize),
      lastSpawnX_(0),
      lastSpawnY_(0)
{
    initializeGrid();
}

FoodSystem::FoodSystem(std::ifstream& inFile)
    : lastSpawnX_(0),
      lastSpawnY_(0)
{
    // Read grid dimensions
    inFile >> gridWidth_;
    inFile >> gridHeight_;
    inFile >> lastSpawnX_;
    inFile >> lastSpawnY_;
    
    // Read and verify marker
    std::string wordBuff;
    inFile >> wordBuff;
    if (wordBuff != "foodMapBegin") {
        throw std::runtime_error("bad format: expected foodMapBegin");
    }
    
    // Read the grid
    grid_.resize(gridWidth_);
    for (int x = 0; x < gridWidth_; x++) {
        grid_[x].resize(gridHeight_);
        for (int y = 0; y < gridHeight_; y++) {
            inFile >> grid_[x][y];
        }
    }
    
    // Read end marker
    inFile >> wordBuff;
    if (wordBuff != "foodMapEnd") {
        throw std::runtime_error("bad format: expected foodMapEnd");
    }
    
    // Cell size needs to be inferred or passed separately
    // For now, we'll compute it based on default conf values
    // This is a limitation - ideally cellSize would be saved/loaded too
    cellSize_ = 1; // Will be set properly by World after loading
}

void FoodSystem::initializeGrid()
{
    grid_.resize(gridWidth_);
    for (int x = 0; x < gridWidth_; x++) {
        grid_[x].resize(gridHeight_);
        for (int y = 0; y < gridHeight_; y++) {
            grid_[x][y] = 0.0f;
        }
    }
}

void FoodSystem::spawnFoodAtRandom(float maxFood)
{
    lastSpawnX_ = randi(0, gridWidth_);
    lastSpawnY_ = randi(0, gridHeight_);
    grid_[lastSpawnX_][lastSpawnY_] = maxFood;
}

float FoodSystem::getFoodAt(float worldX, float worldY) const
{
    int cellX, cellY;
    worldToCell(worldX, worldY, cellX, cellY);
    return getFoodAtCell(cellX, cellY);
}

float FoodSystem::getFoodAtCell(int cellX, int cellY) const
{
    if (cellX < 0 || cellX >= gridWidth_ || cellY < 0 || cellY >= gridHeight_) {
        return 0.0f;
    }
    return grid_[cellX][cellY];
}

float FoodSystem::consumeFood(float worldX, float worldY, float amount)
{
    int cellX, cellY;
    worldToCell(worldX, worldY, cellX, cellY);
    
    if (cellX < 0 || cellX >= gridWidth_ || cellY < 0 || cellY >= gridHeight_) {
        return 0.0f;
    }
    
    float available = grid_[cellX][cellY];
    float consumed = std::min(available, amount);
    grid_[cellX][cellY] -= consumed;
    
    return consumed;
}

void FoodSystem::worldToCell(float worldX, float worldY, int& cellX, int& cellY) const
{
    cellX = static_cast<int>(worldX) / cellSize_;
    cellY = static_cast<int>(worldY) / cellSize_;
    
    // Clamp to valid range
    cellX = std::max(0, std::min(cellX, gridWidth_ - 1));
    cellY = std::max(0, std::min(cellY, gridHeight_ - 1));
}

void FoodSystem::draw(View* view, float maxFood) const
{
    for (int i = 0; i < gridWidth_; i++) {
        for (int j = 0; j < gridHeight_; j++) {
            float f = 0.5f * grid_[i][j] / maxFood;
            view->drawFood(i, j, f);
        }
    }
}

void FoodSystem::saveToFile(std::ofstream& outFile) const
{
    outFile << gridWidth_ << " ";
    outFile << gridHeight_ << " ";
    outFile << lastSpawnX_ << " ";
    outFile << lastSpawnY_ << std::endl;
    
    outFile << "foodMapBegin" << std::endl;
    for (int x = 0; x < gridWidth_; x++) {
        for (int y = 0; y < gridHeight_; y++) {
            outFile << grid_[x][y] << " ";
        }
    }
    outFile << std::endl << "foodMapEnd" << std::endl;
}
