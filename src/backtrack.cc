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

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
        const CandidateSet &cs)
{
    std::cout << "t " << query.GetNumVertices() << "\n";

    // implement your code here.
    // build DAG
    DAG qd(query, cs, data);
    
    const int32_t query_num = query.GetNumVertices();
    const int32_t data_num = data.GetNumVertices();
    
    Vertex *M = new Vertex[query_num];
    for (int32_t i = 0; i < query_num; i++) M[i] = qd.GetParentNum(i) - 1; 
    
    W = new size_t*[query_num];
    
    // a priority queue to store weights of vertices for backtracking
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
        s.pop();
        for (size_t i = 0; i < qd.GetChildNum(u); i++)
        {
            v = qd.GetChild(u, i);
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
                W[u][i] = SIZE_MAX;
            
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
                    if (temp < W[u][i])
                        W[u][i] = temp;
                }
            }
        }   
    }
    
    // array to check if a candidate vertex is visited or not
    int *visited = new int[data_num];
    for (int i = 0; i < data_num; i++) visited[i] = 0;
   
    // Update M as an array to store the matching information of each vertex 
    // If a query vertex has only one candidate, update M to store the match
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
    
    // free 
    delete[] visited;
    delete[] M;
    for (int i = 0; i < query_num; i++)
    {
        delete[] W[i];
    }
    delete[] W;

    std::chrono::duration<double> left = finish - std::chrono::system_clock::now();
    printf("c %lf\n", left.count(), print_count);
}

void Backtrack::Backtracking(const Graph &data, const CandidateSet &cs, DAG &qd,
        Vertex M[], int visited[], int32_t matched_num, const int32_t query_num,
        std::priority_queue<struct weight_info, vector <struct weight_info>, compare> w)
{

    if (std::chrono::system_clock::now() >= finish)
    {
        printf("c 0\n", print_count);
        exit(0);
    }

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
        // Get the root Vertex
        Vertex r = qd.GetRoot();

        size_t candidate_size = cs.GetCandidateSize(r);
        
        vector<size_t> candidates;
        bool fixed = false;
        
        // Check if the root vertex has a fixed match because it has only one candidate
        if (M[r] >= 0)
            fixed = true;

        // If not, implement extendability check for all candidates of the root
        // Add extendable candidates to the candidate array
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
                    candidates.push_back(i);
            } 
        }
        else
            candidates.push_back(0);
        
        int **children_candidates = new int*[qd.GetChildNum(r)];
        // Update to check if a child is extendable or not
        for (size_t j = 0; j < qd.GetChildNum(r); j++)
        {
            Vertex x = qd.GetChild(r, j);

            if (M[x] < 0)
                M[x]++;
                 
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
                        children_candidates[j][l] = W[x][l];
                }
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
            
            // Update extendable vertexes
            for (size_t j = 0; j < qd.GetChildNum(r); j++)
            {
                Vertex c = qd.GetChild(r, j);
                if (M[c] == -1)
                {
                    size_t weight = 0;
                    vector<int> c_candidates;

                    for (size_t l = 0; l < cs.GetCandidateSize(c); l++)
                    {
                        Vertex y = cs.GetCandidate(c, l);
                        if (data.IsNeighbor(u, y)) 
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
                }
            }
            if (cont == true)
                continue;
            
            while (!w.empty())
            {
                Vertex x = w.top().index;
                size_t weight = w.top().weight;
                vector<int> vs = w.top().candidates;
                w.pop();
                if (M[x] >= 0)
                    temp_w.push({weight, x, vs});
            }
            
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
        // find undecided vertex with the smallest weight in query
        Vertex curr = w.top().index;
        vector<int> curr_candidates = w.top().candidates;
        w.pop();
        
        bool fixed = false;

        if (M[curr] >= 0)
            fixed = true;

        int children[query_num];
        int **children_candidates = new int*[qd.GetChildNum(curr)];
        for (int j =0; j < query_num; j++) children[j] = 0;
        for (size_t j = 0; j < qd.GetChildNum(curr); j++)
        {
            Vertex x = qd.GetChild(curr, j);

            if (M[x] < 0)
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

            if (!fixed && visited[candidate] == 1)
                continue;

            if (!fixed)
            {
                visited[candidate] = 1;
                M[curr] = candidate;
            }
            
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
                }
            }

            if (cont == true)
                continue;

            while (!w.empty())
            {
                Vertex x = w.top().index;
                size_t weight = w.top().weight;
                vector<int> vs = w.top().candidates;
                w.pop();
                if (children[x] == 0)
                    temp_w.push({weight, x, vs});
            }
            
            Backtracking(data, cs, qd, M, visited, matched_num + 1, query_num, temp_w);
            
            while (!temp_w.empty())
            {
                Vertex x = temp_w.top().index;
                size_t weight = temp_w.top().weight;
                vector<int> vs = temp_w.top().candidates;
                temp_w.pop();
                if (children[x] == 0)
                    w.push({weight, x, vs});
            }

            if (!fixed)
            {
                visited[candidate] = 0;
                M[curr] = -1;
            }
        }
        
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
    }
}
