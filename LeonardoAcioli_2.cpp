#include "DFA_2.hpp"
#include <algorithm>
#include <unordered_map>
#include <array>
#include <bits/stdc++.h>
using namespace std;

//Graph class represents a directed graph
// using adjacency list representation -> From GeeksForGeeks
// https://www.geeksforgeeks.org/depth-first-search-or-dfs-for-a-graph/
// I've made some changes and additions to it.
class Graph {
public:
	map<int, bool> visited;
	map<int, list<int> > adj;

	// function to add an edge to graph
	void addEdge(int v, int w);

	// DFS traversal of the vertices
	// reachable from v
	bool DFS(int v,int f);//f for final state

	void DFS_2(int v);
};

void Graph::addEdge(int v, int w)
{
	adj[v].push_back(w); // Add w to v’s list.
}

bool Graph::DFS(int v,int f)
{
	// Mark the current node as visited and
	// print it
	visited[v] = true; //important line!
	//cout << v << " ";
	if (v==f){
		//cout<<"final state is reachable from start state!";
		return true;
	}


	// Recur for all the vertices adjacent
	// to this vertex
	list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i)
		if (!visited[*i])
			DFS(*i,f);//keep passing final state
}

void Graph::DFS_2(int v)
{
	visited[v] = true; //important line!
       // cout << v << " ";
	
	list<int>::iterator i;
        for (i = adj[v].begin(); i != adj[v].end(); ++i)
                if (!visited[*i])
                        DFS_2(*i);
}

// Does m accept string w?
bool dfa_is_accepted (const DFA &m, const std::string &w) {
	int len = w.length();
        int array [len];//array of size of characters in the str
        for(int i = 0; i < w.length(); ++i) {
            array[i] = w.at(i) - '0'; //w is a char, so '1' is 49. If I want to get 1 I have to do '1' - '0'
        }
        int currentState = m.initialState;
        int nextState = 0;
        for (std::string::size_type i = 0; i<w.length(); i++){//at the end of loop, currentState will hold the last state.
                nextState = m.transFunc(array[i],currentState);
                currentState = nextState;
        }

        if (m.finalStates[currentState] == true){
               // std::cout<<"Final state is "<<currentState<<", which is a valid final state. String accepted by DFA."<<std::endl;
                return true;
        }
        else{
               // std::cout<<"Final state is "<<currentState<<", which is not a valid final state. String not accepted by DFA."<<std::endl;
                return false;
        }
}

// Does m accept any strings? That is, is L(m) the empty set?
bool dfa_is_empty(const DFA &m)
{

	int finalState = 0;
	for (int i = 0; i<m.finalStates.size(); i++){
                if (m.finalStates[i] == true)
			finalState = i;
	}
	int tempState_zero=0;
	int tempState_one=0;
	Graph g;
	for (int i = 0; i < m.numStates ; i++){
                tempState_zero = m.transFunc(0,i);
                tempState_one = m.transFunc(1,i);
		g.addEdge(i,tempState_zero);
		g.addEdge(i,tempState_one); //CREATED GRAPH WITH ALL EDGES NOW!
        }               
 
	g.DFS(m.initialState,finalState); //Do traversal using initial state as starting node!
	
	if (g.visited[finalState]){} //if final node was visited from initial state, do nothing
	else
		return true; //this means it wasn't visited. So Language of DFA is the empty set
	if (std::find(std::begin(m.finalStates), std::end(m.finalStates), true) != std::end(m.finalStates))                                            
                return false; //this means that there are final states reachable from starting state, so Language isn't empty set.                                                                                                    
        else                                                                                                               
                return true; 
		
}
// Does m accept every string? That is, is L(m) the set of all strings?
bool dfa_is_all (const DFA &m) {

        int tempState_zero=0;
        int tempState_one=0;
        Graph g;
        for (int i = 0; i < m.numStates ; i++){
                tempState_zero = m.transFunc(0,i);
                tempState_one = m.transFunc(1,i);
                g.addEdge(i,tempState_zero);
                g.addEdge(i,tempState_one); //CREATED GRAPH WITH ALL EDGES NOW!
        }

	vector<int> visited_nodes;
	
	g.DFS_2(m.initialState);
	for (int i = 0; i<g.visited.size(); i++){
		if (g.visited[i] == true)
			visited_nodes.push_back(i);		
	}

	for (int i = 0; i<visited_nodes.size(); i++){
		if (m.finalStates[visited_nodes[i]] == false)
			return false;
	}
	return true;
	
}

// Is L(m) infinite?
bool dfa_is_infinite (const DFA &m) {

	      int tempState_zero=0;
        int tempState_one=0;
        int tempStateFinal_zero = 0;
        int tempStateFinal_one = 0;
        bool isThereFinalState = false;
        Graph g;
        for (int i = 0; i < m.numStates ; i++){
                if (m.finalStates[i] == true){
                  isThereFinalState = true; //if there are final states, set variable to true
                  tempStateFinal_zero = m.transFunc(0,i);
                  tempStateFinal_one = m.transFunc(1,i);
                  if (m.finalStates[tempStateFinal_zero] == true || m.finalStates[tempStateFinal_one] == true){ 
                     g.addEdge(i,tempStateFinal_zero); //only add edges after final state if next states are final states are final.
                     g.addEdge(i,tempStateFinal_one); 
                  }
                }
                else{
                tempState_zero = m.transFunc(0,i);
                tempState_one = m.transFunc(1,i);
                g.addEdge(i,tempState_zero);
                g.addEdge(i,tempState_one); //CREATED GRAPH WITH ALL EDGES NOW!
                }
        }

        if (isThereFinalState == false)
	      	return false; //if no final state DFA can't be infinite 
        
        g.DFS_2(m.initialState); //Do traversal using initial state as starting node!	
	      vector<int> visited_nodes;
        for (int i = 0; i<g.visited.size(); i++){
                if (g.visited[i] == true)
                        visited_nodes.push_back(i);
        }
	
	  bool isThereLoop = false; //from here on check if there's loop on the way to final state
		for (int i =0; i<visited_nodes.size();i++){
			tempState_zero = m.transFunc(0,visited_nodes[i]);
			tempState_one = m.transFunc(1,visited_nodes[i]);
			if (tempState_zero == i || tempState_one == i)
				isThereLoop = true;
		}
  
        if (isThereLoop == false){
          return false;
          }
        else if(isThereLoop == true){
          return true;
          }

}
// Is L(m1) equal to L(m2)?
bool dfa_is_equal (const DFA &m1, const DFA &m2) {//If A ⊆ B,And B ⊆ A,Then A = B	
	bool m1_subset_m2 = dfa_is_subset(m1,m2);
	bool m2_subset_m1 = dfa_is_subset(m2,m1);
	if (m1_subset_m2 == true && m2_subset_m1 == true)
		return true;
	else
		return false;
}
// Is L(m1) a subset of L(m2)?
bool dfa_is_subset (const DFA &m1, const DFA &m2) {//is subset if intersection of m2' and m1 is empty.
	DFA m2_complement;
	DFA m2_complement_and_m1;

	m2_complement = dfa_complement(m2);
	m2_complement_and_m1 = dfa_intersection(m2_complement,m1);

	bool answer;
	answer = dfa_is_empty(m2_complement_and_m1);		
	return answer;
}

