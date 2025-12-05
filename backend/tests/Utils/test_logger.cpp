#include <gtest/gtest.h>
#include "../../src/Utils/Logger.h"

TEST(LoggerTest, InfoLogsMessage) {
    testing::internal::CaptureStdout();
    Logger::info("Test message");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "[INFO] Test message\n");
}