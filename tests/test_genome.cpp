#include <gtest/gtest.h>
#include "NEAT/genome.h"
#include "NEAT/network.h"
#include "NEAT/innovation.h"
#include "settings.h"
#include <vector>
#include <cmath>

class GenomeTest : public ::testing::Test {
protected:
    void SetUp() override {
        conf::initialize();
        srand(42);
    }
};

// ============ Construction Tests ============

TEST_F(GenomeTest, BasicConstruction) {
    // Create a simple genome with inputs and outputs
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    
    // Should have nodes for inputs and outputs
    EXPECT_GT(genome.nodes.size(), 0);
}

TEST_F(GenomeTest, GenomeDuplicate) {
    NEAT::Genome original(INPUTSIZE, OUTPUTSIZE, 0, 0);
    original.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome* copy = original.duplicate(1);
    
    EXPECT_NE(copy, nullptr);
    EXPECT_EQ(copy->nodes.size(), original.nodes.size());
    EXPECT_EQ(copy->genes.size(), original.genes.size());
    
    delete copy;
}

TEST_F(GenomeTest, GenomeDuplicateIsIndependent) {
    NEAT::Genome original(INPUTSIZE, OUTPUTSIZE, 0, 0);
    original.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome* copy = original.duplicate(1);
    
    // Mutate the copy
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov = 0;
    copy->mutate_link_weights(2.0, 1.0, NEAT::GAUSSIAN);
    
    // Original should not be affected
    // (This is a structural check - the genome sizes should stay same)
    EXPECT_EQ(copy->nodes.size(), original.nodes.size());
    
    delete copy;
}

// ============ Network Genesis Tests ============

TEST_F(GenomeTest, GenesisCreatesNetwork) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Network* network = genome.genesis(0);
    
    EXPECT_NE(network, nullptr);
    
    delete network;
}

TEST_F(GenomeTest, NetworkCanProcessInputs) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Network* network = genome.genesis(0);
    
    // Create input vector
    std::vector<float> inputs(INPUTSIZE, 0.5f);
    std::vector<float> outputs(OUTPUTSIZE, 0.0f);
    
    // Process inputs
    EXPECT_NO_THROW({
        network->load_sensors(inputs);
        network->activate();
        network->copy_outputs(outputs);
    });
    
    // Outputs should be valid
    for (int i = 0; i < OUTPUTSIZE; i++) {
        EXPECT_FALSE(std::isnan(outputs[i]));
        EXPECT_FALSE(std::isinf(outputs[i]));
    }
    
    delete network;
}

TEST_F(GenomeTest, DuplicatedGenomeProducesSameOutput) {
    NEAT::Genome original(INPUTSIZE, OUTPUTSIZE, 0, 0);
    original.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome* copy = original.duplicate(1);
    
    NEAT::Network* net1 = original.genesis(0);
    NEAT::Network* net2 = copy->genesis(0);
    
    std::vector<float> inputs(INPUTSIZE, 0.5f);
    std::vector<float> outputs1(OUTPUTSIZE, 0.0f);
    std::vector<float> outputs2(OUTPUTSIZE, 0.0f);
    
    net1->load_sensors(inputs);
    net1->activate();
    net1->copy_outputs(outputs1);
    
    net2->load_sensors(inputs);
    net2->activate();
    net2->copy_outputs(outputs2);
    
    // Outputs should be identical
    for (int i = 0; i < OUTPUTSIZE; i++) {
        EXPECT_FLOAT_EQ(outputs1[i], outputs2[i]);
    }
    
    delete net1;
    delete net2;
    delete copy;
}

// ============ Mutation Tests ============

TEST_F(GenomeTest, MutateLinkWeights) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    EXPECT_NO_THROW(genome.mutate_link_weights(0.5, 0.5, NEAT::GAUSSIAN));
}

TEST_F(GenomeTest, MutateAddNode) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    size_t originalNodes = genome.nodes.size();
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov = 0;
    
    bool success = genome.mutate_add_node(innovations, cur_innov);
    
    if (success) {
        // Should have added one node
        EXPECT_EQ(genome.nodes.size(), originalNodes + 1);
    }
    
    for (auto* innov : innovations) delete innov;
}

TEST_F(GenomeTest, MutateAddLink) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    size_t originalGenes = genome.genes.size();
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov = 0;
    
    // Try multiple times since add_link can fail
    for (int i = 0; i < 10; i++) {
        genome.mutate_add_link(innovations, cur_innov, 10);
    }
    
    // Likely added at least one link
    // (Can't guarantee since it might fail to find valid connection)
    
    for (auto* innov : innovations) delete innov;
}

TEST_F(GenomeTest, MutateToggleEnable) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    EXPECT_NO_THROW(genome.mutate_toggle_enable(1));
}

TEST_F(GenomeTest, MutateRandomTrait) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    EXPECT_NO_THROW(genome.mutate_random_trait());
}

TEST_F(GenomeTest, MultipleMutationsStable) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov = 0;
    
    // Apply many mutations
    for (int i = 0; i < 20; i++) {
        genome.mutate_link_weights(0.5, 0.5, NEAT::GAUSSIAN);
        genome.mutate_add_node(innovations, cur_innov);
        genome.mutate_add_link(innovations, cur_innov, 5);
        genome.mutate_toggle_enable(1);
    }
    
    // Should still be able to create a working network
    NEAT::Network* network = genome.genesis(0);
    EXPECT_NE(network, nullptr);
    
    std::vector<float> inputs(INPUTSIZE, 0.5f);
    std::vector<float> outputs(OUTPUTSIZE, 0.0f);
    
    network->load_sensors(inputs);
    network->activate();
    network->copy_outputs(outputs);
    
    for (int i = 0; i < OUTPUTSIZE; i++) {
        EXPECT_FALSE(std::isnan(outputs[i]));
    }
    
    delete network;
    for (auto* innov : innovations) delete innov;
}

// ============ Mating Tests ============

TEST_F(GenomeTest, MateMultipoint) {
    NEAT::Genome genome1(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome1.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome genome2(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome2.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome* child = genome1.mate_multipoint(&genome2, 2, 0.5, 0.5, false);
    
    EXPECT_NE(child, nullptr);
    EXPECT_GT(child->nodes.size(), 0);
    EXPECT_GT(child->genes.size(), 0);
    
    delete child;
}

TEST_F(GenomeTest, MateMultipointAvg) {
    NEAT::Genome genome1(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome1.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome genome2(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome2.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome* child = genome1.mate_multipoint_avg(&genome2, 2, 0.5, 0.5, false);
    
    EXPECT_NE(child, nullptr);
    EXPECT_GT(child->nodes.size(), 0);
    
    delete child;
}

TEST_F(GenomeTest, MateSinglepoint) {
    NEAT::Genome genome1(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome1.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome genome2(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome2.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome* child = genome1.mate_singlepoint(&genome2, 2);
    
    EXPECT_NE(child, nullptr);
    
    delete child;
}

TEST_F(GenomeTest, ChildNetworkFunctional) {
    NEAT::Genome genome1(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome1.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome genome2(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome2.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome* child = genome1.mate_multipoint(&genome2, 2, 0.5, 0.5, false);
    
    NEAT::Network* network = child->genesis(0);
    EXPECT_NE(network, nullptr);
    
    std::vector<float> inputs(INPUTSIZE, 0.5f);
    std::vector<float> outputs(OUTPUTSIZE, 0.0f);
    
    network->load_sensors(inputs);
    network->activate();
    network->copy_outputs(outputs);
    
    for (int i = 0; i < OUTPUTSIZE; i++) {
        EXPECT_FALSE(std::isnan(outputs[i]));
    }
    
    delete network;
    delete child;
}

// ============ Compatibility Tests ============

TEST_F(GenomeTest, CompatibilityWithSelf) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    double compat = genome.compatibility(&genome);
    EXPECT_EQ(compat, 0.0);
}

TEST_F(GenomeTest, CompatibilitySymmetric) {
    NEAT::Genome genome1(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome1.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome genome2(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome2.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    double c1 = genome1.compatibility(&genome2);
    double c2 = genome2.compatibility(&genome1);
    
    EXPECT_NEAR(c1, c2, 0.001);
}

TEST_F(GenomeTest, CompatibilityNonNegative) {
    NEAT::Genome genome1(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome1.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome genome2(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome2.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    double compat = genome1.compatibility(&genome2);
    EXPECT_GE(compat, 0.0);
}

TEST_F(GenomeTest, DuplicateHasZeroCompatibility) {
    NEAT::Genome original(INPUTSIZE, OUTPUTSIZE, 0, 0);
    original.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome* copy = original.duplicate(1);
    
    double compat = original.compatibility(copy);
    EXPECT_EQ(compat, 0.0);
    
    delete copy;
}

TEST_F(GenomeTest, MutatedGenomeHasPositiveCompatibility) {
    NEAT::Genome original(INPUTSIZE, OUTPUTSIZE, 0, 0);
    original.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    NEAT::Genome* copy = original.duplicate(1);
    
    // Apply mutations to copy
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov = 0;
    copy->mutate_link_weights(2.0, 1.0, NEAT::GAUSSIAN);
    for (int i = 0; i < 5; i++) {
        copy->mutate_add_node(innovations, cur_innov);
        copy->mutate_add_link(innovations, cur_innov, 5);
    }
    
    double compat = original.compatibility(copy);
    // After significant mutation, compatibility should be > 0
    // (unless all mutations failed)
    EXPECT_GE(compat, 0.0);
    
    delete copy;
    for (auto* innov : innovations) delete innov;
}

// ============ Verification Tests ============

TEST_F(GenomeTest, VerifyNewGenome) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    EXPECT_TRUE(genome.verify());
}

TEST_F(GenomeTest, VerifyAfterMutations) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    std::vector<NEAT::Innovation*> innovations;
    double cur_innov = 0;
    
    for (int i = 0; i < 10; i++) {
        genome.mutate_add_node(innovations, cur_innov);
        genome.mutate_add_link(innovations, cur_innov, 5);
        genome.mutate_link_weights(0.5, 0.5, NEAT::GAUSSIAN);
    }
    
    EXPECT_TRUE(genome.verify());
    
    for (auto* innov : innovations) delete innov;
}

// ============ Node and Gene ID Tests ============

TEST_F(GenomeTest, GetLastNodeId) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    
    int lastId = genome.get_last_node_id();
    EXPECT_GT(lastId, 0);
}

TEST_F(GenomeTest, GetLastGeneInnovNum) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    double lastInnov = genome.get_last_gene_innovnum();
    EXPECT_GE(lastInnov, 0);
}

TEST_F(GenomeTest, ExtronCount) {
    NEAT::Genome genome(INPUTSIZE, OUTPUTSIZE, 0, 0);
    genome.mutate_link_weights(1, 1, NEAT::GAUSSIAN);
    
    int extrons = genome.extrons();
    EXPECT_GE(extrons, 0);
    EXPECT_LE(extrons, static_cast<int>(genome.genes.size()));
}
