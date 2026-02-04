#include <gtest/gtest.h>
#include "Agent.h"
#include "settings.h"

class AgentTest : public ::testing::Test {
protected:
    void SetUp() override {
        conf::initialize();
    }
};

TEST_F(AgentTest, DefaultConstructor) {
    Agent agent;
    
    // Check basic initialization
    EXPECT_GE(agent.health, 1.0f);
    EXPECT_LE(agent.health, 1.1f);
    EXPECT_EQ(agent.age, 0);
    EXPECT_EQ(agent.spikeLength, 0);
    EXPECT_FALSE(agent.boost);
    EXPECT_EQ(agent.gencount, 0);
    EXPECT_FALSE(agent.hybrid);
    
    // Position should be within world bounds
    EXPECT_GE(agent.pos.x, 0);
    EXPECT_LT(agent.pos.x, conf::WIDTH);
    EXPECT_GE(agent.pos.y, 0);
    EXPECT_LT(agent.pos.y, conf::HEIGHT);
    
    // Angle should be in valid range
    EXPECT_GE(agent.angle, -M_PI);
    EXPECT_LE(agent.angle, M_PI);
    
    // Herbivore should be between 0 and 1
    EXPECT_GE(agent.herbivore, 0);
    EXPECT_LE(agent.herbivore, 1);
    
    // Input/output vectors should be correct size
    EXPECT_EQ(agent.in.size(), INPUTSIZE);
    EXPECT_EQ(agent.out.size(), OUTPUTSIZE);
    
    // Eyes should be initialized
    EXPECT_EQ(agent.eyefov.size(), NUMEYES);
    EXPECT_EQ(agent.eyedir.size(), NUMEYES);
}

TEST_F(AgentTest, MakeBasicBrain) {
    Agent agent;
    agent.makeBasicBrain();
    
    EXPECT_NE(agent.brain, nullptr);
}

TEST_F(AgentTest, Tick) {
    Agent agent;
    agent.makeBasicBrain();
    
    // Set some inputs
    for (int i = 0; i < INPUTSIZE; i++) {
        agent.in[i] = 0.5f;
    }
    
    // Tick should not crash
    EXPECT_NO_THROW(agent.tick());
    
    // Outputs should be set (between 0 and 1 for most)
    for (int i = 0; i < OUTPUTSIZE; i++) {
        EXPECT_FALSE(std::isnan(agent.out[i]));
    }
}

TEST_F(AgentTest, CompatibilityWithSelf) {
    Agent agent;
    agent.makeBasicBrain();
    
    // Agent should be perfectly compatible with itself
    double compat = agent.compatibility(&agent);
    EXPECT_EQ(compat, 0.0);
}

TEST_F(AgentTest, CompatibilityWithOther) {
    Agent agent1;
    agent1.makeBasicBrain();
    
    Agent agent2;
    agent2.makeBasicBrain();
    
    // Two different agents should have some compatibility distance
    double compat = agent1.compatibility(&agent2);
    EXPECT_GE(compat, 0.0);
}

TEST_F(AgentTest, Reproduce) {
    Agent parent;
    parent.makeBasicBrain();
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov_num = 0;
    
    Agent* child = parent.reproduce(0.01f, 0.05f, innovations, cur_innov_num);
    
    EXPECT_NE(child, nullptr);
    EXPECT_NE(child->brain, nullptr);
    EXPECT_EQ(child->gencount, parent.gencount + 1);
    
    // Child should be near parent
    float distance = (child->pos - parent.pos).length();
    EXPECT_LT(distance, conf::BOTRADIUS * 10);
    
    delete child;
    for (auto* innov : innovations) {
        delete innov;
    }
}

TEST_F(AgentTest, Mate) {
    Agent parent1;
    parent1.makeBasicBrain();
    
    Agent parent2;
    parent2.makeBasicBrain();
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov_num = 0;
    
    Agent* child = parent1.mate(&parent2, innovations, cur_innov_num);
    
    EXPECT_NE(child, nullptr);
    EXPECT_NE(child->brain, nullptr);
    EXPECT_TRUE(child->hybrid);
    EXPECT_EQ(child->gencount, std::max(parent1.gencount, parent2.gencount) + 1);
    
    delete child;
    for (auto* innov : innovations) {
        delete innov;
    }
}

TEST_F(AgentTest, InitEvent) {
    Agent agent;
    
    agent.initEvent(10.0f, 1.0f, 0.5f, 0.0f);
    
    EXPECT_FLOAT_EQ(agent.indicator, 10.0f);
    EXPECT_FLOAT_EQ(agent.ir, 1.0f);
    EXPECT_FLOAT_EQ(agent.ig, 0.5f);
    EXPECT_FLOAT_EQ(agent.ib, 0.0f);
}

TEST_F(AgentTest, MutationRatesInRange) {
    // Create multiple agents to check mutation rate ranges
    for (int i = 0; i < 10; i++) {
        Agent agent;
        
        EXPECT_GE(agent.MUTRATE1, conf::MUTRATE1_INIT_LOW);
        EXPECT_LE(agent.MUTRATE1, conf::MUTRATE1_INIT_HIGH);
        EXPECT_GE(agent.MUTRATE2, conf::MUTRATE2_INIT_LOW);
        EXPECT_LE(agent.MUTRATE2, conf::MUTRATE2_INIT_HIGH);
    }
}
