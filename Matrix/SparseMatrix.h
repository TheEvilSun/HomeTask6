#pragma once

#include <map>
#include <tuple>
#include <cstdint>
#include <memory>
#include <functional>

namespace Matrix {

uint64_t hash(uint32_t row, uint32_t column) {
    uint64_t result = row;
    return result << (8 * sizeof(uint32_t)) | column;
}

std::tuple<uint32_t, uint32_t> coordinates(uint64_t key) {
    uint32_t row = key >> (sizeof(uint32_t) * 8);
    uint32_t column = key;
    return {row, column};
}

template <typename Type, Type DefaultValue>
class SparseMatrix {
    std::map<uint64_t, Type> m_map;

    class Index {
        uint32_t m_row;
        uint32_t m_column;
        uint64_t m_key;
        std::reference_wrapper<std::map<uint64_t, Type>> m_map;

    public:
        Index(uint32_t row, uint32_t column, std::reference_wrapper<std::map<uint64_t, Type>> map) :
            m_row(row),
            m_column(column),
            m_key(hash(row, column)),
            m_map(map)
        {
        }

        Index& operator=(const Type& value) {
            std::map<uint64_t, Type>& map = m_map.get();
            auto element = map.find(m_key);

            if(element == map.end()) {
                if(value != DefaultValue) {
                    map.insert({m_key, value});
                }
            }
            else {
                if(value != DefaultValue) {
                    element->second = value;
                }
                else {
                    map.erase(element);
                }
            }

            return *this;
        }

        uint32_t row() const {
            return m_row;
        }

        uint32_t column() const{
            return m_column;
        }

        Type value() const {
            std::map<uint64_t, Type>& map = m_map.get();
            auto val = map.find(m_key);
            return val == map.end() ? DefaultValue : val->second;
        }
    };

    class _Index {
        uint32_t m_row;
        std::reference_wrapper<std::map<uint64_t, Type>> m_map;
    public:
        _Index(uint32_t row, std::reference_wrapper<std::map<uint64_t, Type>> map) :
            m_row(row),
            m_map(map)
        {

        }

        Index operator[](uint32_t column) {
            return {m_row, column, m_map};
        }
    };

    class Iterator : public std::iterator<
                         std::bidirectional_iterator_tag,
                         Index,
                         std::ptrdiff_t,
                         Index*,
                         Index&
                         > {

        typename std::map<uint64_t, Type>::iterator m_iterator;
        std::reference_wrapper<std::map<uint64_t, Type>> m_map;
        Index m_index = {0, 0, m_map};

    public:
        Iterator(typename std::map<uint64_t, Type>::iterator iterator, std::reference_wrapper<std::map<uint64_t, Type>> map) :
            m_iterator(iterator),
            m_map(map)
        {
            uint32_t row;
            uint32_t column;
            std::tie(row, column) = coordinates(m_iterator->first);
            m_index = Index(row, column, m_map);
        }

        Iterator(const Iterator& other) : m_iterator(other.m_iterator), m_map(other.m_map) {
            uint32_t row;
            uint32_t column;
            std::tie(row, column) = coordinates(m_iterator->first);
            m_index = Index(row, column, m_map);
        }

        Iterator& operator++() {
            m_iterator++;
            uint32_t row;
            uint32_t column;
            std::tie(row, column) = coordinates(m_iterator->first);
            m_index = Index(row, column, m_map);
            return *this;
        }

        Iterator& operator--() {
            m_iterator--;
            uint32_t row;
            uint32_t column;
            std::tie(row, column) = coordinates(m_iterator->first);
            m_index = Index(row, column, m_map);
            return *this;
        }

        Index& operator*() {
            return m_index;
        }

        Index* operator->() {
            return &m_index;
        }

        bool operator==(const Iterator& other) const {
            return m_iterator == other.m_iterator;
        }

        bool operator !=(const Iterator& other) {
            return m_iterator != other.m_iterator;
        }
    };

public:
    SparseMatrix() {

    }

    size_t size() const {
        return m_map.size();
    }

    bool empty() const {
        return m_map.empty();
    }

    void clear() {
        m_map.clear();
    }

    Iterator begin() {
        return Iterator(m_map.begin(), std::ref(m_map));
    }

    Iterator end() {
        return Iterator(m_map.end(), std::ref(m_map));
    }

    _Index operator[](uint32_t row) {
        return { row,  std::ref(m_map) };
    }
};


}
