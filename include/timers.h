#pragma once

#include <memory>

#include <boost/system/error_code.hpp>
#include <boost/asio/deadline_timer.hpp>

using namespace boost::system;
using namespace boost::asio;

namespace timers
{
static uint16_t wait_stuff = 5000;
extern uint16_t wait_stuff2;
}

class ExternalAppInterface
{
public:
    virtual bool externalApi() = 0;
};

class ExternalApp : public ExternalAppInterface
{
public:
    bool externalApi() override;
};

class TimedService
{
public:
    using AppPtr = ExternalAppInterface*;
    TimedService(io_context&, AppPtr);
    void doStuff();
    int getData() const;
private:
    void stuffExpired(const error_code&);

    int data_;
    deadline_timer dt_;
    AppPtr app_;
};
