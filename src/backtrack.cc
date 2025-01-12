/**
 * @file backtrack.cc
 *
 */

#include "../include/backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

int print_count = 0;

// an array to store weights of candidates for each vertex
size_t **candidates_weight;
// current vertex for backtracking
Vertex curr_v;

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
    candidates_weight = new size_t*[query_num];
    // a priority queue to store weights of vertexes for backtracking
    priority_queue<struct weight_info, vector<struct weight_info>, compare> weight_queue;

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
            candidates_weight[u] = new size_t[cs.GetCandidateSize(u)];
            for (size_t i = 0; i < cs.GetCandidateSize(u); i++)
                candidates_weight[u][i] = 1;
        }
        else
        {
            candidates_weight[u] = new size_t[cs.GetCandidateSize(u)];
            for (size_t i = 0; i < cs.GetCandidateSize(u); i++)
                candidates_weight[u][i] = 0;
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
                            temp += candidates_weight[v][j];
                    }
                    if (candidates_weight[u][i] == 0 || temp < candidates_weight[u][i])
                        candidates_weight[u][i] = temp;
                }
            }
        }
    }

    int *visited = new int[data_num];
    for (int i = 0; i < data_num; i++) visited[i] = 0;
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
                weight_queue.push({candidates_weight[i][0], i, candidates});
            }

        }
        else
            M[i] = -qd.GetParentNum(i) - 1;
    }

    // do backtracking
    Backtracking(data, cs, qd, M, visited, 0, query_num, weight_queue);

    // free
    delete[] visited;
    delete[] M;
    for (int i = 0; i < query_num; i++)
    {
        delete[] candidates_weight[i];
    }
    delete[] candidates_weight;

}

void Backtrack::Backtracking(const Graph &data, const CandidateSet &cs, DAG &qd,
                             Vertex M[], int visited[], int32_t matched_num, const int32_t query_num,
                             std::priority_queue<struct weight_info, vector <struct weight_info>, compare> weight_queue)
{
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
            exit(0);
    }
    // Case 2: start backtracking
    else if (matched_num == 0)
    {
        Vertex root = qd.GetRoot();

        size_t candidate_size = cs.GetCandidateSize(root);
        // Todo: vector vs priority queue?
        vector<size_t> candidates;
        bool fixed = false;
        if (M[root] >= 0)
            fixed = true;

        // If not, implement extendability check for all candidates of the root
        // Add extendable candidates to the candidate array
        if (!fixed)
        {
            for (size_t i = 0; i < candidate_size; i++)
            {
                Vertex root_candidate = cs.GetCandidate(root, i);
                bool contain = true;
                for (size_t j = 0; j < qd.GetChildNum(root); j++)
                {
                    Vertex root_child = qd.GetChild(root, j);
                    if (M[root_child] >= 0 && !data.IsNeighbor(M[root_child], root_candidate))
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
        
        int **children_candidates = new int*[qd.GetChildNum(root)];

        // Update to check if a child is extendable or not
        for (size_t j = 0; j < qd.GetChildNum(root); j++)
        {
            Vertex x = qd.GetChild(root, j);

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
                        children_candidates[j][l] = candidates_weight[x][l];
                }
            }
        }

        for (size_t i = 0; i < cs.GetCandidateSize(root); i++)
        {
            Vertex root_candidate = cs.GetCandidate(root, candidates[i]);
            std::priority_queue<struct weight_info, vector <struct weight_info>, compare> temp_weight_queue;

            bool cont = false;
            if (!fixed)
            {
                visited[root_candidate] = 1;
                M[root] = root_candidate;
            }
            // Update extendable vertexes
            for (size_t j = 0; j < qd.GetChildNum(root); j++)
            {
                Vertex root_child = qd.GetChild(root, j);
                if (M[root_child] == -1)
                {
                    size_t weight = 0;
                    vector<int> root_child_candidates;
                    for (size_t l = 0; l < cs.GetCandidateSize(root_child); l++)
                    {
                        Vertex child_candidate = cs.GetCandidate(root_child, l);
                        if (data.IsNeighbor(root_candidate, child_candidate))
                        {
                            weight += candidates_weight[root_child][l];
                            root_child_candidates.push_back(l);
                        }
                    }
                    if (weight == 0)
                    {
                        if (!fixed)
                        {
                            visited[root_candidate] = 0;
                            M[root] = -1;
                        }
                        cont = true;
                        break;

                    }
                    temp_weight_queue.push({weight, root_child, root_child_candidates});
                }
            }
            if (cont == true)
                continue;
            
            // do backtracking
            while (!weight_queue.empty())
            {
                Vertex x = weight_queue.top().index;
                size_t weight = weight_queue.top().weight;
                vector<int> vs = weight_queue.top().candidates;
                weight_queue.pop();
                if (M[x] >= 0)
                    temp_weight_queue.push({weight, x, vs});
            }
            Backtracking(data, cs, qd, M, visited, matched_num + 1, query_num, temp_weight_queue);

            while (!temp_weight_queue.empty())
            {
                Vertex x = temp_weight_queue.top().index;
                size_t weight = temp_weight_queue.top().weight;
                vector<int> vs = temp_weight_queue.top().candidates;
                temp_weight_queue.pop();
                if (M[x] >= 0)
                {
                    weight_queue.push({weight, x, vs});
                }
            }
            if (!fixed)
                visited[root_candidate] = 0;
        }
    }
    // Case 3: during backtracking
    else
    {
        // find undecided vertex with the smallest weight in query
        Vertex curr = weight_queue.top().index;
        vector<int> curr_candidates = weight_queue.top().candidates;
        //size_t curr_weight = weight_queue.top().weight;
        weight_queue.pop();
        bool fixed = false;

        if (M[curr] >= 0)
            fixed = true;

        int children[query_num];
        int **children_candidates = new int*[qd.GetChildNum(curr)];
        for (int j =0; j < query_num; j++) children[j] = 0;
        for (size_t j = 0; j < qd.GetChildNum(curr); j++)
        {
            Vertex curr_child = qd.GetChild(curr, j);

            if (M[curr_child] < 0) M[curr_child]++;

            if (M[curr_child] == -1)
            {
                children_candidates[j] = new int[cs.GetCandidateSize(curr_child)];
                for (size_t l = 0; l < cs.GetCandidateSize(curr_child); l++)
                {
                    children_candidates[j][l] = 0;
                    Vertex curr_child_candidate = cs.GetCandidate(curr_child, l);
                    bool contain = true;
                    for (size_t m = 0; m < qd.GetChildNum(curr_child); m++)
                    {
                        Vertex curr_child_child = qd.GetChild(curr_child, m);
                        if (M[curr_child_child] >= 0 && !data.IsNeighbor(M[curr_child_child], curr_child_candidate))
                        {
                            contain = false;
                            break;
                        }
                    }
                    if (contain)
                    {
                        for (size_t m = 0; m < qd.GetParentNum(curr_child); m++)
                        {
                            Vertex curr_child_parent = qd.GetParent(curr_child, m);
                            if (curr_child_parent == curr)
                                continue ;
                            Vertex p = M[curr_child_parent];
                            if (p == -1 || !data.IsNeighbor(p, curr_child_candidate))
                            {
                                contain = false;
                                break;
                            }
                        }
                    }
                    if (contain)
                    {
                        children[curr_child] = 1;
                        children_candidates[j][l] = candidates_weight[curr_child][l];
                    }
                }
            }
        }
        for (size_t j = 0; j < curr_candidates.size(); j++)
        {
            Vertex candidate = cs.GetCandidate(curr, curr_candidates[j]);
            std::priority_queue<struct weight_info, vector <struct weight_info>, compare> temp_weight_queue;

            if (!fixed)
            {
                if (visited[candidate] == 1) continue;
                visited[candidate] = 1;
                M[curr] = candidate;
            }

            bool cont = false;
            for (size_t o = 0; o < qd.GetChildNum(curr); o++)
            {
                Vertex curr_child = qd.GetChild(curr, o);
                if (M[curr_child] == -1)
                {
                    size_t weight = 0;
                    vector<int> vs;
                    for (size_t l = 0; l < cs.GetCandidateSize(curr_child); l++)
                    {
                        Vertex curr_child_candidate = cs.GetCandidate(curr_child, l);
                        if (data.IsNeighbor(candidate, curr_child_candidate) && children_candidates[o][l] > 0)
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
                    temp_weight_queue.push({weight, curr_child, vs});
                }
            }

            if (cont == true)
                continue;
            while (!weight_queue.empty())
            {
                Vertex x = weight_queue.top().index;
                size_t weight = weight_queue.top().weight;
                vector<int> vs = weight_queue.top().candidates;
                weight_queue.pop();
                if (children[x] == 0 || M[x] >= 0)
                {
                    temp_weight_queue.push({weight, x, vs});
                }
            }
            Backtracking(data, cs, qd, M, visited, matched_num + 1, query_num, temp_weight_queue);
            while (!temp_weight_queue.empty())
            {
                Vertex x = temp_weight_queue.top().index;
                size_t weight = temp_weight_queue.top().weight;
                vector<int> vs = temp_weight_queue.top().candidates;
                temp_weight_queue.pop();
                if (children[x] == 0 || M[x] >= 0)
                {
                    weight_queue.push({weight, x, vs});
                }
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
