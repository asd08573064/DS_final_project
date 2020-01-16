#include <fstream>
#include <iostream>
#include <set>
#include <string.h>
#include <map>
#include <algorithm>
#include <time.h>
#include <climits>
#include <queue>
#include <vector>
using namespace std;

int max_vertices = 82168;
bool edges[82168][82168];
int core[82168];
int degree[82168];
bool visited[82168];
int min_core,max_core;


class ver {
            
    public:
        int i, d;
        void set_degree(int dd) { d = dd; }
};

class Maxclique {
    class Vertices {   
        static bool desc_degree( ver vi,  ver vj) { 
            return (vi.d > vj.d);
        }
        public:
            ver *v;
            int sz;

            Vertices(int size) : sz(0) {
                v = new ver[size]; 
            }

            void discard() {
                if (v)
                    delete [] v; 
            }
            
            void initialize_colors();

            void sort(){ 
                std::sort(v, v+sz, desc_degree); 
            }

            void set_degrees(Maxclique&);

           
            void delete_(){
                sz--; 
            };

            void add( int ii){
                v[sz++].i = ii; 
            };

            ver& end()  { 
                return v[sz - 1]; 
            };
    };

    class ColorClass{
    public:
        int *vi;
        int sz;
        ColorClass(){
            this->sz = 0;
            this->vi = NULL;
        }
        ColorClass(int sz){
            this->sz = sz;
            this->vi = NULL;
            this->init(sz); 
        }
        void init(int sz) {
            vi = new int[sz]; 
            reset();
        }
        void add(int ii){ 
            vi[sz++] = ii; 
        };
        void delete_() { 
            sz--; 
        };
        void reset() { 
            sz = 0; 
        };
    };


    Vertices V;
    ColorClass *Color, q_max, q;

    void intersection1( Vertices&, Vertices&);

    void color(Vertices&);

    

    bool intersection2( int,  ColorClass&);

    void _mcq(int*&, int&);

    void expand(Vertices);
    public:
        queue<int> que;
        
        Maxclique (int sz) :V(sz), q(sz), q_max(sz) {
            for (int i=0; i < sz; i++) V.add(i);
            Color = new ColorClass[sz + 1];
            for (int i=0; i < sz + 1; i++) 
                Color[i].init(sz + 1);
        }
        void k_core(int tar);
        void mcq(int* &maxclique, int &sz) { 
            _mcq(maxclique, sz); 
        }
};

void Maxclique::Vertices::initialize_colors() { 
     int max_degree = v[0].d;
    for (int i = 0; i < max_degree; i++)
        v[i].d = (i + 1);
    for (int i = max_degree; i < sz; i++)
        v[i].d = (max_degree + 1);
}

void Maxclique::intersection1( Vertices &A, Vertices &B) {
    for (int i = 0; i < A.sz - 1; i++) {
        if (edges[A.end().i][A.v[i].i])
            B.add(A.v[i].i);
    }
}

bool Maxclique::intersection2( int pi,  ColorClass &A) {
    for (int i = 0; i < A.sz; i++)
        if (edges[pi][A.vi[i]])
            return true;
    return false;
}

void Maxclique::Vertices::set_degrees(Maxclique &m) { 
    for (int i=0; i < sz; i++) {
        int d = 0;
        for (int j=0; j < sz; j++)
            if (edges[v[i].i][v[j].i]) d++;
        v[i].d = d;
    }
}

void Maxclique::k_core(int tar){
    int count = 0;                      
    for(int k = min_core; k <= max_core; k++){
        if(count == max_vertices)
            break;
        for(int i = 0; i < max_vertices; i++){
            if(!visited[i]&&degree[i] <= k){
                visited[i] = true;
                core[i] = k;
                que.push(i);
                while(!que.empty()){
                    //cout << "done" << endl;
                    int now = que.front();
                    for(int j = 0; j < 82168; j++){
                        if(edges[now][j]){
                            degree[j]--;
                            if(!visited[j] && degree[j] <= k){
                                visited[j] = true;
                                que.push(j);
                                core[j] = k;
                                //cout << core[j] << endl;
                                count++;
                            }
                        }
                    }
                    que.pop();
                }  
            }
        }
    }
    int vertex_number = 0;
    FILE *out_kcore;
    out_kcore = fopen ("kcore.txt","w");
    for (int i = 0; i < 82168; i++){
        if(core[i] >= tar){
            fprintf(out_kcore, "%d %d\n", i, core[i]);
        }
        if(core[i] == 1499){
            vertex_number++;
        }
    }
}

void Maxclique::color(Vertices &R) {
    int j = 0,max = 1,k = 1;
    Color[1].reset();
    Color[2].reset();
    int min = (q_max.sz - q.sz + 1);
    for (int i=0; i < R.sz; i++) {
        int pi = R.v[i].i;
        k = 1;
        while (intersection2(pi, Color[k]))
            k++;
        if (k > max) {
            max = k;
            Color[max + 1].reset();
        }
        Color[k].add(pi);
        if (k < min) 
            R.v[j++].i = pi;
    }
    if (j > 0) 
        R.v[j-1].d = (0);
    if (min <= 0) 
        min = 1;
    for (k = min; k <= max; k++){
        for (int i = 0; i < Color[k].sz; i++) {
            R.v[j].i = Color[k].vi[i];
            R.v[j++].d = (k);
        }
    }
}

void Maxclique::expand(Vertices R) {
    while (R.sz) {
        if (q.sz + R.end().d > q_max.sz) {
            q.add(R.end().i);
            Vertices Rp(R.sz);
            intersection1(R, Rp);
            if (Rp.sz) {
                color(Rp);
                expand(Rp);
            }
            else if (q.sz > q_max.sz) { 
                cout <<  "clique size = " << q.sz << std::endl; 
                q_max = q;
                for (int j = 0; j < q.sz; j++)//
                    q_max.vi[j] = q.vi[j];    //
                q_max.sz = q.sz;              // q = q_max
            }    
            Rp.discard();
            q.delete_();
        }
        else {
            return;
        }
        R.delete_();
    }
}

void Maxclique::_mcq(int* &maxclique, int &sz) { 
    V.set_degrees(*this);
    V.sort();
    V.initialize_colors();
    expand(V);
    maxclique = new int[q_max.sz]; 
    for (int i=0; i<q_max.sz; i++) { 
        maxclique[i] = q_max.vi[i];
    }
    sz = q_max.sz;
}

void read_dimacs(string name, int &size) {
    ifstream f (name.c_str());
    string buffer;
    assert(f.is_open());
    set<int> v;
    multimap<int,int> e;
    while (!getline(f, buffer).eof()) {
        int vi, vj;
        sscanf(buffer.c_str(), "%d %d", &vi, &vj);
        v.insert(vi);
        v.insert(vj);
        edges[vi][vj] = true;
        edges[vj][vi] = true;
        degree[vi]++;
        degree[vj]++;
        // adj_list[vi].push_back(vj);
        // adj_list[vj].push_back(vi);
        if(degree[vi] < min_core){
            min_core = degree[vi];
            if(degree[vi] > degree[vj])
                min_core = degree[vj];
        }
        if(degree[vi] > max_core){
            max_core = degree[vi];
            if(degree[vi] < degree[vj])
                max_core = degree[vj];
        }
    }
    size = *v.rbegin() + 1;
    f.close();
}

static bool compare( int vi,  int vj) { 
    return (vi > vj);
}

int main(int argc, char *argv[]) {
    FILE *out_clique;
    out_clique = fopen ("clique.txt","w");
    cout << "args = " << argv[1] << endl;
    int size;
    read_dimacs(argv[1],size);
    clock_t start1 = time(NULL);
    clock_t start2 = clock();
    Maxclique m(size);
    int *q_max;
    int qsize;
    m.mcq(q_max, qsize);
    string k = string(argv[2]);
    m.k_core(atoi(&k[0]));
    std::sort(q_max,q_max+qsize,compare);
    for (int i = 0; i < qsize; i++){
        fprintf(out_clique, "%d\n", q_max[i]);
    }

    cout << endl;
    cout << "Size = " << qsize << endl;
    cout << "Time = " << difftime(time(NULL), start1) << endl;
    cout << "Time (precise) = " << ((double) (clock() - start2)) / CLOCKS_PER_SEC << endl << endl;
    return 0;
}
