#ifndef UNORDEREDMAP_H
#define UNORDEREDMAP_H

/*
    ** class: ordered_map
    ** author: L. Cadamuro (LLR)
    ** date: 25/05/2016
    ** description: container that behaves like std::map but maintains the order of insertion of the objects
    ** templates the type of key and value. NOTE: key must be printable, key and values must be copiable
*/

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>

// using namespace std;

template <class Tkey, class Tvalue> class ordered_map
{
    public:
        ordered_map();
        ordered_map(const ordered_map & omap);
        ~ordered_map();

        Tvalue& at(Tkey key);
        Tvalue& at(int idx);

        const Tvalue& at (Tkey key) const;
        const Tvalue& at (int idx) const;

        Tvalue& back();
        const Tvalue& back() const;

        void append(Tkey key, Tvalue value);
        void insert(int idx, Tkey key, Tvalue value);
        void remove(Tkey key);
        void remove(int idx);

        size_t size() const;

        Tkey key(int idx) const;

        bool has_key(Tkey key);

    private:
        std::vector<Tkey>   keys_;
        std::vector<Tvalue> values_;
        std::unordered_map <Tkey, int> remap_;

        void remap();
};


// ----------------------------------------------------------------
// ------------------ IMPLEMENTATION ------------------------------
template <class Tkey, class Tvalue>
ordered_map<Tkey, Tvalue>::ordered_map()
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
ordered_map<Tkey, Tvalue>::ordered_map(const ordered_map & omap)
{
    keys_   = omap.keys_;
    values_ = omap.values_;
    remap_  = omap.remap_;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
ordered_map<Tkey, Tvalue>::~ordered_map()
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
void ordered_map<Tkey, Tvalue>::append(Tkey key, Tvalue value)
{
    if (find(keys_.begin(), keys_.end(), key) != keys_.end())
    {
        std::cerr << "ordered_map::append - warning: key " << key << " already existing, exiting" << std::endl;
        return;
    }
    keys_.push_back(key);
    values_.push_back(value);
    remap_[key] = keys_.size()-1;
    return;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
void ordered_map<Tkey, Tvalue>::insert(int idx, Tkey key, Tvalue value)
{
    auto itrKey = keys_.begin() + idx;
    auto itrVal = values_.begin() + idx;
    keys_.insert(itrKey, key);
    values_.insert(itrVal, value);
    remap();
    return;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <class Tkey, class Tvalue>
void ordered_map<Tkey, Tvalue>::remove(Tkey key)
{
    int offset = remap_[key];
    keys_.erase(keys_.begin()+offset);
    values_.erase(values_.begin()+offset);
    remap();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <class Tkey, class Tvalue>
void ordered_map<Tkey, Tvalue>::remove(int idx)
{
    keys_.erase(keys_.begin()+idx);
    values_.erase(values_.begin()+idx);
    remap();
}
        
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
size_t ordered_map<Tkey, Tvalue>::size() const
{
    return values_.size();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
Tkey ordered_map<Tkey, Tvalue>::key(int idx) const
{
    return keys_.at(idx);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
Tvalue& ordered_map<Tkey, Tvalue>::at(Tkey key)
{
    return values_.at(remap_.at(key));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
Tvalue& ordered_map<Tkey, Tvalue>::at(int idx)
{
    return values_.at(idx);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
const Tvalue& ordered_map<Tkey, Tvalue>::at(Tkey key) const
{
    return values_.at(remap_.at(key));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
const Tvalue& ordered_map<Tkey, Tvalue>::at(int idx) const
{
    return values_.at(idx);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
Tvalue& ordered_map<Tkey, Tvalue>::back()
{
    return values_.at(size() - 1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
const Tvalue& ordered_map<Tkey, Tvalue>::back() const
{
    return values_.at(size() - 1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
void ordered_map<Tkey, Tvalue>::remap()
{
    remap_.clear();
    for (size_t idx = 0; idx < keys_.size(); idx++)
        remap_[keys_.at(idx)] = (int) idx;
    return;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <class Tkey, class Tvalue>
bool ordered_map<Tkey, Tvalue>::has_key(Tkey key)
{
    return find (keys_.begin(), keys_.end(), key) != keys_.end() ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

// #include "ordered_map.cc"