#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

map<string, int> m;
map<int, string> reverse_map;

int trans[100][100];
int emission[100][100];
double psum[100];
double ptrans[100][100];
double pemission[100][100];

int k = 1;

double P(int state_from, char obs,int state_to){
    if(obs == '[' && state_from==1)
        return ptrans[state_from-1][state_to-1];
    else if(reverse_map[state_from-1]=="." || state_from==1)
        return 0;
    double q = ptrans[state_from-1][state_to-1] * pemission[state_from-1][obs-'A'];
    //cout<<q<<endl;
    return q;
}

int N,T;
double seqscore[1000][1000];
double backptr[1000][1000];

void viterbi() {
    string word;
    cin>>word;
    N = k;
    word = "[[" + word;
    T = word.length();
    
    seqscore[1][1]=(double)1.0;
    backptr[1][1]=0.0;
    for(int i=2; i<=N; i++){
        seqscore[i][1]=0;
    }
    
    //Iterating
    for(int t=2; t<=T; t++){ // t th iteration
        for(int i=1; i<=N; i++){
            double maxa=0;
            int index = -1;
            for(int j=1; j<=N; j++){ //from jth state to ith state
                double temp;
                if(t!=T) {
                    if(reverse_map[i-1]!="." && reverse_map[i-1]!="^")
                        temp = seqscore[j][t-1] * P(j,word[t-1],i); // be careful of index in word I need to append a fixed character to all words which is word[1]
                    else temp = 0;
                }
                else {
                    if(reverse_map[i-1]==".")
                        temp = seqscore[j][t-1] * P(j,word[t-1],i);
                    else
                        temp = 0;
                }
                if(maxa < temp){
                    index = j; //max occurs from state j
                    maxa = temp;
                }
            }
            seqscore[i][t] = maxa;
            backptr[i][t] = index;
        }
    }
    
    //Finding the best sequence
    vector<int> ans(T+1);
    double maxi = 0;
    int index = -1;
    for(int i=1; i<=N; i++){
        if(maxi < seqscore[i][T]){
            maxi = seqscore[i][T];
            index = i;
        }
    }

    ans[T] = index;
    
    for(int i=T-1; i>=1; i--){
        ans[i] = backptr[ans[i+1]][i+1];
        
    }
    for(int i=2; i<=T-1; i++)
        cout<<reverse_map[ans[i]-1]<<" ";
    cout<<endl;
}

int main() {
    
    
    
	ifstream in("cmudict-0.7b.txt");
	ofstream out("clean2.txt");
	string s;
    
	while(getline(in, s)) {
		int j=0;
		string t = "";
		bool flag = true;
		for(int i=0; i<s.length(); i++) {
			if(flag) t+=s[i];
			if(s[i]==' ') {
				j++;
				flag=false;
			}
		}
        
        int repeat_index;
        if(j!=t.length()) {
            char c = t[0];
            for(int mm=1; mm<t.length(); mm++) {
                if(t[mm]==c) {
                    repeat_index = mm;
                    break;
                }
            }
        }
        
		if(j==t.length()) {
            s = "[" + s + " .";
            t = "[" + t;
			out<<s<<endl;
			stringstream ss(s);
			string tok;
			ss>>tok;
			int prev_key = 0;
			int l=0;
			while(ss>>tok) {
				if(m.find(tok)==m.end()) {
					m.insert(make_pair(tok, k));
                    reverse_map.insert(make_pair(k, tok));
					k++;
				}
				int key = m[tok];
				trans[prev_key][key]++;
				emission[prev_key][t[l]-'A']++;
				psum[prev_key]++;
				prev_key = key;
				l++;
			}
		}
	}
    m.insert(make_pair("^", 0));
    reverse_map.insert(make_pair(0, "^"));
	out.close();
	in.close();
	for(int p=0; p<100; p++) {
		for(int q=0; q<100; q++) {
            //cout<<k<<endl;
			if(psum[p]!=0) {
				ptrans[p][q] = (double)trans[p][q]/psum[p];
//                cout<<p<<" "<<reverse_map[p]<<" "<<q<<" "<<reverse_map[q]<<" "<<ptrans[p][q]<<"\t\t\t"<<trans[p][q]<<" "<<psum[p]<<endl;
			}
		}
		for(int q=0; q<27; q++) {
			if(psum[p]!=0) {
				pemission[p][q] = (double)emission[p][q]/psum[p];
//                cout<<p<<" "<<reverse_map[p]<<" "<<q<<" "<<(char)('A'+q)<<" "<<pemission[p][q]<<"\t\t\t"<<emission[p][q]<<" "<<psum[p]<<endl;
			}
		}
	}
    ptrans[0][0] = 0;
    viterbi();
    
    
	return 0;
}
