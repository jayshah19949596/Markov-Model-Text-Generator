// MarkovModel.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <set>
#include <map>
#include <time.h>

using namespace std;


class Node {
public:
    string state_name;
    vector<Node> out_states;

    void set_state_name(string name) {
        state_name = name;
    }

    string get_state_name() {
        return state_name;
    }

};

map<string, float> calculate_frequency(vector<string> tokens){
    map<string, float> frequency;
    map<string, float>::iterator map_itr;

    for (vector<string>::iterator itr = tokens.begin(); itr != tokens.end(); ++itr) {
        frequency[*itr] = frequency[*itr]+1;
    }

    for (map_itr = frequency.begin(); map_itr != frequency.end(); ++map_itr) {
        cout << map_itr->first << " " << map_itr->second << endl;
    }

    return frequency;
}




vector<Node> prepare_graph(vector<string> tokens) {
    set<string> vocab_states;      // contains the unique states
    vector<Node> nodes;            // contains the unique states
    string state_name = "";        // state_name
    unsigned idx, n_idx, current_state_idx, next_state_idx;
    set<string>::iterator set_itr;
    vector<Node>::iterator node_itr;
    Node *node;
    Node current_node, current_state, next_state;

    // =======================
    //    Storing tokens
    // =======================
    for (vector<string>::iterator itr = tokens.begin(); itr != tokens.end(); ++itr){
        vocab_states.insert(*itr);
    }

    // =================================
    //   Making set if stored tokens
    // =================================
    for (set_itr = vocab_states.begin(); set_itr != vocab_states.end(); set_itr++){
        node = new Node;
        node->set_state_name(*set_itr);
        nodes.push_back(*node);
    }


    // =================================
    //       Creating our graph
    // =================================
    for (idx = 0; idx < tokens.size()-1 ; idx++){
        for (n_idx = 0; n_idx < nodes.size() ; n_idx++){
            current_node = nodes.at(n_idx);
            if (current_node.get_state_name() == tokens.at(idx)){
                current_state_idx = n_idx;
            }
            if (current_node.get_state_name() == tokens.at(idx+1)){
                next_state_idx = n_idx;
            }
        }
        //cout << nodes.at(current_state_idx).get_state_name() <<  nodes.at(next_state_idx).get_state_name()<< endl;
        nodes.at(current_state_idx).out_states.push_back(nodes.at(next_state_idx));

    }

    // =======================================
    //    Printing our values for verifying
    // =======================================
    for (n_idx = 0; n_idx < nodes.size() ; n_idx++){
        current_node = nodes.at(n_idx);

        for (idx = 0; idx < current_node.out_states.size() ; idx++){
            state_name = current_node.out_states.at(idx).get_state_name();
            cout << current_node.get_state_name() << "->";
            cout << state_name << endl ;
        }

        if (current_node.out_states.size() == 0){
            cout << current_node.get_state_name() << "->";
            cout << "none" << endl ;
        }
        //cout << endl;
    }

    return nodes;
}

vector<Node> prepare_data(string data) {
    vector<Node> nodes;
    vector<string> tokens;             // token is for storing all the words
    map<string, float> probability;

    // ======================================================================
    // split the data by " " as delimiter and store it in every word in token
    // ======================================================================
    istringstream iss(data);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));

    nodes = prepare_graph(tokens);
    cout << " ======================= " << endl;
    // probability = calculate_frequency(tokens);
    return nodes;

}

void generate_text(vector<Node> markov_graph, int n_sampling, string start, bool stop_at_end){
    int i ;
    int j;
    bool outer_break = false;
    string generated_text = "";
    generated_text = generated_text+start;
    srand ( time(NULL) );

    for (i = 0; i < n_sampling ; i++){
        for (j = 0; j < markov_graph.size() ; j++) {
            if (markov_graph.at(j).get_state_name() == start) {
                if (markov_graph.at(j).out_states.size() == 0){
                    generated_text = generated_text+" "+"end";
                    start = "end";
                    outer_break = true;
                    break;

                }
                else if (markov_graph.at(j).out_states.size() == 1){
                    generated_text = generated_text+" "+markov_graph.at(j).out_states.at(0).get_state_name();
                    start = markov_graph.at(j).out_states.at(0).get_state_name();
                    if (start == "end"){
                        outer_break = true;
                    }
                    break;

                }
                else{
                    int random_number = rand() % markov_graph.at(j).out_states.size();
                    //cout << "random_number "<< random_number << endl;
                    generated_text = generated_text+" "+markov_graph.at(j).out_states.at(random_number).get_state_name();
                    start = markov_graph.at(j).out_states.at(random_number).get_state_name();
                    if (start == "end"){
                        outer_break = true;
                    }
                    break;
                }
            }
        }
        if (outer_break and stop_at_end){
            break;
        }
    }
    cout << generated_text << endl;
}

int main()
{
    vector<Node> markov_graph;
    int n_words = 10;
    string start_token = "start";
    string train_data = "start My name is Jay end start what is your name end start wonderful name Jay";
    train_data = "start the quick brown fox jumps over the brown fox who is slow jumps over the brown fox who is dead end";
    markov_graph = prepare_data(train_data);
    generate_text(markov_graph, n_words, start_token, true);
    generate_text(markov_graph, n_words, start_token, false);

}

