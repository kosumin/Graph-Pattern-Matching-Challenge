/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"
#include "DAG.h"
#include <stack>
#include <queue>

using namespace std;

struct weight_info {
    size_t weight;
    Vertex index;
};

struct compare {
    bool operator() (const struct weight_info& w1, const struct weight_info& w2) {
        return (w1.weight > w2.weight);
    }
};

class Backtrack {
 public:
  Backtrack();
  ~Backtrack();

  void PrintAllMatches(const Graph &data, const Graph &query,
                       const CandidateSet &cs);
  void Backtracking(const Graph &data, const CandidateSet &cs, DAG &qd,
                  Vertex M[], int visited[], int32_t matched_num, const int32_t query_num,
                  size_t* &W, priority_queue<struct weight_info, vector <struct weight_info>, compare> w);
};

#endif  // BACKTRACK_H_
