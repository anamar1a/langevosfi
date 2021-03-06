#ifndef SELFITERATOR_HPP
#define SELFITERATOR_HPP

#include <iterator>
#include <algorithm>

static const char SELFITERATOR_HPP_SCCS_ID[] __attribute__((used)) = "@(#)selfiterator.h++: $Id$";

// Stupid C++ does not allow one to iterate over integers
// Here is a set of classes to allow that
template<typename T, bool Increasing> class SelfIterator:
  public std::iterator<std::random_access_iterator_tag,T> {
  T value;
public:
  SelfIterator(const T& v): value(v) {}
  SelfIterator(const SelfIterator<T,!Increasing> o): value(Increasing?o.value-1:o.value+1) {}
  T operator*(void) const {return Increasing?value+0:value-1;}
  SelfIterator operator+(const T &i) const {return Increasing?value+i:value-i;}
  SelfIterator operator-(const T &i) const {return Increasing?value-i:value+i;}
  auto& operator++(void) {Increasing?value++:value--; return *this;}
  auto operator++(int) {auto ret(*this); Increasing?value++:value--; return ret;}
  auto& operator--(void) {Increasing?value--:value++; return *this;}
  auto operator--(int) {auto ret(*this); Increasing?value--:value++; return ret;}
  auto& operator+=(const T &i) {Increasing?value+=i:value-=i; return *this;}
  auto& operator-=(const T &i) {Increasing?value-=i:value+=i; return *this;}
  auto operator<(const SelfIterator other) const {return Increasing?value < other.value:other.value<value;}
  auto operator>(const SelfIterator other) const {return Increasing?value > other.value:other.value>value;}
  auto operator<=(const SelfIterator other) const {return !(Increasing?value > other.value:other.value > value);}
  auto operator>=(const SelfIterator other) const {return !(Increasing?value < other.value:other.value < value);}
  auto operator==(const SelfIterator other) const {return value == other.value;}
  auto operator!=(const SelfIterator other) const {return value != other.value;}
};

template<typename T, bool Increasing> class Range {
  T start, beyond;
public:
  Range(const T &s, const T &b): start(s), beyond(b) {}
  typedef T value_type, reference, const_reference;
  typedef SelfIterator<T,Increasing> iterator, const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator, const_reverse_iterator;
  typedef size_t size_type;
  typedef std::ptrdiff_t difference_type;
  iterator begin(void) const {return start;}
  iterator end(void) const {return beyond;}
  reverse_iterator rbegin(void) const {return end();}
  reverse_iterator rend(void) const {return begin();}
  const_iterator cbegin(void) const {return start;}
  const_iterator cend(void) const {return beyond;}
  const_reverse_iterator crbegin(void) const {return cend();}
  const_reverse_iterator crend(void) const {return cbegin();}
  auto empty() const {return Increasing?start >= beyond:start<=beyond;}
  const_reference operator[] (const T &i) const {return start+i;}
  const_reference at (const T &i) const {return start+i;} 
  const_reference front (void) const {return start;}
  const_reference back (void) const {return Increasing?beyond-1:beyond+1;}
  void swap(Range&& o) {
    const auto s = start, b = beyond;
    start = o.start; beyond = o.beyond;
    o.start = s; o.beyond = b;
  }
  void swap(Range& o) {swap(static_cast<Range&&>(o));}
};

template<typename T> auto range(const T begin, const T end) {
  return Range<T,true>(begin,end);
}

template<typename T> auto range(const T end) {
  return range(static_cast<T>(0), end);
}

template<typename T> auto rrange(const T begin, const T end=0) {
  return Range<T,false>(begin,end);
}

template<typename T> auto indices(const T &o) {
  return range(o.size());
}

template<typename T> auto indices(size_t s, const T &o) {
  return range(decltype(o.size())(static_cast<int>(s)),o.size());
}
#endif
