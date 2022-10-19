#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mock_injection.h"

//    ____ ___  ____  _____ 
//   / ___/ _ \|  _ \| ____|
//  | |  | | | | | | |  _|  
//  | |__| |_| | |_| | |___ 
//   \____\___/|____/|_____|

BoostAPI::BoostAPI(const IoPtr& io) : resolver_(*io), socket_(*io)
{
    std::cout << __FUNCTION__ << "\n";
}

void BoostAPI::resolve(ResolverCb callback)
{
    std::cout << __FUNCTION__ << "\n";
    resolver_.async_resolve(
        host_,
        std::to_string(port_),
        callback);
}

void BoostAPI::connect(tcp::resolver::results_type IPResult, ConnectCb callBack)
{
    std::cout << __FUNCTION__ << "\n";
    boost::asio::async_connect(
        socket_,
        IPResult,
        callBack);

    // we end here
    socket_.shutdown(tcp::socket::shutdown_both);
    socket_.close();
}

Client::Client(const IoPtr& io): io_(io)
{
}

void Client::send()
{
    std::cout << __FUNCTION__ << "\n";
    bapi_ = std::make_unique<BoostAPI>(io_);
    bapi_->resolve(std::bind(&Client::handleResolve, this, std::placeholders::_1, std::placeholders::_2));
}

void Client::handleResolve(boost::system::error_code ec, tcp::resolver::results_type IPResult)
{
    std::cout << __FUNCTION__ << "\n";
    if (ec)
    {
        std::cout << "Oops, I received ec: " << ec.message() << "\n";
        return;
    }

    bapi_->connect(IPResult, std::bind(&Client::handleConnect, this, std::placeholders::_1, std::placeholders::_2));
}

void Client::handleConnect(boost::system::error_code ec, const tcp::endpoint& endpoint)
{
    std::cout << __FUNCTION__ << "\n";
    if (ec)
    {
        std::cout << "Oops, I received ec: " << ec.message() << "\n";
        return;
    }
    std::cout << "I connected to: " << endpoint << "\n";
}

//   _____ _____ ____ _____ 
//  |_   _| ____/ ___|_   _|
//    | | |  _| \___ \ | |  
//    | | | |___ ___) || |  
//    |_| |_____|____/ |_|

class ClientTest: public ::testing::Test
{
protected:
    void SetUp() override
    {
        io_ = std::make_shared<boost::asio::io_context>();
        client_ = std::make_shared<Client>(io_);
        io_->run();
    }

    void TearDown() override
    {
        io_->stop();
    }

    IoPtr io_;
    std::shared_ptr<Client> client_;
};

TEST_F(ClientTest, test1)
{
    client_->send();
}

class BoostAPIMock : public IBoostAPI
{
public:
    MOCK_METHOD1(resolve, void(ResolverCb));
    MOCK_METHOD2(connect, void(tcp::resolver::results_type, ConnectCb));
};

class TestableClient : public Client
{
public:
    TestableClient(const IoPtr& io): Client(io)
    {
        this->bapi_ = std::make_unique<BoostAPIMock>();
    }

    void send() override;

    BoostAPIMock* getBapiMock()
    {
        return dynamic_cast<BoostAPIMock*>(this->bapi_.get());
    }
};

void TestableClient::send()
{
    std::cout << __FUNCTION__ << "\n";
    // bapi_ = std::make_unique<BoostAPI>(io_);
    bapi_->resolve(std::bind(&TestableClient::handleResolve, this, std::placeholders::_1, std::placeholders::_2));
}

class TestableClientTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        io_ = std::make_shared<boost::asio::io_context>();
        client_ = std::make_shared<TestableClient>(io_);
    }

    void TearDown() override
    {
        // getBapiMock() generates a pointer to a mock that's not deleted
        ::testing::Mock::VerifyAndClearExpectations(client_->getBapiMock());
        ::testing::Mock::VerifyAndClear(client_->getBapiMock());
        ::testing::Mock::AllowLeak(client_->getBapiMock());
    }
    IoPtr io_;
    std::shared_ptr<TestableClient> client_;
};

TEST_F(TestableClientTest, testFullConnection)
{
    EXPECT_CALL(*(client_->getBapiMock()), resolve(::testing::_))
        .WillOnce(::testing::Invoke(
            [this](ResolverCb callback)
            {
                using namespace boost::system;
                error_code succes_erc(errc::success, system_category());
                callback(succes_erc, tcp::resolver::results_type());
            }));

    EXPECT_CALL(*(client_->getBapiMock()), connect(::testing::_, ::testing::_))
        .WillOnce(::testing::Invoke(
            [this](tcp::resolver::results_type endpoints, ConnectCb callback)
            {
                using namespace boost::system;
                error_code succes_erc(errc::success, system_category());
                boost::asio::ip::address bip = boost::asio::ip::make_address("127.0.0.1");
                tcp::endpoint ep(bip, 1337);
                callback(succes_erc, ep);
            }));
    client_->send();
    io_->run();
}

TEST_F(TestableClientTest, testResolveError)
{
    EXPECT_CALL(*(client_->getBapiMock()), resolve(::testing::_))
        .WillOnce(::testing::Invoke(
            [this](ResolverCb callback)
            {
                using namespace boost::system;
                error_code bad_erc(errc::io_error, system_category());
                callback(bad_erc, tcp::resolver::results_type());
            }));

    client_->send();
    io_->run();
}
