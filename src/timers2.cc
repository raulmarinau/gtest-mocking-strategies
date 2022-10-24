#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "timers.h"

TEST(TimedServiceTest, otherCppFile)
{
    EXPECT_EQ(timers::wait_stuff, 5000);
    EXPECT_EQ(timers::wait_stuff2, 1500);
}