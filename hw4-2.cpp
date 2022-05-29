// hw4-2: Max-flow min-cut implementation
// environment: Notepad++ 8.2.1(64bits X64)
// C++ type: C++ 14
// g++ type: g++ version 6.3.0
#include <iostream>
#include <string>

#define INT_MAX 2147483647 // for finding bottle-neck

using namespace std;

// Solve Method
// 1. Naïve algorithm: Use Residual Graph, but NOT sure about finding MAX FLOW 
// 2. Ford-Fulkerson: Compared with Naïve algorithm, add the REVERSE edge, so it make sure we can undo the operations, which leads to MAX FLOW for sure
// (Cons: Time complexity O(f*m) might be too large (f: MAX FLOW/ m: #EDGES))

// 3. Edmonds-Karp: Special case of Ford-Fulkerson, which finds the shortest path each time
// (Time complexity O(m^2*n)), much smaller than Ford-Fulkerson (m: #EDGES/ n: #VERTICES))

/*
	<< Adjacency graph Implemntation >>
Since already known the vertices count && the vertices' id is from 0 to vertices count-1,
we use Adjacency graph to implement, also practice both way to implement graph

		0   1   2	3   ....
	0  0 .. ......(capacity: 0 means no connection b/t two vertices)
	1  .
	2  .
	3  .
	.  .
	.  .
	.  .
*/

// build graph with given edge && vertices count
int** buildGraph(int v_count){
	// initialization
	int** graph=new int*[v_count];
	for(int i=0;i<v_count;i++){
		graph[i]=new int[v_count];
		for(int j=0;j<v_count;j++){
			graph[i][j]=0;
		}
	}
	int aa=0,bb=0,capacity=0;
	// assign value to graph
	while(cin>>aa){
		cin>>bb;
		cin>>capacity;
		graph[aa][bb]=capacity;
	}
	return graph;
}

// build residual graph
int** buildResidualGraph(int v_count, int** graph){
	// initialization
	int** res_graph=new int*[v_count];
	for(int i=0;i<v_count;i++){
		res_graph[i]=new int[v_count];
		for(int j=0;j<v_count;j++){
			res_graph[i][j]=graph[i][j];
		}
	}
	return res_graph;
}

// keep searching to check if a path exist, if not, return FALSE
bool tryReachSink(int** res_graph, string& result, int currRow, int currCol, int v_count, bool* visited){
	visited[currRow]=true; // mark as visited
	result+=to_string(currRow);
	if(currRow==v_count-1){
		// reset all vertices as unvisited
		for(int i=0;i<v_count;i++){
			visited[i]=false;
		}
		return true; // find a path
	}
	for(int col=0;col<v_count;col++){
		if(currRow==col); // prevent self-loop exist, and keep calling each other
		else if(res_graph[currRow][col]>0&&visited[col]==false){ // if not zero, and the direction is going to sink, and "to" vertex is unvisited, then keep digging
			bool successful=tryReachSink(res_graph, result, col, 0, v_count, visited);
			if(successful)return true;
			else{
				visited[col]=false;
				result.pop_back();
			}
		}
		else; // keep looping
	}
	return false; // if the currRow runs all col, but no edge exists, then no path
}

// find a path from source 0 to sink v_count-1 using DFS
// return type: string-record each passed-by vertices
string findAugmentedPath(int v_count, int** res_graph, bool* visited){
	// find the edge spread from source
	for(int i=0;i<v_count;i++){
		if(i==0); // prevent self-loop of source vertex
		else if(res_graph[0][i]>0){ // find the start point, try to reach sink
			string result="0"; // must start from source 0
			visited[0]=true;
			bool pathExist=tryReachSink(res_graph, result, i, 0, v_count, visited);
			if(pathExist)return result;
			else visited[0]=false; // reset
		}
		else if(i==v_count-1){ // reach end and no result, which means no edge start from source
			string failed="failed: no augmented path";
			return failed;
		}
		else;
	}
}

// find bottleneck of a given path
int findBottleNeck(int** res_graph, string aug_path){
	int bottle_neck=INT_MAX;
	for(int i=0;i<aug_path.size()-1;i++){
		int edge_weight=res_graph[aug_path[i]-'0'][aug_path[i+1]-'0'];
		if(edge_weight<bottle_neck){
			bottle_neck=edge_weight;
		}
		else;
	}
	return bottle_neck;
}

// update residual graph
void updateResidual(int** res_graph, string aug_path, int bottle_neck){
	for(int i=0;i<aug_path.size()-1;i++){
		int row=aug_path[i]-'0';
		int col=aug_path[i+1]-'0';
		res_graph[row][col]-=bottle_neck; // update path capacity
		res_graph[col][row]+=bottle_neck; // update backward path 
	}
	return;
}

// No need for backward path
void rmBackwardPath(int** graph, int** res_graph, int v_count){
	for(int i=0;i<v_count;i++){
		for(int j=0;j<v_count;j++){
			if(graph[i][j]>0)res_graph[j][i]=0;
		}
	}
	return;
}

// print out result
void printResult(int** graph, int** res_graph, int v_count){
	// print flow && capacity
	int max_flow=0;
	for(int i=0;i<v_count;i++){
		for(int j=0;j<v_count;j++){
			if(graph[i][j]!=0){
				int flow=graph[i][j]-res_graph[i][j];
				cout<<"<"<<i<<","<<j<<"> ";
				cout<<flow;
				// print out beautiful space
				if(flow<10)cout<<"    ";
				else if(flow<100)cout<<"   ";
				else if(flow<1000)cout<<"  ";
				else cout<<" ";
				cout<<graph[i][j]<<endl;
				if(i==0)max_flow+=flow;
			}
		}
	}
	// print total max-flow
	cout<<"total: "<<max_flow<<endl;
	return;
}

// max-flow min-cut function combination
void maxFlow_minCut(){
	int v_count=0;
	cin>>v_count;
	int** graph=buildGraph(v_count);
	int** res_graph=buildResidualGraph(v_count, graph);
	bool* visited=new bool[v_count]{}; // record if the vertex is visited
	
	// start process
	string aug_path=findAugmentedPath(v_count, res_graph, visited);
	while(aug_path!="failed: no augmented path"){
		int bottle_neck=findBottleNeck(res_graph, aug_path);
		updateResidual(res_graph, aug_path, bottle_neck);		
		aug_path=findAugmentedPath(v_count, res_graph, visited);
	}	
	rmBackwardPath(graph, res_graph, v_count);
	printResult(graph, res_graph, v_count);
	return;
}

int main(){
	
	maxFlow_minCut();
	
	system("pause");
	return 0;
}
