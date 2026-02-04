#include <gtest/gtest.h>
#include "NEAT/Species.h"
#include "Agent.h"
#include "settings.h"

class SpeciesTest : public ::testing::Test {
protected:
    void SetUp() override {
        conf::initialize();
    }
    
    void TearDown() override {
        // Clean up any agents we created
    }
};

TEST_F(SpeciesTest, Constructor) {
    NEAT::Species species(1);
    
    EXPECT_EQ(species.getId(), 1);
    EXPECT_EQ(species.getNumberOfAgents(), 0);
    EXPECT_TRUE(species.empty());
}

TEST_F(SpeciesTest, AddAgent) {
    NEAT::Species species(1);
    
    Agent* agent = new Agent();
    agent->makeBasicBrain();
    
    species.addAgent(agent);
    
    EXPECT_EQ(species.getNumberOfAgents(), 1);
    EXPECT_FALSE(species.empty());
    EXPECT_EQ(species.first(), agent);
    
    delete agent;
}

TEST_F(SpeciesTest, AddMultipleAgents) {
    NEAT::Species species(1);
    
    Agent* agent1 = new Agent();
    agent1->makeBasicBrain();
    Agent* agent2 = new Agent();
    agent2->makeBasicBrain();
    Agent* agent3 = new Agent();
    agent3->makeBasicBrain();
    
    species.addAgent(agent1);
    species.addAgent(agent2);
    species.addAgent(agent3);
    
    EXPECT_EQ(species.getNumberOfAgents(), 3);
    EXPECT_FALSE(species.empty());
    
    delete agent1;
    delete agent2;
    delete agent3;
}

TEST_F(SpeciesTest, RemoveAgent) {
    NEAT::Species species(1);
    
    Agent* agent1 = new Agent();
    agent1->makeBasicBrain();
    Agent* agent2 = new Agent();
    agent2->makeBasicBrain();
    
    species.addAgent(agent1);
    species.addAgent(agent2);
    
    EXPECT_EQ(species.getNumberOfAgents(), 2);
    
    species.removeAgent(agent1);
    
    EXPECT_EQ(species.getNumberOfAgents(), 1);
    EXPECT_EQ(species.first(), agent2);
    
    delete agent1;
    delete agent2;
}

TEST_F(SpeciesTest, RemoveAllAgents) {
    NEAT::Species species(1);
    
    Agent* agent1 = new Agent();
    agent1->makeBasicBrain();
    Agent* agent2 = new Agent();
    agent2->makeBasicBrain();
    
    species.addAgent(agent1);
    species.addAgent(agent2);
    
    species.removeAgent(agent1);
    species.removeAgent(agent2);
    
    EXPECT_EQ(species.getNumberOfAgents(), 0);
    EXPECT_TRUE(species.empty());
    
    delete agent1;
    delete agent2;
}

TEST_F(SpeciesTest, PopMaxRecord) {
    NEAT::Species species(1);
    
    std::vector<Agent*> agents;
    
    // Add 5 agents
    for (int i = 0; i < 5; i++) {
        Agent* agent = new Agent();
        agent->makeBasicBrain();
        agents.push_back(agent);
        species.addAgent(agent);
    }
    
    EXPECT_EQ(species.getPopMaxRecord(), 5);
    
    // Remove 2 agents
    species.removeAgent(agents[0]);
    species.removeAgent(agents[1]);
    
    // Max should still be 5
    EXPECT_EQ(species.getPopMaxRecord(), 5);
    EXPECT_EQ(species.getNumberOfAgents(), 3);
    
    for (Agent* agent : agents) {
        delete agent;
    }
}

TEST_F(SpeciesTest, TotalMembersOverTime) {
    NEAT::Species species(1);
    
    Agent* agent1 = new Agent();
    agent1->makeBasicBrain();
    Agent* agent2 = new Agent();
    agent2->makeBasicBrain();
    Agent* agent3 = new Agent();
    agent3->makeBasicBrain();
    
    species.addAgent(agent1);
    species.addAgent(agent2);
    species.removeAgent(agent1);
    species.addAgent(agent3);
    
    // Total members over time should count all additions
    EXPECT_EQ(species.getTotalMembersOverTime(), 3);
    
    delete agent1;
    delete agent2;
    delete agent3;
}

TEST_F(SpeciesTest, DrawnFlag) {
    NEAT::Species species(1);
    
    EXPECT_FALSE(species.isDrawn());
    
    species.setDrawn(true);
    EXPECT_TRUE(species.isDrawn());
    
    species.setDrawn(false);
    EXPECT_FALSE(species.isDrawn());
}

TEST_F(SpeciesTest, UniqueIds) {
    NEAT::Species species1(1);
    NEAT::Species species2(2);
    NEAT::Species species3(42);
    
    EXPECT_EQ(species1.getId(), 1);
    EXPECT_EQ(species2.getId(), 2);
    EXPECT_EQ(species3.getId(), 42);
}

TEST_F(SpeciesTest, FirstOnEmptySpecies) {
    NEAT::Species species(1);
    
    // First on empty species should return nullptr
    EXPECT_EQ(species.first(), nullptr);
}
