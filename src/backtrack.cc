/**
 * @file backtrack.cc
 *
 */

#include "../include/backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

extern std::chrono::system_clock::time_point finish;
int print_count = 0;

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs)
{
    std::cout << "t " << query.GetNumVertices() << "\n";

    // implement your code here.
    //
    DAG qd(query, cs, data);

    /*for (int j = 0; j < query.GetNumVertices(); j++)
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
    // path-size order
    Vertex *M = new Vertex[query_num];
    for (int32_t i = 0; i < query_num; i++) M[i] = qd.GetParentNum(i) - 1; 
    size_t **W = new size_t*[query_num];
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
            W[u] = new size_t[cs.GetCandidateSize(u)];
            for (size_t i = 0; i < cs.GetCandidateSize(u); i++)
                W[u][i] = 1;
        }
        else
        {
            W[u] = new size_t[cs.GetCandidateSize(u)];
            for (size_t i = 0; i < cs.GetCandidateSize(u); i++)
                W[u][i] = 0; 
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
    }
    int sum = 0;
    for (size_t i = 0; i < cs.GetCandidateSize(qd.GetRoot()); i++)
    {
        sum += W[qd.GetRoot()][i];
    }
    w.push({sum, qd.GetRoot()});
    for (size_t i = 0; i < query_num; i++) M[i] = -qd.GetParentNum(i) - 1;
    int *visited = new int[data_num];
    for (int32_t i = 0; i < data_num; i++) visited[i] = 0;
    Backtracking(data, cs, qd, M, visited, 0, query_num, W, w);
}

void Backtrack::Backtracking(const Graph &data, const CandidateSet &cs, DAG &qd,
                             Vertex M[], int visited[], int32_t matched_num, const int32_t query_num,
                             size_t** &W, priority_queue<struct weight_info, vector<struct weight_info>, compare> w)
{
    //std::cout << "matched num " << matched_num << "\n";
    if (matched_num == query_num)
    {
        if (std::chrono::system_clock::now() >= finish)
            exit(0);
        printf("a");
        for (int32_t i = 0; i < query_num; i++)
        {
            printf(" %d", M[i]);
        }
        printf("\n");
        print_count++;
        if (print_count >= 100000)
            exit(0);
    }
    else if (matched_num == 0)
    {
        Vertex v = qd.GetRoot();
        //printf("current: %d\n",v);
        for (size_t j = 0; j < qd.GetChildNum(v); j++)
            M[qd.GetChild(v, j)]++; 
        for (size_t i = 0; i < cs.GetCandidateSize(v); i++)
        {
            priority_queue<struct weight_info, vector<struct weight_info>, compare> temp_w;
            Vertex u = cs.GetCandidate(v, i);
            visited[u] = 1;
            M[v] = u;
            vector<int> store_can;
            size_t store_weight = 0;
            bool cont = false;
            //std::cout << "start point " << u << "\n";
            /*for (int32_t l = 0; l < query_num; l++)
            {
                std::cout << M[l] << " ";
            }
            std::cout << "\n";*/
            /*for (int32_t m = 0; m < data.GetNumVertices(); m++)
            {
                std::cout << visited[m] <<  " ";
            }
            std::cout << "\n\n";*/
            for (size_t j = 0; j < qd.GetChildNum(v); j++)
            {
                Vertex x = qd.GetChild(v, j);
                if (M[x] == -1)
                {
                    int weight = 0;
                    vector<int> vs;
                    for (size_t l = 0; l < cs.GetCandidateSize(x); l++)
                    {
                        Vertex y = cs.GetCandidate(x, l);
                        if (data.IsNeighbor(u, y)) 
                        {
                            weight += W[x][l];
                            vs.push_back(y);
                        }
                    }
                    if (weight == 0)
                    {
                        visited[u] = 0;
                        M[v] = -1;
                        cont = true;
                        break;  
                    }
                    temp_w.push({weight, x, vs});
                    //printf("%d: (%d) \n", x, weight);
                }
            }
            if (cont == true)
            {
                continue;
            }
            //printf("\n");
            while (!w.empty())
            {
                Vertex x = w.top().index;
                size_t weight = w.top().weight;
                vector<int> vs = w.top().candidates;
                w.pop();
                if (x == v)
                {
                    store_weight = weight;
                    store_can = vs;
                    continue;
                }
                temp_w.push({weight, x, vs});
                //printf("%d: (%d) ", x, weight); 
            }
            //printf("\n");
            Backtracking(data, cs, qd, M, visited, matched_num+1, query_num, W, temp_w);
            w.push({store_weight, v, store_can});
            while (!temp_w.empty())
            {
                Vertex x = temp_w.top().index;
                size_t weight = temp_w.top().weight;
                vector<int> vs = temp_w.top().candidates;
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
                    w.push({weight, x, vs});
                }
            }
            visited[u] = 0;
        }
    }
    else
    {
        // find undecided vertex in query
        Vertex i = w.top().index;
        vector<int> temp_can = w.top().candidates;
        printf("current: %d list %d weight: %d\n", i, temp_can.size(), w.top().weight);
        /*for (size_t k = 0; k < temp_can.size(); k++)
            printf("%d ", temp_can[k]);
        printf("\n");*/
        size_t sum = w.top().weight;
        int children[query_num];
        for (size_t j =0; j < query_num; j++) children[j] = 0;
        for (size_t j = 0; j < qd.GetChildNum(i); j++)
        {
            M[qd.GetChild(i, j)]++;
            children[qd.GetChild(i,j)] = 1;
        }
        for (size_t j = 0; j < temp_can.size(); j++)
        {
            if (sum <= 0)
                continue ;
            bool extendable = true;
            Vertex candidate = temp_can[j];
                    //std::cout << i << " candidate: " << candidate << "\n";
                    //for each candidate see if the parents are valid
            if (visited[candidate] == 1)
                continue;
            /*for (size_t k = 0; k < qd.GetParentNum(i); k++)
            {
                Vertex c_parent = M[qd.GetParent(i, k)];
                //std::cout << i << " candidate: " << candidate << " parent: " << qd.GetParent(i, k) << "\n";
                if (c_parent == -1) extendable = false;
                else if (!data.IsNeighbor(candidate, c_parent)) extendable = false;
            }*/
            if (extendable && visited[candidate] == 0)
            {
                sum -= W[i][j];
                priority_queue<struct weight_info, vector<struct weight_info>, compare> temp_w;
                size_t store_weight = 0;
                visited[candidate] = 1;
                M[i] = candidate;
                vector<int> store_can;
                bool cont = false;
                    std::cout << "QV " << i << " DV " << candidate << "\n";
                    std::cout << "matched " << matched_num << "\n";
                    for (int32_t l = 0; l < query_num; l++)
                    {
                        std::cout << M[l] << " ";
                    }
                        std::cout << "\n";
                        /*for (int32_t m = 0; m < data.GetNumVertices(); m++)
                        {
                            std::cout << visited[m] <<  " ";
                        }*/
                        //std::cout << "\n\n";
                for (size_t o = 0; o < qd.GetChildNum(i); o++)
                {
                    Vertex x = qd.GetChild(i, o);
                    if (M[x] == -1)
                    {
                      
                            int weight = 0;
                            vector<int> vs;
                            for (size_t l = 0; l < cs.GetCandidateSize(x); l++)
                            {
                                Vertex y = cs.GetCandidate(x, l);
                                if (visited[y] == 0 && data.IsNeighbor(candidate, y)) 
                                {
                                    bool contain = true;
                                    for (size_t m = 0; m < qd.GetParentNum(x); m++)
                                    {
                                        Vertex pp = M[qd.GetParent(x, m)];
                                        if (!data.IsNeighbor(pp, y) || pp == y)
                                        {
                                            contain = false;
                                            break;
                                        }
                                    }
                                    if (contain)
                                    {
                                        weight += W[x][l];
                                        vs.push_back(y);  
                                    }
                                }

                            }
                            if (weight == 0)
                            {
                                visited[candidate] = 0;
                                M[i] = -1;
                                cont = true;
                                break;
                            }
                            temp_w.push({weight, x, vs});
                            //printf("%d: (%d %d) ", x, weight);
                        }       
                    }
                    if (cont == true)
                    {
                    continue;}
                while (!w.empty())
                {
                    Vertex x = w.top().index;
                    size_t weight = w.top().weight;
                    vector<int> vs = w.top().candidates;
                    w.pop();
                    if (x == i)
                    {
                        store_weight = weight;
                        store_can = vs;
                        continue;
                    }
                    if (children[x] == 0)
                    {
                        temp_w.push({weight, x, vs});
                        //printf("%d: (%d) ", x, weight); 
                    }
                }
                //printf("\n");
                Backtracking(data, cs, qd, M, visited, matched_num+1, query_num, W, temp_w);
                w.push({store_weight, i, store_can});
                while (!temp_w.empty())
                {
                    Vertex x = temp_w.top().index;
                    size_t weight = temp_w.top().weight;
                    vector<int> vs = temp_w.top().candidates;
                    temp_w.pop();
                    if (children[x] == 0)
                    {
                        w.push({weight, x, vs});
                    }
                }   
                visited[candidate] = 0;
                M[i] = -1;
            }
        }
        for (size_t j = 0; j < qd.GetChildNum(i); j++)
        {
            M[qd.GetChild(i, j)]--;
        }
    }
}
