#include <iostream>
#include <math.h>
using namespace std;

struct inout
{
	int *input;
	int output;
	inout(int size, int in[],int out)
	{
		input=new int[size];
		for(int i=0;i<size;i++)
		{
			input[i]=in[i];
		}
		output=out;
	}
};

int mult(int size,int *weight,int *inp)
{
	int ans=0;
	for(int i=0;i<size;i++)
	{
		ans+=(weight[i]*inp[i]);
	}
	return ans;
}

//size includes input -1
int pta(int size,int * weight,inout *x)
{
	int numin=(int)pow(2,size-1);
	
	//Negate the zero class
	for(int i=0;i<numin;i++)
	{
		if(x[i].output==0)
		{
			for(int j=0;j<size;j++)
			{
				(x[i].input)[j]=-(x[i].input)[j];
			}
		}
	}

	int iter=0;
	while(true)
	{
		bool br=true;
		for(int i=0;i<numin;i++)
		{
			
				for(int k=0;k<size;k++)
				{
				cout<<"w"<<k+1<<"="<<weight[k]<<" ";
				}
				cout<<endl;
			//cout<<mult(size,weight,x[i].input)<<endl;
			if(mult(size,weight,x[i].input)<=0)
			{
				//cout<<"yo";
				br=false;
				iter++;
				for(int j=0;j<size;j++)
				{
					weight[j]+=(x[i].input)[j];
				}
			}
		}

		if(br)
		{
			return iter;
		}
	}


}

int main()
{
	/*//AND
	cout<<"AND:\n";
	int size=3;
	int weight[3]={2,3,3};
	int in1[3]={1,1,-1};
	int in2[3]={1,0,-1};
	int in3[3]={0,1,-1};
	int in4[3]={0,0,-1};
	inout x1(size,in1,1);
	inout x2(size,in2,0);
	inout x3(size,in3,0);
	inout x4(size,in4,0);
	
	inout x[4]={x1,x2,x3,x4}; 
	int iter=pta(size,weight,x);
	for(int i=0;i<size-1;i++)
	{
		cout<<"w"<<i+1<<"="<<weight[i]<<" ";
	}
	cout<<"Theta="<<weight[size-1]<<endl;
	cout<<"Number of iterations: "<<iter<<endl;
	
	//OR
	cout<<"\nOR:\n";
	inout x11(size,in1,1);
	inout x21(size,in2,1);
	inout x31(size,in3,1);
	inout x41(size,in4,0);
	
	inout orx[4]={x11,x21,x31,x41}; 
	iter=pta(size,weight,orx);
	for(int i=0;i<size-1;i++)
	{
		cout<<"w"<<i+1<<"="<<weight[i]<<" ";
	}
	cout<<"Theta="<<weight[size-1]<<endl;
	cout<<"Number of iterations: "<<iter<<endl;
	
	//NAND
	cout<<"\nNAND:\n";
	inout x12(size,in1,0);
	inout x22(size,in2,1);
	inout x32(size,in3,1);
	inout x42(size,in4,1);
	
	inout nandx[4]={x12,x22,x32,x42}; 
	iter=pta(size,weight,nandx);
	for(int i=0;i<size-1;i++)
	{
		cout<<"w"<<i+1<<"="<<weight[i]<<" ";
	}
	cout<<"Theta="<<weight[size-1]<<endl;
	cout<<"Number of iterations: "<<iter<<endl;
	*/
	
	cout<<"PALINDROME:\n";
	int size=4;
	int weight[4]={1,1,1,1};
	int in1[4]={1,1,1,-1};
	int in2[4]={1,0,1,-1};
	int in3[4]={0,1,1,-1};
	int in4[4]={0,0,1,-1};
	int in5[4]={1,1,0,-1};
	int in6[4]={1,0,0,-1};
	int in7[4]={0,1,0,-1};
	int in8[4]={0,0,0,-1};

	inout x1(size,in1,1);
	inout x2(size,in2,1);
	inout x3(size,in3,0);
	inout x4(size,in4,0);
	inout x5(size,in5,0);
	inout x6(size,in6,0);
	inout x7(size,in7,1);
	inout x8(size,in8,1);
	
	inout x[8]={x1,x2,x3,x4,x5,x6,x7,x8}; 
	int iter=pta(size,weight,x);
	for(int i=0;i<size-1;i++)
	{
		cout<<"w"<<i+1<<"="<<weight[i]<<" ";
	}
	cout<<"Theta="<<weight[size-1]<<endl;
	cout<<"Number of iterations: "<<iter<<endl;
	
	
	return 0;
}
