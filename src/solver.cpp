/**
 * @file solver.cpp
 * @author Paul Kirth
 * @date 3/20/15
 */

#include <iostream>
#include "solver.hpp"

solver::solver(std::string filename) {
  std::ifstream file(filename);
  std::string line;
  // clauses.reserve(10);//preemptivly reserve space for at least 10 clauses
  int max = 0, val;

  if (file.is_open()) {
    while (std::getline(file, line)) {
      clause c;
      size_t size;

      c.a = std::stoi(line, &size);
      c.b = std::stoi(line.substr(size));
      val = std::max(abs(c.a), abs(c.b));
      if (val > max)
        max = val;
      clauses.push_back(c);
    } // end while
  }   // end if
  file.close();

  x.resize(max + 1); // use an extra bit to signal completion
  nodes.resize(2 * max);

  // printf("X.size() = %lu\n", x.size());

  int id = 0;
  bool b = true;
  for (auto &item : nodes) {
    if (id >= max) {
      id = 0;
      b = false;
    }
    item.id = id++;
    item.val = b;

  } // end for

  int idx;
  for (auto item : clauses) {
    idx = std::abs(item.a);
    node *ptr = &nodes[0];
    // printf("max:%d, idx: %d\n", max, idx);
    if (item.a > 0) {
      ptr->count++;
    } else {
      (ptr + max)->count++;
    }

    idx = std::abs(item.b);
    // printf("max:%d, idx: %d\n", max, idx);
    if (item.b > 0) {
      ptr->count++;
    } else {
      (ptr + max)->count++;
    }
  } // end for

  // std::sort(nodes.begin(), nodes.end(),
  //   [](node first, node second) { return first.count > second.count; });
  // printf("x: %d\nclauses: %d\nnodes: %d\n", x.size(), clauses.size(),
  // nodes.size());
}

solution solver::exact() {
  int max = 0;
  boost::dynamic_bitset<> ans;
  int len = x.size() - 1;
  int count;
  int m = clauses.size();

  do {
    count = 0;
    for (int i = 0; i < m; ++i) {
      if ((m - i + count) < max)
        break;
      if (check(clauses[i]))
        count++;
    }

    if (count > max) {
      max = count;
      ans = x;
    }
    increment(x);
  } while (max < clauses.size() && !x[len]);
  ans.resize(ans.size() - 1); // remove extra bit
  solution ret(ans, max);
  return ret;
}

solution solver::approx() {
  int max = 0;
  boost::dynamic_bitset<> ans;
  const int len = x.size() - 1;
  const int num_iter = len;
  int count = 0;
  const int m = clauses.size();

  for (int i = 0; i < m; ++i) {
    if (check(clauses[i]))
      max++;
  }

  for (int j = 0; j < len; ++j) {
    count = 0;
    x[nodes[j].id] = nodes[j].val > nodes[j + len].val;

    /* for (int i = 0; i < m; ++i) {
       if ((m - i + count) < max)
         break;
       if (check(clauses[i]))
         ++count;
     } // end for

     if (count > max) {
       max = count;
       ans = x;
     }*/
    max = count;
    ans = x;
    // else
    // x.flip(nodes[j].id); // if the number went down, flip the bit;
  } // end for;

  for (int i = 0; i < num_iter && max < clauses.size(); ++i) {
    x = ans;
    for (int j = 0; j < len; ++j) {
      count = 0;

      x.flip(j); // flip each bit and see what happens
      for (int k = 0; k < m; ++k) {
        if ((m - k + count) < max)
          break;
        if (check(clauses[k]))
          count++;
      }
      if (count > max) {
        max = count;
        ans = x;
      } else {
        x.flip(j); // if the number went down, flip the bit;
      }            // end if else
    }              // end for
  }                // end for

  ans.resize(ans.size() - 1); // remove extra bit
  solution ret(ans, max);
  return ret;
}

int main() {
  std::string filename = "../Data/instance.txt";
  solver s(filename);
  solver s2(filename);
 
  solution ans;
 
  // if(n == 1)
   //ans = s.exact();

  // else
   //ans = s.approx();

  //std::cout << "Exact\nSAT: " << ans.value << "\nAssignment: " << ans.ans << std::endl;

  // solution ans3 = s.approx();
  // std::cout << "SAT: " << ans3.value << "\nAssignment: " << ans3.ans <<
  // std::endl;

  solution ans2 = s2.approx();
  std::cout << "\nApprox\nSAT: " << ans2.value << "\nAssignment: " << ans2.ans
            << std::endl;

  return 0;
}
