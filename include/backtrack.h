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

using namespace std;

struct weight_info {
    size_t weight;
    Vertex index;
    vector<int> candidates;    
};

struct compare {
    bool operator() (const struct weight_info& w1, const struct weight_info& w2) {
        /*if (w1.weight > w2.weight)
            return true;*/
        if (w1.weight != 0 && w1.candidates.size() == 1)
            return false;
        else if (w2.weight != 0 && w2.candidates.size() == 1)
            return true;
        else if (w1.weight * w1.candidates.size() > w2.weight * w2.candidates.size())
            return true;
        /*else if (w1.weight == w2.weight)
        {
            if (w1.candidates.size() > w2.candidates.size())
                return true;
            else
                return false;
        }*/
        else
            return false;
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
                  priority_queue<struct weight_info, vector <struct weight_info>, compare> w);
};

#endif  // BACKTRACK_H_
