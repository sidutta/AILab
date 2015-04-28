#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
using namespace std;
int glob_limit=20;

struct axiom;
struct node {
	bool use_axtemp;
	bool hintb;//whether node was given as a hint
	bool axiom_parent;//whether derived from axiom
    node * left;
    node * right;
    node * parent1;//parent 1 in MP derivation
    node * parent2;//parent 2 in MP derivation
    axiom* axparent;//pointer to axiom (makes sense only when axiom_parent is true)
    char let;
    node(char temp){
		use_axtemp=false;
        let = temp;
        left  = NULL;
        right = NULL;
        parent1=NULL;
        parent2=NULL;
        axparent=NULL;
        axiom_parent=false;
        hintb=false;
    }
    node(){
        left  = NULL;
        right = NULL;
        parent1=NULL;
        parent2=NULL;
        axparent=NULL;
        axiom_parent=false;
        hintb=false;
        use_axtemp=false;
    }
};

//convert tree to string
string conv_str(node* cur)
{
    if(cur == NULL)return "";
    string temp ="";
    if(cur->left==NULL)
        temp = temp +cur->let;
    else
    {
        temp = temp + "(";
        
        temp =temp + conv_str(cur->left);
        if(cur->let == '#')temp = temp + "->";
        else
            temp = temp + cur->let;
        temp = temp + conv_str(cur->right);
        temp = temp + ")";
    }
    
    return temp;
}

//print the tree
void print(node* cur)
{
    if(cur == NULL)return;
    if(cur->left==NULL)
        cout<<cur->let;
    else
    {
        cout<<'(';
        print(cur->left);
        if(cur->let == '#')cout<<"->";
        else
        cout<<cur->let;
        print(cur->right);
        cout<<')';
    }
    
}


struct axiom
{
	node *A;
	node *B;
	node *C;
	node *val;
	int axnum;
	axiom()
	{
		A=NULL;
		B=NULL;
		C=NULL;
		val=NULL;
		axnum=-1;
	}
	axiom(node *x,node *y)//axiom 1
	{//A->(B->A)
		A=x;
		B=y;
		C=NULL;
		axnum=1;
		val=new node('#');
		val->left=A;
		val->right=new node('#');
		val->right->left=B;
		val->right->right=A;
		val->axiom_parent=true;
		val->axparent=this;
	}
	axiom(node *x,node *y,node *z)//axiom 2
	{//(A->(B->C))->((A->B)->(A->C))
		A=x;
		B=y;
		C=z;
		axnum=2;
		val=new node('#');
		val->left=new node('#');
		val->left->left=A;
		val->left->right=new node('#');
		val->left->right->left=B;
		val->left->right->right=C;
		val->right=new node('#');
		val->right->left=new node('#');
		val->right->right=new node('#');
		val->right->left->left=A;
		val->right->left->right=B;
		val->right->right->left=A;
		val->right->right->right=C;
		
		val->axiom_parent=true;
		val->axparent=this;
	}
	axiom(node *x)//axiom 3
	{//(((A->F)->F)->A)
		
		A=x;
		B=NULL;
		C=NULL;
		axnum=3;
		val=new node('#');
		val->right=A;
		val->left=new node('#');
		val->left->right=new node('F');
		val->left->left=new node('#');
		val->left->left->left=A;
		val->left->left->right=new node('F');
		
		val->axiom_parent=true;
		val->axparent=this;
	}
	void print()
	{
		if(axnum==1)
		{
			cout<<"Axiom 1: A->(B->A) where"<<endl;
			cout<<"A: "<<conv_str(A)<<endl;
			cout<<"B: "<<conv_str(B)<<endl;
			
		}
		else if(axnum==2)
		{
			cout<<"Axiom 2: (A->(B->C))->((A->B)->(A->C)) where"<<endl;
			cout<<"A: "<<conv_str(A)<<endl;
			cout<<"B: "<<conv_str(B)<<endl;
			cout<<"C: "<<conv_str(C)<<endl;
		}
		else if(axnum==3)
		{
			cout<<"Axiom 3: (((A->F)->F)->A) where"<<endl;
			cout<<"A: "<<conv_str(A)<<endl;
		}
		else
		{
			cout<<"Invalid axiom"<<endl;
		}
	}
};

set<string> check;//contains all the hypotheses in string form
vector<node *> hypothesis;//contains all the hypotheses in tree form
vector <node *> single_lit;// contains all single literal of the form p and ~p (tree form)
set <string> check_lit;//// contains all single literal of the form p and ~p (string form

//removes '~' from string and convert it to form with only implications
string conv_standard(string s)
{
    stringstream ss;
    for(int i=0;i<s.size();i++)
    {
        if(s[i]!='~')
            ss<<s[i];
        else
        {
            if(s[i+1]!='(')\
            {
                i++;
                ss<<'('<<s[i]<<"->F)";
            }
            else
            {
                ss<<'(';
                int count=0;
                stringstream temp;
                bool exist=false;
                while(true)
                {
                    i++;
                    temp<<s[i];
                    if(s[i]=='~')
                        exist=true;
                    if(s[i]=='(')
                        count++;
                    if(s[i]==')')
                        count--;
                    if(count==0)
                        break;
                }
                temp<<"->F)";
                string tempstring;
                temp>>tempstring;
                if(exist)
                    tempstring=conv_standard(tempstring);
                
                ss<<tempstring;
            }
        }
    }
    string ans;
    ss>>ans;
    return ans;
}

//construct the hypotheses set
void hypo(node *root){
    
    if(root->left != NULL){
		if(check.find(conv_str(root->left))==check.end())//not already present
		{
        hypothesis.push_back(root->left);
        //cout<<"yo "<<conv_str(root->left)<<" "<<conv_str(root->right)<<endl;
        check.insert(conv_str(root->left));
        hypo(root->right);
        /*if(root->left->left==NULL)//single literal
        {
			single_lit.push_back(root->left);
			check_lit.insert(conv_str(root->left));
		}*/
	}
	else
	{
		hypo(root->right);
	}
    }
    else
    {
        if(root->let == 'F'){
                //done;
        }
        else{
            node * r1 = new node('#');
            node * l = new node(root->let);
            node * r = new node('F');
            r1->left = l;
            r1->right = r;
            hypothesis.push_back(r1);
           // cout<<"yo "<<conv_str(r1)<<"\n";
            check.insert(conv_str(r1));
            //hypo(root->right);
        }
    }
	
	if(check_lit.find("F")==check_lit.end())//F not present in single literal list
	{
		node * rootf=new node('F');
		single_lit.push_back(rootf);
		check_lit.insert("F");
	}
}

//convert string to tree
node* make_tree(string inp){
	//cout<<inp<<endl;
	if(inp.size()!=0 && ((inp[0]>'a'&& inp[0]<'z') || (inp[0]>'A' && inp[0]<'Z')) && inp[0]!='V')
	{
	string temps(1,inp[0]);
        //cout<<"temps is "<<temps<<endl;
        if(check_lit.find(temps)==check_lit.end())
        {
			//cout<<"yo\n";
			check_lit.insert(temps);
			node *temproot=new node(inp[0]);
			single_lit.push_back(temproot);
			
		}
	}	
	
	
      if(inp.size()!=0 && inp[0]=='('){
        int cnt=1;
        int j=1;
        while(cnt>0){
            if(inp[j]=='('){
                cnt++;
            }
            else if(inp[j]==')'){
                cnt--;
            }
            
            j++;
        }
        if(j==inp.size()){
            return make_tree(inp.substr(1,inp.size()-2));
        }
    }
    
    if(inp.size()==0){
        return NULL;
    }
    else if(inp.size()==1){
        node * root = new node(inp[0]);
        return root;
    }
    else if(inp[0]=='('){
        int cnt=1;
        int j=1;
        while(cnt>0){
            if(inp[j]=='('){
                cnt++;
            }
            else if(inp[j]==')'){
                cnt--;
            }
            
            j++;
        }
        
        node * root = new node('#');
       
        if(inp[j+1] == '>'){
		 string l =inp.substr(1,j-2);
        string r = inp.substr(j+2);
        //cout<<l<<" & "<<r<<endl;
        root->left = make_tree(l);
        root->right = make_tree(r);
        
	    }
	    else if(inp[j] == '^'){
			string l12 = inp.substr(1,j-2);
			string r12 = inp.substr(j+1);
			//cout<<l<<" & "<<r<<endl;
			node * r = make_tree(r12);
			node * l = make_tree(l12);
			node * var1 = new node('F');
			node * var2 = new node('#');
			node * var3 = new node('#');
			node * var4 = new node('F');
			root->left = var2;
			var2->left = l;
			var2->right = var3;
			var3->left  = r;
			var3->right = var4;
			root->right = var1;
			return root;
			
		}
		else if(inp[j] == 'V'){
			string l12 = inp.substr(1,j-2);
			string r12 = inp.substr(j+1);
			node * l = make_tree(l12);
			node * r = make_tree(r12);
			node * var1 = new node('#');
			node * var2 = new node('F');
			root->left = var1;
			root->right = r;
			var1->left = l;
			var1->right = var2;
			return root;
		}
        return root;
        
    }
    else{ // no left bracket
        node * l = new node(inp[0]);
        if(inp[2]=='>'){
        node * r1 = make_tree(inp.substr(3));
        node * root = new node('#');
        root->left = l;
        root->right = r1;
        // cout<<inp[0]<<" & "<<inp.substr(3)<<endl;
        return root;
	    }
	    else if(inp[1]=='^'){
			node * root = new node('#');
			node * r = make_tree(inp.substr(2));		
			node * var1 = new node('F');
			node * var2 = new node('#');
			node * var3 = new node('#');
			node * var4 = new node('F');
			root->left = var2;
			var2->left = l;
			var2->right = var3;
			var3->left  = r;
			var3->right = var4;
			root->right = var1;
			return root;
		}
		else if(inp[1]=='V'){
		    node * root = new node('#');
			node * r = make_tree(inp.substr(2));
			node * var1 = new node('#');
			node * var2 = new node('F');
			root->left = var1;
			root->right = r;
			var1->left = l;
			var1->right = var2;
			return root;
		}
        
    }
    return NULL;
}

//find MP closure starting from int start position in the hypotheses vector
//if while finding closure, F is found, then return pointer to it, else return NULL
bool closure_added=true;
node* closure(){
    //int add =0;
    closure_added=false;
    static int start=0;
    node* answer=NULL;
    int limit=hypothesis.size();
    //int init_size=hypothesis.size(); //changed
    //int i=start;//changed
    
    for(int i=start; i<limit; i++){
		//cout<<"i is "<<i<<" limit is "<<limit<<endl;
        for(int j=0; j<i; j++){
            
            if(conv_str(hypothesis[i]) == conv_str(hypothesis[j]->left)){
                if(check.find(conv_str(hypothesis[j]->right)) == check.end())
                {
					hypothesis[j]->right->parent1=hypothesis[i];//changed
					hypothesis[j]->right->parent2=hypothesis[j];//changed
					hypothesis.push_back(hypothesis[j]->right);
					check.insert(conv_str(hypothesis[j]->right));
			//		cout<<"Closure----------\n";
			//		cout<<conv_str(hypothesis[j]->right)<<endl;
			//		cout<<"----------\n";
					closure_added=true;
					//add++;
					if(hypothesis[j]->right->let=='F')
					return hypothesis[j]->right;
					}
            }
            if(conv_str(hypothesis[i]->left) == conv_str(hypothesis[j])){
                if(check.find(conv_str(hypothesis[i]->right)) == check.end())
                {
					hypothesis[i]->right->parent1=hypothesis[j];
					hypothesis[i]->right->parent2=hypothesis[i];
					hypothesis.push_back(hypothesis[i]->right);
					check.insert(conv_str(hypothesis[i]->right));
				//	cout<<"Closure----------\n";
				//	cout<<conv_str(hypothesis[i]->right);
				//	cout<<"----------\n";
					closure_added=true;
					//add++;
					if(hypothesis[i]->right->let=='F')
					return hypothesis[i]->right;
					}
                
            }
        }
    }
    start=limit;
    //start=i;//changed
    return answer;
}

bool check_ax1(node *root){ // (A->(B->A))
if(root==NULL)
return false;	
if(root->left == NULL || root->right == NULL || root->right->right == NULL)return false;
if(conv_str(root->left) == conv_str(root->right->right))return true;
else return false;
}

bool check_ax2(node *root){ // (A->(B->C) -> (A->B)->(A->C))
	if(root==NULL)
return false;
if(root->left == NULL || root->right == NULL)return false;
if(root->left->left == NULL || root->left->right == NULL || root->right->right == NULL || root->right->left == NULL)return false;
if(root->right->left->left == NULL || root->right->left->right == NULL || root->right->right->left == NULL || root->right->right->right == NULL)return false;
string A = conv_str(root->left->left);
string B = conv_str(root->left->right->left);
string C = conv_str(root->left->right->right);

string temp1 = conv_str(root->right->left->left);
string temp2 = conv_str(root->right->left->right);
string temp3 = conv_str(root->right->right->left);
string temp4 = conv_str(root->right->right->right);
if(A == temp1 && A == temp3 && B == temp2 && C == temp4)return true;
else return false;

}

bool check_ax3(node *root){//((A->F)->F)->A
if(root==NULL)
return false;
if(root->right == NULL || root->left == NULL || root->left->left == NULL || root->left->left->left == NULL)return false;

string temp1 = conv_str(root->right);
string temp2 = conv_str(root->left->left->left);
if(temp1 == temp2 && root->left->right->let == 'F' && root->left->left->right->let == 'F')return true;
else return false;

}

bool check_left_ax3(node *root){//((A->F)->F)
if(root==NULL)
return false;
if(root->right == NULL || root->left == NULL || root->left->left == NULL )return false;

//string temp1 = conv_str(root->left->left);
//string temp2 = conv_str(root->left->left->left);
if(root->left->right->let == 'F' && root->right->let == 'F')return true;
else return false;

}


bool check_axiom(node * root){

return check_ax1(root) || check_ax2(root) || check_ax3(root);
}

node* axiomize(){
//cout<<"-----------------\n";
//cout<<"Entered axiomize\n";
//cout<<"-----------------\n";
for(int i=0; i<hypothesis.size(); i++){
bool tempb=check_axiom(hypothesis[i]->left);
if(hypothesis[i]->left!=NULL)
{
if(tempb && hypothesis[i]->right->let == 'F')
{
	hypothesis[i]->left->use_axtemp=true;
	hypothesis[i]->right->parent1=hypothesis[i]->left;
	hypothesis[i]->right->parent1=hypothesis[i];
	return hypothesis[i]->right;
	
}
else if(tempb && check.find(conv_str(hypothesis[i]->right))==check.end())
{ hypothesis[i]->left->use_axtemp=true;
	hypothesis[i]->right->parent1=hypothesis[i];
	hypothesis[i]->right->parent1=hypothesis[i]->left;
	
	//if(hypothesis[i]->right->let=='F')
	//return hypothesis[i]->rightl;
	
	hypothesis.push_back(hypothesis[i]->right);
	check.insert(conv_str(hypothesis[i]->right));
	cout<<"Inserted "<<conv_str(hypothesis[i]->right)<<endl;
}
}

if(check_left_ax3(hypothesis[i]))
{
		axiom * tempax3=new axiom(hypothesis[i]->left->left);
		node *temp1=hypothesis[i]->left->left;
		//tempax3->print();
				if(check.find(conv_str(temp1))==check.end())
				{
				//cout<<"yo entered "<<conv_str(temp1)<<endl;	
				temp1->parent1=	tempax3->val;
				temp1->parent2=	hypothesis[i];
				hypothesis.push_back(temp1);
				check.insert(conv_str(temp1));
	//			node *tempnode=closure(hypothesis.size()-1);
				
	//			if(tempnode!=NULL)
	//			return tempnode;
			}
}

}
return NULL;
}

//print derivation of x (using parent pointers)
void print_derivation(node *x)
{
	//cout<<"yo\n";
	if(x==NULL)
	{
		cout<<"Invalid\n";
		return;
	}
	if(x->use_axtemp)
	{
		cout<<conv_str(x)<<"  Using axiom\n";
		return;
	}
	if(x->hintb)
	{
		cout<<conv_str(x)<<"  [Theorem given as hint]\n\n";
		return;
	}
	if(x->axiom_parent)
	{
		cout<<conv_str(x)<<"  using axiom "<<x->axparent->axnum<<endl;
		x->axparent->print();
		cout<<endl;
		return;	
	}
	
	if(x->parent1==NULL)
	{
	cout<<conv_str(x)<<"  [Hypothesis]\n\n";	
	return;
	}
	if(x->parent1!=NULL)
	{
		print_derivation(x->parent1);
		print_derivation(x->parent2);
		cout<<conv_str(x)<<" using\n(i) "<<conv_str(x->parent1)<<"\n(ii) "<<conv_str(x->parent2)<<"\n\n";
	}
}


//Apply axioms to literals of length 1, add them to hypotheses vector
//then take closure of hypotheses vector and return pointer to F if F is found while taking closure
void basic_axiom()
{
	//cout<<".......................................................................\n";
	//cout<<"axiommmmmmmmm\n";
	//cout<<".......................................................................\n";
	int single_lit_size=single_lit.size();
	for(int i=0;i<single_lit_size;i++)
	{//add not of single literals to vectors containing single literals
		node *temp=new node('#');
		temp->left=single_lit[i];
		temp->right=new node('F');;
		single_lit.push_back(temp);
		check_lit.insert(conv_str(temp));
	}
	single_lit_size=single_lit.size();
	for(int i=0;i<single_lit_size;i++)
	{
		
		axiom * tempax3=new axiom(single_lit[i]);
		//tempax3->print();
				if(check.find(conv_str(tempax3->val))==check.end())
				{
				hypothesis.push_back(tempax3->val);
				check.insert(conv_str(tempax3->val));
	//			node *tempnode=closure(hypothesis.size()-1);
				
	//			if(tempnode!=NULL)
	//			return tempnode;
			}
		
		for(int j=0;j<single_lit_size;j++)
		{
			axiom * tempax1=new axiom(single_lit[i],single_lit[j]);
			//tempax1->print();
				if(check.find(conv_str(tempax1->val))==check.end())
				{
				hypothesis.push_back(tempax1->val);
				check.insert(conv_str(tempax1->val));
	//			node *tempnode=closure(hypothesis.size()-1);
				
	//			if(tempnode!=NULL)
	//			return tempnode;
			}
			
			for(int k=0;k<single_lit_size;k++)
			{
				axiom * tempax2=new axiom(single_lit[i],single_lit[j],single_lit[k]);
				//tempax2->print();
				if(check.find(conv_str(tempax2->val))==check.end())
				{
				hypothesis.push_back(tempax2->val);
				check.insert(conv_str(tempax2->val));
	//			node *tempnode=closure(hypothesis.size()-1);
	//			
	//			if(tempnode!=NULL)
	//			return tempnode;
			}
			}
		}
	}
	//return NULL;
}

//take any theorem as hint and add it to hypotheses vector and take its closure
node* take_hint(node* hint)
{
	if(check.find(conv_str(hint))==check.end())
	{
		check.insert(conv_str(hint));
		hypothesis.push_back(hint);
		node *temp;
		closure_added=true;
		int h_limit=glob_limit;
		int h_start=0;
		while(closure_added)
		{
		h_start++;	
		temp=closure();
		if(temp!=NULL)
		break;
		if(h_limit==h_start)
		break;
		}
		return temp;
	}
	return NULL;
}

void give_hint()
{
	for(int i=0;i<hypothesis.size();i++)
	{
		for(int j=0;j<hypothesis.size();j++)
		{
			node *temp=new node('#');
			temp->left=hypothesis[i];
			temp->right=new node('F');
			if(conv_str(hypothesis[j]->right)==conv_str(temp))
			{
				if(conv_str(hypothesis[j]->left)!="F")
				cout<<conv_str(hypothesis[j]->left)<<endl;
			}
			delete(temp->right);
			delete(temp);
		}
	}
}

//convert expressions of the form (p->F)->F to p
void prune_tree(node * &n)
{
	//cout<<"yo1\n";
	if(n!=NULL)
	{
		//cout<<"yo2\n";
		if(n->left!=NULL && n->left->left!=NULL)
		{
			//cout<<"yo3\n";
			if(n->right->let=='F' && n->left->right->let=='F')
			{
				cout<<conv_str(n)<<" pruned\n";
				n=n->left->left;
				prune_tree(n);
			}
			else
			{
				prune_tree(n->left);
				prune_tree(n->right);
			}
		}
	}
}
//prune every tree in the hypothesis set
/*void prune_complete()
{
	for(int i=0; i< hypothesis.size(); i++){
		check.erase(conv_str(hypothesis[i]));
		prune_tree(hypothesis[i]);
		check.insert(conv_str(hypothesis[i]));
		
	}
}*/

int main(){
    
    string inp;
    cin>>inp;
    string inp1 = conv_standard(inp);
    //cout<<inp1<<endl;;
    node * root = make_tree(inp1);
    //prune_tree(root);
    cout<<"Convert to only implication form\n";
    print(root);
    cout<<"\n*****************\n";
    hypo(root);
    cout<<"Printing the Hypotheses\n";
    for(int i=0; i< hypothesis.size(); i++){
        cout<<i+1<<") "<<conv_str(hypothesis[i])<<endl;;
    }
    
    node * ans_final;
    closure_added=true;
    while(closure_added)
    {
    ans_final=closure();
    if(ans_final!=NULL)
    break;
	}
    //cout<<"Closure called\n";
    if(ans_final!=NULL)
    {
		cout<<"\nDerivation can be found :)\n";
		print_derivation(ans_final);
	}
	else
	{
		node *ans_final2;
		basic_axiom();
		closure_added=true;
		int ax_limit=glob_limit;
		int ax_start=0;
		while(closure_added)
		{
		//cout<<"yooooooooooooooooooooooooooooooooo\n";	
		ax_start++;	
		ans_final2=closure();
		if(ans_final2!=NULL)
		break;
		if(ax_start==ax_limit)
		break;
		}
		/*cout<<"yoyo\n";
		cout<<ans_final2->let<<endl;
		cout<<ans_final2->pare*/
		if(ans_final2!=NULL)
		{
			
		cout<<"\nDerivation can be found :)\n";
		print_derivation(ans_final2);
		}
		else
		{
			
			
			
		node *ans_final3=axiomize();
		if(ans_final3!=NULL)
		{
			cout<<"\nDerivation can be found :)\n";
		print_derivation(ans_final3);
		return 0;
		}
		
		closure_added=true;
		int f_limit=glob_limit;
		int f_start=0;
		while(closure_added)
		{
		//cout<<"yooooooooooooooooooooooooooooooooo\n";	
		f_start++;	
		ans_final3=closure();
		if(ans_final3!=NULL)
		{
		cout<<"\nDerivation can be found :)\n";
		print_derivation(ans_final3);
		return 0;
		}
		if(f_start==f_limit)
		break;
		}
		
		
		///////////////////
		/*cout<<"Printing Hypothesis\n";
		for(int i=0;i<hypothesis.size();i++)
		{
			cout<<conv_str(hypothesis[i])<<endl;
		}
		cout<<"Ending Hypothesis\n";*/
		
		///////////////////
			
		node *tempans=NULL;
		while(tempans==NULL)
		{		
		cout<<"\nDerivation cannot be found :( Please give a hint\n";
		cout<<"#######"<<endl;
		cout<<"Possible hints\n";
		give_hint();
		cout<<"#######"<<endl;
		string hint;
		cin>>hint;
		hint = conv_standard(hint);
		node *hintroot=make_tree(hint);
		hintroot->hintb=true;
		tempans=take_hint(hintroot);
		
		if(tempans==NULL)
		{
			closure_added=true;
		int h_limit=glob_limit;
		int h_start=0;
		while(closure_added)
		{
		//cout<<"yooooooooooooooooooooooooooooooooo\n";	
		f_start++;	
		tempans=closure();
		if(tempans!=NULL)
		{
		cout<<"\nDerivation can be found :)\n";
		print_derivation(ans_final3);
		return 0;
		}
		if(h_start==h_limit)
		break;
		}
		}
		
		}
		
		cout<<"\nDerivation can be found :)\n";
		print_derivation(tempans);
		}
	}
	
    
    
}
//((p->q)->((r->s)->t))->((u->((r->s)->t))->((p->u)->(s->t))):~p->(p->q),~p->t
//(((p->F)->(((q->F)->(((p->F)->q)->F))->F))->F)
//(((p->F)->(((q->F)->(((p->F)->q)->F))->F))->F)

//(p->q)->((~p->q)->q)
//(~(p^q))->(~pV~q)
//(~(pVq))->(~p^~q): (p->F)->q
//(~pV~q)->(~(p^q)): p->(q->F)
//(~p^~q)->(~(pVq)):((p->F)->q)->F
