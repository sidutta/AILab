#include <iostream>
#include <cmath>
using namespace std;


int manhat(int arr1[3][3],int arr2[3][3])
{
	int arr1x[9];
	int arr2x[9];
	int arr1y[9];
	int arr2y[9];
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			arr1x[arr1[i][j]]=i;
			arr1y[arr1[i][j]]=j;
			arr2x[arr2[i][j]]=i;
			arr2y[arr2[i][j]]=j;
			
		}
	}
	int ans=0;
	for(int i=1;i<9;i++)
	{
		ans+=abs(arr1x[i]-arr2x[i]);
		ans+=abs(arr1y[i]-arr2y[i]);
	}
	return ans;
}

int main()
{
	int arr1[3][3],arr2[3][3];
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			cin>>arr1[i][j];
			
		}
	}
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			cin>>arr2[i][j];
			
		}
	}
	
	cout<<manhat(arr1,arr2);
}