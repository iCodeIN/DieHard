// -*- C++ -*-

#ifndef DH_HALFLOG2_H
#define DH_HALFLOG2_H

#include <stddef.h>

#include "staticif.h"
#include "staticlog.h"
#include "log2.h"

/*
  Static and dynamic functions to handle "half-log" size classes;
  that is, where there is a size class for objects both of size 2^n
  (e.g., 16, 32, 64) and the "halfway-between" sizes 3 * 2^n (e.g., 24, 48).
*/

#if __cplusplus > 199711L
template <class TYPE>
TYPE constexpr staticlog2ceiling(TYPE v) {
  return ((1 << staticlog(v)) < v) ?
    staticlog(v) + 1 :
    staticlog(v);
}

template <class TYPE>
TYPE constexpr statichalflog2(TYPE v) {
  return 2 * staticlog2ceiling(v) + 
    (staticlog2ceiling(v - (1 << (staticlog2ceiling(v)-1))) + 1 == staticlog2ceiling(v))
    - 1;
}

template <class TYPE>
TYPE constexpr statichalfpow2(TYPE v) {
  return (1 << (v/2)) + (v & 1) * ((1 << (v/2)) / 2);
}

#else

template <size_t v>
class StaticLog2Ceiling {
 public:
  enum { VALUE = StaticIf<((1 << StaticLog<v>::VALUE) < v),
	 StaticLog<v>::VALUE + 1,
	 StaticLog<v>::VALUE>::VALUE };
};

template <size_t v>
class StaticHalfLog2 {
 private:
  enum { A = StaticLog2Ceiling<v>::VALUE };
  enum { B = StaticLog2Ceiling<v - (1 << (A-1))>::VALUE };
 public:
  enum { VALUE = 2 * A + (B + 1 == A) - 1 };
};


template <int v>
class StaticHalfPow2 {
 private:
  enum { SZ1 = 1 << (v/2) };
  enum { SZ2 = (v & 1) * (SZ1 / 2) };
 public:
  enum { VALUE = SZ1 + SZ2 };
};

#endif

inline int halflog2 (size_t v) {
  int a = log2(v);
  int b = log2(v - (1 << (a-1)));
  return 2 * a + (b + 1 == a) - 1;
}
  
inline size_t halfpow2 (int v) {
  size_t sz1 = 1 << (v/2);
  return sz1 + ((v & 1) * (sz1 / 2));
}

#endif

