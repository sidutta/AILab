#include <iostream>
#include <cmath>
#include <time.h>
#include <stdlib.h>

#define str_len  3   // palindrome length
#define alpha 0.2    // learning rate
#define layers 4

using namespace std;

double weights[str_len][str_len][layers-1];
double bias[str_len][layers-1];

int input[str_len];               // input that's to be checked

double sigmoid(double x) {
    return 1.00/(1+exp(-x));         // logistic function
}

void initialize_weights() {
    for(int i=0; i<str_len; i++)
        for(int j=0; j<str_len; j++)
            for(int k=0; k<layers-1; k++)
                weights[i][j][k] = ((rand()%100)/100.0)*(rand()%2)*(-1);
}

void output(double *input, double *output, int layer) {
    int neurons = str_len;
    if(layer==4) neurons = 1;
    for(int i=0; i<neurons; i++) {
        double neti = bias[i][layer-2];
        for(int j=0; j<str_len; j++) {
            neti += input[j] * weights[j][i][layer-2];
        }
        output[i] = sigmoid(neti);
    }
}

bool is_palindrome(double *vec, int len) {
    for(int i=0; i<len/2; i++) {
        if(vec[i]!=vec[len-1-i])
            return false;
    }
    return true;
}

void generate_random_vector(double *vec, int len) {
    for(int i=0; i<len; i++) {
        vec[i] = (int)((rand()%10)/10.0 + 0.5);
    }
}

void train() {
    int palin = 0;
    int not_palin = 0;
    
    int testdata[][4] = {{1,1,1,1},{1,0,1,1},{0,1,0,1},{1,0,0,0},{1,1,0,0},{0,1,1,0}};

    for(int l=0; l<10000000; l++) {
        
        double input[str_len];
        
       /* Uncomment to train using testdata defined above */
       for(int i=0; i<str_len; i++)
           input[i] = testdata[l%6][i];
       int expected_output = testdata[l%6][3];

        /* Uncomment to train using random data */
        // generate_random_vector(input, str_len);
        // int expected_output = is_palindrome(input, str_len);
        
        double hidden_layer_output1[str_len];
        output(input, hidden_layer_output1, 2);
        
        double hidden_layer_output2[str_len];
        output(hidden_layer_output1, hidden_layer_output2, 3);
        
        double result[1];
        output(hidden_layer_output2, result, 4);
        
        double final_del;
        final_del = (expected_output - result[0]) * result[0] * (1-result[0]);
        
        double delta2[str_len];
        double delta1[str_len];
        
        for(int i=0; i<str_len; i++) {
            delta2[i] = weights[i][0][2] * final_del * hidden_layer_output2[i] * (1-hidden_layer_output2[i]);
        }
        
        for(int i=0; i<str_len; i++) {
            delta1[i] = 0;
            for(int j=0; j<str_len; j++) {
                delta1[i] += weights[i][j][1] * delta2[j] * hidden_layer_output1[i] * (1-hidden_layer_output1[i]);
            }
        }
        
        for(int i=0; i<str_len; i++) {
            weights[i][0][2] += (alpha * final_del * hidden_layer_output2[i]);
        }
        bias[0][2] += alpha * final_del;
        
        for(int i=0; i<str_len; i++) {
            for(int j=0; j<str_len; j++) {
                weights[i][j][1] += (alpha * delta2[j] * hidden_layer_output1[i]);
            }
            bias[i][1] += (alpha * delta2[i]);
        }
        
        for(int i=0; i<str_len; i++) {
            for(int j=0; j<str_len; j++) {
                weights[i][j][0] += (alpha * delta1[j] * input[i]);
            }
            bias[i][0] += (alpha * delta1[i]);
        }
    }
    
}

bool test(int manual) {
    
    // test vector
    double input[str_len];
    
    // generating random test vector
    generate_random_vector(input, str_len);
    
    // for manual testing, enter or take user input
    if(manual) {
        for(int i=0; i<str_len; i++)
            cin>>input[i];
    }
    
    int expected_output = is_palindrome(input, str_len);
    
    double hidden_layer_output1[str_len];
    output(input, hidden_layer_output1, 2);
    
    double hidden_layer_output2[str_len];
    output(hidden_layer_output1, hidden_layer_output2, 3);
    
    double result[1];
    output(hidden_layer_output2, result, 4);
    
    /*  Uncomment the following to see test data */
    cout<<"Input Vector: ";
    for(int i=0; i<str_len; i++) {
        cout<<input[i]<<" ";
    }
    cout<<endl;
    double threshold = 0.6;
    cout<<"Result: "<<result[0]<<" ~ "<<(result[0]>threshold)<<endl;
    cout<<"Expected Output: "<<expected_output<<endl;
    
    return ((result[0]>threshold)==expected_output);
}

int main() {
    srand(time(NULL));
    initialize_weights();
    train();
    int sum = 0;
    int tests = 1000;
    for(int i=0; i<tests; i++) {
        if(test(0))                 // 0: auto; 1: manual
            sum++;
        else cout<<"---- Wrong!!! ----"<<endl;
    }
    cout<<"Success Rate: "<<sum*100.0/tests<<endl;
    return 0;
}
