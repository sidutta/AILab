#include <iostream>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

#define str_len  5                          // palindrome length
#define hidden_layer_neurons 175 // str_len*68     // no. of neurons in hidden layers
#define grapheme_count 7   // log(68)
#define alpha 0.2                           // learning rate
#define alphabets 5        // log(26)
using namespace std;

double weight1[str_len*grapheme_count][hidden_layer_neurons];  // weight of arcs from input to hidden layer
double weight2[hidden_layer_neurons][str_len*alphabets];         // weight of arcs from output of hidden layer to output layer
double weight3[hidden_layer_neurons][str_len*alphabets];

//double bias1[hidden_layer_neurons];
//double bias2[str_len*grapheme_count];

//double input[str_len*alphabets];               // input that's to be checked
//double expected_output[str_len*grapheme_count];               // input that's to be checked

vector<double *> inputs;
vector<double *> expected_outputs;

map<string, int> graphemes;
map<int, string> reverse_graphemes;

double sigmoid(double x) {
    return 1.00/(1.00+exp(-x));      // logistic function
}

void initialize_weights() {
    for(int i=0; i<str_len*grapheme_count; i++) {
        for(int j=0; j<hidden_layer_neurons; j++) {
            weight1[i][j] = (rand()%100)/100.0 * ((rand()%2) * -1);
        }
    }
    for(int i=0; i<hidden_layer_neurons; i++) {
        for(int j=0; j<str_len*alphabets; j++) {
            weight2[i][j] = (rand()%100)/100.0 * ((rand()%2) * -1);
        }
    }
}

void output_from_hidden_layer(double *input, double *output) {
    for(int i=0; i<hidden_layer_neurons; i++) {
        double neti = 0; // -bias1[i];
        for(int j=0; j<str_len*grapheme_count; j++) {
            neti += input[j] * weight1[j][i];
        }
        output[i] = sigmoid(neti);
        //        cout<<output[i]<<" "<<neti<<" ";
    }
}

void final_output(double *hidden_layer_output, double *output) {
    for(int i=0; i<str_len*alphabets; i++) {
        double neti = 0; // -bias2[i];
        for(int j=0; j<hidden_layer_neurons; j++) {
            neti += hidden_layer_output[j] * weight2[j][i];
        }
        output[i] = sigmoid(neti);
        //        cout<<output[i]<<" ";
    }
}

void train(double *input, double *expected_output) {
    double hidden_layer_output[hidden_layer_neurons];
    output_from_hidden_layer(input, hidden_layer_output);
    
    double result[str_len*alphabets];
    final_output(hidden_layer_output, result);
    
    double final_del[str_len*alphabets];
    
    for(int i=0; i<str_len*alphabets; i++) {
        final_del[i] = (expected_output[i] - result[i]) * result[i] * (1-result[i]);
        //        cout<<result[i];
    }
    //    cout<<endl;
    
    for(int i=0; i<hidden_layer_neurons; i++) {
        for(int j=0; j<str_len*alphabets; j++) {
            weight3[i][j] += (alpha * final_del[j] * hidden_layer_output[i]);
        }
    }
    
    for(int i=0; i<hidden_layer_neurons; i++) {
        double del=0;
        for(int k=0; k<str_len*alphabets; k++)
            del += weight2[i][k] * final_del[k] * hidden_layer_output[i] * (1-hidden_layer_output[i]);
        for(int j=0; j<str_len*grapheme_count; j++) {
            weight1[j][i] += (alpha * del * input[j]);
            
        }
        //        bias1[i] += alpha * del;
    }
    
    for(int i=0; i<hidden_layer_neurons; i++) {
        for(int j=0; j<str_len*alphabets; j++) {
            weight2[i][j] = weight3[i][j];
        }
    }
}

long long total_checks = 0;
long long correct_ans = 0;

void auto_test(double *input, double *expected_output) {
    //    double *input = new double[str_len*alphabets];
    
    double hidden_layer_output[hidden_layer_neurons];
    output_from_hidden_layer(input, hidden_layer_output);
    
    double result[str_len*grapheme_count];
    final_output(hidden_layer_output, result);
    
    for(int i=0; i<str_len; i++) {
        for(int j=0; j<alphabets; j++) {
            total_checks++;
            if(result[i*str_len+j]>=0.5 && expected_output[i*str_len+j]>=0.5)
                correct_ans++;
            if(result[i*str_len+j]<0.5 && expected_output[i*str_len+j]<0.5)
                correct_ans++;
        }
    }
}

int main() {
    srand(time(NULL));
    string s;
    initialize_weights();
    
    ifstream grapheme_list("grapheme-list.txt");
    int i=0;
    while(getline(grapheme_list, s)) {
        graphemes.insert(make_pair(s, i));
        reverse_graphemes.insert(make_pair(i, s));
        i++;
    }
    grapheme_list.close();
    
    //ifstream in("cmudict-0.7b.txt");
    ifstream in("grapheme-phoneme-ann.txt");
    //ofstream out("grapheme-phoneme-ann.txt");
    
    while(getline(in, s)) {
        int j=0;
        string t = "";
        bool flag = true;
        bool bracket_found = false;
        bool valid_string = true;
        for(int i=0; i<s.length(); i++) {
            if(s[i]=='(') {
                bracket_found = true;
            }
            if(flag && !((s[i]>='A'&&s[i]<='Z')||s[i]==' ') && !bracket_found)
                valid_string = false;
            if(flag && !bracket_found) t+=s[i];
            if(s[i]==')') {
                bracket_found = false;
            }
            if(s[i]==' ') {
                j++;
                flag=false;
            }
        }
        
        if(j==t.length() && t.length()-1==str_len) {
            
            double *temp_input = new double[str_len*alphabets];
            double *temp_expected_output = new double[str_len*grapheme_count];
            
            for(int i=0; i<str_len; i++) {
                int mask[alphabets];
                int alphabet = t[i] - 'A';
                //                cout<<t[i]<<" ";
                for(int j=0; j<alphabets; j++) {
                    temp_input[i*alphabets+j] = alphabet%2;
                    alphabet /= 2;
                    //                    cout<<temp_input[i*alphabets+j]<<" ";
                }
                //                cout<<endl;
            }
            //out<<s<<endl;
            stringstream ss(s);
            string tok;
            ss>>tok;
            i=0;
            while(ss>>tok) {
                //                cout<<tok<<" ";
                if(graphemes.find(tok)==graphemes.end()) {
                    cout<<"Error: Token not found";
                }
                else {
                    int mask[grapheme_count];
                    int token = graphemes[tok];
                    for(int j=0; j<grapheme_count; j++) {
                        temp_expected_output[i*grapheme_count+j]=token%2;
                        token /= 2;
                        //                        cout<<temp_expected_output[i*grapheme_count+j];
                    }
                    i++;
                }
                //                cout<<endl;
            }
            inputs.push_back(temp_expected_output);
            expected_outputs.push_back(temp_input);
        }
    }
    //out.close();
    in.close();
    
    double average = 0;
    
    for(int k=0; k<5; k++) {
        for(int i=0; i<15; i++)
            for(int j=0; j<inputs.size(); j++) {
                if(j%5!=k)
                    train(inputs[j], expected_outputs[j]);
            }
        for(int j=0; j<inputs.size(); j++) {
            if(j%5==k)
                auto_test(inputs[j], expected_outputs[j]);
        }
        //        cout<<(double)correct_ans/total_checks<<endl;
        //        cout<<correct_ans<<" "<<total_checks<<endl;
        average += (double)correct_ans/total_checks;
        cout<<"Done processing fold "<<k+1<<endl;
        correct_ans = 0;
        total_checks = 0;
        initialize_weights();
    }
    
    cout<<"5-fold croos validation: "<<average*20<<"%"<<endl;
    
    for(int i=0; i<inputs.size(); i++)
        delete[] inputs[i];
    
    return 0;
}

