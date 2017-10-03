#include<iostream>
#include<cstring>
#include<fstream>
#include<ctime> 
using namespace std;
int main()
{
	clock_t t1start,t1end;
	string t1[100000];
	int t1ans[100000] = {0};
	int counter1 = 0,ans = 1;
	char* a; char*b ; a=new char; b=new char;
	cin >> a >> b;	
	fstream fin(a,fstream::in );
	t1start = clock();
	while(!fin.eof()){
		fin >> t1[counter1];

		counter1++;			
	}
	int first=t1[0].length();
	t1ans[0] = 1;
	for(int i=1;i<counter1;i++){
		int length=t1[i].length()-t1[0].length();
		for(int j=0;j<=length;j++)
		{
			if(t1[i].at(j)==t1[0].at(0))
			{
				int checkans=1;
				for(int k=1;k<first;k++)
				{
					if(t1[i].at(j+k)!=t1[0].at(k))
					{
						checkans=0;
						break;
					}
				}
				if(checkans==1)
				{
					t1ans[ans] = i+1;
					ans++;
				}
			}
		}
	}
	t1end = clock();
	fin.close();
	fstream fout(b,fstream::out);
	fout<<(t1end-t1start)/CLK_TCK<<"ms"<<endl;
	for(int i = 0;i < ans;i++){
		fout << t1ans[i];		
	}	
	fout.close();	
}
