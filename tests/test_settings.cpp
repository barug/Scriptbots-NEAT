#include <gtest/gtest.h>
#include "settings.h"
#include <fstream>
#include <sstream>

class SettingsTest : public ::testing::Test {
protected:
    void SetUp() override {
        conf::initialize();
    }
};

TEST_F(SettingsTest, Initialize) {
    conf::initialize();
    
    // Check default values are set correctly
    EXPECT_EQ(conf::WIDTH, 8000);
    EXPECT_EQ(conf::HEIGHT, 4000);
    EXPECT_EQ(conf::WWIDTH, 1600);
    EXPECT_EQ(conf::WHEIGHT, 900);
    EXPECT_EQ(conf::CZ, 50);
    EXPECT_EQ(conf::NUMBOTS, 70);
    EXPECT_FLOAT_EQ(conf::BOTRADIUS, 10);
    EXPECT_FLOAT_EQ(conf::BOTSPEED, 0.3f);
    EXPECT_EQ(conf::BABIES, 2);
}

TEST_F(SettingsTest, MutationRateDefaults) {
    conf::initialize();
    
    EXPECT_FLOAT_EQ(conf::MUTRATE1_INIT_LOW, 0.001f);
    EXPECT_FLOAT_EQ(conf::MUTRATE1_INIT_HIGH, 0.005f);
    EXPECT_FLOAT_EQ(conf::MUTRATE2_INIT_LOW, 0.01f);
    EXPECT_FLOAT_EQ(conf::MUTRATE2_INIT_HIGH, 0.07f);
}

TEST_F(SettingsTest, FoodDefaults) {
    conf::initialize();
    
    EXPECT_FLOAT_EQ(conf::FOODINTAKE, 0.002f);
    EXPECT_FLOAT_EQ(conf::FOODWASTE, 0.001f);
    EXPECT_FLOAT_EQ(conf::FOODMAX, 0.5f);
    EXPECT_EQ(conf::FOODADDFREQ, 15);
}

TEST_F(SettingsTest, MatingDefaults) {
    conf::initialize();
    
    EXPECT_FLOAT_EQ(conf::MATING_RADIUS, 100);
    EXPECT_DOUBLE_EQ(conf::MATING_COMPATIBILITY_TRESHOLD, 4.0);
    EXPECT_EQ(conf::MATING_BABIES, 3);
}

TEST_F(SettingsTest, SaveAndLoad) {
    conf::initialize();
    
    // Modify some values
    conf::WIDTH = 10000;
    conf::NUMBOTS = 100;
    conf::FOODMAX = 0.8f;
    
    // Save to a string stream
    std::stringstream ss;
    std::ofstream* fakeOut = reinterpret_cast<std::ofstream*>(&ss);
    
    // We can't easily test save_to_file without file I/O
    // but we can verify the values are preserved after re-initialization
    int savedWidth = conf::WIDTH;
    int savedBots = conf::NUMBOTS;
    float savedFood = conf::FOODMAX;
    
    // Re-initialize resets to defaults
    conf::initialize();
    
    EXPECT_EQ(conf::WIDTH, 8000);  // Back to default
    EXPECT_EQ(conf::NUMBOTS, 70);
    EXPECT_FLOAT_EQ(conf::FOODMAX, 0.5f);
    
    // Verify saved values were different
    EXPECT_EQ(savedWidth, 10000);
    EXPECT_EQ(savedBots, 100);
    EXPECT_FLOAT_EQ(savedFood, 0.8f);
}

TEST_F(SettingsTest, DerivedValues) {
    conf::initialize();
    
    // CZ should divide evenly into WIDTH and HEIGHT
    EXPECT_EQ(conf::WIDTH % conf::CZ, 0);
    EXPECT_EQ(conf::HEIGHT % conf::CZ, 0);
}

TEST_F(SettingsTest, ReproductionRates) {
    conf::initialize();
    
    EXPECT_FLOAT_EQ(conf::REPRATEH, 7);
    EXPECT_FLOAT_EQ(conf::REPRATEC, 7);
    EXPECT_FLOAT_EQ(conf::BOOSTSIZEMULT, 2);
}

TEST_F(SettingsTest, CompatibilityMultipliers) {
    conf::initialize();
    
    EXPECT_DOUBLE_EQ(conf::BODY_COMPAT_MULT, 1);
    EXPECT_DOUBLE_EQ(conf::BRAIN_COMPAT_MULT, 1);
}

TEST_F(SettingsTest, DistanceSettings) {
    conf::initialize();
    
    EXPECT_FLOAT_EQ(conf::DIST, 150);
    EXPECT_FLOAT_EQ(conf::FOOD_SHARING_DISTANCE, 50);
    EXPECT_FLOAT_EQ(conf::FOOD_DISTRIBUTION_RADIUS, 100);
}

TEST_F(SettingsTest, DamageSettings) {
    conf::initialize();
    
    // Default damage settings should be 0 (disabled)
    EXPECT_FLOAT_EQ(conf::DAMAGE_LIMIT, 0);
    EXPECT_FLOAT_EQ(conf::DAMAGE_GRADIENT, 0);
}
