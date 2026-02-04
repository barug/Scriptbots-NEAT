#include <gtest/gtest.h>
#include "helpers.h"
#include <cmath>
#include <vector>
#include <algorithm>

class HelpersTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set a fixed seed for reproducible tests where needed
        srand(42);
    }
};

// Test randf - uniform random float in [a, b)
TEST_F(HelpersTest, RandfBasicRange) {
    for (int i = 0; i < 1000; i++) {
        float val = randf(0.0f, 1.0f);
        EXPECT_GE(val, 0.0f);
        EXPECT_LT(val, 1.0f);
    }
}

TEST_F(HelpersTest, RandfCustomRange) {
    for (int i = 0; i < 1000; i++) {
        float val = randf(5.0f, 10.0f);
        EXPECT_GE(val, 5.0f);
        EXPECT_LT(val, 10.0f);
    }
}

TEST_F(HelpersTest, RandfNegativeRange) {
    for (int i = 0; i < 1000; i++) {
        float val = randf(-5.0f, 5.0f);
        EXPECT_GE(val, -5.0f);
        EXPECT_LT(val, 5.0f);
    }
}

TEST_F(HelpersTest, RandfDistribution) {
    // Check that values are reasonably distributed
    int lower = 0, upper = 0;
    for (int i = 0; i < 10000; i++) {
        float val = randf(0.0f, 1.0f);
        if (val < 0.5f) lower++;
        else upper++;
    }
    // Should be roughly 50/50, allow 5% deviation
    EXPECT_GT(lower, 4500);
    EXPECT_LT(lower, 5500);
}

// Test randi - uniform random int in [a, b)
TEST_F(HelpersTest, RandiBasicRange) {
    for (int i = 0; i < 1000; i++) {
        int val = randi(0, 10);
        EXPECT_GE(val, 0);
        EXPECT_LT(val, 10);
    }
}

TEST_F(HelpersTest, RandiNegativeRange) {
    for (int i = 0; i < 1000; i++) {
        int val = randi(-10, 10);
        EXPECT_GE(val, -10);
        EXPECT_LT(val, 10);
    }
}

TEST_F(HelpersTest, RandiSingleValue) {
    // Range of 1 should always return the same value
    for (int i = 0; i < 100; i++) {
        int val = randi(5, 6);
        EXPECT_EQ(val, 5);
    }
}

TEST_F(HelpersTest, RandiAllValuesReachable) {
    // Check that all values in range can be reached
    std::vector<bool> reached(10, false);
    for (int i = 0; i < 10000; i++) {
        int val = randi(0, 10);
        reached[val] = true;
    }
    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE(reached[i]) << "Value " << i << " was never generated";
    }
}

// Test cap - clamp value between 0 and 1
TEST_F(HelpersTest, CapInRange) {
    EXPECT_FLOAT_EQ(cap(0.5f), 0.5f);
    EXPECT_FLOAT_EQ(cap(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(cap(1.0f), 1.0f);
    EXPECT_FLOAT_EQ(cap(0.25f), 0.25f);
    EXPECT_FLOAT_EQ(cap(0.75f), 0.75f);
}

TEST_F(HelpersTest, CapBelowZero) {
    EXPECT_FLOAT_EQ(cap(-0.1f), 0.0f);
    EXPECT_FLOAT_EQ(cap(-1.0f), 0.0f);
    EXPECT_FLOAT_EQ(cap(-100.0f), 0.0f);
}

TEST_F(HelpersTest, CapAboveOne) {
    EXPECT_FLOAT_EQ(cap(1.1f), 1.0f);
    EXPECT_FLOAT_EQ(cap(2.0f), 1.0f);
    EXPECT_FLOAT_EQ(cap(100.0f), 1.0f);
}

// Test randn - normal distribution
TEST_F(HelpersTest, RandnMeanApproximation) {
    double sum = 0;
    int n = 10000;
    for (int i = 0; i < n; i++) {
        sum += randn(5.0, 1.0);
    }
    double mean = sum / n;
    // Mean should be close to 5.0
    EXPECT_NEAR(mean, 5.0, 0.1);
}

TEST_F(HelpersTest, RandnStdDevApproximation) {
    std::vector<double> values;
    int n = 10000;
    double mu = 0.0;
    double sigma = 2.0;
    
    for (int i = 0; i < n; i++) {
        values.push_back(randn(mu, sigma));
    }
    
    // Calculate sample mean
    double mean = 0;
    for (double v : values) mean += v;
    mean /= n;
    
    // Calculate sample std dev
    double variance = 0;
    for (double v : values) {
        variance += (v - mean) * (v - mean);
    }
    double stddev = sqrt(variance / n);
    
    // Std dev should be close to sigma
    EXPECT_NEAR(stddev, sigma, 0.2);
}

TEST_F(HelpersTest, RandnZeroSigma) {
    // With sigma=0, should always return mu
    for (int i = 0; i < 100; i++) {
        double val = randn(3.14, 0.0);
        EXPECT_DOUBLE_EQ(val, 3.14);
    }
}
