#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
//迭代的方法，时间复杂度(nW)
/***************物体属性******************/
class item
{
public:
	long value;
	int weight;
	item() : value(0), weight(0){}
};

class Knapsack
{
public:
/***************构造函数读取数据*********************/
	Knapsack()
	{
		ifstream fin("4.txt");
		string line;
		stringstream stream;
		if(getline(fin, line))
		{
			stream.clear();
			stream << line;
			stream >> knapsack_size;
			stream >> num_of_items;
		}
		int cnt = 1;
		init_items();
		while(getline(fin, line))
		{
			long _value;
			int _weight;
			stream.clear();
			stream << line;
			stream >> _value;
			stream >> _weight;
			items[cnt].value = _value;
			items[cnt].weight = _weight;
			cnt++;
		}
	}

	void init_items()
	{
		items.resize(num_of_items+1);
		Avalue.resize(num_of_items+1);
		for(int i = 0; i <= num_of_items; i++)
		{	
			Avalue[i].resize(knapsack_size+1);
		}
	}
	/***************返回最大值**************/
	long max(long a, long b)
	{
		return a > b ? a : b;		
	}
/***************遍历填表*****************/
	void sack()
	{	
		for(int k = 0; k <= knapsack_size; k++)		
		{
			Avalue[0][k] = 0;  //初始化边界
			
		}
		for(int i = 1; i <= num_of_items; i++)  //填表
		{
			for(int x = 0; x <= knapsack_size; x++)
			{
				if(x-items[i].weight >= 0)
				{
					Avalue[i][x] = max(Avalue[i-1][x], Avalue[i-1][x-items[i].weight] + items[i].value);
				} else {
					Avalue[i][x] = Avalue[i-1][x];
				}
			}
		}
	}
/****************根据填好的表进行重构最优解的组成元素*************/
	void reconstruct()
	{
		int temp = Avalue[num_of_items][knapsack_size];
		int i = num_of_items;
		int x = knapsack_size;
		while( i > 0)
		{
//			temp = Avalue[i][x];
			if( (temp > Avalue[i-1][x]) )
			{
				cout << "(" << x << "," << i << ")" << temp <<  endl;
				set.push_back(i);
				x = x - items[i].weight;
				i = i-1;
			} else  {
				//set.push_back(i-1);
				i = i - 1;
			}
			temp = Avalue[i][x]; 
			cout << "(" << x << "," << i << ")" << temp  << endl;
		}
		
	}

/**********打印看输出*********/
	void print()
	{
		ofstream fout;
		fout.open("out1.txt");
		for(int j = knapsack_size; j >= 0; j--)
		{
			for(int i = 0; i <= num_of_items; i++)		
			{
				fout << "[w,i] [" << j << "," << i << "]" << Avalue[i][j] << " ;";
			}
			fout << endl;
		}
	}
	
	void write()
	{
		ofstream fout;
		fout.open("out2.txt");
		for(int i = 0; i < set.size(); i++)
		{
			fout << set[i] << " ";	
		}
		cout << endl;
	}
	
public:
	int knapsack_size;   //背包容量
	int num_of_items;    //物体数目
	vector<item> items;  //物体集合
	vector< vector<long> > Avalue; //表的数据
	vector<int> set; //存储最优解
};

int main()
{
	clock_t start, end;
	start = clock();
	Knapsack knapsack;
	knapsack.sack();
	knapsack.print();
	knapsack.reconstruct();
	knapsack.print();
	knapsack.write();
	end = clock();
	cout << "running time:" << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;
	return 0;
}
