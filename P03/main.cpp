#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "SICGraph.h"

using namespace std;
float ***data =  new float**[GRAPH_X];
Vertex **data_2d = new Vertex*[GRAPH_X];
Node *graph;
int **D = new int*[GRAPH_L];
int time_count;
int comp_size;
int comp_count;




/*
 * This functions creates the original weighted 2d array for the
 * floyd warshall algorithm. It uses 10,000 as infinity, since there are 63*63 = 3969 maximum vertices.
 */
void SICGraph::create_weight_D() {
    printf("Creating weight graph...\n");
    for (int a = 0; a<GRAPH_L;a++)
        D[a] = new int[GRAPH_L];

    Node *temp;
    int key;
    for(int i = 0; i<GRAPH_L;i++){
        for(int j = 0; j<GRAPH_L;j++)
            if(i == j)
                D[i][j] = 0;
            else
            D[i][j] = 10000;
    }

    for(int i = 0; i<GRAPH_L;i++){
        if(!graph[i].landmass) {
            temp = graph[i].next;
            while (temp != NULL) {
                key = (temp->xpos * GRAPH_X) + temp->ypos;
                D[i][key] = 1;
                temp = temp->next;
            }
        }

    }

}
/*
 * returns min of two values
 */
int SICGraph::min(int x, int y) {
    if(x >= y)
        return y;
    else return x;
}

/*
 * Floyd-Warshall algorithm that calculates All-Pairs-Shortest-Path for the graph which has
 * a weight array representation D.
 */
void SICGraph::Floyd_Warshall() {
    printf("Floyd-Warshalling the weights...\n");
    for( int k = 0; k<GRAPH_L;k++) {
        if (!graph[k].landmass) {
            for (int i = 0; i < GRAPH_L; i++) {
                if (!graph[i].landmass) {
                    for (int j = 0; j < GRAPH_L; j++) {
                        D[i][j] = min(D[i][j], D[i][k] + D[k][j]);
                    }
                }
            }
        }
    }
}

/*
 * This function calculates the Characteristic Path length of the graph
 * with the APSP array created by the Floyd-Warshall algorithm. It calculates
 * the CPL for every component, then averages all the component's CPL over the
 * number of components.
 */
void SICGraph::CPL() {
    printf("Characterizing path lengths...\n");
    float graph_sum = 0;
    float comp_sum = 0;
    float comp_size = 0;
    float nC2 = 0;
    for(int i = 1; i<comp_count;i++){
        for(int a = 0;a<GRAPH_L;a++){
            if(graph[a].comp == i) {
                comp_size = comp_size+1;
                for (int b = a + 1; b < GRAPH_L; b++) {
                    if(D[a][b]!= 0 && D[a][b]!= 10000){
                        comp_sum = comp_sum + D[a][b];
                    }

                }
            }

        }
        if(comp_size > 1) {
            nC2 = (comp_size * (comp_size - 1)) / 2;
            graph_sum = graph_sum + (comp_sum / nC2);
        }
            comp_size = 0;
            comp_sum = 0;


    }
    float CPL = graph_sum/comp_count;
    printf("The CPL for this graph is %f\n",CPL);

}

/*
 * CL_CO1/2 calculate the clustering coefficient of the graph. They calculate the clustering coefficient
 * for every component in the graph, then averages it over the number of components.
 */
void SICGraph::CL_CO1() {
    printf("Coefficienting the clusters...\n");
    float CC=0;
    float graph_sum=0;
    int comp_sum=0;
    int comp_size=0;
    int sum=0;
    Node *temp;

    for(int i = 1; i<comp_count;i++){

        for(int a = 0;a<GRAPH_L;a++){

            if(graph[a].comp == i) {

                comp_size = comp_size+1;
                temp = graph[a].next;
                while(temp != NULL){

                    sum = sum+1;
                    temp = temp->next;
                }

                comp_sum = comp_sum + sum;
                sum = 0;
            }


        }
        if(comp_size >1) {
            graph_sum = graph_sum + (comp_sum / (comp_size * (comp_size - 1)));
        }
        comp_size = 0;
        comp_sum = 0;


    }
    CC = graph_sum/comp_count;
    printf("The Clustering Coefficient for this graph is %f\n",CC);
}

/*
 * This function prints out the
 * degree distribution of the current graph.
 * Uses a loop to count the number of elements in the linked list of edges for each
 * vertex in the graph
 */
void SICGraph::distr() {
    int sum = 0;
    int landmasses = 0;
    int *distributions = new int[300];
    Node *temp;

    for(int a = 0; a<300;a++)
        distributions[a] = 0;

    for(int i = 0;i<GRAPH_L;i++){
        sum = 0;
        temp = graph[i].next;
        if(graph[i].landmass)
            landmasses = landmasses+1;
        while(temp != NULL){
            sum = sum+1;
            temp = temp->next;
        }
        distributions[sum] = distributions[sum]+1;
    }
    printf("Degree distributions:\n");
    for(int b = 0;b<300;b++)
        printf("With %d degrees: %d\n",b,distributions[b]);

    printf("Out of these, there are %d landmasses\n",landmasses);
}

/*
 * This function creates a 2d array of data used to create the graph more easily,
 * calculates the mean, and Sx for each element in the 3d array.
 */
void SICGraph::create2d() {
    for (int a = 0; a<GRAPH_X;a++)
        data_2d[a] = new Vertex[GRAPH_Y];
int landmass_count = 0;
    float mean = 0;
    float sum = 0;
    int n = 0;
    float sum_2 = 0;
    float x = 0;
    for(int i = 0;i<GRAPH_X;i++){
        for(int j = 0;j<GRAPH_Y;j++){
            mean=0;
            sum=0;
            n=0;
            data_2d[i][j].calculated = false;
            for(int k = 0;k<GRAPH_Z;k++){
                if(data[i][j][k] == 168) {
                    data_2d[i][j].landmass = true;
                    landmass_count = landmass_count + 1;
                    break;
                }
                if(data[i][j][k] != 157 && !data_2d[i][j].landmass){
                    n = n+1;
                    sum = sum + data[i][j][k];
                }
            }
                      // caclulate sxx,xpos,ypos
            mean = sum/n;
            data_2d[i][j].x_pos=i;
            data_2d[i][j].y_pos=j;
            data_2d[i][j].x_bar = mean;
             sum_2 = 0;
             x = 0;
            for (int k =0;k<GRAPH_Z;k++){
                if(data[i][j][k] != 157 && !data_2d[i][j].landmass){
                    x = data[i][j][k] - data_2d[i][j].x_bar;
                    sum_2 = sum_2 + powf(x,2);
                }
            }
            data_2d[i][j].Sxx = sum_2;
        }
    }
    printf("There are a total of %d landmasses\n",landmass_count);
}
/*
 * This function begins the depth first search of the graph
 * marking every vertex with its discovery and finish time, while at the same time
 * counting the number of components and their sizes witht the help of the recursive
 * function DFS_visit.
 */
void SICGraph::DFS() {
    for(int i = 0; i<GRAPH_L;i++)
        graph[i].distime = -1;

    int *counter = new int[5800];
    for(int i = 0; i<5800;i++)
        counter[i] = 0;
    time_count = 0;
    comp_count = 0;
    int key = 0;
    for(int i = 0;i<GRAPH_L;i++){
        if(graph[i].distime <=0){
            comp_count = comp_count+1;
            comp_size = 0;
             key = (graph[i].xpos*GRAPH_X) + graph[i].ypos;
            DFS_visit(key);
            counter[comp_size]++;
        }

    }
    for(int i = 0; i<5800;i++) {
        if(counter[i]>0)
            printf("There are %d components of size %d\n", counter[i], i);
    }
}
/*
 * This function helps DFS to visit every adjacent node
 * that has not been discovered yet by recursively calling itself
 */
void SICGraph::DFS_visit(int key) {
    comp_size = comp_size+1;
    time_count = time_count+1;
    graph[key].distime = time_count;
    graph[key].comp = comp_count;
    Node *temp;
    int key2 = 0;
    temp = graph[key].next;
    while(temp != NULL){

        key2 = (temp->xpos*GRAPH_X) + temp->ypos;
        if(graph[key2].distime == -1)
            DFS_visit(key2);
        temp = temp->next;

    }
    graph[key].fintime = time_count;
}
/*
 * This function creates a graph based on the given r threshhold (rt)
 * Uses the previously created 2d array of vertices to calculate if there is an edge between
 * any two possible vertices, makes usre it does not count itself,landmasses, or previosuly
 * calculated vertices.
 */
void SICGraph::creategraph(float rt) {
    graph = new Node[GRAPH_L];
    for(int i = 0;i<GRAPH_L;i++){
        graph[i].distime = -1;
        graph[i].fintime = -1;
        graph[i].next = NULL;

    }
    int key=0;
    int key2 = 0;
    float r = 0;
    float Sxy = 0;
    for(int i = 0;i<GRAPH_X;i++){
        for(int j = 0;j<GRAPH_Y;j++) {
            key = (i * GRAPH_X) + j;
            graph[key].xpos = i;
            graph[key].ypos = j;
            graph[key].landmass = data_2d[i][j].landmass;
            if (!data_2d[i][j].landmass) {

            for (int a = 0; a < GRAPH_X; a++) {
                for (int b = 0; b < GRAPH_Y; b++) {
                    key2 = (a*GRAPH_X) + b;
                    if ((((key) != (key2)) && (!data_2d[a][b].landmass)) && !data_2d[a][b].calculated) {
                        Sxy = 0;
                        for (int k = 0; k < GRAPH_Z; k++) {
                            if (data[i][j][k] != 157 && data[a][b][k] != 157)
                                Sxy = Sxy +
                                      ((data[i][j][k] - data_2d[i][j].x_bar) * (data[a][b][k] - data_2d[a][b].x_bar));
                        }
                         r = Sxy/(sqrtf(data_2d[i][j].Sxx*data_2d[a][b].Sxx));
                        if(fabsf(r) >= rt){
                            Node *newnode1 = new Node;
                            newnode1->xpos = a;
                            newnode1->ypos = b;
                            newnode1->distime=-1;
                            newnode1->fintime = -1;
                            newnode1->landmass = false;
                            newnode1->next = graph[key].next;
                            graph[key].next = newnode1;

                            Node *newnode2 = new Node;
                            newnode2->xpos = i;
                            newnode2->ypos = j;
                            newnode2->distime=-1;
                            newnode2->fintime = -1;
                            newnode2->landmass = false;
                            newnode2->next = graph[key2].next;
                            graph[key2].next = newnode2;


                        }
                    }
                }
            }
        }
           data_2d[i][j].calculated = true;
        }

    }


}
/*
 * Main function for the SICGrap class, calls run to run the program,
 * then finalizes making sure there is no memory leaks.
 */
int SICGraph::SIC_MAIN(float rt) {
    run(rt);
    delete data;
    delete data_2d;
    delete graph;
    printf("-----------------------------------------------------");
    printf("-----------------------------------------------------");
return 0;
}

/*
 * This function reads in the files into the 3d array called data
 * It runs a for loop to create the names of every file, then calls another function
 * that read is that specific file(2d array) into its position in the 3d array.
 */
void SICGraph::read() {
for(int i = 0;i<GRAPH_X;i++) {
    data[i] = new float *[GRAPH_Y];

    for (int j = 0; j < GRAPH_Y; j++)
        data[i][j] = new float[GRAPH_Z];

}
string fname;
    int k = 0;
    for(int i = 1990;i<=2005;i++){
        for(int j = 1;j<=52;j++){
            if(j <10){
                fname = "Data_for_subregion/CS310_project_subregion/" + to_string(i) + "/Beaufort_Sea_diffw0" +
                        to_string(j) + "y" + to_string(i) + "+landmask";
                read_2d(fname, k);
            }
            else{
                fname = "Data_for_subregion/CS310_project_subregion/" + to_string(i) + "/Beaufort_Sea_diffw" +
                        to_string(j) + "y" + to_string(i) + "+landmask";
                read_2d(fname,k);

            }
        }
        k = k+1;
    }

}
/*
 * This function opens a file, and reads it in to its respective position
 * in the 3d array (given its position k). Uses File opertion to read in the given binary files
 * into the array
 */
void SICGraph::read_2d(string filename, int k) {
const char *fname = filename.c_str();
    FILE *fbin;
    float p;
    fbin = fopen(fname,"rb");
    if(fbin == NULL){
        printf("Failed to open file %s\n",fname);
        return;
    }
    for(int i = 0; i<GRAPH_X;i++){
        for(int j = 0;j<GRAPH_Y;j++) {
            fread(&p, sizeof(p), 1, fbin);
            data[i][j][k] = p;
        }

    }
    fclose(fbin);
}
/*
 * This function calls the program, calls function as they
 * need to be called for every given threshold r correlation.
 */
void SICGraph::run(float rt) {
    read();
    create2d();
    printf("--------------------------------------------------------\n");
    printf("For rt = %f:\n",rt);
    printf("--------------------------------------------------------\n");
    creategraph(rt);
    distr();
    DFS();
    create_weight_D();
    Floyd_Warshall();
    CPL();
    CL_CO1();


}

/*
 * Main function of main.cpp, created a SICGraph and calls
 * its main function.
 */
int main() {
    SICGraph *SIC_Beauf_95 = new SICGraph();
    SIC_Beauf_95->SIC_MAIN(0.925);
    delete SIC_Beauf_95;
/*
    SICGraph *SIC_Beauf_925 = new SICGraph();
    SIC_Beauf_925->SIC_MAIN(0.925);
    delete SIC_Beauf_925;

    SICGraph *SIC_Beauf_90 = new SICGraph();
    SIC_Beauf_90->SIC_MAIN(0.90);
*/
    printf("-----------------------------------------------------");
    printf("PROGRAM_END");
    printf("-----------------------------------------------------");

    return 0;
}
