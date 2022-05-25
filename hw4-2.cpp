// hw4-2: Max-flow min-cut implementation
// environment: Notepad++ 8.2.1(64bits X64)
// C++ type: C++ 14
// g++ type: g++ version 6.3.0
#include <iostream>

using namespace std;

/*
	<< Adjacency Matrix Implemntation >>
Since already known the vertices count && the vertices' id is from 0 to vertices count-1,
we use Adjacency Matrix to implement, also practice both way to implement graph

		0   1   2	3   ....
	0 -1/-1 ..........(capacity/flow)
	1  .
	2  .
	3  .
	.  .
	.  .
	.  .
*/

// node that record each edge capacity && flow
struct Node{
	// default value is -1, which means no edge exist
	int capacity=-1;
	int flow=-1;
};

// print function for matrix check 
void printMatrix(Node** matrix, int v_count){
	cout<<"=============================\n    ";
	for(int i=0;i<v_count;i++){
		cout<<i<<"    ";
	}
	cout<<endl;
	for(int i=0;i<v_count;i++){
		cout<<i<<" ";
		for(int j=0;j<v_count;j++){
			if(matrix[i][j].flow==-1)cout<<"-/";
			else cout<<matrix[i][j].flow<<"/";
			if(matrix[i][j].capacity==-1)cout<<"-  ";
			else cout<<matrix[i][j].capacity<<"  ";
		}
		cout<<endl;
	}
	cout<<"=============================\n";
	return;
}

// build matrix with given edge && vertices count
Node** buildMatrix(int v_count){
	// initialization
	Node** matrix=new Node*[v_count];
	for(int i=0;i<v_count;i++){
		matrix[i]=new Node[v_count];
	}
	int aa=0,bb=0,capacity=0;
	// assign value to matrix
	while(cin>>aa){
		cin>>bb;
		cin>>capacity;
		matrix[aa][bb].capacity=capacity;
	}
	return matrix;
}

// max-flow min-cut function combination
void maxFlow_minCut(){
	int v_count=0;
	cin>>v_count;
	Node** matrix=buildMatrix(v_count);
	
	printMatrix(matrix, v_count);
	
	return;
}

int main(){
	
	maxFlow_minCut();
	
	system("pause");
	return 0;
}