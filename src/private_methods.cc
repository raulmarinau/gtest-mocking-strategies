#include "private_methods.h"

int ProductionFoo::Bar(void* x)
{
    if (x == NULL) return 0;
    return 1;
}

TEST(FooTest, BarReturnsZeroOnNull)
{
    ProductionFoo foo;
    EXPECT_EQ(foo.Bar(NULL), 0);
}
