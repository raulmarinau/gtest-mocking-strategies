#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;
using ResolverCb = std::function<void(boost::system::error_code ec, tcp::resolver::results_type)>;
using ConnectCb = std::function<void(boost::system::error_code ec, const tcp::endpoint& endpoint)>;
using IoPtr = std::shared_ptr<boost::asio::io_context>;

class IBoostAPI
{
public:
    virtual void resolve(ResolverCb) = 0;
    virtual void connect(tcp::resolver::results_type, ConnectCb) = 0;
};

class BoostAPI : public IBoostAPI
{
public:
    BoostAPI(const IoPtr&);
    void resolve(ResolverCb) override;
    void connect(tcp::resolver::results_type, ConnectCb) override;
private:
    const std::string host_ = "google.com";
    const uint16_t port_ = 22;
    tcp::resolver resolver_;
    tcp::socket socket_;
};

class Client
{
public:
    Client(const IoPtr&);
    virtual void send();
protected:
    void handleResolve(boost::system::error_code ec, tcp::resolver::results_type);
    void handleConnect(boost::system::error_code ec, const tcp::endpoint& endpoint);

    std::unique_ptr<IBoostAPI> bapi_;
    const IoPtr& io_;
};
