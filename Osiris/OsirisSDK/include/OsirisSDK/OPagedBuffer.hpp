#pragma once

#include <memory>
#include <cstdint>
#include <utility>
#include <type_traits>
#include <new>
#include <concepts>
#include <cstddef>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OBaseAllocators.h"

/**
 * @brief Concept for objects that can be stored in a paged buffer.
 */
template<typename T>
concept OPagedBufferObject = std::is_trivially_destructible_v<T>
    && alignof(T) <= alignof(std::max_align_t);

/**
 * @brief A fast paged buffer implementation.
 * 
 * This is aimed to create a cache-friendly buffer, where items are stacked and consumed 
 * and cleared, in this order.
 */
template <class Allocator=OBaseAllocators::Default>
class OPagedBuffer : public OMemoryManagedObject<Allocator> 
{
private:
    using Page = std::uint8_t[];
    using ResizingPolicy = OArrayLinearResizingPolicy<4>; // Allocate 4 pages at a time
    using PageArray = OArray<std::unique_ptr<Page>, ResizingPolicy, Allocator>;
    using PageUsageArray = OArray<std::size_t, ResizingPolicy, Allocator>;

public:
    using DispatcherFn = void(*)(const void* eventData, void* context);

    struct alignas(std::max_align_t) Header
    {
        size_t size;
        DispatcherFn dispatcher;
    };

public:

    /**
     * @brief Class constructor.
     * @param pageSize Page size.
     */
    explicit OPagedBuffer(size_t pageSize = 16384);

    /**
     * @brief Class destructor.
     */
    ~OPagedBuffer(); 

    /**
     * @brief Adds an item to the buffer.
     * 
     * @tparam T The type of the object to be added
     * @tparam Args Variadic argument list to be fed to the object constructor.
     * 
     * @param dispatcher function pointer to be used to dispatch the object.
     * @param args Variadic argument list to be fed to the object constructor.
     */
    template<OPagedBufferObject T, typename... Args>
    void emplace(DispatcherFn&& dispatcher, Args&&... args); 

    /**
     * @brief High performance contiguous iteration over the buffer.
     * 
     * @param callback Function to be called for each element in the buffer. 
     */
    template<typename Fn>
    requires std::invocable<Fn, const OPagedBuffer<Allocator>::Header&, const void*>
    void forEach(Fn&& callback) const;

    /**
     * @brief Clears the buffer without deallocating.
     */
    void clear(); 

    /**
     * @brief Number of elements in the buffer.
     */
    size_t size() const;

    /**
     * @cond INTERNAL
     */
private:
    void allocateNewPage(); 

    static size_t alignOffset(size_t baseOffset);

private:
    size_t _pageSize;
    size_t _activePageIndex;
    size_t _currentOffset;
    size_t _totalItems;
    
    PageArray _pages;
    PageUsageArray _pageUsage;
    /**
     * @endcond
     */
};

template <typename Allocator>
inline OPagedBuffer<Allocator>::OPagedBuffer(size_t aPageSize)
        : _pageSize(aPageSize)
        , _activePageIndex(0)
        , _currentOffset(0)
        , _totalItems(0) 
{
    this->allocateNewPage();
}

template <typename Allocator>
inline OPagedBuffer<Allocator>::~OPagedBuffer()
= default;

template <typename Allocator>
template<OPagedBufferObject T, typename... Args>
inline void OPagedBuffer<Allocator>::emplace(DispatcherFn&& aDispatcher, Args&&... aArgs) 
{
    constexpr size_t itemSize = sizeof(T);
    auto recordOffset = alignOffset(_currentOffset);
    auto payloadOffset = recordOffset + sizeof(Header);

    if (payloadOffset > _pageSize || itemSize > _pageSize - payloadOffset) {
        if (recordOffset == 0) {
            throw OEx("OPagedBuffer page is too small for this object");
        }

        _activePageIndex++;
        
        if (_activePageIndex >= _pages.size()) {
            allocateNewPage();
        }

        _currentOffset = 0;
        recordOffset = 0;
        payloadOffset = sizeof(Header);
        if (payloadOffset > _pageSize || itemSize > _pageSize - payloadOffset) [[unlikely]]{
            throw OEx("OPagedBuffer page is too small for this object");
        }
    }

    auto* page = _pages[_activePageIndex].get();
    new (page + recordOffset) Header{itemSize, aDispatcher};

    new (page + payloadOffset) T(std::forward<Args>(aArgs)...);

    _currentOffset = payloadOffset + itemSize;
    _totalItems++;

    // Keep the active page's limit updated
    _pageUsage[_activePageIndex] = _currentOffset;
}

template <typename Allocator>
template <typename Fn>
requires std::invocable<Fn, const typename OPagedBuffer<Allocator>::Header&, const void*>
inline void OPagedBuffer<Allocator>::forEach(Fn&& callback) const 
{
    for (size_t p = 0; p < _pages.size(); ++p) {
        size_t pageOffset = 0;

        const auto pageStart = _pages[p].get();
        while (pageOffset < _pageUsage[p]) {
            pageOffset = alignOffset(pageOffset);

            if (pageOffset >= _pageUsage[p]) break;

            const auto current = pageStart + pageOffset;
            const auto* header = reinterpret_cast<const Header*>(current);
            const auto payloadOffset = pageOffset + sizeof(Header);
            const void* data = pageStart + payloadOffset;

            callback(*header, data);

            pageOffset = payloadOffset + header->size;
        }
    }
}

template <typename Allocator>
inline void OPagedBuffer<Allocator>::clear()
{
    _activePageIndex = 0;
    _currentOffset = 0;
    _totalItems = 0;
    
    for (auto& size : _pageUsage) {
        size = 0;
    }
}

template <typename Allocator>
inline size_t OPagedBuffer<Allocator>::size() const 
{
    return _totalItems; 
}

template <typename Allocator>
inline void OPagedBuffer<Allocator>::allocateNewPage()
{
    _pages.append(std::make_unique<uint8_t[]>(_pageSize));
    _pageUsage.append(0);
}

template <typename Allocator>
inline size_t OPagedBuffer<Allocator>::alignOffset(size_t aAddress)
{
    // Use max_align_t to guarantee safe alignment for any arbitrary type T
    constexpr size_t kAlignment = alignof(std::max_align_t);
    const auto offset = (kAlignment - (aAddress % kAlignment)) % kAlignment;
    return aAddress + offset;
}