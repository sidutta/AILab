#include <iostream>
#include <cmath>
#include <time.h>
#include <stdlib.h>

#define str_len  3   // palindrome length
#define alpha 0.01   // learning rate

using namespace std;

double weight1[str_len][str_len/2];  // weight of arcs from input to hidden layer
double weight2[str_len/2];         // weight of arcs from output of hidden layer to output layer

double bias1[str_len/2];
double bias2;

int input[str_len];               // input that's to be checked

double sigmoid(double x) {
    return 1.00/(1+exp(-x));      // logistic function
}

void initialize_weights() {
    for(int i=0; i<str_len/2; i++) {
        for(int j=0; j<str_len; j++) {
            weight1[j][i] = 0;
        }
        weight2[i] = 0;
    }
}

void output_from_hidden_layer(int *input, double *output) {
    for(int i=0; i<str_len/2; i++) {
        double neti = -bias1[i];
        for(int j=0; j<str_len; j++) {
            neti += input[j] * weight1[j][i];
        }
        output[i] = sigmoid(neti);
    }
}

void final_output(double *input, double *output) {
    double neti = -bias2;
    for(int i=0; i<str_len/2; i++) {
        neti += input[i] * weight2[i];
    }
    *output = sigmoid(neti);
}

bool is_palindrome(int *vec, int len) {
    for(int i=0; i<len/2; i++) {
        if(vec[i]!=vec[len-1-i])
            return false;
    }
    return true;
}

void generate_random_vector(int *vec, int len) {
    for(int i=0; i<len; i++) {
        vec[i] = ((rand()%10)/10.0 + 0.5);
    }
}

void train() {
    int palin = 0;
    int not_palin = 0;
    int vec[][4] = {};
    for(int i=0; i<1000000; i++) {
        
        int input[str_len];
        generate_random_vector(input, str_len);
        int expected_output = is_palindrome(input, str_len);
        
     /*
           Uncomment to keep no. of palindromes and non-palindromes
           within a certain range during testing
       */
//        if(expected_output)
//            palin++;
//        else
//            not_palin++;
//        
//        if(abs(palin-not_palin)>1) {
//            i--;
//            if(expected_output)
//                palin--;
//            else
//                not_palin--;
//            continue;
//        }
        
        double hidden_layer_output[str_len/2];
        output_from_hidden_layer(input, hidden_layer_output);
        
        double result;
        final_output(hidden_layer_output, &result);
        
        double final_del;
        final_del = (expected_output - result) * result * (1-result);
        
        for(int i=0; i<str_len/2; i++) {
            weight2[i] += (alpha * final_del * hidden_layer_output[i]);
            bias2 = alpha * final_del;
        }
        
        for(int i=0; i<str_len/2; i++) {
            double del;
            del = weight2[i] * final_del * hidden_layer_output[i] * (1-hidden_layer_output[i]);
            for(int j=0; j<str_len; j++) {
                weight1[j][i] += (alpha * del * input[j]);
                bias1[i] += alpha * del;
            }
            
        }
        
//        cout<<bias2<<endl;
//        for(int i=0; i<str_len/2; i++) cout<<weight2[i]<<" ";
//        cout<<endl;
        
    }
    
}

bool test(int manual) {
    
    // test vector
    int input[str_len];
    
    // generating random test vector
    generate_random_vector(input, str_len);
    
    // for manual testing, enter or take user input
    if(manual) {
        for(int i=0; i<str_len; i++)
            cin>>input[i];
    }
    
    int expected_output = is_palindrome(input, str_len);
    
    double hidden_layer_output[str_len/2];
    output_from_hidden_layer(input, hidden_layer_output);
    
    double result;
    final_output(hidden_layer_output, &result);
    
    /*  Uncomment the following to see test data */
    cout<<"Input Vector: ";
    for(int i=0; i<str_len; i++) {
        cout<<input[i]<<" ";
    }
    cout<<endl;
    cout<<"Result: "<<result<<" ~ "<<(int)(result+0.5)<<endl;
    cout<<"Expected Output: "<<expected_output<<endl;
    
    // return true if ans. is correct, false otherwise
    return ((int)(result+0.5)==expected_output);
    
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
    }
    cout<<"Success Rate: "<<sum*100.0/tests<<endl;
    return 0;
}
