#include <algorithm>
#include <functional>
#include <memory>

#include <amulet/faster/faster.hpp>

#include <faster.h>

#ifdef _WIN32
typedef FASTER::environment::ThreadPoolIoHandler handler_t;
#else
typedef FASTER::environment::QueueIoHandler handler_t;
#endif

namespace Amulet {

struct ShallowKey {
    std::uint64_t v;

    ShallowKey(std::uint64_t value)
        : v(value)
    {
    }

    ShallowKey(const ShallowKey& other)
        : v(other.v)
    {
    }

    inline static constexpr uint32_t size()
    {
        return static_cast<uint32_t>(sizeof(ShallowKey));
    }

    inline FASTER::core::KeyHash GetHash() const
    {
        return FASTER::core::KeyHash { v };
    }

    bool operator==(const ShallowKey& other) const { 
        return v == other.v; 
    }

    bool operator!=(const ShallowKey& other) const
    {
        return v != other.v;
    }
};

struct Value {
    std::uint64_t v;
    Value()
        : v(0)
    {
    }
    Value(std::uint64_t x)
        : v(x)
    {
    }
    inline static constexpr uint32_t size()
    {
        return static_cast<uint32_t>(sizeof(Value));
    }
    // convenience
    operator std::uint64_t() const { return v; }
};

struct ReadContext : public FASTER::core::IAsyncContext {
    using key_t = ShallowKey;
    using value_t = Value;

    key_t k;
    value_t value;
    bool found { false };

    ReadContext(std::uint64_t key)
        : k { key }
        , value()
        , found(false)
    {
    }

    // Required by FASTER
    const ShallowKey& key() const { return k; }

    // Called by FASTER on successful read (synchronous path).
    void Get(const value_t& v)
    {
        value = v;
        found = true;
    }
    void GetAtomic(const value_t& v)
    {
        value = v;
        found = true;
    }

protected:
    // Use the helper provided by IAsyncContext to deep-copy this context.
    FASTER::core::Status DeepCopy_Internal(FASTER::core::IAsyncContext*& context_copy) final
    {
        return FASTER::core::IAsyncContext::DeepCopy_Internal(*this, context_copy);
    }
};

struct UpsertContext : public FASTER::core::IAsyncContext {
    using key_t = ShallowKey;
    using value_t = Value;

    key_t k;
    value_t v;

    UpsertContext(std::uint64_t key, std::uint64_t value)
        : k { key }
        , v(value)
    {
    }

    const ShallowKey& key() const { return k; }

    // Called by FASTER to populate the record's value portion.
    void Put(value_t& dst) { dst = v; }
    bool PutAtomic(value_t& dst)
    {
        dst = v;
        return true;
    }

    // Size of the value being written.
    uint32_t value_size() const { return static_cast<uint32_t>(sizeof(value_t)); }

protected:
    FASTER::core::Status DeepCopy_Internal(FASTER::core::IAsyncContext*& context_copy) final
    {
        return FASTER::core::IAsyncContext::DeepCopy_Internal(*this, context_copy);
    }
};

struct DeleteContext : public FASTER::core::IAsyncContext {
    using key_t = ShallowKey;
    using value_t = Value;
    key_t k;
    DeleteContext(std::uint64_t key)
        : k { key }
    {
    }
    const ShallowKey& key() const { return k; }
    uint32_t value_size() const { return static_cast<uint32_t>(sizeof(value_t)); }

protected:
    FASTER::core::Status DeepCopy_Internal(FASTER::core::IAsyncContext*& context_copy) final
    {
        return FASTER::core::IAsyncContext::DeepCopy_Internal(*this, context_copy);
    }
};

class FasterKVImpl {
public:
    // Use FASTER's FileSystemDisk with platform-appropriate IO handler and 1MiB segments.
    using disk_t = FASTER::device::FileSystemDisk<handler_t, (1ull << 20)>;

    // store_t uses uint64_t keys and Value values (not primitive uint64_t).
    using store_t = FASTER::core::FasterKv<ShallowKey, Value, disk_t>;

private:
    std::unique_ptr<store_t> _store;

public:
    FasterKVImpl(std::filesystem::path directory)
    {
        // Convert directory to string and ensure trailing separator is handled by FASTER.
        std::string dir = directory.string();

        // Index table size (power of two). 1<<20 = 1,048,576 buckets.
        uint64_t index_table_size = 2ull << 20;

        // In-memory hybrid log size
        uint64_t hlog_mem_size = 1ull << 30;

        // Build index config and create the FasterKv store.
        typename store_t::IndexConfig index_config { index_table_size };
        _store = std::make_unique<store_t>(index_config, hlog_mem_size, dir);
    }

    ~FasterKVImpl() = default;

    std::optional<std::uint64_t> get(std::uint64_t key)
    {
        // Start a session on this thread, perform the read, then stop the session.
        _store->StartSession();
        ReadContext ctx(key);
        auto status = _store->Read(ctx, nullptr, /*monotonic_serial_num=*/1);
        _store->StopSession();

        using Status = FASTER::core::Status;
        if (status == Status::Ok && ctx.found) {
            return ctx.value.v;
        }

        // Not found or error -> return 0 (the Python binding currently expects a uint64_t).
        // Consider changing to an optional/exception in future if you need to signal "not found".
        return std::nullopt;
    }

    void set(std::uint64_t key, std::uint64_t value)
    {
        _store->StartSession();
        UpsertContext uc(key, value);
        _store->Upsert(uc, nullptr, /*monotonic_serial_num=*/1);
        _store->StopSession();
    }

    void remove(std::uint64_t key)
    {
        _store->StartSession();
        DeleteContext dc(key);
        _store->Delete(dc, nullptr, /*monotonic_serial_num=*/1);
        _store->StopSession();
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

std::optional<std::uint64_t> FasterKV::get(std::uint64_t key) const
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
