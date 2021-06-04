/**
 * @file DAG.cc
 *
 */

#include "../include/DAG.h"

const Graph *gg;
const CandidateSet *gcs;

// Todo: how to sort vertexes at the same level
bool cand_compare(const Vertex& v1, const Vertex& v2)
{
    if (gcs->GetCandidateSize(v1) < gcs->GetCandidateSize(v2))
        return true;
    else
        return false;
}

bool lf_compare(const Vertex& v1, const Vertex& v2)
{
    if (gg->GetLabelFrequency(gg->GetLabel(v1)) < gg->GetLabelFrequency(gg->GetLabel(v2)))
        return true;
    else
        return false;
}

bool deg_compare(const Vertex& v1, const Vertex& v2)
{
    if (gg->GetDegree(v1) > gg->GetDegree(v2))
         return true;
    else
        return false;
}

DAG::DAG(const Graph &G, const CandidateSet &cs, const Graph &data) {
    gg = &G;
    gcs = &cs;
    
    int size = G.GetNumVertices();
    int visited[size];
    for (int i = 0; i < size; i++) visited[i] = 0;
    
    child.resize(size);
    parent.resize(size);

    Vertex rt = 0;
    int count;
    int temp;
    size_t min_size = SIZE_MAX;
    std::vector<Vertex> q;
   /* 
    int avg[size];

    for (Vertex v = 0; v < size; v++)
    {
        avg[v] = 0;
        for (size_t i = G.GetNeighborStartOffset(v); i < G.GetNeighborEndOffset(v); i++)
        {
            Vertex u = G.GetNeighbor(i);
            avg[v] += cs.GetCandidateSize(u);
        }
        avg[v] += cs.GetCandidateSize(v);
        avg[v] /= (1 + G.GetDegree(v));
    }*/
    // Todo: how to select root?
    for (Vertex v = 0; v < size; v++) { 
        
        //printf("%d :", v); 
        // Calulate initial C 
        count = 0;
        /*(for (size_t i = 0; i < data.GetNumVertices(); i++)
        {
            if (data.GetLabel(i) == G.GetLabel(v) && data.GetDegree(i) >= G.GetDegree(v))
                count++;
        }*/
        count = cs.GetCandidateSize(v);  
        //printf("%d : %d %d\n", v, count / G.GetDegree(v), G.GetDegree(v));
        if (min_size >= count / G.GetDegree(v)) {
            rt = v;
            min_size = count / G.GetDegree(v);
       } 
       temp = 0;
       //count = cs.GetCandidateSize(v);
       /*for (size_t i = G.GetNeighborStartOffset(v); i < G.GetNeighborEndOffset(v); i++)
       {
           Vertex u = G.GetNeighbor(i);
           //printf(" %d", u);
           temp += cs.GetCandidateSize(u); 
           //temp += avg[u];
       }*/
       //printf("\n");
       //count = ((cs.GetCandidateSize(v) + temp) / (G.GetDegree(v) + 1));
       /*if (min_size > count)
       {
           rt = v;
           min_size = count;
       }*/
       /*
       if (min_size > count / (G.GetNeighborEndOffset(v) - G.GetNeighborStartOffset(v)))
       {
           rt = v;
           min_size = count / (G.GetNeighborEndOffset(v) - G.GetNeighborStartOffset(v));
       }*/
       /*
       if (min_size > count / G.GetDegree(v))
       {
           rt = v;
           min_size = count / G.GetDegree(v);
       }*/

    }
    //printf("rt: %d\n", rt);
    
    root = rt;
    q.push_back(root);
    visited[rt] = 1;

    while (q.size() != 0)
    {
        std::sort(q.begin(), q.end(), deg_compare);
        //std::sort(q.begin(), q.end(), lf_compare);
        std::sort(q.begin(), q.end(), cand_compare);
        
        Vertex u = q[0];
        //printf("node: %d\n", u);
        q.erase(q.begin());
        for (size_t i = G.GetNeighborStartOffset(u); i < G.GetNeighborEndOffset(u); i++)
        {
                Vertex v = G.GetNeighbor(i);
                
            
                std::set<Vertex>::iterator it = parent[u].find(v);
                if (it == parent[u].end()) {
                   // printf("p: %d c: %d cd: %d lf: %d\n", u, v, G.GetDegree(v), G.GetLabelFrequency(G.GetLabel(v)));
                    /*if (cs.GetCandidateSize(v) == 1)
                    {
                            child[v].insert(u);
                            parent[u].insert(v);
                    }
                    else
                    {*/
                        child[u].insert(v);

                        parent[v].insert(u);
                    //}
                    if (visited[v] == 0)
                    {
                        visited[v] = 1;
                        q.push_back(v);
                    }
                }
         }

    }
    /*while (true) {

        std::sort(q.begin(), q.end(), deg_compare);
        //std::sort(q.begin(), q.end(), lf_compare);
        std::sort(q.begin(), q.end(), cand_compare);
         
        count = q.size();
        if (count == 0)
            break;

        while(count > 0) {
            Vertex u = q[0];
            //printf("node: %d\n", u);
            q.erase(q.begin());
            count--;
        
            for (size_t i = G.GetNeighborStartOffset(u); i < G.GetNeighborEndOffset(u); i++)
            {
                Vertex v = G.GetNeighbor(i);
                
            
                std::set<Vertex>::iterator it = parent[u].find(v);
                if (it == parent[u].end()) {
                    //printf("p: %d c: %d cd: %d lf: %d\n", u, v, G.GetDegree(v), G.GetLabelFrequency(G.GetLabel(v)));
                    child[u].insert(v);

                    parent[v].insert(u);
                    if (visited[v] == 0)
                    {
                        visited[v] = 1;
                        q.push_back(v);
                    }
                }
            }
        }
    }*/
    
}

DAG::~DAG() {}
