#include <cstring>
#include <unistd.h>
#include <vector>
#include "ValueManager.hpp"

R::ValueManager::ValueManager(TcpClient &tcpclient, std::string filename)
    : m_TcpClient{tcpclient}
    , m_filename{filename}
    , m_ifstream{m_filename, std::ios::in}
    , m_endianess{false}
    , m_dimension{0x01}
    , m_headcsv{true}
{
    if (m_ifstream.is_open())
    {
        std::cout << "input file opened successfully: " << m_filename << std::endl;
        std::string line;
        getline(m_ifstream, line);
        std::cout << line << std::endl;
        size_t pre = 0;
        size_t pos = std::string::npos;
        while ((pos = line.find(',', pre)) != std::string::npos)
        {
            m_dimension++;
            pre = pos + 1;
        }
        m_ifstream.seekg(0);
        std::cout << "dimension: " << static_cast<int32_t>(m_dimension) << std::endl;
    }
}


void R::ValueManager::start_streaming()
{
    std::cout << "start streaming" << std::endl;

    uint8_t init_buffer[3];
    init_buffer[0] = m_endianess ? R::ENDIANESS::BE : R::ENDIANESS::LE;
    init_buffer[1] = R::TYPE::INT32;
    init_buffer[2] = m_dimension;
    std::cout << static_cast<uint32_t>(init_buffer[2]) << std::endl;

    std::vector<int32_t> lestore {41451, 8418414, -591941, 41999191, 33, 44, 19, 13};
    m_valuebuffer.insert(m_valuebuffer.end(), init_buffer, init_buffer + 3);
    std::vector<int32_t> chunk_buffer;
    chunk_buffer.reserve(chunk_size);

    std::string head;
    if (m_headcsv) // skip column name
    {
        getline(m_ifstream, head);
    }

    while (!m_ifstream.eof())
    {
        read_chunk_from_file(chunk_buffer);
        std::cout << "copy to send" << std::endl;
        copy_to_send_buffer(chunk_buffer);
        chunk_buffer.clear();
    }

}

void R::ValueManager::read_chunk_from_file(std::vector<int32_t> &store)
{
    uint32_t current_read_size = 0;
    std::string line;
    while (current_read_size < chunk_size && getline(m_ifstream, line) )
    {
        std::cout << line << std::endl;
        size_t pos = std::string::npos;
        size_t pre = 0;
        while((pos = line.find(',', pre)) != std::string::npos)
        {
            store.push_back(std::stoi(line.substr(pre, pos - pre)));
            pre = pos + 1;
        }
        store.push_back(std::stoi(line.substr(pre, pos)));
        current_read_size++;
    }
}

void R::ValueManager::copy_to_send_buffer(std::vector<int32_t> &values)
{
    uint8_t *byte_index = reinterpret_cast<uint8_t*>(&values[0]);
    size_t type_size = sizeof(int32_t);
    if (m_endianess)
    {
        for (int32_t i = 0, j = type_size - 1; j < values.size() * type_size; i += type_size, j += type_size)
        {
            int32_t index = j;
            while (index >= i)
            {
                m_valuebuffer.push_back(byte_index[index--]);
            }
        }
    }
    else
    {
        for (uint32_t i = 0; i < values.size() * sizeof (int32_t); i++)
        {
            m_valuebuffer.push_back(byte_index[i]);
        }
    }

    std::cout << "m_valuebuffer size: " << m_valuebuffer.size() << std::endl;
    memcpy(m_TcpClient.get_send_buffer(), &m_valuebuffer[0], m_valuebuffer.size());
    m_TcpClient.send(m_valuebuffer.size());
    m_valuebuffer.clear();
}
