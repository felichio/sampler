#include <vector>

namespace R
{

    template<typename T>
    class OutputStream
    {
        public:
        OutputStream(bool endianess, int dimension);

        std::vector<T>& get_buffer();
        void fill_buffer(uint8_t *begin, uint8_t *end);

        private:
        std::vector<T> m_buffer;
        bool m_endianess;
        int m_dimension;
    };



}