#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "timers.h"

//   ____  ____   ___  ____  _   _  ____ _____ ___ ___  _   _ 
//  |  _ \|  _ \ / _ \|  _ \| | | |/ ___|_   _|_ _/ _ \| \ | |
//  | |_) | |_) | | | | | | | | | | |     | |  | | | | |  \| |
//  |  __/|  _ <| |_| | |_| | |_| | |___  | |  | | |_| | |\  |
//  |_|   |_| \_\\___/|____/ \___/ \____| |_| |___\___/|_| \_|

namespace timers
{
    uint16_t wait_stuff2 = 3000;
}

bool ExternalApp::externalApi()
{
    std::cout << __FUNCTION__ << "\n";
    return false;
}

TimedService::TimedService(io_context& io, AppPtr app): dt_(io), app_(app), data_(42)
{}

int TimedService::getData() const
{
    return data_;
}

void TimedService::doStuff()
{
    std::cout << __FUNCTION__ << " waiting for " << timers::wait_stuff << "ms \n";
    dt_.expires_from_now(boost::posix_time::milliseconds(timers::wait_stuff));
    dt_.async_wait(std::bind(&TimedService::stuffExpired, this, std::placeholders::_1));
    while (app_->externalApi())
    {
        data_ = 0;
        dt_.cancel();
    }
}

void TimedService::stuffExpired(const error_code& ec)
{
    std::cout << __FUNCTION__ << "\n";
    if (ec != error::operation_aborted)
    {
        std::cerr << "Oopsie: " << ec.message() << "\n";
    }
    else
    {
        std::cerr << "Timer was cancelled\n";
    }
}

TEST(TimedServiceTest, test1)
{
    io_context io;
    TimedService::AppPtr app = new ExternalApp();
    TimedService ts(io, app);
    ts.doStuff();
    io.run();
}

TEST(TimedServiceTest, test2)
{
    timers::wait_stuff = 100;
    io_context io;
    TimedService::AppPtr app = new ExternalApp();
    TimedService ts(io, app);
    ts.doStuff();
    io.run();
}

TEST(TimedServiceTest, test3)
{
    timers::wait_stuff2 = 1500;
    EXPECT_EQ(timers::wait_stuff, 100);
}

//   _____ _____ ____ _____ 
//  |_   _| ____/ ___|_   _|
//    | | |  _| \___ \ | |  
//    | | | |___ ___) || |  
//    |_| |_____|____/ |_|  

class ExternalAppMock : public ExternalAppInterface
{
public:
    // MOCK_METHOD0(externalApi, bool());
    MOCK_METHOD(bool, externalApi, (), (override));
};

TEST(TimedServiceTest, testTimerCancel)
{
    // SETUP
    timers::wait_stuff = 1000;
    io_context io;
    ::testing::StrictMock<ExternalAppMock> appMock;
    TimedService ts(io, &appMock);
    // EXPECT
    auto& exp1 = EXPECT_CALL(appMock, externalApi()).Times(2).WillRepeatedly(::testing::Return(false));
    // EXPECT_CALL(appMock, externalApi()).After(exp1).WillOnce(::testing::Return(true));
    EXPECT_EQ(0, ts.getData());
    // TRIGGER
    ts.doStuff();
    io.run();
}

TEST(TimedServiceTest, testTimerExpires)
{
    // SETUP
    timers::wait_stuff = 10;
    io_context io;
    ::testing::StrictMock<ExternalAppMock> appMock;
    TimedService ts(io, &appMock);
    // EXPECT
    EXPECT_CALL(appMock, externalApi()).WillRepeatedly(::testing::Return(false));
    EXPECT_EQ(42, ts.getData());
    // TRIGGER
    ts.doStuff();
    io.run();
}
