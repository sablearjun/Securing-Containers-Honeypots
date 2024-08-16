#ifndef FLEET_LRUCACHE_H
#define FLEET_LRUCACHE_H

#include <cstddef>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <stdexcept>

template <typename ObjType, std::size_t SIZE = 1000>
class LRUCache {
public:
    LRUCache() {
        static_assert(SIZE, "LRUCache size can not be set to value 0");
        umap.reserve(SIZE);
    }

    std::size_t size() {
        return umap.size();
    }

    std::size_t capacity() {
        return SIZE;
    }

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;
    std::shared_ptr<ObjType> find(const std::string& key) {
        {
            std::lock_guard<std::mutex> guard(mtx);
            auto uit = umap.find(key);
            if (uit != umap.end()) {
                if (!uit->second->obj->isStale(key)) {
                    itr_list.erase(uit->second->list_itr);
                    uit->second->list_itr = itr_list.insert(itr_list.end(), uit);
                    return uit->second->obj;
                } else {
                    itr_list.erase(uit->second->list_itr);
                    umap.erase(uit);
                }
            }
        }

        auto ce = std::make_unique<CacheEntry>();
        ce->obj = std::make_shared<ObjType>();
        if (!ce->obj->update(key)) {
            // update failed, dont create new entry
            throw std::runtime_error("");
        }

        {
            std::lock_guard<std::mutex> guard(mtx);
            auto uit = umap.find(key);
            if (uit != umap.end()) {
                // two or more threads were calling update for same entry
                uit->second->obj = ce->obj;
                return uit->second->obj;
            }

            if (itr_list.size() >= SIZE) {
                // Cache full, evict an LRU entry
                umap.erase(itr_list.front());
                itr_list.pop_front();
            }

            auto rp = umap.emplace(std::make_pair(key, std::move(ce)));
            rp.first->second->list_itr = itr_list.insert(itr_list.end(), rp.first);
            return rp.first->second->obj;
        }
    }

private:
    struct CacheEntry;
    typedef std::unordered_map<std::string, std::unique_ptr<CacheEntry>> UMAP;
    struct CacheEntry {
        std::shared_ptr<ObjType> obj;
        typename std::list<typename UMAP::iterator>::iterator list_itr;
    };
    UMAP umap;
    std::list<typename UMAP::iterator> itr_list;
    std::mutex mtx;
};

#endif //FLEET_LRUCACHE_H
