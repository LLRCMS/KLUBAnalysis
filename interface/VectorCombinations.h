#ifndef VectorCombinations_h
#define VectorCombinations_h

// -------------------------------------------------------------------------------------------------------------- //
//                                                                                                                //
//   class VectorCombinations                                                                                     //
//                                                                                                                //
//   Class to compute combinations for a vector of any type. Computes n C k, where                                //
//   C = combinations, n = number of elements in the container, k = number of elements in the combination.        //
//   It becomes slow for high n values (n ~ 20).                                                                  //
//                                                                                                                //
//   `combine_all_k`: calculates combinations from 1 <= k <= n.                                                   //
//                                                                                                                //
//   Developed for the implementation of the inclusion method for trigger scale factors.                          //
//   Standalone Python framework: https://github.com/b-fontana/METTriggerStudies                                  //
//                                                                                                                //
//   Author: Bruno Alves (LLR, Ecole Polytechnique, Paris)                                                        //
//   Date  : April 2022                                                                                           //
//   Contact: bruno.alves@cern.ch                                                                                 //
//                                                                                                                //
//   Usage example:                                                                                               //
//                                                                                                                //
//   #include "VectorCombinations.h"                                                                              //
//                                                                                                                //
//   int main ()                                                                                                  //
//   {                                                                                                            //
//     VectorCombinations comb;                                                                                   //
//     std::vector<std::string> strs = {"aaa", "bbb", "ccc", "ddd"};                                              //
//     std::vector< std::vector<std::string> > res = comb.combine_all_k<std::string>(strs).join();                //
//     comb.combine_all_k<std::string>(strs).print();                                                             //
//     return 0;                                                                                                  //
//   }                                                                                                            //
// -------------------------------------------------------------------------------------------------------------- //

#include <iostream>
#include <string>
#include <vector>

class VectorCombinations
{
private:
  template<typename T> using vec = std::vector<T>;
  template<typename T> using vec2 = vec<vec<T>>;
  template<typename T> using vec3 = vec2<vec<T>>;

  // one should not use this algorithm for anything beyond n ~ 20, since is of O(n2^n)
  // reference (+ explanation): https://www.cs.utexas.edu/users/djimenez/utsa/cs3343/lecture25.html
  // n C k (C = combinations)
  template<typename T>
  vec2<T> combine_single_k(const vec<T>& v, const unsigned n, unsigned k)
  {
	if (k>n or k<1) {
	  std::cout << "[VectorCombinations.h] Bad `n` and `k` parameters!" << std::endl;
	  std::exit(0);
	}

	unsigned counter = 0;
	vec2<T> res;

	for (unsigned i=0; i<static_cast<unsigned>(1<<n); i++) {
	  // masking the j'th bit as j goes through all the bits,
	  // count the number of 1 bits.  this is called finding
	  // a population count.
	  counter = 0;
	  for (unsigned j=0; j<32; j++) {
		if (i & (1<<j))
		  counter++;
	  }

	  // if that number is equal to k, prunsigned the combination...
	  if (counter == k) {
		res.push_back(vec<T>());

		//by again going through all the bits indices, 
		//printing only the ones with 1-bits
		for (unsigned j=0;j<32; j++) {
		  if (i & (1<<j)) {
			res.back().push_back(v[j]);
		  }
		}
		
		if (res.back().empty())
		  res.erase(res.end()-1);
	  }
	}
	return res;
  }
  
public:
  VectorCombinations() {}

  template<typename T>
  class VectorCombinationsTransform
  {
  public:
	VectorCombinationsTransform(vec3<T> mTransform_): mTransform(mTransform_) {}

	//join result in two dimensions, one entry for each `k`
	const vec2<T> join(std::string str="_") const
	{
	  vec2<T> v_str;
	  for(auto &x : mTransform) {
		v_str.push_back( vec<T>() );
		for(auto &y : x) {
		  std::string join_specialized;
		  if (std::is_same<T, std::string>::value)	 
			join_specialized = join_str(y, str);
		  else
			throw std::invalid_argument("[VectorCombinationsTransform.h] `join` does not support the provided type.");
		  v_str.back().push_back(join_specialized);
		}
	  }
	  return v_str;
	}

	//flatten to one dimension
	const vec<std::string> flatten(std::string str="_") const
	{
	  vec<std::string> flat;
	  vec2<std::string> res = this->join(str);
	  for(auto &x : res) {
		for(auto &y : x)
		  flat.push_back(y);
	  }
	  return flat;
	}

	//print the result of `join()`
	void print(std::string str="_") const
	{
	  vec2<std::string> res = this->join(str);
	  for(auto &x : res) {
		for(auto &y : x)
		  std::cout << y << " ";
		std::cout << std::endl;
	  }
	}


  private:
	vec3<T> mTransform;

	//`v` is pass-by-value for sorting
	std::string join_str(vec<std::string> v, std::string str) const
	{
	  //make sure the joining is done always in the same order
	  std::sort(v.begin(), v.end());
	  
	  std::string join = "";
	  for(auto &elem : v)
		join += join=="" ? elem : str + elem;
	  return join;
	}

  };

  // n C k (C = combinations)
  template<typename T>
  VectorCombinationsTransform<T> combine_all_k(vec<T> v) {
	const unsigned n = v.size();
	vec3<T> v3;
	for(unsigned k=1; k<=n; ++k)
	  v3.push_back( combine_single_k(v, n, k) );
	VectorCombinationsTransform<T> c(v3);
	return c;
  }
};

#endif // VectorCombinations
