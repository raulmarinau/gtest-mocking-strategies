#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "free_functions.h"

// Free functions / C-like APIs

// ************* libfoo.so ***************
bool foo(const char* bar, int baz)
{
    printf("%s are %d mere", bar, baz);
    return (strlen(bar) == baz);
}
// ***************************************

class MyClass
{
public:
    void doStuff();
};

void MyClass::doStuff()
{
    if (foo("Ana", 3)) std::cout << "Ana are destule mere\n";
    else std::cout << "Nu sunt destule mere\n";
}


// SOLUTION 1
// create a class to interface with the library

class FooInterface
{
public:
    virtual bool useFoo(const char*, int) = 0;
};


class Foo : public FooInterface
{
public:
    bool useFoo(const char* str, int i) override
    {
        return foo(str, i);
    }
};


class FooMock : public FooInterface
{
public:
    MOCK_METHOD(bool, useFoo, (const char*, int), (override));
};


class MyBetterDesignedClass
{
public:
    MyBetterDesignedClass(std::shared_ptr<FooInterface> const&);
    void doStuff();

private:
    std::shared_ptr<FooInterface> myFoo;
};

MyBetterDesignedClass::MyBetterDesignedClass(std::shared_ptr<FooInterface> const& fooPtr) : myFoo(fooPtr)
{
}

void MyBetterDesignedClass::doStuff()
{
    if (myFoo->useFoo("Ana", 3)) std::cout << "Ana are destule mere\n";
    else std::cout << "Nu sunt destule mere\n";
}

class FooParametrizedFixture : public ::testing::TestWithParam<bool>
{};

TEST_P(FooParametrizedFixture, FooVariants)
{
    auto foo = std::make_shared<FooMock>();
    MyBetterDesignedClass bdc(foo);
    bool returnValue = GetParam();

    EXPECT_CALL(*foo, useFoo("Ana", 3)).WillOnce(::testing::Return(returnValue));
    bdc.doStuff();
}

INSTANTIATE_TEST_CASE_P(
    FooTest,
    FooParametrizedFixture,
    ::testing::Values(
            true, false
    ));


// SOLUTION 2
// templates

class FooTemplateParametrizedFixture : public ::testing::TestWithParam<bool>
{};

TEST_P(FooTemplateParametrizedFixture, FooVariants)
{
    MyBetterDesignedTemplateClass<FooMock> bdc;
    bool returnValue = GetParam();

    EXPECT_CALL(*bdc.getFoo(), useFoo("Ana", 3)).WillOnce(::testing::Return(returnValue));
    bdc.doStuff();
}

INSTANTIATE_TEST_CASE_P(
    FooTest,
    FooTemplateParametrizedFixture,
    ::testing::Values(
            true, false
    ));

