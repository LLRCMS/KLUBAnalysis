#ifndef NESTEDMAP_H
#define NESTEDMAP_H

/*
  class: nested_map
  author: Bruno Alves (LLR)
  date: April 2022
  description: nested unordered map with templated depth
  reference: https://stackoverflow.com/questions/1500208/nested-stdmaps
  usage:
  - read-only: map("key1", "key2", ..., "keyN")
  - write: map.set("key1", "key2", ..., "keyN")
*/

#include <iostream>
#include <unordered_map>

template <typename KEY_T, typename VAL_T, unsigned int DEPTH>
  struct nested_map_def
{
 public:
  typedef std::unordered_map<KEY_T,
   	typename nested_map_def<KEY_T, VAL_T, DEPTH-1>::map_type> map_type;
};

template<typename KEY_T, typename VAL_T>
  struct nested_map_def<KEY_T, VAL_T, 1>
{
 public:
  typedef std::unordered_map<KEY_T, VAL_T> map_type;
};

// template<typename KEY_T, typename VAL_T>
//   struct nested_map_def<KEY_T, VAL_T, 0>
// {
//   //reference: https://stackoverflow.com/questions/22791969/how-to-make-nested-maps-of-variable-depth
//   static_assert(false, "The `nested_map_def` struct should not be called with a depth of zero.");
// };

template <typename KEY_T, typename VAL_T, unsigned int DEPTH>
  class nested_map
{
 public:
  auto operator[](KEY_T key) const -> void
	{
	  std::cout << "The `[]` operator was not implemented. Use `()` instead."
				<< std::endl;
	  std::exit(0);
	}

  //Read-only
  template <class ... Ts>
	auto operator()(KEY_T key, Ts... args) const -> const VAL_T&
	{
	  if(sizeof...(args)+1 != DEPTH) {
		std::cout << "Number of arguments does not match depth of nested map."
				  << std::endl;
		std::exit(0);
	  }
	  return internal_read_only_call(catch_out_of_range(m, key), args...);
	}

  //Modifier
  template <class ... Ts>
	auto set(VAL_T val, KEY_T key, Ts... args) -> void
  {
	if(sizeof...(args)+1 != DEPTH) {
	  std::cout << "Number of arguments does not match depth of nested map."
				<< std::endl;
	  std::exit(0);
	}
	internal_modifier_call(m[key], val, args...);
  }

  template <class ... Ts>
	auto contains(KEY_T key, Ts... args) -> bool
  {
	try {
	  (*this)(key, args...);
	}
	catch (std::out_of_range const&) {
	  return false;
	}
	return true;
  }


 private:
  typename nested_map_def<KEY_T, VAL_T, DEPTH>::map_type m;

  //catch situations where entry does not exist
  template <class U>
	auto& catch_out_of_range(const U& map, KEY_T key) const {
	try {
	  return map.at(key);
	}
	catch (std::out_of_range e) {
	  std::string mess;
	  mess = "The entry was not set (key = " + key + ").\n";
	  if(map.size() != 0) {
		  mess += "The following keys were already set";
		  mess += " in the same level:\n";
		  for (auto const &pair: map) {
			mess += "- " + pair.first + "\n";
		  }
	  }
	  else
		mess += "Currently no keys are defined for the offending map nesting level.\n";
	  throw std::out_of_range(mess);
	}
  }

  //Read-only
  template <class U>
	auto internal_read_only_call(const U& map, KEY_T key) const -> const VAL_T&
	{
	  return catch_out_of_range(map, key);
	}
  template <class U, class... RestKeys>
	auto internal_read_only_call(const U& map, KEY_T key, RestKeys... args) const -> const VAL_T&
	{
	  return internal_read_only_call(catch_out_of_range(map, key), args...);
	}

  //Modifier
  template <class U>
	auto internal_modifier_call(U& map, VAL_T val, KEY_T key) -> void
	{
	  map.emplace(key, val); // map.insert( std::make_pair( key, val ) ) for C++<11
	}  
  template <class U, class... RestKeys>
	auto internal_modifier_call(U& map, VAL_T val, KEY_T key, RestKeys... args) -> void
  {
	internal_modifier_call(map[key], val, args...);
  }
  
};

#endif
