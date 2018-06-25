#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <set>
//https://www.geeksforgeeks.org/set-in-cpp-stl/




/*

We use a greedy coloring algorithm. 

 - The first fit coloring selects at random an uncolored vertex and assign the smallest
 color available. The algorithm finish when there is no uncolored vertices. This algorithm
 runs in O(n) steps


- Largest degree ordering Select the uncolored vertex with highest degree
   and color such vertex with the smallest possible color. This algorithm runs in 
   O(n2) steps


- In this version we implement the first fit algorithm. A possible assignment is to
implement the largest degree ordering

*/

using namespace std;

int N; //Size of the graph

ifstream edge;

struct vertex{
    int id;
    int *neighborhood; //Id of its neighbors
    bool is_color;
    int color;
    int degree;  
};	

//
class Graph
{
	// Parameters 
	int V;			// no. of vertices
	vertex *vertice;	// adjacency List
	int **A;  //Adj Matrix. We need to do it more dynamic
	map <int,int> * xCol;	// A List to note what colors can't be used to color a particular vertex
	map <int,int> col;	//vertex number to color used map 
	int colors;	// number of colors used
	int * deg;	//array to store degree
	multimap <int,int> deg_ver; // for sorting degree
	int* ver_order;
	
public:
	Graph(int V,char* fname)    //contructor
	{
		this->V = V;
		A= new int*[V];
		vertice = (vertex*) malloc (sizeof(vertex)*V); //This is the list of vertices
		xCol = new map<int,int> [V];  //This is part of the algorithm
		colors = 0;
		readGraph(fname);
	    set <int, greater <int> > colors;
		colors=set_of_colors(vertice[0]);
		colorGraph(V);
		printColoring(V);
		//printSetColors(colors);
		//printAdjMatrix(V);
		//printList(V);
		//printDegrees(V);
	}

    //vertex b is neighbor of vertex a
	void addEdge(int a,int iNeighbor,int b) //Add edge a,b to the Graph
	{
        
       // cout<<"vertice"<<a<<"pos:"<<iNeighbor<<"="<<b<<endl;
        vertice[a].neighborhood[iNeighbor]=b;

	}

	///Added by Joel

  void initAdjMatrix(int size){
  	for (int i = 0; i < size; ++i){
      A[i] = new int[size];
  	}
    for (int i=0;i<size;i++){
  	  for (int j=0;j<size;j++){
  		A[i][j]=0;
  	  }
    }
  }

  void readGraph(char *fileName){
  	initAdjMatrix(this->V);
    string line;
    int degree;
    int contRow;
    int contCol;
    istringstream iss;
    ifstream myfile (fileName);
    if (myfile.is_open())
     {
       contRow=0;
       while ( myfile.good() )
         { 
           getline (myfile,line);
           cout << line << endl;
           degree=countNeighbors(line);
           //cout<<"degree:"<<degree<<endl;
           createVertex(contRow,degree);
           istringstream iss(line); 
           contCol=0;           
    	   do {
               string val;
               iss >> val;
               if (!val.empty()){
                // cout << "Substring: " << val << endl;
                 A[contRow][stoi(val)-1]=1;
                 addEdge(contRow, contCol ,stoi(val)-1);
                 contCol++;
               }
           } while (iss);
           contRow++;
         }
       myfile.close();
      }
   else 
     {  
       cout << "Unable to open file"; 
     }
 }


 ///count number of neighbors in vertex

 int countNeighbors(string line){
   int degree;
   istringstream iss(line); 
   degree=0;           
   do {
        string val;
        iss >> val;
        if (!val.empty()){
                 degree++;
            }
       } while (iss);
    return degree;
 }



 ////Create vertex

void createVertex(int xPos, int xDegree){
   vertex vectorx;
   vertice[xPos].id=xPos;
   vertice[xPos].degree=xDegree;
   vertice[xPos].is_color=false;
   vertice[xPos].color=0;
   vertice[xPos].neighborhood =  (int*) malloc (sizeof(int)*xDegree); //This is the list of vertices

   //vertice[xPos].neighborhood=new list<int>[xDegree];
}

 ////create vertex

  void printAdjMatrix(int size){
  	for (int i=0;i<size;i++){
		for (int j=0;j<size;j++){
		  cout<<A[i][j];	
		}
		cout<<""<<endl;
	}
  }

   void printList(int size){
  	int xid,z;
    for (int i=0;i<size;i++){
         cout<<"Vertex"<<i<<": ";
         z=vertice[i].degree;
         for (int j=0;j<z;j++){
         	cout<<"Degree:"<<vertice[i].degree<<" ";
         	cout<<vertice[i].neighborhood[j]<<" ";
         }
         cout<<" "<<endl;
      }
    }  



  //Start coloring algorithm


  set <int, greater <int> > set_of_colors(vertex v){
     set <int, greater <int> > colors;
     int j,z;
     z=v.degree;
     for (j=0;j<z;j++){
     	//cout<<"vecino:"<<v.neighborhood[j]<<" ";
     	//cout<<"Color: "<<vertice[v.neighborhood[j]].color<<endl;
     	colors.insert(vertice[v.neighborhood[j]].color);
     }
     return colors;
  }

  void printSetColors(set <int, greater <int> > colors){
  	   set <int, greater <int> > :: iterator itr;
  	   for (itr = colors.begin(); itr != colors.end(); ++itr){
        cout << "Colors of vertex:" << *itr<<endl;
    }
  }

  void colorGraph(int size){
  	for (int i=0;i<size;i++){
  		setColor(i);
  	}
  }

   void printColoring(int size){
  	for (int i=0;i<size;i++){
  		cout<<"vertex-"<<i<<"::color->"<<vertice[i].color<<endl;
  	}
  }

  void setColor(int pos){
  	//upperColor=v.degree+1; //v can take at most color upperColor in the worst case
  	int vColor=1;
  	set <int, greater <int> > colors=set_of_colors(vertice[pos]);
  	//printSetColors(colors);
   	set<int, greater <int> >::iterator it;
   	it = colors.find(vColor);
   	while (it!=colors.end()){
       vColor++;
       it = colors.find(vColor);
   	}
   	vertice[pos].color=vColor;
  }
  
  //End coloring algorithm

  ///End added by Joel	


};



int main(int argc,char* argv[])
{
	if(argc!=3) { 
		cerr<<"Usage : ./color fileName N "<<endl;return 0;
	}
	N = atoi(argv[2]);
	Graph G(N,argv[1]);
	//G.printAdjMatrix(N);
	return 0;	
	/*
	if(argc!=2) { 
		cerr<<"Indique el nombre del archivo"<<endl;
		return 0;
     }
    readFile(argv[1]);
    printAdjMatrix(5);
	return 0;*/
}