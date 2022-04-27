#include "../src/util/colors.h"
#include <gtest/gtest.h>

TEST(TestColors, u32ToRgba) {
    const u32 color = 0xffffffff;
    const vec4 colorVec4Compare = vec4(255, 255, 255, 255);
    const vec4 output = vx::util::u32ToRgba(color);

    EXPECT_EQ(output, colorVec4Compare);
}

TEST(TestColors, rgbaToU32) {
    const u32 colorCompare = 0xffffffff;
    const vec4 vecValue = vec4(255, 255, 255, 255);
    const u32 output = vx::util::rgbaToU32(vecValue);
    EXPECT_EQ(output, colorCompare);
}
