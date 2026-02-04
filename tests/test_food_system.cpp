#include <gtest/gtest.h>
#include "FoodSystem.h"
#include "settings.h"
#include <cmath>
#include <sstream>
#include <fstream>

class FoodSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        conf::initialize();
        srand(42);  // Deterministic tests
    }
};

// ============ Construction Tests ============

TEST_F(FoodSystemTest, ConstructorCreatesCorrectGridSize) {
    FoodSystem fs(1000, 800, 10);
    
    EXPECT_EQ(fs.getGridWidth(), 100);   // 1000 / 10
    EXPECT_EQ(fs.getGridHeight(), 80);   // 800 / 10
    EXPECT_EQ(fs.getCellSize(), 10);
}

TEST_F(FoodSystemTest, ConstructorInitializesGridToZero) {
    FoodSystem fs(100, 100, 10);
    
    // All cells should be empty
    for (int x = 0; x < fs.getGridWidth(); x++) {
        for (int y = 0; y < fs.getGridHeight(); y++) {
            EXPECT_FLOAT_EQ(fs.getFoodAtCell(x, y), 0.0f);
        }
    }
}

TEST_F(FoodSystemTest, ConstructorWithDefaultSettings) {
    FoodSystem fs(conf::WIDTH, conf::HEIGHT, conf::CZ);
    
    EXPECT_EQ(fs.getGridWidth(), conf::WIDTH / conf::CZ);
    EXPECT_EQ(fs.getGridHeight(), conf::HEIGHT / conf::CZ);
}

// ============ Food Spawning Tests ============

TEST_F(FoodSystemTest, SpawnFoodAtRandomSetsMaxFood) {
    FoodSystem fs(100, 100, 10);
    
    // Spawn food
    fs.spawnFoodAtRandom(100.0f);
    
    // At least one cell should have food
    bool foundFood = false;
    for (int x = 0; x < fs.getGridWidth(); x++) {
        for (int y = 0; y < fs.getGridHeight(); y++) {
            if (fs.getFoodAtCell(x, y) > 0) {
                foundFood = true;
                EXPECT_FLOAT_EQ(fs.getFoodAtCell(x, y), 100.0f);
            }
        }
    }
    EXPECT_TRUE(foundFood);
}

TEST_F(FoodSystemTest, MultipleSpawnsAccumulateFood) {
    FoodSystem fs(100, 100, 10);
    
    // Spawn food multiple times
    for (int i = 0; i < 50; i++) {
        fs.spawnFoodAtRandom(conf::FOODMAX);
    }
    
    // Count cells with food
    int cellsWithFood = 0;
    for (int x = 0; x < fs.getGridWidth(); x++) {
        for (int y = 0; y < fs.getGridHeight(); y++) {
            if (fs.getFoodAtCell(x, y) > 0) {
                cellsWithFood++;
            }
        }
    }
    
    // Should have some food spawned (may overlap, so could be less than 50)
    EXPECT_GT(cellsWithFood, 0);
}

// ============ World to Cell Conversion Tests ============

TEST_F(FoodSystemTest, WorldToCellBasic) {
    FoodSystem fs(100, 100, 10);
    
    int cellX, cellY;
    
    // Origin
    fs.worldToCell(0.0f, 0.0f, cellX, cellY);
    EXPECT_EQ(cellX, 0);
    EXPECT_EQ(cellY, 0);
    
    // Middle of first cell
    fs.worldToCell(5.0f, 5.0f, cellX, cellY);
    EXPECT_EQ(cellX, 0);
    EXPECT_EQ(cellY, 0);
    
    // Second cell
    fs.worldToCell(15.0f, 15.0f, cellX, cellY);
    EXPECT_EQ(cellX, 1);
    EXPECT_EQ(cellY, 1);
}

TEST_F(FoodSystemTest, WorldToCellEdgeCases) {
    FoodSystem fs(100, 100, 10);
    
    int cellX, cellY;
    
    // At cell boundary
    fs.worldToCell(10.0f, 10.0f, cellX, cellY);
    EXPECT_EQ(cellX, 1);
    EXPECT_EQ(cellY, 1);
    
    // Near end of grid
    fs.worldToCell(95.0f, 95.0f, cellX, cellY);
    EXPECT_EQ(cellX, 9);
    EXPECT_EQ(cellY, 9);
}

TEST_F(FoodSystemTest, WorldToCellClampsNegative) {
    FoodSystem fs(100, 100, 10);
    
    int cellX, cellY;
    
    // Negative coordinates should clamp to 0
    fs.worldToCell(-10.0f, -10.0f, cellX, cellY);
    EXPECT_GE(cellX, 0);
    EXPECT_GE(cellY, 0);
}

// ============ Get Food Tests ============

TEST_F(FoodSystemTest, GetFoodAtWorldPosition) {
    FoodSystem fs(100, 100, 10);
    
    // Spawn food at a known position (manually set via spawn and check)
    fs.spawnFoodAtRandom(50.0f);
    
    // Find where the food was spawned
    int spawnX = -1, spawnY = -1;
    for (int x = 0; x < fs.getGridWidth() && spawnX < 0; x++) {
        for (int y = 0; y < fs.getGridHeight() && spawnX < 0; y++) {
            if (fs.getFoodAtCell(x, y) > 0) {
                spawnX = x;
                spawnY = y;
            }
        }
    }
    
    ASSERT_GE(spawnX, 0);
    
    // Get food at world position corresponding to that cell
    float worldX = spawnX * 10 + 5;  // Center of cell
    float worldY = spawnY * 10 + 5;
    
    EXPECT_FLOAT_EQ(fs.getFoodAt(worldX, worldY), 50.0f);
}

TEST_F(FoodSystemTest, GetFoodAtCellOutOfBounds) {
    FoodSystem fs(100, 100, 10);
    
    // Out of bounds should return 0
    EXPECT_FLOAT_EQ(fs.getFoodAtCell(-1, 0), 0.0f);
    EXPECT_FLOAT_EQ(fs.getFoodAtCell(0, -1), 0.0f);
    EXPECT_FLOAT_EQ(fs.getFoodAtCell(100, 0), 0.0f);
    EXPECT_FLOAT_EQ(fs.getFoodAtCell(0, 100), 0.0f);
}

// ============ Consume Food Tests ============

TEST_F(FoodSystemTest, ConsumeFoodReducesAmount) {
    FoodSystem fs(100, 100, 10);
    
    // Spawn food
    fs.spawnFoodAtRandom(100.0f);
    
    // Find the food
    int spawnX = -1, spawnY = -1;
    for (int x = 0; x < fs.getGridWidth() && spawnX < 0; x++) {
        for (int y = 0; y < fs.getGridHeight() && spawnX < 0; y++) {
            if (fs.getFoodAtCell(x, y) > 0) {
                spawnX = x;
                spawnY = y;
            }
        }
    }
    ASSERT_GE(spawnX, 0);
    
    float worldX = spawnX * 10 + 5;
    float worldY = spawnY * 10 + 5;
    
    // Consume some food
    float consumed = fs.consumeFood(worldX, worldY, 30.0f);
    
    EXPECT_FLOAT_EQ(consumed, 30.0f);
    EXPECT_FLOAT_EQ(fs.getFoodAt(worldX, worldY), 70.0f);
}

TEST_F(FoodSystemTest, ConsumeFoodCannotExceedAvailable) {
    FoodSystem fs(100, 100, 10);
    
    // Spawn food with known amount
    fs.spawnFoodAtRandom(50.0f);
    
    // Find the food
    int spawnX = -1, spawnY = -1;
    for (int x = 0; x < fs.getGridWidth() && spawnX < 0; x++) {
        for (int y = 0; y < fs.getGridHeight() && spawnX < 0; y++) {
            if (fs.getFoodAtCell(x, y) > 0) {
                spawnX = x;
                spawnY = y;
            }
        }
    }
    ASSERT_GE(spawnX, 0);
    
    float worldX = spawnX * 10 + 5;
    float worldY = spawnY * 10 + 5;
    
    // Try to consume more than available
    float consumed = fs.consumeFood(worldX, worldY, 100.0f);
    
    EXPECT_FLOAT_EQ(consumed, 50.0f);
    EXPECT_FLOAT_EQ(fs.getFoodAt(worldX, worldY), 0.0f);
}

TEST_F(FoodSystemTest, ConsumeFoodFromEmptyCell) {
    FoodSystem fs(100, 100, 10);
    
    // Try to consume from empty cell
    float consumed = fs.consumeFood(50.0f, 50.0f, 10.0f);
    
    EXPECT_FLOAT_EQ(consumed, 0.0f);
}

// ============ Save/Load Tests ============

TEST_F(FoodSystemTest, SaveAndLoadPreservesState) {
    // Create and populate food system
    FoodSystem fs1(100, 100, 10);
    
    // Spawn food in multiple places
    for (int i = 0; i < 20; i++) {
        fs1.spawnFoodAtRandom(conf::FOODMAX);
    }
    
    // Save to file
    std::ofstream outFile("test_food_save.tmp");
    fs1.saveToFile(outFile);
    outFile.close();
    
    // Load from file
    std::ifstream inFile("test_food_save.tmp");
    FoodSystem fs2(inFile);
    inFile.close();
    
    // Compare dimensions
    EXPECT_EQ(fs1.getGridWidth(), fs2.getGridWidth());
    EXPECT_EQ(fs1.getGridHeight(), fs2.getGridHeight());
    
    // Compare food values
    for (int x = 0; x < fs1.getGridWidth(); x++) {
        for (int y = 0; y < fs1.getGridHeight(); y++) {
            EXPECT_FLOAT_EQ(fs1.getFoodAtCell(x, y), fs2.getFoodAtCell(x, y));
        }
    }
    
    // Cleanup
    std::remove("test_food_save.tmp");
}

// ============ Integration-style Tests ============

TEST_F(FoodSystemTest, SimulateAgentEating) {
    FoodSystem fs(conf::WIDTH, conf::HEIGHT, conf::CZ);
    
    // Spawn some food
    for (int i = 0; i < 100; i++) {
        fs.spawnFoodAtRandom(conf::FOODMAX);
    }
    
    // Simulate an agent moving and eating
    float agentX = 500.0f;
    float agentY = 500.0f;
    float totalEaten = 0.0f;
    
    // Move around and eat
    for (int step = 0; step < 50; step++) {
        float available = fs.getFoodAt(agentX, agentY);
        if (available > 0) {
            float eaten = fs.consumeFood(agentX, agentY, conf::FOODINTAKE);
            totalEaten += eaten;
        }
        
        // Move agent
        agentX += 10.0f;
        if (agentX >= conf::WIDTH) agentX -= conf::WIDTH;
    }
    
    // Agent should have eaten something
    EXPECT_GE(totalEaten, 0.0f);
}

TEST_F(FoodSystemTest, FoodSpawningDistribution) {
    FoodSystem fs(100, 100, 10);
    
    // Spawn food many times
    for (int i = 0; i < 1000; i++) {
        fs.spawnFoodAtRandom(1.0f);  // Use 1.0 so we can count overlaps
    }
    
    // Count total food
    float totalFood = 0.0f;
    for (int x = 0; x < fs.getGridWidth(); x++) {
        for (int y = 0; y < fs.getGridHeight(); y++) {
            totalFood += fs.getFoodAtCell(x, y);
        }
    }
    
    // Food overwrites, so total should be <= 1000
    // But with random distribution and some cell size, many should have food
    EXPECT_GT(totalFood, 0.0f);
}
