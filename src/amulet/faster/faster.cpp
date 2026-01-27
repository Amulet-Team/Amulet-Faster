#include <algorithm>

#include <amulet/faster/faster.hpp>

#include <faster.h>

namespace Amulet {

class FasterKVImpl {
public:
    FasterKVImpl(std::filesystem::path directory)
    {
    }

    // std::string get(std::string_view key)
    //{
    //     return "test";
    // }

    // void set(std::string_view key, std::string_view value)
    //{
    // }

    // void remove(std::string_view key)
    //{
    // }

    std::uint64_t get(std::uint64_t key)
    {
        return 0;
    }

    void set(std::uint64_t key, std::uint64_t value)
    {
    }

    void remove(std::uint64_t key)
    {
    }
};

FasterKV::FasterKV(std::filesystem::path directory)
    : _impl(new FasterKVImpl(directory))
{
}

FasterKV::~FasterKV()
{
    delete _impl;
    _impl = nullptr;
}

// std::string FasterKV::get(std::string_view key) const
//{
//     return _impl->get(key);
// }
//
// void FasterKV::set(std::string_view key, std::string_view value)
//{
//     _impl->set(key, value);
// }
//
// void FasterKV::remove(std::string_view key)
//{
//     _impl->remove(key);
// }

std::uint64_t FasterKV::get(std::uint64_t key) const
{
    return _impl->get(key);
}

void FasterKV::set(std::uint64_t key, std::uint64_t value)
{
    _impl->set(key, value);
}

void FasterKV::remove(std::uint64_t key)
{
    _impl->remove(key);
}

} // namespace Amulet
