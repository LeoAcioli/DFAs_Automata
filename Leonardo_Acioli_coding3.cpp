/*
 *LEONARDO ACIOLI ARRUDA - APRIL 13TH, 2022 - CODING 3 ASSIGNMENT - AUTOMATA THEORY
*/

#include "DFA_3.hpp"
#include "algorithm"
#include "unordered_map"
#include <array>
#include <bits/stdc++.h>
#include <string>
#include <bitset>
#include <iostream>

using namespace std;

/*
Description of string encoding:

first_part = 4 bit binary representation of number of states

second_part = 4 bit binary representation of initial state

third part = Number of states bits long for final states (0 stands for false, 1 stands for true). First digit of third_part is state 0, last digit is state #ofstates

fourth_part = (4 bit binary representation of state on a 0 + 4 bit representation of state on a 1) * number of states

String encoding: 4 bits # states | 4 bits initialState | number of states bits for final states | (4 bits (state on a 0) + 4 bits (state on a 1) * number of states)

encoded_string = first_part + second_part + third_part + fourth_part
*/



string dfa_to_string(const DFA &m)
{
	string complete = "";
	string str_number_states;
	str_number_states = bitset<4>(m.numStates).to_string();
	complete.append(str_number_states); // first 4 bits is # states

	string str_initial_state;
	str_initial_state = bitset<4>(m.initialState).to_string(); // second 4 bits is initial states
	complete.append(str_initial_state);

	
	string str_final_states_bool;
	for (int i = 0; i<m.finalStates.size(); i++){
		string boolean = to_string(m.finalStates[i]);
		str_final_states_bool.append(boolean);
        }
	complete.append(str_final_states_bool); // #states is next #statesbits


	string str_states_and_transitions;
	for (int i = 0; i<m.finalStates.size(); i++){
                int on_a_0 = m.transFunc(0,i);
		int on_a_1 = m.transFunc(1,i);
		string state_on_a_0_binary4bits = bitset<4>(on_a_0).to_string();
		string state_on_a_1_binary4bits = bitset<4>(on_a_1).to_string();
		
		str_states_and_transitions.append(state_on_a_0_binary4bits);
		str_states_and_transitions.append(state_on_a_1_binary4bits);
		//format is state on a 0 + state on a 1
		//total is 8 bits
        }
	complete.append(str_states_and_transitions);
	return(complete);
}



DFA dfa_from_string(const string &w)
{
	string str_bin_num_states = w.substr(0,4); //gets first 4 characters
	int num_states = stoi(str_bin_num_states, nullptr, 2);
	string str_num_states = to_string(num_states);//now we have number of states in decimal string form!
	
	string str_bin_init_state = w.substr(4,4); //get second set of 4 bits
	int init_state = stoi(str_bin_init_state, nullptr, 2);	
	string str_init_state = to_string(init_state);//now we have init state in decimal string form!

	
	int num_final_states = 0;
	string list_final_states = "";	
	for (int i = 0; i<num_states; i++){ //finalStates.size() is equal to m.numStates
		string boolean = w.substr(8+i,1); //final booleans start at position 8 on string (considering that first character is at index 0)
		if (boolean == "1"){
			num_final_states = num_final_states+1;
			string str_i = to_string(i);
			list_final_states.append(str_i);
		}
	}	
	string str_num_final_states = to_string(num_final_states);

	string transition_states = "";	
	int str_ptr = 4 + 4 + num_states; //transitions start after 4 bits for #states, 4 bits for initial state, and m.numStates bits for booleans 
	for (int i = 0; i < num_states; i++){
		string state_on_0 = w.substr(str_ptr,4);
		string state_on_1 = w.substr(str_ptr+4,4);

		int int_state_on_0 = stoi(state_on_0, nullptr, 2);
		int int_state_on_1 = stoi(state_on_1, nullptr, 2);

		string str_state_on_0 = to_string(int_state_on_0);
		string str_state_on_1 = to_string(int_state_on_1);
			
		transition_states.append(str_state_on_0);
		transition_states.append(str_state_on_1);

		str_ptr = str_ptr + 8; //jump to next block of transitions
	}	

	string complete = "";
	complete.append(str_num_states); //first append number of states in decimal
	complete.append(transition_states); //then append transitions for each state
	complete.append(str_init_state);//then append initial state
	complete.append(str_num_final_states); //then append number of final states
	complete.append(list_final_states); //then append list of final states
	
	string result( 2 * complete.size() - 1, ' ' ); //adding spaces between every character of the string so it's accepted
   	for ( int i = 0, j = 0; i < complete.size(); i++, j+=2 ){
		result[j] = complete[i];
	}

	DFA test;	
	std::stringstream (result) >> test;		
	return(test);		 	
}

bool dfa_accepts_self (const DFA &m, const string &w)
{
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
        	std::cout<<"Final state is "<<currentState<<", which is a valid final state. String accepted by DFA."<<std::endl;
       		return true;
        }
        else{
        	std::cout<<"Final state is "<<currentState<<", which is not a valid final state. String not accepted by DFA."<<std::endl;
        	return false;
        }	
}

int main () {

DFA begEndSame; // Begin and end with same symbol
std::stringstream ("5 1 3 1 2 1 2 4 3 4 3 0 2 1 3") >> begEndSame;

cout<<"Original DFA..."<<endl;
cout<<begEndSame<<endl;

string encoded_string = dfa_to_string(begEndSame);//call enconding function

cout <<"String enconding of DFA: "<< encoded_string<<endl;

DFA test;
test = dfa_from_string(encoded_string);//call decoding function
cout<<"DFA derived from encoded string: "<<endl;
cout<<test<<endl;

cout<<"Does the DFA accept its own string encoding?"<<endl;
bool boolean = dfa_accepts_self(test,encoded_string);
cout<<"Furthemore, answer for if DFA accepts its own string is: "<<boolalpha<<boolean<<endl;

return 0;

}
