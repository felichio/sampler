#include <iostream>
#include <unistd.h>
#include "TcpConnection.hpp"



R::TcpConnection::TcpConnection(R::TcpServer &server, int socketfd)
    : m_tcpServer{server}
    , m_socketfd{socketfd}
    , m_locked{false}
    , m_buffer_pending{true}
{
    std::cout << "TcpConnection created with fd: " << m_socketfd << std::endl;
}


R::TcpConnection::~TcpConnection()
{
    std::cout << "TcpConnection destroyed" << " fd: " << m_socketfd << std::endl;
    close(m_socketfd);
}


void R::TcpConnection::start_reading_thread()
{
    std::cout << "Reading thread started" << std::endl;
    std::thread t(&TcpConnection::fill_buffer, this);
    std::cout << "Thread id: " << t.get_id() << std::endl;
    t.detach();
}

void R::TcpConnection::fill_buffer()
{
    int numOfRead = 0; // number of bytes read in current loop
    int sumOfRead = 0;
    int globalSumOfRead = 0;
    std::cout << "reading " << std::endl;
    while (!m_locked && (numOfRead = read(m_socketfd, m_buffer + sumOfRead, buffer_size - sumOfRead)) > 0)
    {
        sumOfRead += numOfRead; // number of unflushed bytes
        globalSumOfRead += numOfRead; // number of total bytes read
        std::cout << "numOfRead: " << numOfRead << std::endl; 
        std::cout << "sumOfRead: " << sumOfRead << std::endl; 
        std::cout << "globalSumOfRead: " << globalSumOfRead << std::endl; 
        std::cout << "buffer: " << reinterpret_cast<void *>(m_buffer) << std::endl; 
        std::cout << "buffer ofs: " << reinterpret_cast<void *>(m_buffer + sumOfRead)<< std::endl; 
        std::cout << "available sp: " << buffer_size - sumOfRead << std::endl; 
        
        if (m_buffer_pending && globalSumOfRead > 2)
        {
            choose_buffer();
        }
        // static_cast<int>(buffer_size / 64);
        // if (sumOfRead > 10)
        // {
            // first time sumOfRead will equal to globalSumOfRead             

        if (globalSumOfRead > 2)
        {
            if (sumOfRead == globalSumOfRead)
            {
                flush_buffer(m_buffer + 3, m_buffer + sumOfRead);
            }
            else
            {
                flush_buffer(m_buffer, m_buffer + sumOfRead);
            }
            sumOfRead = 0;
        }
        // }


    }
    close(m_socketfd);
    // reset fd to -1 for destructor operation 
    m_socketfd = -1;
    std::cout << "finsihed reading" << std::endl;
}

void R::TcpConnection::flush_buffer(uint8_t *beg, uint8_t *end)
{
    for (uint8_t *it = beg; it != end; ++it)
    {
        m_RbufferManager->push_back_to_stream(*it);
    }
}


void R::TcpConnection::choose_buffer()
{
    uint8_t end = m_buffer[0u];
    uint8_t type = m_buffer[1u];
    uint8_t dimension = m_buffer[2u];
    
    if (end == 0x00 || end == 0x01)
    {
        if (dimension > 0x00 && dimension < 256)
        {
            if (type == TYPE::INT64)
            {
                std::cout << "Constructing rbuffer type: int64_t endianess: " << static_cast<int>(end) << " dimension: " << static_cast<int>(dimension) << std::endl;
                m_RbufferManager.reset(new RbufferManager<int64_t>(static_cast<bool>(end), dimension, r_buffer_size));
                m_buffer_pending = false;
            }
            else if (type == TYPE::INT32)
            {
                std::cout << "Constructing rbuffer type: int32_t endianess: " << static_cast<int>(end) << " dimension: " << static_cast<int>(dimension) << std::endl;
                m_RbufferManager.reset(new RbufferManager<int32_t>(static_cast<bool>(end), dimension, r_buffer_size));
                m_buffer_pending = false;
            }
            else
            {
                m_locked = true;
            }
        }    
        else
        {
            m_locked = true;
        }
    }
    else
    {
        m_locked = true;
    }
}
