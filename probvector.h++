#ifndef PROBVECTOR_HPP
#define PROBVECTOR_HPP

#include "enumvector.h++"
#include "myutil.h++"
#include "selfiterator.h++"
#include <utility>
#include <random>
#include <cmath>

// #include <iostream>

static const char PROBVECTOR_HPP_SCCS_ID[] __attribute__((used)) = "@(#)probvector.h++: $Id$";

template<typename E,typename generator=std::mt19937> class Probvector:
  public Enumvector<E,double> {
public:
  using Generator = generator;
  using Index = E;
  virtual ~Probvector(void) {}
  Probvector(const int mask=-1, double norm=1.0):
    Enumvector<E,double>(mask>0?1:0), weight(norm) {
    if(mask>0)
      for(const auto &num: indices(mask,*this))
	(*this)[num] = 0; 
    normalize(false);
  }
  Probvector(const Enumvector<E,double>& e, bool unnorm=true):
    Enumvector<E,double>(e){normalize(unnorm);}
  Probvector(Enumvector<E,double>&& e, bool unnorm=true):
    Enumvector<E,double>(std::forward<decltype(e)>(e))
  {normalize(unnorm);}
  Probvector(generator &r,const int mask= -1,double norm=1.0):weight(norm) {
    int num = 0;
    for(auto &d: *this) {
      if (mask<=0 || num++ < mask) 
	d = std::generate_canonical<double, 20>(r);
      else d = 0;
    }
    normalize(false);
  }
  constexpr Probvector(const Probvector&) = default;
  constexpr Probvector(Probvector&&p): Enumvector<E,double>(static_cast<Enumvector<E,double>&&>(p)),
			     weight(p.weight), setupdone(p.setupdone), s(std::move(p.s)) {}

  Probvector& cshift(int n=1) {
    Enumvector<E,double>::cshift(n);
    setupdone=false;
    return *this;
  }
  Probvector& shuffle(generator &g) {
    Enumvector<E,double>::shuffle(g);
    setupdone=false;
    return *this;
  }
  Probvector& permute(const Enumvector<E,E> &p) {
    Enumvector<E,double>::permute(p);
    setupdone=false;
    return *this;
  }
  virtual Probvector& operator=(const Enumvector<E,double>& e) {
    Enumvector<E,double>::operator=(e);
    normalize();
    setupdone=false;
    return *this;
  }
  virtual Probvector& operator=(Enumvector<E,double>&& e) {
    Enumvector<E,double>::operator=(std::forward<decltype(e)>(e));
    normalize();
    setupdone=false;
    return *this;
  }
  virtual Probvector& operator=(const Probvector&) = default;
  virtual Probvector& operator=(Probvector&&) = default;
  Probvector operator*(const double c) const {
    Probvector r(*this);
    return r *= c;
  }
  virtual Probvector& operator*=(const double c) {
    weight *= c;
    return *this;
  }
  virtual Probvector& operator+=(const Probvector& e) {
    for (const auto i: indices(e))
      (*this)[i] += e.weight/weight * e[i];
    return *this;
  }
  Probvector operator+ (const Probvector& e) const {
    Probvector r(*this);
    return r+=e;
  }


  typename Enumvector<E,double>::const_reference
  operator[] (const E& e) const {
    return Enumvector<E,double>::operator[](e);
  }
  constexpr double norm(void) const {
    return weight;
  }
  double& norm(void) {
    return weight;
  }
  //template<typename T> friend auto indices(const T &);
  E generate(generator &r) const
  {
    setup();
    const auto ran = std::generate_canonical<double, 20>(r);
    for(auto e: indices(s))
      if(ran <= s[e]) return e;
    return indices(s).back();
  }
  virtual void mutate(const double sigma, generator &r) {
    for (auto &d: *this)
      d = invprobit(probit(d)+BoxMueller(0,sigma,r));
    normalize(false);
    setupdone = false;
  }
  void reinforce(const E&e, double by) {
//   	std::cout << "Probvector::reinforce" << std::endl;
  	(*this)[e] += by * ((*this)[e])*(1.0-(*this)[e]);
  	// If by < 1 we stay in the allowed range; but we'll apply a [0,1] cap
  	if((*this)[e] < 0.0) (*this)[e] = 0.0;
  	else if((*this)[e] > 1.0) (*this)[e] = 1.0;
  	normalize(false);
  	setupdone = false;
  }
  double entropy(bool weighted=false) {
    double retval = 0.0;
    for (auto p: *this)
      if (p>0.0) retval += p*std::log(p);
    if(weighted) retval *= weight;
    return retval;
  }
private:
  double weight = 1.0;
  mutable bool setupdone = false;
  mutable Enumvector<E,double> s;
  void normalize(bool updateweight=true) {
    auto sum = 0.0;
    for (auto d: *this) sum += d;
    if (sum > 0)
      for (auto &d: *this) d /= sum;
    else
      Enumvector<E,double>::assign(1.0/Enumvector<E,double>::numsize());
    if (updateweight) weight=sum;
  }
  void setup() const {
    if (setupdone) return;
    auto cumsum=0.0;
    for(const auto e:indices(s))
      s[e]=cumsum += (*this)[e];
    setupdone = true;
  }
  typename Enumvector<E,double>::reference
  operator[] (const E& e) {
    return Enumvector<E,double>::operator[](e);
  }
};

template<typename E, typename generator=std::mt19937>
inline const Probvector<E,generator> unitprob() {
  return Probvector<E,generator>(unitvec<E,double>());
}

#endif
