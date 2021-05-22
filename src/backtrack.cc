/**
 * @file backtrack.cc
 *
 */

#include "../include/backtrack.h"
#include "../include/DAG.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs)
{
    std::cout << "t " << query.GetNumVertices() << "\n";

    // implement your code here.
    //
    DAG qd(query);

    /*for (Vertex v = 0; v < query.GetNumVertices(); v++) {
      for (size_t i = 0; i < qd.GetChildNum(v); i++) {
          std::cout << qd.GetChild(v, i) << " ";
      }
      std::cout << "\n";
    }

    for (Vertex v = 0; v < query.GetNumVertices(); v++) {
      for (size_t i = 0; i < qd.GetParentNum(v); i++) {
          std::cout << qd.GetParent(v, i) << " ";
      }
      std::cout << "\n";
    }*/

    const int32_t query_num = query.GetNumVertices();
    Vertex *M = new Vertex[query_num];
    for (int32_t i = 0; i < query_num; i++) M[i] = -1;
    int *visited = new int[data.GetNumVertices()];
    for (int32_t i = 0; i < data.GetNumVertices(); i++) visited[i] = 0;

    Backtracking(data, cs, qd, M, visited, 0, query_num);
}

void Backtrack::Backtracking(const Graph &data, const CandidateSet &cs, DAG &qd,
                             Vertex M[], int visited[], int32_t matched_num, const int32_t query_num)
{
    //std::cout << "matched num " << matched_num << "\n";
    if (matched_num == query_num)
    {
        std::cout << "a";
        for (int32_t i = 0; i < query_num; i++)
        {
            std::cout << " " << M[i];
        }
        std::cout << "\n";
    }
    else if (matched_num == 0)
    {
        Vertex v = qd.GetRoot();
        for (size_t i = 0; i < cs.GetCandidateSize(v); i++)
        {
            Vertex u = cs.GetCandidate(v, i);
            visited[u] = 1;
            M[v] = u;
            //std::cout << "start point " << u << "\n";
            /*for (int32_t l = 0; l < query_num; l++)
            {
                std::cout << M[l] << " ";
            }
            std::cout << "\n";
            for (int32_t m = 0; m < data.GetNumVertices(); m++)
            {
                std::cout << visited[m] <<  " ";
            }
            std::cout << "\n\n";*/

            Backtracking(data, cs, qd, M, visited, matched_num+1, query_num);
            visited[u] = 0;
            M[v] = -1;


        }
    }
    else
    {
        // find undecided vertex in query
        for (Vertex i = 0; i < query_num; i++)
        {
            if (M[i] == -1)   // undecided vertex
            {
                //std::cout << i << " unvisited\n";
                // find the candidates of the vertex
                for (size_t j = 0; j < cs.GetCandidateSize(i); j++)
                {
                    bool extendable = true;
                    Vertex candidate = cs.GetCandidate(i, j);
                    //std::cout << i << " candidate: " << candidate << "\n";
                    // for each candidate see if the parents are valid
                    for (size_t k = 0; k < qd.GetParentNum(i); k++)
                    {
                        Vertex c_parent = M[qd.GetParent(i, k)];
                        //std::cout << i << " candidate: " << candidate << " parent: " << c_parent << "\n";
                        if (c_parent == -1) extendable = false;
                        else if (!data.IsNeighbor(candidate, c_parent)) extendable = false;
                    }
                    if (extendable && visited[candidate] == 0)
                    {
                        visited[candidate] = 1;
                        M[i] = candidate;
                        //std::cout << "QV " << i << " DV " << candidate << "\n";
                        /*std::cout << "matched " << matched_num << "\n";
                        for (int32_t l = 0; l < query_num; l++)
                        {
                            std::cout << M[l] << " ";
                        }
                        std::cout << "\n";
                        /*for (int32_t m = 0; m < data.GetNumVertices(); m++)
                        {
                            std::cout << visited[m] <<  " ";
                        }
                        std::cout << "\n\n";*/
                        Backtracking(data, cs, qd, M, visited, matched_num+1, query_num);
                        visited[candidate] = 0;
                        M[i] = -1;
                    }
                }
            }
        }
    }
}
