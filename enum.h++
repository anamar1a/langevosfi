#ifndef ENUM_HPP
#define ENUM_HPP

#include <iostream>
#include <string>

template<const char *id,int n>
class Enum {
public:
  friend inline auto& operator<< (std::ostream& o, const Enum& e) {
    return o << std::string(id) << static_cast<int>(e);
  }
  int uniqid() const {return val;}
  double match(const Enum& m) const {
    return m==*this?1.0-1.0*(n-1)/(n*n):1.0/(n*n);
  }
protected:
  Enum(const Enum &m):val(static_cast<int>(m)){}
  Enum(const int val):val(val){}
  ~Enum(void){}
private:
  int val;
  Enum(void):val(-1){}
  static int number(void) {return n;}
  explicit operator int (void) const{return val;}
  explicit operator int& (void) {return val;}
  auto& operator=(const Enum&m){
    static_cast<int&>(*this)=static_cast<int>(m);
    return *this;
  }
  auto operator+(const Enum &i) const {
    return Enum(static_cast<int>(*this)+static_cast<int>(i));
  }
  auto operator-(const Enum &i) const {
    return Enum(static_cast<int>(*this)-static_cast<int>(i));
  }
  auto& operator++(void) {
    static_cast<int&>(*this)++; return *this;
  }
  auto operator++(int) {
    auto ret(*this);
    static_cast<int&>(*this)++;
    return ret;
  }
  auto& operator--(void) {
    static_cast<int&>(*this)--; return *this;
  }
  auto operator--(int) {
    auto ret(*this);
    static_cast<int&>(*this)--;
    return ret;}
  auto& operator+=(const Enum &i) {
    static_cast<int&>(*this)+=static_cast<int>(i);
    return *this;
  }
  auto& operator-=(const Enum &i) {
    static_cast<int&>(*this)+=static_cast<int>(i);
    return *this;
  }
  auto operator<(const Enum &other) const {
    return static_cast<int>(*this) < static_cast<int>(other);
  }
  auto operator>(const Enum &other) const {
    return static_cast<int>(*this) > static_cast<int>(other);
  }
  auto operator<=(const Enum &other) const {
    return static_cast<int>(*this) <= static_cast<int>(other);
  }
  auto operator>=(const Enum &other) const {
    return static_cast<int>(*this) >= static_cast<int>(other);
  }
public:
  auto operator==(const Enum &other) const {
    return static_cast<int>(*this) == static_cast<int>(other);
  }
  auto operator!=(const Enum &other) const {
    return static_cast<int>(*this) != static_cast<int>(other);
  }
  template<typename,typename> friend class Enumvector;
  template<typename,bool> friend class SelfIterator;
  template<typename,bool> friend class Range;
  template<typename T> friend auto range(const T);
};
#endif