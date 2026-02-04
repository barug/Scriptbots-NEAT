#include <gtest/gtest.h>
#include "NEAT/NeatBrain.h"
#include "NEAT/innovation.h"
#include "settings.h"

class NEATBrainTest : public ::testing::Test {
protected:
    void SetUp() override {
        conf::initialize();
    }
};

TEST_F(NEATBrainTest, DefaultConstructor) {
    NEAT::NEATBrain brain;
    
    // Should start with null pointers
    EXPECT_EQ(brain._gen, nullptr);
    EXPECT_EQ(brain._net, nullptr);
}

TEST_F(NEATBrainTest, InitiateBasicBrain) {
    NEAT::NEATBrain brain;
    brain.initiateBasicBrain();
    
    EXPECT_NE(brain._gen, nullptr);
    EXPECT_NE(brain._net, nullptr);
}

TEST_F(NEATBrainTest, Tick) {
    NEAT::NEATBrain brain;
    brain.initiateBasicBrain();
    
    std::vector<float> inputs(INPUTSIZE, 0.5f);
    std::vector<float> outputs(OUTPUTSIZE, 0.0f);
    
    EXPECT_NO_THROW(brain.tick(inputs, outputs));
    
    // Outputs should be set
    bool hasNonZero = false;
    for (float out : outputs) {
        EXPECT_FALSE(std::isnan(out));
        if (out != 0.0f) hasNonZero = true;
    }
    // At least some output should be non-zero with non-zero inputs
    EXPECT_TRUE(hasNonZero);
}

TEST_F(NEATBrainTest, TickWithZeroInputs) {
    NEAT::NEATBrain brain;
    brain.initiateBasicBrain();
    
    std::vector<float> inputs(INPUTSIZE, 0.0f);
    std::vector<float> outputs(OUTPUTSIZE, 0.0f);
    
    EXPECT_NO_THROW(brain.tick(inputs, outputs));
    
    for (float out : outputs) {
        EXPECT_FALSE(std::isnan(out));
    }
}

TEST_F(NEATBrainTest, TickWithMaxInputs) {
    NEAT::NEATBrain brain;
    brain.initiateBasicBrain();
    
    std::vector<float> inputs(INPUTSIZE, 1.0f);
    std::vector<float> outputs(OUTPUTSIZE, 0.0f);
    
    EXPECT_NO_THROW(brain.tick(inputs, outputs));
    
    for (float out : outputs) {
        EXPECT_FALSE(std::isnan(out));
    }
}

TEST_F(NEATBrainTest, Duplicate) {
    NEAT::NEATBrain brain;
    brain.initiateBasicBrain();
    
    NEAT::NEATBrain* copy = brain.duplicate();
    
    EXPECT_NE(copy, nullptr);
    EXPECT_NE(copy->_gen, nullptr);
    EXPECT_NE(copy->_net, nullptr);
    
    // Test that the copy produces same outputs for same inputs
    std::vector<float> inputs(INPUTSIZE, 0.5f);
    std::vector<float> outputs1(OUTPUTSIZE, 0.0f);
    std::vector<float> outputs2(OUTPUTSIZE, 0.0f);
    
    brain.tick(inputs, outputs1);
    copy->tick(inputs, outputs2);
    
    for (int i = 0; i < OUTPUTSIZE; i++) {
        EXPECT_FLOAT_EQ(outputs1[i], outputs2[i]);
    }
    
    delete copy;
}

TEST_F(NEATBrainTest, Mutate) {
    NEAT::NEATBrain brain;
    brain.initiateBasicBrain();
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov_num = 0;
    
    // Mutate should not crash
    EXPECT_NO_THROW(brain.mutate(0.5f, 0.5f, innovations, cur_innov_num));
    
    // Brain should still be functional after mutation
    std::vector<float> inputs(INPUTSIZE, 0.5f);
    std::vector<float> outputs(OUTPUTSIZE, 0.0f);
    EXPECT_NO_THROW(brain.tick(inputs, outputs));
    
    for (auto* innov : innovations) {
        delete innov;
    }
}

TEST_F(NEATBrainTest, CompatibilityWithSelf) {
    NEAT::NEATBrain brain;
    brain.initiateBasicBrain();
    
    double compat = brain.compatibility(&brain);
    EXPECT_EQ(compat, 0.0);
}

TEST_F(NEATBrainTest, CompatibilityWithOther) {
    NEAT::NEATBrain brain1;
    brain1.initiateBasicBrain();
    
    NEAT::NEATBrain brain2;
    brain2.initiateBasicBrain();
    
    double compat = brain1.compatibility(&brain2);
    EXPECT_GE(compat, 0.0);
}

TEST_F(NEATBrainTest, Crossover) {
    NEAT::NEATBrain brain1;
    brain1.initiateBasicBrain();
    
    NEAT::NEATBrain brain2;
    brain2.initiateBasicBrain();
    
    NEAT::NEATBrain* child = brain1.crossover(&brain2);
    
    EXPECT_NE(child, nullptr);
    EXPECT_NE(child->_gen, nullptr);
    EXPECT_NE(child->_net, nullptr);
    
    // Child should be functional
    std::vector<float> inputs(INPUTSIZE, 0.5f);
    std::vector<float> outputs(OUTPUTSIZE, 0.0f);
    EXPECT_NO_THROW(child->tick(inputs, outputs));
    
    delete child;
}

TEST_F(NEATBrainTest, CopyConstructor) {
    NEAT::NEATBrain brain;
    brain.initiateBasicBrain();
    
    NEAT::NEATBrain copy(brain);
    
    EXPECT_NE(copy._gen, nullptr);
    EXPECT_NE(copy._net, nullptr);
    
    // Should be independent copies
    EXPECT_NE(copy._gen, brain._gen);
    EXPECT_NE(copy._net, brain._net);
}

TEST_F(NEATBrainTest, AssignmentOperator) {
    NEAT::NEATBrain brain1;
    brain1.initiateBasicBrain();
    
    NEAT::NEATBrain brain2;
    brain2 = brain1;
    
    EXPECT_NE(brain2._gen, nullptr);
    EXPECT_NE(brain2._net, nullptr);
    
    // Should be independent copies
    EXPECT_NE(brain2._gen, brain1._gen);
    EXPECT_NE(brain2._net, brain1._net);
}

TEST_F(NEATBrainTest, MultipleTicksConsistent) {
    NEAT::NEATBrain brain;
    brain.initiateBasicBrain();
    
    std::vector<float> inputs(INPUTSIZE, 0.5f);
    std::vector<float> outputs1(OUTPUTSIZE, 0.0f);
    std::vector<float> outputs2(OUTPUTSIZE, 0.0f);
    
    // First tick
    brain.tick(inputs, outputs1);
    
    // Reset and tick again with same inputs should give same outputs
    // (after network stabilizes)
    for (int i = 0; i < 10; i++) {
        brain.tick(inputs, outputs2);
    }
    
    // Network should reach a stable state
    std::vector<float> outputs3(OUTPUTSIZE, 0.0f);
    brain.tick(inputs, outputs3);
    
    for (int i = 0; i < OUTPUTSIZE; i++) {
        EXPECT_NEAR(outputs2[i], outputs3[i], 0.01);
    }
}
