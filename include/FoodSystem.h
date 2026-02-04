#ifndef FOODSYSTEM_H
#define FOODSYSTEM_H

#include <vector>
#include <fstream>

class View;

/**
 * FoodSystem manages the food grid in the simulation.
 * 
 * The world is divided into cells of size CZ x CZ pixels.
 * Each cell can contain food that herbivore agents can consume.
 */
class FoodSystem {
public:
    /**
     * Create a food system with the given world dimensions and cell size.
     * @param worldWidth Width of the world in pixels
     * @param worldHeight Height of the world in pixels
     * @param cellSize Size of each food cell in pixels
     */
    FoodSystem(int worldWidth, int worldHeight, int cellSize);
    
    /**
     * Create a food system by loading from a save file.
     * @param inFile Input file stream positioned at food data
     */
    FoodSystem(std::ifstream& inFile);
    
    ~FoodSystem() = default;
    
    /**
     * Spawn food at a random location (sets it to max food value).
     * @param maxFood Maximum food value for a cell
     */
    void spawnFoodAtRandom(float maxFood);
    
    /**
     * Get the food amount at a world position.
     * @param worldX X position in world coordinates
     * @param worldY Y position in world coordinates
     * @return Food amount at that position
     */
    float getFoodAt(float worldX, float worldY) const;
    
    /**
     * Get the food amount at a cell position.
     * @param cellX Cell X coordinate
     * @param cellY Cell Y coordinate
     * @return Food amount at that cell
     */
    float getFoodAtCell(int cellX, int cellY) const;
    
    /**
     * Consume food at a world position.
     * @param worldX X position in world coordinates
     * @param worldY Y position in world coordinates
     * @param amount Amount to consume
     * @return Actual amount consumed (may be less if cell has less food)
     */
    float consumeFood(float worldX, float worldY, float amount);
    
    /**
     * Convert world coordinates to cell coordinates.
     * @param worldX X position in world coordinates
     * @param worldY Y position in world coordinates
     * @param cellX Output cell X coordinate
     * @param cellY Output cell Y coordinate
     */
    void worldToCell(float worldX, float worldY, int& cellX, int& cellY) const;
    
    /**
     * Draw the food grid using the provided view.
     * @param view The view to draw to
     * @param maxFood Maximum food value (for normalization)
     */
    void draw(View* view, float maxFood) const;
    
    /**
     * Save the food system state to a file.
     * @param outFile Output file stream
     */
    void saveToFile(std::ofstream& outFile) const;
    
    // Accessors for grid dimensions
    int getGridWidth() const { return gridWidth_; }
    int getGridHeight() const { return gridHeight_; }
    int getCellSize() const { return cellSize_; }
    
private:
    int gridWidth_;   // Number of cells horizontally (FW)
    int gridHeight_;  // Number of cells vertically (FH)
    int cellSize_;    // Size of each cell in pixels (CZ)
    
    int lastSpawnX_;  // Last spawn position X (fx)
    int lastSpawnY_;  // Last spawn position Y (fy)
    
    std::vector<std::vector<float>> grid_;  // The food grid
    
    /**
     * Initialize the grid with zeros.
     */
    void initializeGrid();
};

#endif // FOODSYSTEM_H
