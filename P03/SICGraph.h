//
// Created by Cesar on 11/6/2016.
//

#ifndef SICGRAPH_H
#define SICGRAPH_H

#include <string.h>
#define GRAPH_X         63
#define GRAPH_Y         63
#define GRAPH_Z         832
#define GRAPH_L         3969
using namespace std;



struct Vertex{
    int x_pos;
    int y_pos;
    float x_bar;
    float Sxx;
    bool landmass;
    bool calculated;

};
struct Node{
    int xpos;
    int ypos;
    int comp;
    Node *next;
    int distime;
    int fintime;
    bool landmass;
};

class SICGraph{
public:
    int SIC_MAIN(float rt);
    void create2d();
    void creategraph(float rt);
    void run(float rt);
    void read();
    void read_2d(string filename, int k);
    void distr();
    void DFS();
    void DFS_visit(int key);
    void create_weight_D();
    void CPL();
    void CL_CO1();
    void Floyd_Warshall();
    int min(int x, int y);
};


#endif //SICGRAPH_H
