#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>

#include <amulet/faster/faster.hpp>

#include <faster.h>

namespace Amulet {
namespace Faster {

    using Status = FASTER::core::Status;

    struct UInt64Key {
        std::uint64_t v;
        std::uint64_t v2;
        std::uint64_t v3;
        std::uint64_t v4;
        std::uint64_t v5;
        std::uint64_t v6;
        std::uint64_t v7;
        std::uint64_t v8;

        UInt64Key(std::uint64_t value)
            : v(value)
            , v2(value)
            , v3(value)
            , v4(value)
            , v5(value)
            , v6(value)
            , v7(value)
            , v8(value)
        {
        }

        UInt64Key(const UInt64Key& other)
            : v(other.v)
            , v2(other.v2)
            , v3(other.v3)
            , v4(other.v4)
            , v5(other.v5)
            , v6(other.v6)
            , v7(other.v7)
            , v8(other.v8)
        {
        }

        inline static constexpr uint32_t size()
        {
            return static_cast<uint32_t>(sizeof(UInt64Key));
        }

        inline FASTER::core::KeyHash GetHash() const
        {
            return FASTER::core::KeyHash { v };
        }

        bool operator==(const UInt64Key& other) const
        {
            return v == other.v;
        }

        bool operator!=(const UInt64Key& other) const
        {
            return v != other.v;
        }
    };

    struct UInt64Value {
        std::uint64_t v;
        std::uint64_t v2;
        std::uint64_t v3;
        std::uint64_t v4;
        std::uint64_t v5;
        std::uint64_t v6;
        std::uint64_t v7;
        std::uint64_t v8;
        UInt64Value()
            : v(0)
            , v2(0)
            , v3(0)
            , v4(0)
            , v5(0)
            , v6(0)
            , v7(0)
            , v8(0)
        {
        }
        UInt64Value(std::uint64_t value)
            : v(value)
            , v2(value)
            , v3(value)
            , v4(value)
            , v5(value)
            , v6(value)
            , v7(value)
            , v8(value)
        {
        }
        inline static constexpr uint32_t size()
        {
            return static_cast<uint32_t>(sizeof(UInt64Value));
        }
        // convenience
        operator std::uint64_t() const { return v; }
    };

    using Key = UInt64Key;
    using Value = UInt64Value;

#ifdef _WIN32
    using Handler = FASTER::environment::ThreadPoolIoHandler;
#else
    using Handler = FASTER::environment::QueueIoHandler;
#endif

    using Disk = FASTER::device::FileSystemDisk<Handler, (1ull << 26)>; // 64 MiB files
    using DB = FASTER::core::FasterKv<Key, Value, Disk>;

    struct ReadContext : public FASTER::core::IAsyncContext {
        using key_t = Key;
        using value_t = Value;

        key_t k;
        value_t* v;
        Status* status;

        ReadContext(key_t key, value_t& value, Status& status)
            : k { key }
            , v { &value }
            , status(&status)
        {
        }

        const Key& key() const { return k; }

        void Get(const value_t& value)
        {
            *v = value;
        }
        void GetAtomic(const value_t& value)
        {
            *v = value;
        }

    protected:
        // Use the helper provided by IAsyncContext to deep-copy this context.
        FASTER::core::Status DeepCopy_Internal(FASTER::core::IAsyncContext*& context_copy) final
        {
            return FASTER::core::IAsyncContext::DeepCopy_Internal(*this, context_copy);
        }
    };

    struct UpsertContext : public FASTER::core::IAsyncContext {
        using key_t = Key;
        using value_t = Value;

        key_t k;
        value_t v;

        UpsertContext(std::uint64_t key, std::uint64_t value)
            : k { key }
            , v(value)
        {
        }

        const Key& key() const { return k; }
        uint32_t value_size() const { return static_cast<uint32_t>(sizeof(value_t)); }

        // Called by FASTER to populate the record's value portion.
        void Put(value_t& dst) { dst = v; }
        bool PutAtomic(value_t& dst)
        {
            dst = v;
            return true;
        }

    protected:
        FASTER::core::Status DeepCopy_Internal(FASTER::core::IAsyncContext*& context_copy) final
        {
            return FASTER::core::IAsyncContext::DeepCopy_Internal(*this, context_copy);
        }
    };

    struct DeleteContext : public FASTER::core::IAsyncContext {
        using key_t = Key;
        using value_t = Value;

        key_t k;

        DeleteContext(std::uint64_t key)
            : k { key }
        {
        }

        const Key& key() const { return k; }
        uint32_t value_size() const { return static_cast<uint32_t>(sizeof(value_t)); }

    protected:
        FASTER::core::Status DeepCopy_Internal(FASTER::core::IAsyncContext*& context_copy) final
        {
            return FASTER::core::IAsyncContext::DeepCopy_Internal(*this, context_copy);
        }
    };

    class FasterKVImpl {
    private:
        std::unique_ptr<DB> _db;

    public:
        FasterKVImpl(std::filesystem::path directory)
        {
            // Convert directory to string and ensure trailing separator is handled by FASTER.
            std::string dir = directory.string();

            // Index table size (power of two).
            uint64_t index_table_size = 1ull << 11;

            // In-memory hybrid log size. Must be at least 32MiB
            uint64_t hlog_mem_size = 1ull << 28;

            // Build index config and create the FasterKv store.
            DB::IndexConfig index_config { index_table_size };
            _db = std::make_unique<DB>(index_config, hlog_mem_size, dir, 0.5);
        }

        ~FasterKVImpl() = default;

        FasterKVImpl(const FasterKVImpl&) = delete;
        FasterKVImpl& operator=(const FasterKVImpl&) = delete;
        FasterKVImpl(FasterKVImpl&&) = delete;
        FasterKVImpl& operator=(FasterKVImpl&&) = delete;

        std::optional<std::uint64_t> get(std::uint64_t key)
        {
            // Start a session on this thread, perform the read, then stop the session.
            _db->StartSession();
            Value value;
            Status status = Status::Pending;
            ReadContext ctx(key, value, status);
            auto on_read_finished = [](IAsyncContext* ctxt, Status result) {
                if (auto* ctx = dynamic_cast<ReadContext*>(ctxt)) {
                    // std::cout << "set status " << static_cast<std::uint32_t>(result) << std::endl;
                    *ctx->status = result;
                }
            };
            // std::cout << "read" << std::endl;
            status = _db->Read(ctx, on_read_finished, 1);
            if (status == Status::Pending) {
                // std::cout << "wait" << std::endl;
                _db->CompletePending(true);
            }
            // std::cout << "stop" << std::endl;
            _db->StopSession();
            // std::cout << "stopped" << std::endl;

            switch (status) {
            case Status::Ok:
                return value.v;
            case Status::NotFound:
                return std::nullopt;
            case Status::Pending:
                throw std::runtime_error { "Unexpected Pending status after CompletePending" };
            case Status::OutOfMemory:
                throw std::bad_alloc {};
            case Status::IOError:
                throw std::runtime_error { "IOError during read operation" };
            case Status::Corruption:
                throw std::runtime_error { "Data corruption detected during read operation" };
            case Status::Aborted:
                throw std::runtime_error { "Read operation was aborted" };
            default:
                throw std::runtime_error { "Unknown status" };
            }
        }

        void set(std::uint64_t key, std::uint64_t value)
        {
            _db->StartSession();
            UpsertContext uc(key, value);
            auto status = _db->Upsert(uc, nullptr, 1);
            if (status == FASTER::core::Status::Pending) {
                _db->CompletePending(true);
            }
            _db->StopSession();
        }

        void remove(std::uint64_t key)
        {
            _db->StartSession();
            DeleteContext dc(key);
            auto status = _db->Delete(dc, nullptr, 1);
            if (status == FASTER::core::Status::Pending) {
                _db->CompletePending(true);
            }
            _db->StopSession();
        }

        void compact() {
            _db->StartSession();
            _db->CompactWithLookup(
                _db->hlog.safe_read_only_address.control(),
                true,
                8,
                false,
                false
            );
            _db->StopSession();
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

    std::optional<std::uint64_t> FasterKV::get(std::uint64_t key)
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

    void FasterKV::compact()
    {
        _impl->compact();
    }

} // namespace Faster
} // namespace Amulet
