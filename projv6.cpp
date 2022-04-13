#include <iostream>
#include <list>
#include <stack>
#include <queue>
#include <vector>
#include <iterator>
#include <cmath>
#include <unordered_map>
#include <utility>
#include <boost/functional/hash.hpp>

using namespace std;
using namespace boost;



#define N 12    // Number of unknowns


class graph {
    public:
    int v, e;
    list <int> *adj;
    list <int> *ie;
    int **edges, *kr, *kv, *bfsa, *bfsp, *bfsd;
    class AlEdge {
        public :
        int bfsa,bfsp;
        AlEdge(int a, int p)
        {
            bfsa = a;
            bfsp = p;
        } 
        

        bool operator==(const AlEdge& a) const
        {
            return bfsa == a.bfsa && bfsp == a.bfsp;
        }
    };
    struct hash_fn
    {
        std::size_t operator() (const AlEdge& ae) const
        {
            pair<int,int> AEpair;
            AEpair = {ae.bfsa,ae.bfsp};
            boost::hash<pair<int,int>> pair_hash;
            return pair_hash(AEpair);
        }
    };
    unordered_map<AlEdge, int, hash_fn> AlEdgDepth;
    graph(int, int);
    void add_edge(int, int, int, int);
    bool check(int, int, int[], int[]);
    int re1(int, int);
    int re2(int, int);
    void modbfs();
    void gencycle();
    void geneq(vector<int>*);
    void solve(int[N][N]);
    int determinant(int[N][N], int);
};

graph ::graph(int x, int y){
    v = x;
    e = y;
    adj = new list <int>[v];
    edges = new int*[e];
    for(int i = 0; i < e; i++)
        edges[i] = new int[2];
    ie = new list <int>[v];
    kr = new int[e];
    kv = new int[e];
    bfsa = new int[e+1];
    bfsd = new int[e+1];
    bfsp = new int[e+1];
}

void graph::add_edge(int x, int y, int ro, int vo){
    static int f = 0;
    adj[x].push_back(y);
    adj[y].push_back(x);
    ie[x].push_back(f);
    ie[y].push_back(f);
    edges[f][0] = x;
    edges[f][1] = y;
    kr[f] = ro;
    kv[f++] = vo;
}

bool graph::check(int x, int y, int bfsp[], int bfsa[]){
    
   AlEdge ae(x,y);
    if(bfsa[y] == y && bfsp[y] == x)
        return false;
    else if(AlEdgDepth.find(ae) == AlEdgDepth.end())
        return true;    
    
}

int graph::re1(int s, int p){
    for(int i = 0; i < e; i++){
        if(edges[i][0] == s && edges[i][1] == p)
            return i;
    }
    return -1;
}

int graph::re2(int s, int p){
    for(int i = 0; i < e; i++){
        if(edges[i][0] == p && edges[i][1] == s)
            return i;
    }
    return -1;
}

int graph::determinant(int matrix[N][N], int n) {
   int det = 0;
   int submatrix[N][N];
   if (n == 2)
   return ((matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]));
   else {
      for (int x = 0; x < n; x++) {
         int subi = 0;
         for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
               if (j == x)
                continue;
               submatrix[subi][subj] = matrix[i][j];
               subj++;
            }
            subi++;
         }
         det = det + (pow(-1, x) * matrix[0][x] * determinant(submatrix, n-1));
      }
   }
   return det;
}

void graph::solve(int r[N][N]){
    double f = determinant(r, e) + 0.0;
    int mat01[N][N];
    for(int k = 0; k < e; k++){
        for(int i = 0; i < e; i++){
            for(int j = 0; j < e+1; j++){
            if(j == k)
                mat01[i][j] = r[i][e];
            else
                mat01[i][j] = r[i][j];
            }
        }
        cout << "I(" << edges[k][0] << "->" << edges[k][1] << ") = " << -determinant(mat01, e) / f << "\n";
    }
}


void graph::geneq(vector<int> *cycles){
    int r[N][N];

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            r[i][j] = 0;
        }
    }

    
    for(int i = 0; i < v-1; i++){
        for (auto j = ie[i].begin(); j != ie[i].end(); j++){
            if(i == edges[*j][0])
                r[i][*j] = -1;
            else if(i == edges[*j][1])
                r[i][*j] = 1;
        }
    }

    
    for(int i = v-1; i < e; i++){
        for(int j = 0; j < e; j++){
            for(int k = 0; k < cycles[i-v+1].size()-1; k++){
                if(j == re1(cycles[i-v+1][k], cycles[i-v+1][k+1])){
                    r[i][j] = kr[j];
                }
                else if(j == re2(cycles[i-v+1][k], cycles[i-v+1][k+1])){
                    r[i][j] = -kr[j];
                }
            }

        }
    }
    
    for(int i = v-1; i < e; i++){
        for(int j = 0; j < e; j++){
            for(int k = 0; k < cycles[i-v+1].size()-1; k++){
                if(j == re1(cycles[i-v+1][k], cycles[i-v+1][k+1])){
                    r[i][e] += kv[j];
                }
                else if(j == re2(cycles[i-v+1][k], cycles[i-v+1][k+1])){
                    r[i][e] -= kv[j];
                }
            }
        }
    }

    solve(r);
}

void graph::gencycle(){
    int dcount,Av,Aov;
    vector<int> *cycles;
    cycles = new vector<int>[e-v+1];
    for(int i = 0; i <= e; i++)
        cout<<bfsa[i]<<" ";
    cout<<'\n';
    for(int i = 0; i <= e; i++)
        cout<<bfsp[i]<<" ";
    cout<<'\n';
    for(int i = 0; i <= e; i++)
        cout<<bfsd[i]<<" ";
    cout<<'\n';
    for(int i = v; i <= e; i++){
        stack <int> temp;
        dcount = bfsd[i];
        Aov = bfsa[i];
        cycles[i - v].push_back(bfsa[i]);
        Av = bfsp[i];
        cycles[i - v].push_back(Av);
        dcount--;
        
        while(dcount != bfsd[bfsa[i]]){
            Av = bfsp[Av];

            cycles[i - v].push_back(Av);
            dcount--;
        }

        temp.push(Aov);

        while(Av != Aov){
            Av = bfsp[Av];
            Aov = bfsp[Aov];
            cycles[i - v].push_back(Av);
            temp.push(Aov);
        }

        temp.pop();
        while(!temp.empty())
        {
            cycles[i - v].push_back(temp.top());
            temp.pop();
        }
    }
    geneq(cycles);
}

void graph::modbfs(){
    int s = 0, p;
    bool visited[v] = {false};
    bfsa[0] = 0;
    bfsd[0] = -1;
    bfsp[0] = 0;
    queue<int> node, parent;
    visited[s] = true;
    node.push(s);
    parent.push(s);
    int count = v;
    while(!node.empty()){
        s = node.front();
        p = parent.front();
        bfsa[s] = s;
        bfsp[s] = p;
        bfsd[s] = bfsd[p]+1;
        node.pop();
        parent.pop();
        for (auto i = adj[s].begin(); i != adj[s].end(); ++i){
            if (!visited[*i]){
                visited[*i] = true;
                node.push(*i);
                parent.push(s);
            }
            else{
                if(check(*i,s,bfsp,bfsa)){
                    bfsa[count] = s;
                    bfsp[count++] = *i;
                    AlEdge ae(s,*i);
                    AlEdgDepth.insert(make_pair(ae,bfsd[*i]+1));
                }   
            }
        }
    }
    for(int j = v; j <= e; j++)
        bfsd[j] = bfsd[bfsp[j]] + 1;
    gencycle();
}



int main(){
    
   int n, e;
    int n1,n2,bv,br;
    int i,j,k;
    char ch;
    cin>>n;
    cin>>e;
    graph g(n,e);
    cout<<"Add edges, enter resistance and voltage source in them : \n";
    cout<<"(Note : Enter 0 if no resistance or voltage source exists) \n";
    for(int i = 0; i < n-1; i++)
    {
        for(int k = i+1; k  < n; k++)
        {
            cout<<'('<<i<<','<<k<<')';
            cout<<"Add edge(Y/N) : ";
            cin>>ch;
            cin.ignore();
            if(ch == 'Y' || ch == 'y')
            {
                cout<<"\nResistance(if any) : ";
                cin>>br;
                cout<<"\nVoltage source(if any) : ";
                cin>>bv;
                g.add_edge(i,k,br,bv);
            }
            cout<<'\n';
        }
        
    }
    g.modbfs();
    return 0;
}