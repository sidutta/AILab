#include <set>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <algorithm>

using namespace std;

string ats(int arr[3][3]) {
		string s = "";
		for(int i=0; i<3; i++)
			for(int j=0; j<3; j++){
				s = s + to_string(arr[i][j]);
			}
				
		return s;
}




/* int[][] string_to_array(string s) {
		int arr[3][3];
		for(int i=0; i<3; i++)
			for(int j=0; j<3; j++)
				arr[i][j] = (int)s.charAt(3*i+j);
		return arr;
	}
*/
class eight_puzzle_node   {
	public:
	int x, y ; // x & y coordinates of empty tile
	int matrix[3][3];
	 void print_state(){
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				cout<<matrix[i][j];
			}
			cout<<endl;;
		}
		cout<<endl;
	}
	eight_puzzle_node * parent;
	eight_puzzle_node(){
		parent  = NULL;
		
	}

	// bool operator == (const eight_puzzle_node & other){
	// 	if(ats(matrix) == ats(other.matrix) ){
	// 		return true;
	// 	}
	// 	return false;
	// }
	
	int g,h,f;
	

	 int calc_h(){
		int h =0; //Manhattan Distance
		for(int i=0; i<3; i++){ 
			for(int j=0; j<3; j++){
				if(matrix[i][j]==1)h+=(i+j);
				else if(matrix[i][j]==2)h+=(i+abs((j-1)));
				else if(matrix[i][j]==3)h+=(i+abs((j-2)));
				else if(matrix[i][j]==4)h+=(abs(i-1)+j);
				else if(matrix[i][j]==5)h+=(abs(i-1)+abs((j-1)));
				else if(matrix[i][j]==6)h+=(abs(i-1)+abs((j-2)));
				else if(matrix[i][j]==7)h+=(abs(i-2)+j);
				else if(matrix[i][j]==8)h+=(abs(i-2)+abs((j-1)));
			}
		}
		
		int h1=0;//Number of displaced tiles
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				if(matrix[i][j] != 0 && matrix[i][j] != 3*i+j+1)
					h1++;
			}
		}
		
		int h2 = 0;//Displacement of Blank Tile
		int l=0, m = 0;
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				if(matrix[i][j] == 0) {
					l = i;
					m = j;
				}
			}
		}
		h2 = 2-l + 2-m;
		int y = rand()%40+20;
		return h+y;
	   
		
	}

	int calc_other_h(){
		return 0;
	}


};



struct _compare {
    bool operator() (const eight_puzzle_node * lhs, const eight_puzzle_node * rhs) const{
        
       //cout<<lhs->matrix[1][2];
       int arr1[3][3],arr2[3][3];
       string l1,r1;
       for(int i=0; i<3; i++){
       	for(int j=0; j<3; j++){
       		arr1[i][j] =lhs->matrix[i][j];
       	arr2[i][j] =rhs->matrix[i][j];
       }
       }
       l1= ats(arr1);r1 =ats(arr2);
		if(lhs->h+lhs->g == rhs->h+rhs->g) return l1<r1;
		if(lhs->h+lhs->g<rhs->h+rhs->g) return true;
		if(lhs->h+lhs->g>rhs->h+rhs->g) return false;
		return false;
        
    }
};
map<string,set<eight_puzzle_node * ,_compare>::iterator> map_o;
map<string,set<eight_puzzle_node * ,_compare>::iterator> map_c;
class Openlist {
	
   public:
    set<eight_puzzle_node *,_compare> ts;
	
};

class Closedlist {
	
	public:
	 set<eight_puzzle_node *,_compare>  ts;

};

bool _find(set<eight_puzzle_node * ,_compare> &St, eight_puzzle_node * node,string yo){
	
	
	if(yo == "CL"){
		if(map_c.find(ats(node->matrix)) != map_c.end())return true;
		return false;
	}
	else{
		if(map_o.find(ats(node->matrix)) != map_o.end())return true;
		return false;
	}


}

void _erase(set<eight_puzzle_node * ,_compare> &St, eight_puzzle_node * node,string yo){
	
			if(yo == "CL")
			{St.erase(map_c[ats(node->matrix)]); map_c.erase(ats(node->matrix));}
		    else{ St.erase(map_o[ats(node->matrix)]); map_o.erase(ats(node->matrix));}
}
 
pair< set<eight_puzzle_node * ,_compare>::iterator,bool> ret;
class Astar {
	public:
 
//	public static Closedlist CL = null;
//	public static Openlist OL = null;
	static bool reachable(int arr[][3]) {
		int sum = 0;
		string cur_state =  ats(arr);
		for(int i=0; i<8; i++) {
			for(int j=i+1; j<9; j++) {
				char a = cur_state[j];
				char b = cur_state[i];
				if(cur_state[j]!='0'&&a <b)
					sum += 1;
			}
		}
		//System.out.println("Sum turned to be " + sum);
		return (sum%2==0);
	}
	
	
	
};	
map<string ,int> curstate_to_g;

void node_expansion(Openlist &OL, Closedlist &CL, eight_puzzle_node * toadd) {
		if(_find(OL.ts,toadd,"OL")){ 
			if(curstate_to_g[ ats(toadd->matrix)]>toadd->g){
			_erase(OL.ts,toadd,"OL");
			ret = OL.ts.insert(toadd);
			map_o[ats(toadd->matrix)] = ret.first;
			
			curstate_to_g[ ats(toadd->matrix)] = toadd->g;
			}

		}
		else if(_find(CL.ts,toadd,"CL") ){
			if(curstate_to_g[( ats(toadd->matrix))] > toadd->g){
			_erase(CL.ts,toadd,"CL");
			ret = OL.ts.insert(toadd);
			map_o[ats(toadd->matrix)] = ret.first;
			curstate_to_g[ ats(toadd->matrix)] = toadd->g;
			}
		}
		else{
			curstate_to_g[ ats(toadd->matrix)] =toadd->g;
			ret = OL.ts.insert(toadd);
			map_o[ats(toadd->matrix)] = ret.first;

		}
	}
	int puzzle[3][3] ;
//////////////INT MAIN ////////////
	
	int  main(){
		
		
			

		for(int i=0;i<3; i++){
			for(int j=0; j<3; j++){
				int a;
				cin>>a;
				
				puzzle[i][j] = a;
			}
		}
		
		if(!Astar::reachable(puzzle)) {
			cout<<"Goal state can't be reached->";
			return 0;
		}
		
		int ans;
		Openlist OL;
		Closedlist CL;
		Openlist DOL;
		Closedlist DCL;
		
		int dest[3][3];
		//Create the destination
		for(int i=0;i<3; i++){
			for(int j=0; j<3; j++){
				dest[i][j] = i*3+j+1; ;
			}
		}
		dest[2][2] = 0;
		
		eight_puzzle_node *tp = new eight_puzzle_node();
		//tp->matrix = puzzle;
		for(int i=0;i<3; i++){
			for(int j=0; j<3; j++){
				tp->matrix[i][j] = puzzle[i][j] ;
			}
		}
		tp->g =0;
		tp->h = tp->calc_h();
		tp->f = tp->g + tp->h;
		
		
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				if(puzzle[i][j]==0){
					tp->x = i;
					tp->y = j;
					break;
					
				}
			}
		};
		ret = OL.ts.insert(tp);;
		map_o[ats(tp->matrix)] = ret.first;
		
		curstate_to_g[ ats(puzzle)] =0;
		eight_puzzle_node * the_end;
		
		while(true){

			eight_puzzle_node *top = *OL.ts.begin();
			
			//cout<<CL.ts.size()<<endl;
			int tx = top->x;
			int ty = top->y;
			

			_erase(OL.ts,top,"OL");
			
			ret = CL.ts.insert(top);
			map_c[ats(top->matrix)] = ret.first;
			curstate_to_g[ ats(top->matrix)] = top->g;
			if( ats(top->matrix)=="123456780" ){ans = top->g;the_end = top;break;}
			//cout<<"LOOP\n";
			
			if(tx+1<3){
				eight_puzzle_node  *toadd = new eight_puzzle_node();
				for(int i=0; i<3; i++){
					for(int j=0; j<3; j++){
						toadd->matrix[i][j] = top->matrix[i][j];
					}
				}
				toadd->matrix[tx+1][ty] = top->matrix[tx][ty];
				toadd->matrix[tx][ty]=top->matrix[tx+1][ty];
				toadd->parent = top;
				toadd->g = top->g +1;
				toadd->h = toadd->calc_h();
				toadd->f = toadd->g + toadd->h;
				toadd->x = tx+1;
				toadd->y = ty;
				
				node_expansion(OL, CL, toadd);
			
			}
			
			if(tx-1>=0){
				eight_puzzle_node * toadd = new eight_puzzle_node();

				for(int i=0; i<3; i++){
					for(int j=0; j<3; j++){
						toadd->matrix[i][j] = top->matrix[i][j];
					}
				}
				
				toadd->parent = top;
				toadd->g = top->g +1;
				toadd->x = tx-1;
				toadd->y = ty;
				toadd->matrix[tx-1][ty] = top->matrix[tx][ty];
				toadd->matrix[tx][ty] =  top->matrix[tx-1][ty];
				toadd->h = toadd->calc_h();
				toadd->f = toadd->g + toadd->h;

				node_expansion(OL, CL, toadd);
			}
			if(ty+1<3){
				eight_puzzle_node * toadd = new eight_puzzle_node();
				for(int i=0; i<3; i++){
					for(int j=0; j<3; j++){
						toadd->matrix[i][j] = top->matrix[i][j];
					}
				}
				toadd->matrix[tx][ty+1] = top->matrix[tx][ty];
				toadd->matrix[tx][ty]=top->matrix[tx][ty+1];
				toadd->parent = top;
				toadd->g = top->g +1;
				toadd->h = toadd->calc_h();
				toadd->f = toadd->g + toadd->h;
				toadd->x = tx;
				toadd->y = ty+1;
			
				node_expansion(OL, CL, toadd);
			}
			if(ty-1>=0){
				eight_puzzle_node * toadd = new eight_puzzle_node();
				for(int i=0; i<3; i++){
					for(int j=0; j<3; j++){
						toadd->matrix[i][j] = top->matrix[i][j];
					}
				}
			
				toadd->parent = top;
				toadd->g = top->g +1;
				
				toadd->x =tx;
				toadd->y = ty-1;
				toadd->matrix[tx][ty-1] = top->matrix[tx][ty];
				toadd->matrix[tx][ty]=top->matrix[tx][ty-1];
				toadd->h = toadd->calc_h();
				toadd->f = toadd->g + toadd->h;
	
				node_expansion(OL, CL, toadd);
			}
			
		}
		
		
		int k = ans;
		eight_puzzle_node * temp;
		
		cout<<"-------------------------------------\n";
		cout<<"Answer is \n";
		cout<<ans<<endl;
		cout<<"Number of nodes expanded\n";
		cout<<CL.ts.size()<<endl;
		return 0;
	}
	





