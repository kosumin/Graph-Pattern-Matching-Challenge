/**
 * @file backtrack.cc
 *
 */

#include "../include/backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

extern std::chrono::system_clock::time_point finish;
int print_count = 0;

// an array to store weights of candidates for each vertex
size_t **W;
// current vertex for backtracking
Vertex curr_v;

const Graph *tg;
const CandidateSet *tcs;

// compare weights of two vertexes for sorting candidate sets
bool weight_compare(const size_t & v1, const size_t & v2)
{
    if (W[curr_v][v1] < W[curr_v][v2])
        return true;
    else
        return false;
}

bool temp_compare(const size_t &v1, const size_t &v2)
{
    if (tg->GetDegree(tcs->GetCandidate(curr_v,v1)) > tg->GetDegree(tcs->GetCandidate(curr_v,v2)))
        return true;
    else
        return false;
}

int *visited_q;

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
        const CandidateSet &cs)
{
    tg = &data;
    tcs = &cs;
    std::cout << "t " << query.GetNumVertices() << "\n";

    // implement your code here.
    // build DAG
    DAG qd(query, cs, data);

    /* debugging
       for (int j = 0; j < query.GetNumVertices(); j++)
       {
       int count = 0;
       for (int i = 0; i < data.GetNumVertices(); i++)
       {
       if (data.GetLabel(i) == query.GetLabel(j) && data.GetDegree(i) >= query.GetDegree(j))
       count++;

       }
       printf("%d count: %d cand: %d\n", j, count, cs.GetCandidateSize(j));
       }*/

    const int32_t query_num = query.GetNumVertices();
    const int32_t data_num = data.GetNumVertices();
    Vertex *M = new Vertex[query_num];
    for (int32_t i = 0; i < query_num; i++) M[i] = qd.GetParentNum(i) - 1; 
    W = new size_t*[query_num];
    // a priority queue to store weights of vertexes for backtracking
    priority_queue<struct weight_info, vector<struct weight_info>, compare> w;
    stack<Vertex> s;
    queue<Vertex> q;
    q.push(qd.GetRoot());
    M[qd.GetRoot()] = 0;
    Vertex u;
    Vertex v;
    // build bottom-up order
    while (!q.empty())
    {
        u = q.front();
        q.pop();
        s.push(u);
        for (size_t i = 0; i < qd.GetChildNum(u); i++)
        {
            v = qd.GetChild(u, i);
            M[v]--;
            if (M[v] == -1)
                q.push(v);
        }
    }
    // calculate the weights of candidates for each vertex
    while (!s.empty())
    {
        u = s.top();
        //printf("%d: ", u);
        s.pop();
        for (size_t i = 0; i < qd.GetChildNum(u); i++)
        {
            v = qd.GetChild(u, i);
            //printf(" %d", v);
            if (qd.GetParentNum(v) == 1)
                q.push(v);
        }
        if (q.empty())
        {
            W[u] = new size_t[cs.GetCandidateSize(u)];
            for (size_t i = 0; i < cs.GetCandidateSize(u); i++)
                W[u][i] = 1;
        }
        else
        {
            W[u] = new size_t[cs.GetCandidateSize(u)];
            for (size_t i = 0; i < cs.GetCandidateSize(u); i++)
                W[u][i] = 0;
            // Todo: better way?
            while (!q.empty())
            {
                v = q.front();
                q.pop();
                for (size_t i = 0; i < cs.GetCandidateSize(u); i++)
                {
                    Vertex w = cs.GetCandidate(u, i);  
                    size_t temp = 0;

                    for (size_t j = 0; j < cs.GetCandidateSize(v); j++)
                    {
                        Vertex x = cs.GetCandidate(v, j);
                        if (data.IsNeighbor(w, x) == true)
                            temp += W[v][j];
                    }
                    if (W[u][i] == 0 || temp < W[u][i])
                        W[u][i] = temp;
                }
            }
        }
        //printf("\n");
    }
    //printf("\n");
    // an array to check if a vertex is visited or not
    int *visited = new int[data_num];
    visited_q = new int[query_num];
    for (int i = 0; i < data_num; i++) visited[i] = 0;
    for (int i = 0; i < query_num; i++) visited_q[i] = 0;
   // Update M as an array to store the matching information of each vertex 
    for (int i = 0; i < query_num; i++) 
    {
        if (cs.GetCandidateSize(i) == 1)
        {
            Vertex u = cs.GetCandidate(i, 0);
            M[i] = u;
            visited[u] = 1;
            if (i != qd.GetRoot())
            {
                vector<int> candidates;
                candidates.push_back(0);
                w.push({W[i][0], i, candidates});
            }

        }
        else
            M[i] = -qd.GetParentNum(i) - 1;
    }
    // do backtracking
    Backtracking(data, cs, qd, M, visited, 0, query_num, w);

    delete[] visited;
    delete[] M;
    for (int i = 0; i < query_num; i++)
    {
        delete[] W[i];
    }
    delete[] W;

    std::chrono::duration<double> left = finish - std::chrono::system_clock::now();
    printf("c %lf %d\n", left.count(), print_count);
}

void Backtrack::Backtracking(const Graph &data, const CandidateSet &cs, DAG &qd,
        Vertex M[], int visited[], int32_t matched_num, const int32_t query_num,
        std::priority_queue<struct weight_info, vector <struct weight_info>, compare> w)
{

    if (std::chrono::system_clock::now() >= finish)
    {
        printf("c 0 %d\n", print_count);
        exit(0);
    }

    //std::cout << "matched num " << matched_num << "\n";
    // Case 1: print the result of backtracking
    if (matched_num == query_num)
    {
        
        printf("a");
        for (int32_t i = 0; i < query_num; i++)
        {
            printf(" %d", M[i]);
        }
        printf("\n");

        print_count++;
        if (print_count >= 100000)
        {
            std::chrono::duration<double> left = finish - std::chrono::system_clock::now();
            printf("c %lf %d\n", left.count(), print_count);
            exit(0);
        }
    }
    // Case 2: start backtracking
    else if (matched_num == 0)
    {
        Vertex r = qd.GetRoot();
        //printf("current: %d list: %d\n",r, cs.GetCandidateSize(r));

        size_t candidate_size = cs.GetCandidateSize(r);
        // Todo: vector vs priority queue?
        vector<size_t> candidates;
        bool fixed = false;
        visited_q[r] = 1;
        if (M[r] >= 0)
            fixed = true;

        if (!fixed)
        {
            for (size_t i = 0; i < candidate_size; i++)
            {
                Vertex c = cs.GetCandidate(r, i);
                bool contain = true;
                for (size_t j = 0; j < qd.GetChildNum(r); j++)
                {
                    Vertex v = qd.GetChild(r, j);
                    if (M[v] >= 0 && !data.IsNeighbor(M[v], c))
                    {
                        contain = false;
                        break;
                    }
                }
                if (contain)
                {
                    //printf("%d %d\n", c, W[r][i]);
                    candidates.push_back(i);
                }
            } 
        }
        else
        {
            candidates.push_back(0);
        }
        curr_v = r;
        if (candidates.size() > 1)
        {
            //sort(candidates.begin(), candidates.end(), temp_compare);
            //sort(candidates.begin(), candidates.end(), weight_compare);
        }

        // Update to check if a child is extendable or not
        for (size_t j = 0; j < qd.GetChildNum(r); j++)
        {
            if (M[qd.GetChild(r, j)] < 0)
            {
                M[qd.GetChild(r, j)]++; 
            }
        }

        for (size_t i = 0; i < cs.GetCandidateSize(r); i++)
        {
            Vertex u = cs.GetCandidate(r, candidates[i]);
                std::priority_queue<struct weight_info, vector <struct weight_info>, compare> temp_w;

           bool cont = false;
            if (!fixed)
            {
                visited[u] = 1;
                M[r] = u;
            }

            /*debugging*/
               /*std::cout << r << "start point " << u << " index " << i <<  " weight " << W[r][candidates[i]] << "\n";
               for (int32_t l = 0; l < query_num; l++)
               {
               std::cout << M[l] << " ";
               }
               std::cout << "\n";*/
               /*for (int32_t m = 0; m < data.GetNumVertices(); m++)
               {
               std::cout << visited[m] <<  " ";
               }
               std::cout << "\n\n";*/

            // Update extendable vertexes
            for (size_t j = 0; j < qd.GetChildNum(r); j++)
            {
                Vertex c = qd.GetChild(r, j);
               if (M[c] == -1)
                {
                    size_t weight = 0;
                    vector<int> c_candidates;
                    bool contain = true;
                    for (size_t l = 0; l < cs.GetCandidateSize(c); l++)
                    {
                        Vertex y = cs.GetCandidate(c, l);
                        for (size_t m = 0; m < qd.GetChildNum(c); m++)
                        {
                            Vertex z = qd.GetChild(c, m);
                            if (M[z] >= 0 && !data.IsNeighbor(M[z], y))
                            {
                                contain = false;
                                break;
                            }
                        }
                        if (contain && data.IsNeighbor(u, y)) 
                        {
                            weight += W[c][l];
                            c_candidates.push_back(l);
                        }
                   }
                   if (weight == 0)
                   {
                       if (!fixed)
                       {
                           visited[u] = 0;
                           M[r] = -1;
                       }
                       cont = true;
                       break;

                   }
                   temp_w.push({weight, c, c_candidates});
                    //printf("%d: (%d) \n", c, weight);
                }/*else if (M[c] >= 0)
                {
                    vector<int> c_candidates;
                    c_candidates.push_back(0);
                    size_t weight = W[c][0];
                    temp_w.push({weight, c, c_candidates});
                        //printf("%d: (%d) \n", c, weight);
                }*/
            }
            if (cont == true)
                continue;
            // do backtracking
            while (!w.empty())
            {
                Vertex x = w.top().index;
                size_t weight = w.top().weight;
                vector<int> vs = w.top().candidates;
                w.pop();
                if (M[x] >= 0)
                {
                    temp_w.push({weight, x, vs});
                    //printf("%d: (%d) ", x, weight); 
                }
            }
           // printf("\n");
            Backtracking(data, cs, qd, M, visited, matched_num + 1, query_num, temp_w);

            while (!temp_w.empty())
            {
                Vertex x = temp_w.top().index;
                size_t weight = temp_w.top().weight;
                vector<int> vs = temp_w.top().candidates;
                temp_w.pop();
                if (M[x] >= 0)
                {
                    w.push({weight, x, vs});
                }
            }
            if (!fixed)
                visited[u] = 0;
        }
    }
    // Case 3: during backtracking
    else
    {
        if (w.empty())
            return;
        // find undecided vertex with the smallest weight in query
        Vertex curr = w.top().index;
        curr_v = curr;
        vector<int> curr_candidates = w.top().candidates;
        size_t curr_weight = w.top().weight;
        w.pop();
        bool fixed = false;

        visited_q[curr] = 1;
        
        if (M[curr] >= 0)
            fixed = true;
        // Todo: how to choose the order of the candidate set?
        // sort the candidate set
        if (curr_candidates.size() > 1)
        {
            //sort(curr_candidates.begin(), curr_candidates.end(), temp_compare);
            //sort(curr_candidates.begin(), curr_candidates.end(), weight_compare);
        }
        /* debugging*/
          //printf("current: %d list %d weight: %d\n", curr, curr_candidates.size(), curr_weight);
           /*for (size_t k = 0; k < temp_can.size(); k++)
           printf("%d ", temp_can[k]);
           printf("\n");*/

        int children[query_num];
        int **children_candidates = new int*[qd.GetChildNum(curr)];
        for (int j =0; j < query_num; j++) children[j] = 0;
        for (size_t j = 0; j < qd.GetChildNum(curr); j++)
        {
            Vertex x = qd.GetChild(curr, j);

            if (M[x] > -1)
            { 
                /*if (visited_q[x] == 0)
                {
                    children_candidates[j] = new int[1];
                    children_candidates[j][0] = W[x][0];
                    children[x] = 1;   
                }*/
            }
            else
            {
                M[x]++;
            }
            if (M[x] == -1)
            {
                children_candidates[j] = new int[cs.GetCandidateSize(x)];
                for (size_t l = 0; l < cs.GetCandidateSize(x); l++)
                {
                    children_candidates[j][l] = 0;
                    Vertex y = cs.GetCandidate(x, l);
                    bool contain = true;
                    for (size_t m = 0; m < qd.GetChildNum(x); m++)
                    {
                        Vertex cc = qd.GetChild(x, m);
                        if (M[cc] >= 0 && !data.IsNeighbor(M[cc], y))
                        {
                            contain = false;
                            break;
                        }
                    }
                    if (contain)
                    {
                        for (size_t m = 0; m < qd.GetParentNum(x); m++)
                        {
                            Vertex pp = qd.GetParent(x, m);
                            if (pp == curr)
                                continue ;
                            Vertex p = M[pp];
                           // printf("%d\n", pp);
                            if (p == -1 || !data.IsNeighbor(p, y))
                            {
                                contain = false;
                                break;
                            }
                        }
                    }
                    if (contain)
                    {
                        children[x] = 1;
                        children_candidates[j][l] = W[x][l];
                    }
                }
            }
        }
        for (size_t j = 0; j < curr_candidates.size(); j++)
        {
            Vertex candidate = cs.GetCandidate(curr, curr_candidates[j]);
            std::priority_queue<struct weight_info, vector <struct weight_info>, compare> temp_w;

            //std::cout << i << " candidate: " << candidate << "\n";
            // check if the candidate is visited or not
            if (!fixed && visited[candidate] == 1)
                continue;

            /*bool extendable = true;
              for (size_t k = 0; k < qd.GetParentNum(curr); k++)
              {
              Vertex c_parent = M[qd.GetParent(curr, k)];
            //std::cout << i << " candidate: " << candidate << " parent: " << qd.GetParent(i, k) << "\n";
            if (c_parent == -1) extendable = false;
            else if (!data.IsNeighbor(candidate, c_parent)) extendable = false;
            }
            if (extendable == false)
            continue;
             */
             if (!fixed)
             {
                visited[candidate] = 1;
                M[curr] = candidate;
             }
            /*std::cout << "QV " << curr << " DV " << candidate << " index " << j  << " weight " << W[curr][curr_candidates[j]] << "\n";
               std::cout << "matched " << matched_num << "\n";
               for (int32_t l = 0; l < query_num; l++)
               {
               std::cout << M[l] << " ";
               }
               std::cout << "\n";*/
            /*for (int32_t m = 0; m < data.GetNumVertices(); m++)
              {
              std::cout << visited[m] <<  " ";
              }*/
            //std::cout << "\n\n";
            bool cont = false;
            for (size_t o = 0; o < qd.GetChildNum(curr); o++)
            {
                Vertex x = qd.GetChild(curr, o);
                if (M[x] == -1)
                {
                    size_t weight = 0;
                    vector<int> vs;
                    for (size_t l = 0; l < cs.GetCandidateSize(x); l++)
                    {
                        Vertex y = cs.GetCandidate(x, l);
                        if (data.IsNeighbor(candidate, y) && children_candidates[o][l] > 0)
                        {
                            weight += children_candidates[o][l];
                            vs.push_back(l);  
                        }
                    }
                    if (weight == 0)
                    {
                        cont = true;
                        if (!fixed)
                        {
                            visited[candidate] = 0;
                            M[curr] = -1;
                            }
                        break;
                    }
                    temp_w.push({weight, x, vs});
                   // printf("%d: (%d %d) ", x, weight);
                }/* else if (children[x] == 1)
                {
                    size_t weight = children_candidates[o][0];
                    vector<int> vs;
                    vs.push_back(0);
                    temp_w.push({weight, x, vs});
                   // printf("%d: (%d %d) ", x, weight);
                }*/
            }
            if (cont == true)
                continue;
            while (!w.empty())
            {
                Vertex x = w.top().index;
                size_t weight = w.top().weight;
                vector<int> vs = w.top().candidates;
                w.pop();
                if (children[x] == 0 || M[x] >= 0)
                {
                    temp_w.push({weight, x, vs});
                    //printf("%d: (%d) ", x, weight); 
                }
            }
            //printf("\n");
            Backtracking(data, cs, qd, M, visited, matched_num + 1, query_num, temp_w);
            while (!temp_w.empty())
            {
                Vertex x = temp_w.top().index;
                size_t weight = temp_w.top().weight;
                vector<int> vs = temp_w.top().candidates;
                temp_w.pop();
                if (children[x] == 0 || M[x] >= 0)
                {
                    w.push({weight, x, vs});
                }
            }
            if (!fixed)
            {
                visited[candidate] = 0;
                M[curr] = -1;
            }
        }
        //w.push({curr_weight, curr, curr_candidates});
        for (size_t j = 0; j < qd.GetChildNum(curr); j++)
        {
            if (M[qd.GetChild(curr,j)] == -1)
            {
                delete children_candidates[j];
            }
            if (M[qd.GetChild(curr, j)] < 0)
                M[qd.GetChild(curr, j)]--;
        }
        delete children_candidates;
        visited_q[curr] = 0;
    }
}
