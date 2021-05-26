/**
 * @file main.cc
 *
 */

#include "../include/backtrack.h"
#include "../include/candidate_set.h"
#include "../include/common.h"
#include "../include/graph.h"
#include "../include/DAG.h"

std::chrono::system_clock::time_point finish;

int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: ./program <data graph file> <query graph file> "
                 "<candidate set file>\n";
    return EXIT_FAILURE;
  }
  
  finish = std::chrono::system_clock::now() + std::chrono::minutes(1);
  std::string data_file_name = argv[1];
  std::string query_file_name = argv[2];
  std::string candidate_set_file_name = argv[3];

  Graph data(data_file_name);
  Graph query(query_file_name, true);
  CandidateSet candidate_set(candidate_set_file_name);

  Backtrack backtrack;

  backtrack.PrintAllMatches(data, query, candidate_set);

  return EXIT_SUCCESS;
}
