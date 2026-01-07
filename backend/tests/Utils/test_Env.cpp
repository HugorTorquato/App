#include <gtest/gtest.h>

#include "../../src/Utils/Env.h"

TEST(EnvTest, GetEnvVariable) {
    // Set an environment variable for testing
    setenv("TEST_ENV_VAR", "test_value", 1);

    // Test retrieval of the environment variable
    std::string value1 = Env::getEnv("TEST_ENV_VAR");
    EXPECT_EQ(value1, "test_value");

    // Test retrieval of the environment variable
    std::string value2 = Env::getEnv("TEST_ENV_VAR", "default_value");
    EXPECT_EQ(value2, "test_value");

    // Test retrieval of a non-existent environment variable with default value
    std::string defaultValue = Env::getEnv("NON_EXISTENT_VAR", "default_value");
    EXPECT_EQ(defaultValue, "default_value");
}

TEST(EnvTest, GetEnvVariableWithoutDefault) {
    // Set an environment variable for testing
    setenv("ANOTHER_TEST_ENV_VAR", "another_test_value", 1);

    // Test retrieval of the environment variable
    std::string value = Env::getEnv("ANOTHER_TEST_ENV_VAR");
    EXPECT_EQ(value, "another_test_value");

    // Test retrieval of a non-existent environment variable without default value
    std::string emptyValue = Env::getEnv("YET_ANOTHER_NON_EXISTENT_VAR");
    EXPECT_EQ(emptyValue, "");
}
