#pragma once
#include <string>
namespace ip {
struct Endpoint
{
    Endpoint(std::string address_, std::uint32_t port_) : address(address_), port(port_) {}
    Endpoint() = default;
    std::string address;
    std::uint32_t port;
};

}  // namespace ip
