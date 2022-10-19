#include <gmock/gmock.h>

class ProductionFoo
{
private:
    int Bar(void*);
    FRIEND_TEST(FooTest, BarReturnsZeroOnNull);
};
