#ifndef ENUMVECTOR_HPP
#define ENUMVECTOR_HPP

#include <vector>
#include <utility>
#include <ostream>
#include <algorithm>
#include "myutil.h++"

static const char ENUMVECTOR_HPP_SCCS_ID[] __attribute__((used)) = "@(#)enumvector.h++: $Id$";

template<typename E,typename T> class Enumvector: std::vector<T> {
public:
  using typename std::vector<T>::value_type;
  using typename std::vector<T>::allocator_type;
  using typename std::vector<T>::reference;
  using typename std::vector<T>::const_reference;
  using typename std::vector<T>::pointer;
  using typename std::vector<T>::const_pointer;
  using typename std::vector<T>::iterator;
  using typename std::vector<T>::const_iterator;
  using typename std::vector<T>::reverse_iterator;
  using typename std::vector<T>::const_reverse_iterator;
  using typename std::vector<T>::difference_type;
  typedef E size_type;
  using std::vector<T>::begin;
  using std::vector<T>::end;
  using std::vector<T>::rbegin;
  using std::vector<T>::rend;
  using std::vector<T>::cbegin;
  using std::vector<T>::cend;
  using std::vector<T>::crbegin;
  using std::vector<T>::crend;
  using std::vector<T>::front;
  using std::vector<T>::back;
  using std::vector<T>::data;
  using std::vector<T>::swap;
  using std::vector<T>::get_allocator;
  friend inline auto& operator<< (std::ostream& o, const Enumvector& e) {
    for (auto a: e) o << a << "\t";
    return o << std::endl;
  }
  E size(void) const {return static_cast<E>(E::number());}
  int numsize(void) const {return E::number();}
  explicit Enumvector(const allocator_type& alloc=allocator_type()):
    std::vector<T>(E::number(),alloc){}
  explicit Enumvector(const value_type& val,
		  const allocator_type& alloc=allocator_type()):
    std::vector<T>(E::number(),val,alloc){}
  Enumvector(const Enumvector& x): std::vector<T>(x) {}
  Enumvector(const Enumvector& x, const allocator_type& alloc):
    std::vector<T>(x,alloc) {}
  Enumvector(Enumvector&& x):
    std::vector<T>(std::forward<Enumvector>(x)) {}
  Enumvector(Enumvector&& x, const allocator_type& alloc):
    std::vector<T>(std::forward<Enumvector(x)>(x),alloc) {}
  Enumvector& cshift(int n=1) {
    size_t s = E::number();
    if (s>0) {
      const Enumvector tmp(*this);
      n %= s;
      for (size_t i=0; i<s; i++)
	(*this)[static_cast<E>(static_cast<int>(i))] = std::move(tmp[static_cast<E>(static_cast<int>((i+n)%s))]);
    }
    return *this;
  }
  template<typename generator>
  Enumvector& shuffle(generator &r) {
    myshuffle(begin(), end(), r);
    return *this;
  }
  Enumvector& permute(const Enumvector<E,E> &reorder) {
    const Enumvector tmp(*this);
    for (const auto i: indices(reorder))
      (*this)[i] = std::move(tmp[reorder[i]]);
    return *this;
  }
  reference operator[](const E& m){return std::vector<T>::operator[](static_cast<int>(m));}
  const_reference operator[](const E& m) const{
    return std::vector<T>::operator[](static_cast<int>(m));
  }
  Enumvector& operator= (const Enumvector& x) {
    std::vector<T>::operator=(x); return *this;
  }
  Enumvector& operator= (Enumvector&& x) {
    std::vector<T>::operator=(std::forward<decltype(x)>(x)); return *this;
  }
  reference at(const E& m){return std::vector<T>::at(m);}
  const_reference at(const E& m) const{return std::vector<T>::at(m);}
  void assign(const value_type& val){std::vector<T>::assign(E::number(),val);}
};

template<typename E, typename T>
inline const Enumvector<E,T> unitvec() {
  Enumvector<E,T> res;
  res[static_cast<E>(0)] = 1;
  return res;
}
#endif
