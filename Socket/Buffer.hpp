#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace ip {
struct Buffer
{
    Buffer() {}
    Buffer(uint8_t* data, std::size_t size)
    {
        for (std::size_t i = 0; i < size; i++) {
            mData.emplace_back(data[i]);
        }
    }
    Buffer(std::string str)
    {
        for (auto s : str) {
            mData.emplace_back(s);
        }
    }
    auto get()
    {
        return mData;
    }

  private:
    std::vector<std::uint8_t> mData;
};

}  // namespace ip
