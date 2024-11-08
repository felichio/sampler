#ifndef _R_BUFFER_MANAGER_HPP
#define _R_BUFFER_MANAGER_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include "Statistics.hpp"
#include "TcpClient.hpp"
#include "Enum.hpp"

namespace R
{
    class RbufferManagerBase
    {
        public:
            RbufferManagerBase(bool endianess /* true for BE */ , uint8_t dimension, size_t type_size, size_t RBUFFER_SIZE);
            virtual ~RbufferManagerBase();
            void push_back_to_stream(const uint8_t value);
            virtual void flush_buffer() = 0;
        protected:
            bool m_endianess;
            uint8_t m_dimension;
            size_t m_rbuffer_size;
            size_t m_type_size;
            std::vector<uint8_t> m_agnostic_stream;
            std::vector<uint8_t> m_agnostic_buffer;
             
            virtual void push_back_concrete_value(uint8_t *begin, uint8_t *end) = 0;
            uint64_t m_bytes_seen;
    };


    template<typename T>
    class RbufferManager : public RbufferManagerBase
    {
        public:
            RbufferManager(bool end, uint8_t dimension, size_t RBUFFER_SIZE, std::ofstream &file, std::unique_ptr<TcpClient> tcp_client, double threshold);
            
            virtual void push_back_concrete_value(uint8_t *begin, uint8_t *end);
            void push_back_to_stream();
            void push_back_to_reservoir(const typename std::vector<T>::iterator &location);
            virtual void flush_buffer();
            std::vector<T>& get_stream();
            std::vector<T>& get_buffer();
            std::ofstream &get_file();
            std::pair<typename std::vector<T>::iterator, std::vector<T>>& get_rejected_buffer_data();
            uint64_t get_stream_size();
            void print_buffer(std::ostream &os);
            
        private:
            std::vector<T> m_stream;
            std::vector<T> m_buffer;
            std::vector<T> m_local_stream_buffer;
            std::pair<typename std::vector<T>::iterator, std::vector<T>> m_rejected_buffer_data;
            Statistics<T> m_Statistics;
            std::ofstream &m_file;

            uint64_t m_local_stream_index;
            uint64_t m_stream_index;
            std::unique_ptr<TcpClient> m_tcp_client;

            double m_threshold;
            
    };


} // R



#endif
