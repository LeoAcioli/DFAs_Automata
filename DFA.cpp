#include <algorithm>
#include <fstream>
#include "DFA.hpp"

/*
To run this code, compile using gcc and the flag provided by professor Willson: -std=c++11. As you create
the object, run the executable and as a command line argument [argv], provide the input string.
Whenever you click enter, enter the DFA in the following format:
Example:
If your DFA only accepts binary strings where the number of 1's are a multiple of 4, input DFA as: 4 0 1 1 2 2 3 3 0 0 1 0
This is how I tested my code: by providing DFAs and different input strings to see if that string is accepted or rejected by the DFA. 

-Leonardo Acioli Arruda-, Fri, Jan 28th, 2022.
 */

bool dfa_is_accepted (const DFA &m, const std::string &w)
{
	int len = w.length();	
	int array [len];//array of size of characters in the str
	for(int i = 0; i < w.length(); ++i) {
	    array[i] = w.at(i) - '0'; //w is a char, so '1' is 49. If I want to get 1 I have to do '1' - '0'
	}
	int currentState = m.initialState;
	int nextState = 0;
	for (std::string::size_type i = 0; i<w.length(); i++){//at the end of loop, currentState will hold the last state.
		nextState = m.transFunc[array[i]][currentState];
		currentState = nextState;
	}
	
	if (m.finalStates[currentState] == true){
		std::cout<<"Final state is "<<currentState<<", which is a valid final state. String accepted by DFA."<<std::endl;
		return true;
	}
	else{
		std::cout<<"Final state is "<<currentState<<", which is not a valid final state. String not accepted by DFA."<<std::endl;
		return false;
	} 	
}

std::ostream & operator << (std::ostream &out, const DFA &m) {
	out << m.numStates << std::endl;
	for (int q = 0; q < m.numStates; ++q) {
		out << m.transFunc [0][q] << ' ' << m.transFunc [1][q] << std::endl;
	}
	out << m.initialState << std::endl;
	out << std::count (m.finalStates.begin (), m.finalStates.end (), true) << ' ';
	for (int i = 0; i < m.numStates; ++i) {
		if (m.finalStates [i])
			out << i << ' ';
	}
	out << std::endl;
	return out;
}

std::istream & operator >> (std::istream &in, DFA &m) {
	in >> m.numStates;
	boost::multi_array<int, 2>::extent_gen extents;
	m.transFunc.resize (extents [2][m.numStates]);
	for (int q = 0; q < m.numStates; ++q) {
		in >> m.transFunc [0][q];
		in >> m.transFunc [1][q];
	}
	in >> m.initialState;
	m.finalStates.resize (m.numStates, false);
	int k;
	in >> k;
	for (int i = 0; i < k; ++i) {
		int q;
		in >> q;
		m.finalStates [q] = true;
		std::cout<<i;
	}
	return in;
}

int main(int argc, char** argv)
{
std::cout<<"Please enter the DFA in the following format: #states #state0 #state_after_state0 #InitialState #numberFinalStates #finalState"<<std::endl;
DFA m;
std::string w = argv[1]; //input string is command line argument
std::cin>>m; //input DFA from input
std::cout<<m;
bool answer = dfa_is_accepted(m,w);
return 0;
}

