#include <vector>
#include <numeric>

#include <gtest/gtest.h>
#include <boost/array.hpp>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions)
{
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(HelloTest, BoostArray)
{
    boost::array<int, 4> arr = {{1,2,3,4}};
    EXPECT_EQ(arr[0], 1);
}

TEST(HelloTest, AssertFailure)
{
    // GTEST_SKIP();
    std::vector<int> x{11, 12, 13, 14}, y{11, 12, 14};
    ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";
    for (int i = 0; i < x.size(); ++i)
    {
        EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
    }
}

// TEST(HelloTest, DISABLED_ExpectFailure)
TEST(HelloTest, ExpectFailure)
{
    std::vector<int> x{11, 12, 13, 14}, y{11, 12, 14, 13};
    ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";
    for (int i = 0; i < x.size(); ++i)
    {
        EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
    }
}

class MyFixture: public ::testing::Test
{
protected:
    void SetUp() override
    {

    }

    void TearDown() override
    {
    }

    std::vector<int> x{11, 14, 17, 20};
};

TEST_F(MyFixture, MyTest)
{
    EXPECT_EQ(4, x.size());
}

TEST_F(MyFixture, MyTestSum)
{
    EXPECT_LT(40, std::accumulate(x.begin(), x.end(), 0));
}
