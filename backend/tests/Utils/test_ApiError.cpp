#include <gtest/gtest.h>

#include "../../src/Utils/ApiError.h"

TEST(ApiErrorTest, ApiErrorThrowsRuntimeError) {
    testing::internal::CaptureStdout();
    ApiError error(404, "Not Found");
    EXPECT_EQ(error.code, 404);
    EXPECT_STREQ(error.what(), "Not Found");
}