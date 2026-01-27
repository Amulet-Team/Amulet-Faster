#include <string>
#include <string_view>
#include <filesystem>

#include "dll.hpp"

namespace Amulet {

class FasterKVImpl;

class AMULET_FASTER_EXPORT FasterKV {
private:
    FasterKVImpl* _impl;

public:
    FasterKV() = delete;
    FasterKV(std::filesystem::path directory);
    ~FasterKV();

    FasterKV(const FasterKV&) = delete;
    FasterKV(FasterKV&&) = delete;
    FasterKV& operator=(const FasterKV&) = delete;
    FasterKV& operator=(FasterKV&&) = delete;

    std::string get(std::string_view key) const;
    void set(std::string_view key, std::string_view value);
    void remove(std::string_view key);
};

} // namespace Amulet
