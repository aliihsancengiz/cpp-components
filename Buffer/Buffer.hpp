#pragma once
#include <cstdint>
#include <cstring>
#include <vector>

namespace buffer {

template<typename element_type = std::int8_t>
struct IBuffer
{
    using size_type = std::size_t;
    virtual std::pair<element_type*, size_type> getBuffer() = 0;
};

template<typename element_type = std::uint8_t>
struct DynamicBuffer : IBuffer<element_type>
{
    using size_type = std::size_t;

    DynamicBuffer() = default;
    DynamicBuffer(const element_type buffer[], size_type size)
    {
        for (size_t i = 0; i < size; i++) {
            mBuffer.push_back(buffer[i]);
        }
    }

    std::pair<element_type*, size_type> getBuffer() override
    {
        return {mBuffer.data(), mBuffer.size()};
    }

    std::size_t push(const element_type buffer[], size_type size)
    {
        for (size_t i = 0; i < size; i++) {
            mBuffer.push_back(buffer[i]);
        }
        return size;
    }

    std::size_t push(const std::vector<element_type>& bufferVec)
    {
        mBuffer.push_back(bufferVec);
        return bufferVec.size();
    }

  private:
    std::vector<element_type> mBuffer;
};

}  // namespace buffer