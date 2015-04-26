/**
 * @file solver.hpp
 * @author Paul Kirth
 * @date 3/20/15
 */

#ifndef _SOLVER_HPP_
#define _SOLVER_HPP_

#include <string>
#include <fstream>
#include <algorithm>
#include <boost/dynamic_bitset.hpp>

struct node {
  size_t count;
  size_t id;
  bool val;
};

struct clause {
  int a;
  int b;
};

struct solution {
  boost::dynamic_bitset<> ans;
  int value;
  solution(size_t size = 0, int val = 0) : ans(size) { value = val; };
  solution(boost::dynamic_bitset<> bs, int val = 0) : ans(bs), value(val) {}
};

/**
 * Finds solutions for MAX 2 SAT instances
 * has exact solution, and approximate solutions
 *
 */
class solver {
public:
  solver(std::string file);

  solution exact();
  solution approx();

private:
  boost::dynamic_bitset<> x;
  std::vector<clause> clauses;
  std::vector<node> nodes;

  inline bool check(const clause &c) noexcept {
    bool a;
    bool b;
    int ida = abs(c.a), idb = abs(c.b);
    a = (c.a > 0) ? x[ida] : !x[ida];
    b = (c.b > 0) ? x[idb] : !x[idb];
    return a || b;
  }

  void increment(boost::dynamic_bitset<> &bitset) {
    auto len = std::max(bitset.size(), (size_t)1);
    // printf("count:%d\nlen:  %d\n", bitset.count(), len);
    for (int loop = 0; loop < len; ++loop) {
      if ((bitset[loop] ^= 0x1) == 0x1) {
        break;
      }
    }
  }

}; // end class solver

#endif // end solver.hpp
