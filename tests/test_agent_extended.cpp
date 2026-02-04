#include <gtest/gtest.h>
#include "Agent.h"
#include "settings.h"
#include "helpers.h"
#include <cmath>

// Extended Agent tests for refactoring safety
class AgentExtendedTest : public ::testing::Test {
protected:
    void SetUp() override {
        conf::initialize();
        srand(42);  // Deterministic tests
    }
};

// ============ Physical Properties Tests ============

TEST_F(AgentExtendedTest, EyesInitializedCorrectly) {
    Agent agent;
    
    ASSERT_EQ(agent.eyefov.size(), NUMEYES);
    ASSERT_EQ(agent.eyedir.size(), NUMEYES);
    
    // All eye FOVs should be positive
    for (int i = 0; i < NUMEYES; i++) {
        EXPECT_GT(agent.eyefov[i], 0.0f);
    }
}

TEST_F(AgentExtendedTest, ColorValuesInRange) {
    for (int i = 0; i < 10; i++) {
        Agent agent;
        EXPECT_GE(agent.red, 0.0f);
        EXPECT_LE(agent.red, 1.0f);
        EXPECT_GE(agent.gre, 0.0f);
        EXPECT_LE(agent.gre, 1.0f);
        EXPECT_GE(agent.blu, 0.0f);
        EXPECT_LE(agent.blu, 1.0f);
    }
}

TEST_F(AgentExtendedTest, ClockFrequenciesInRange) {
    for (int i = 0; i < 10; i++) {
        Agent agent;
        EXPECT_GT(agent.clockf1, 0.0f);
        EXPECT_GT(agent.clockf2, 0.0f);
    }
}

TEST_F(AgentExtendedTest, SensorModifiersInitialized) {
    Agent agent;
    
    // All sensor modifiers should be positive
    EXPECT_GT(agent.smellmod, 0.0f);
    EXPECT_GT(agent.soundmod, 0.0f);
    EXPECT_GT(agent.hearmod, 0.0f);
    EXPECT_GT(agent.eyesensmod, 0.0f);
    EXPECT_GT(agent.bloodmod, 0.0f);
}

TEST_F(AgentExtendedTest, TemperaturePreferenceInRange) {
    for (int i = 0; i < 10; i++) {
        Agent agent;
        EXPECT_GE(agent.temperature_preference, 0.0f);
        EXPECT_LE(agent.temperature_preference, 1.0f);
    }
}

// ============ Initial State Tests ============

TEST_F(AgentExtendedTest, InitialWheelSpeeds) {
    Agent agent;
    // Initial wheel speeds should be 0 or small
    EXPECT_GE(agent.w1, 0.0f);
    EXPECT_LE(agent.w1, 1.0f);
    EXPECT_GE(agent.w2, 0.0f);
    EXPECT_LE(agent.w2, 1.0f);
}

TEST_F(AgentExtendedTest, InitialSpikeRetracted) {
    Agent agent;
    EXPECT_EQ(agent.spikeLength, 0.0f);
    EXPECT_FALSE(agent.spiked);
}

TEST_F(AgentExtendedTest, InitialRepcounter) {
    Agent agent;
    // Repcounter should start positive (not ready to reproduce)
    EXPECT_GT(agent.repcounter, 0.0f);
}

TEST_F(AgentExtendedTest, InitialSelectionFlag) {
    Agent agent;
    EXPECT_EQ(agent.selectflag, 0);
}

// ============ Input/Output Vector Tests ============

TEST_F(AgentExtendedTest, InputVectorSize) {
    Agent agent;
    EXPECT_EQ(agent.in.size(), INPUTSIZE);
}

TEST_F(AgentExtendedTest, OutputVectorSize) {
    Agent agent;
    EXPECT_EQ(agent.out.size(), OUTPUTSIZE);
}

TEST_F(AgentExtendedTest, InputVectorInitializedToZero) {
    Agent agent;
    for (int i = 0; i < INPUTSIZE; i++) {
        EXPECT_FLOAT_EQ(agent.in[i], 0.0f);
    }
}

TEST_F(AgentExtendedTest, OutputVectorInitializedToZero) {
    Agent agent;
    for (int i = 0; i < OUTPUTSIZE; i++) {
        EXPECT_FLOAT_EQ(agent.out[i], 0.0f);
    }
}

// ============ Brain Integration Tests ============

TEST_F(AgentExtendedTest, TickProducesValidOutputs) {
    Agent agent;
    agent.makeBasicBrain();
    
    // Set varied inputs
    for (int i = 0; i < INPUTSIZE; i++) {
        agent.in[i] = randf(0.0f, 1.0f);
    }
    
    agent.tick();
    
    // All outputs should be valid numbers
    for (int i = 0; i < OUTPUTSIZE; i++) {
        EXPECT_FALSE(std::isnan(agent.out[i]));
        EXPECT_FALSE(std::isinf(agent.out[i]));
    }
}

TEST_F(AgentExtendedTest, MultipleTicksStable) {
    Agent agent;
    agent.makeBasicBrain();
    
    // Set constant inputs
    for (int i = 0; i < INPUTSIZE; i++) {
        agent.in[i] = 0.5f;
    }
    
    // Run many ticks
    for (int t = 0; t < 100; t++) {
        EXPECT_NO_THROW(agent.tick());
    }
    
    // Outputs should still be valid
    for (int i = 0; i < OUTPUTSIZE; i++) {
        EXPECT_FALSE(std::isnan(agent.out[i]));
        EXPECT_FALSE(std::isinf(agent.out[i]));
    }
}

// ============ Reproduction Tests ============

TEST_F(AgentExtendedTest, ReproducePreservesSpeciesLink) {
    Agent parent;
    parent.makeBasicBrain();
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov_num = 0;
    
    Agent* child = parent.reproduce(0.01f, 0.05f, innovations, cur_innov_num);
    
    // Child should have valid brain
    EXPECT_NE(child->brain, nullptr);
    
    // Child's gencount should increment
    EXPECT_EQ(child->gencount, parent.gencount + 1);
    
    // Child should not be hybrid (asexual reproduction)
    EXPECT_FALSE(child->hybrid);
    
    delete child;
    for (auto* innov : innovations) delete innov;
}

TEST_F(AgentExtendedTest, MateProducesHybrid) {
    Agent parent1;
    parent1.makeBasicBrain();
    
    Agent parent2;
    parent2.makeBasicBrain();
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov_num = 0;
    
    Agent* child = parent1.mate(&parent2, innovations, cur_innov_num);
    
    // Child should be marked as hybrid
    EXPECT_TRUE(child->hybrid);
    
    delete child;
    for (auto* innov : innovations) delete innov;
}

TEST_F(AgentExtendedTest, ChildPositionNearParent) {
    Agent parent;
    parent.makeBasicBrain();
    parent.pos.x = 500.0f;
    parent.pos.y = 500.0f;
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov_num = 0;
    
    Agent* child = parent.reproduce(0.01f, 0.05f, innovations, cur_innov_num);
    
    float distance = (child->pos - parent.pos).length();
    EXPECT_LT(distance, conf::BOTRADIUS * 10);
    
    delete child;
    for (auto* innov : innovations) delete innov;
}

TEST_F(AgentExtendedTest, ChildInheritsHerbivoreType) {
    Agent parent;
    parent.makeBasicBrain();
    parent.herbivore = 0.9f;  // Herbivore
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov_num = 0;
    
    Agent* child = parent.reproduce(0.01f, 0.05f, innovations, cur_innov_num);
    
    // Child's herbivore value should be close to parent's (with some mutation)
    EXPECT_GT(child->herbivore, 0.5f);  // Should still be more herbivore
    
    delete child;
    for (auto* innov : innovations) delete innov;
}

// ============ Compatibility Tests ============

TEST_F(AgentExtendedTest, CompatibilitySymmetric) {
    Agent agent1;
    agent1.makeBasicBrain();
    
    Agent agent2;
    agent2.makeBasicBrain();
    
    double c1 = agent1.compatibility(&agent2);
    double c2 = agent2.compatibility(&agent1);
    
    // Compatibility should be symmetric
    EXPECT_NEAR(c1, c2, 0.001);
}

TEST_F(AgentExtendedTest, CompatibilityWithSimilarAgents) {
    Agent parent;
    parent.makeBasicBrain();
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov_num = 0;
    
    // Create child with low mutation rate
    Agent* child = parent.reproduce(0.001f, 0.001f, innovations, cur_innov_num);
    
    // Child should be very compatible with parent
    double compat = parent.compatibility(child);
    EXPECT_LT(compat, conf::MATING_COMPATIBILITY_TRESHOLD);
    
    delete child;
    for (auto* innov : innovations) delete innov;
}

// ============ Event System Tests ============

TEST_F(AgentExtendedTest, InitEventSetsValues) {
    Agent agent;
    
    agent.initEvent(50.0f, 1.0f, 0.5f, 0.25f);
    
    EXPECT_FLOAT_EQ(agent.indicator, 50.0f);
    EXPECT_FLOAT_EQ(agent.ir, 1.0f);
    EXPECT_FLOAT_EQ(agent.ig, 0.5f);
    EXPECT_FLOAT_EQ(agent.ib, 0.25f);
}

TEST_F(AgentExtendedTest, IndicatorDecaysOverTime) {
    Agent agent;
    agent.indicator = 10.0f;
    
    // Simulate indicator decay (normally done in World::update)
    for (int i = 0; i < 5; i++) {
        if (agent.indicator > 0) agent.indicator -= 1;
    }
    
    EXPECT_FLOAT_EQ(agent.indicator, 5.0f);
}

// ============ ID and Generation Tracking ============

TEST_F(AgentExtendedTest, IdCanBeSet) {
    Agent agent;
    agent.id = 42;
    EXPECT_EQ(agent.id, 42);
}

TEST_F(AgentExtendedTest, GencountIncrementsThroughGenerations) {
    Agent grandparent;
    grandparent.makeBasicBrain();
    grandparent.gencount = 0;
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov_num = 0;
    
    Agent* parent = grandparent.reproduce(0.01f, 0.05f, innovations, cur_innov_num);
    EXPECT_EQ(parent->gencount, 1);
    
    Agent* child = parent->reproduce(0.01f, 0.05f, innovations, cur_innov_num);
    EXPECT_EQ(child->gencount, 2);
    
    delete child;
    delete parent;
    for (auto* innov : innovations) delete innov;
}

// ============ Stress Tests ============

TEST_F(AgentExtendedTest, CreateManyAgents) {
    std::vector<Agent*> agents;
    
    for (int i = 0; i < 100; i++) {
        Agent* a = new Agent();
        a->makeBasicBrain();
        agents.push_back(a);
    }
    
    EXPECT_EQ(agents.size(), 100);
    
    for (Agent* a : agents) {
        EXPECT_NE(a->brain, nullptr);
        delete a;
    }
}

TEST_F(AgentExtendedTest, ReproductionChain) {
    Agent* current = new Agent();
    current->makeBasicBrain();
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov_num = 0;
    
    // Create a chain of 10 generations
    for (int gen = 0; gen < 10; gen++) {
        Agent* next = current->reproduce(0.05f, 0.05f, innovations, cur_innov_num);
        EXPECT_EQ(next->gencount, gen + 1);
        delete current;
        current = next;
    }
    
    EXPECT_EQ(current->gencount, 10);
    delete current;
    for (auto* innov : innovations) delete innov;
}
