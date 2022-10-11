#include <gmock/gmock.h>

class ProductionFoo
{
private:
    int Bar(void*);
    FRIEND_TEST(FooTest, BarReturnsZeroOnNull);
};


// TCP/HTTP CLIENT

class BoostAPI
{
public:
    void connect();
    void resolve();
};

class Client
{
public:

protected:
    std::unique_ptr<BoostAPI> bapi_;
};
