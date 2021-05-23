/**
 * @file backtrack.cc
 *
 */

#include "../include/backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs)
{
    std::cout << "t " << query.GetNumVertices() << "\n";

    // implement your code here.
    //
    DAG qd(query, cs);

    const int32_t query_num = query.GetNumVertices();
    const int32_t data_num = data.GetNumVertices();
    // path-size order
    Vertex *M = new Vertex[query_num];
    for (int32_t i = 0; i < query_num; i++) M[i] = qd.GetParentNum(i) - 1; 
    size_t *W = new size_t[data_num];
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
            for (size_t i = 0; i < cs.GetCandidateSize(u); i++)
                W[cs.GetCandidate(u, i)] = 1;
        }
        else
        {
            for (size_t i = 0; i < cs.GetCandidateSize(u); i++)
                W[cs.GetCandidate(u, i)] = 0; 
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
                            temp += W[x];
                    }
                    if (W[w] == 0 || temp < W[w])
                        W[w] = temp;
                }
            }
        }
    }
    int sum = 0;
    for (size_t i = 0; i < cs.GetCandidateSize(qd.GetRoot()); i++)
    {
        sum += W[cs.GetCandidate(qd.GetRoot(), i)];
    }
    w.push({sum, qd.GetRoot()});
    int *visited = new int[data_num];
    for (int32_t i = 0; i < data_num; i++) visited[i] = 0;
    Backtracking(data, cs, qd, M, visited, 0, query_num, W, w);
}

void Backtrack::Backtracking(const Graph &data, const CandidateSet &cs, DAG &qd,
                             Vertex M[], int visited[], int32_t matched_num, const int32_t query_num,
                             size_t* &W, priority_queue<struct weight_info, vector<struct weight_info>, compare> w)
{
    //std::cout << "matched num " << matched_num << "\n";
    if (matched_num == query_num)
    {
        printf("a");
        for (int32_t i = 0; i < query_num; i++)
        {
            printf(" %d", M[i]);
        }
        printf("\n");
    }
    else if (matched_num == 0)
    {
        Vertex v = qd.GetRoot();
        for (size_t i = 0; i < cs.GetCandidateSize(v); i++)
        {
            priority_queue<struct weight_info, vector<struct weight_info>, compare> temp_w;
            Vertex u = cs.GetCandidate(v, i);
            visited[u] = 1;
            M[v] = u;
            size_t store_weight = 0;
            /*std::cout << "start point " << u << "\n";
            for (int32_t l = 0; l < query_num; l++)
            {
                std::cout << M[l] << " ";
            }
            std::cout << "\n";
            for (int32_t m = 0; m < data.GetNumVertices(); m++)
            {
                std::cout << visited[m] <<  " ";
            }
            std::cout << "\n\n";*/
            for (size_t j = 0; j < qd.GetChildNum(v); j++)
            {
                Vertex x = qd.GetChild(v, j);
                if (M[x] != -1)
                    continue;
                bool check = true;
                for (size_t k = 0; k < qd.GetParentNum(x); k++)
                {
                    if (M[qd.GetParent(x, k)] == -1)
                    {
                        check = false;
                        break;
                    }
                }
                if (check)
                {
                    int weight = 0;
                    for (size_t l = 0; l < cs.GetCandidateSize(x); l++)
                    {
                        Vertex y = cs.GetCandidate(x, l);
                        if (data.IsNeighbor(u, y)) weight += W[y];
                    }
                    temp_w.push({weight, x});
                    //printf("%d: (%d) \n", x, weight);
                }
            }
            //printf("\n");
            while (!w.empty())
            {
                Vertex x = w.top().index;
                size_t weight = w.top().weight;
                w.pop();
                if (x == v)
                {
                    store_weight = weight;
                    continue;
                }
                temp_w.push({weight, x});
                //printf("%d: (%d) ", x, weight); 
            }
            //printf("\n");
            Backtracking(data, cs, qd, M, visited, matched_num+1, query_num, W, temp_w);
            w.push({store_weight, v});
            while (!temp_w.empty())
            {
                Vertex x = temp_w.top().index;
                size_t weight = temp_w.top().weight;
                temp_w.pop();
                bool isChild = false;
                for (size_t j = 0; j < qd.GetChildNum(v); j++)
                {
                    Vertex y = qd.GetChild(v, j);
                    if (y == x)
                    {
                        isChild = true;
                        break;
                    }
                }
                if (!isChild)
                {
                    w.push({weight, x});
                }
            }
            visited[u] = 0;
        }
    }
    else
    {
        // find undecided vertex in query
        Vertex i = w.top().index;
        //printf("current: %d\n", i);
        for (size_t j = 0; j < cs.GetCandidateSize(i); j++)
        {
            bool extendable = true;
            Vertex candidate = cs.GetCandidate(i, j);
                    //std::cout << i << " candidate: " << candidate << "\n";
                    //for each candidate see if the parents are valid
            for (size_t k = 0; k < qd.GetParentNum(i); k++)
            {
                Vertex c_parent = M[qd.GetParent(i, k)];
                //std::cout << i << " candidate: " << candidate << " parent: " << qd.GetParent(i, k) << "\n";
                if (c_parent == -1) extendable = false;
                else if (!data.IsNeighbor(candidate, c_parent)) extendable = false;
            }
            if (extendable && visited[candidate] == 0)
            {
                    priority_queue<struct weight_info, vector<struct weight_info>, compare> temp_w;
                    size_t store_weight = 0;
                    visited[candidate] = 1;
                    M[i] = candidate;
                    /*std::cout << "QV " << i << " DV " << candidate << "\n";
                    std::cout << "matched " << matched_num << "\n";
                    for (int32_t l = 0; l < query_num; l++)
                    {
                        std::cout << M[l] << " ";
                    }*/
                        //std::cout << "\n"; 
                        /*for (int32_t m = 0; m < data.GetNumVertices(); m++)
                        {
                            std::cout << visited[m] <<  " ";
                        }*/
                        //std::cout << "\n\n";
                    for (size_t o = 0; o < qd.GetChildNum(i); o++)
                    {
                        Vertex x = qd.GetChild(i, o);
                        if (M[x] != -1)
                            continue;
                        bool check = true;
                        for (size_t k = 0; k < qd.GetParentNum(x); k++)
                        {
                            if (M[qd.GetParent(x, k)] == -1)
                            {
                                check = false;
                                break;
                            }
                        }          
                        if (check)
                        {
                            int weight = 0;
                            for (size_t l = 0; l < cs.GetCandidateSize(x); l++)
                            {
                                Vertex y = cs.GetCandidate(x, l);
                                if (data.IsNeighbor(candidate, y)) weight += W[y];
                            }       
                            temp_w.push({weight, x});
                            //printf("%d: (%d %d) ", x, weight, 0);
                        }       
                    }
                while (!w.empty())
                {
                    Vertex x = w.top().index;
                    size_t weight = w.top().weight;
                    w.pop();
                    if (x == i)
                    {
                        store_weight = weight;
                        continue;
                    }
                    temp_w.push({weight, x});
                    //printf("%d: (%d) ", x, weight); 
                }
                //printf("\n");
                Backtracking(data, cs, qd, M, visited, matched_num+1, query_num, W, temp_w);
                w.push({store_weight, i});
                while (!temp_w.empty())
                {
                    Vertex x = temp_w.top().index;
                    size_t weight = temp_w.top().weight;
                    temp_w.pop();
                    bool isChild = false;
                    for (size_t j = 0; j < qd.GetChildNum(i); j++)
                    {
                        Vertex y = qd.GetChild(i, j);
                        if (y == x)
                        {
                            isChild = true;
                            break;
                        }
                    }   
                    if (!isChild)
                    {
                        w.push({weight, x});
                    }
                }   
                visited[candidate] = 0;
                M[i] = -1;
            }
        }
    }
}
