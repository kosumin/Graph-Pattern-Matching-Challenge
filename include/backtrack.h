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

class Backtrack {
 public:
  Backtrack();
  ~Backtrack();

  void PrintAllMatches(const Graph &data, const Graph &query,
                       const CandidateSet &cs);
  void Backtracking(const Graph &data, const CandidateSet &cs, DAG &qd,
                  Vertex M[], int visited[], int32_t matched_num, const int32_t query_num);
};

#endif  // BACKTRACK_H_
