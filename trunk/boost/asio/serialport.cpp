/**
 * example code for asio / serialport
 *
 * Reference:
 *  http://boost.2283326.n4.nabble.com/Simple-serial-port-demonstration-with-boost-asio-asynchronous-I-O-td2582657.html
 */
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

class ComListener {
public:
    virtual ~ComListener() {}
    virtual void onReadComplete(const std::string & data) = 0;
};

class ComReader 
    : public boost::enable_shared_from_this<ComReader>
    , protected boost::noncopyable {
public:
    ComReader(boost::asio::io_service & ioService, const std::string & name, ComListener & listener)
        , m_ioService(ioService)
        , m_name(name)
        , m_comListener(listener) {
    }
    ~ComReader() {
        doClose(boost::system::error_code());
    }
    /**
     * @return false if serial port open fail
     * @return true if serial port open success
     */
    bool start() {
        boost::shared_ptr<boost::asio::serial_port> pSerialPort(new boost::asio::serial_port(m_ioService));
        boost::system::error_code ec;
        if (!pSerialPort || pSerialPort->open(m_name, ec)) {
            LOG(ERROR) << "open serial port error.\n";
            return false;
        }
        m_pSerialPort.swap(pSerialPort);
        readOnce();
        return true;
    }
    void stop() {
        if (m_pSerialPort && m_pSerialPort->is_open()) {
            m_ioService.post(boost::bind(&ComReader::doClose
                , shared_from_this()
                , boost::system::error_code()
            ));
        }
    }
private:
    void readOnce() {
        if (m_pSerialPort) {
            m_pSerialPort->async_read_some(boost::asio::buffer(&m_buf[0], sizeof(m_buf) - 1)
                , boost::bind(&ComReader::readComplete
                    , shared_from_this()
                    , boost::asio::placeholders::error
                    , boost::asio::placeholders::bytes_transferred
                )
            );
        }
    }
    
    void readComplete(const boost::system::error_code & ec, size_t bytes_transferred) {
        if (!ec) {
            m_buf[bytes_transferred] = '\0';
            m_comListener.onReadComplete(std::string(m_buf));
            readOnce();
        } else {
            LOG(ERROR) << "reading error\n";
            doClose(ec);
        }
    }
    
    void doClose(const boost::system::error_code & ec) {
        if (m_pSerialPort) {
            if (ec) {
                LOG(ERROR) << "Error : " << ec.message() << "\n";
                LOG(ERROR) << "Close serial port reader\n";
            }
            m_pSerialPort->close();
            m_pSerialPort.reset();
        }
    }
    
    boost::asio::io_service & m_ioService;
    boost::shared_ptr<boost::asio::serial_port> m_pSerialPort;
    char m_buf[4096];
    std::string m_name;
    ComListener & m_comListener;
};

class ComListenerImpl : public ComListener {
public:
    virtual void onReadComplete(const std::string & data) {
        std::cout << "read " << data.length() << " bytes : " << data << std::endl;
    }
};

int main(int argc, char** argv) {
    boost::asio::io_service ioservice;
    ComListenerImpl listener;
    boost::shared_ptr<ComReader> pReader(ioservice, "\\\\.\\COM12", listener);
    if (!pReader || pReader->start() == false) {
        std::cerr << "start com reader failure. quit\n";
        pReader.reset();
        return -1;
    }
    boost::thread t(boost::bind(&boost::asio::io_service::run, &ioservice));
    for (int i = 0; i < 20; i++) {
        boost::this_thread::sleep_for(boost::chrono::seconds(1));
    }
    pReader->stop();
    t.join();
    return 0;
}