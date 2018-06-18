#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <stack>
#include <sstream>
using namespace std;
/*template <class Key,  ：主键的类型
 *			class T,	：被映射的值的类型
 *			class Hash = hash<Key>, ：一元谓词，以一个key类型对象为参数
 *			返回一个基于该对象的size_t类型的唯一值,类模板内部，使用其别名为hasher的成员类型
 *			class Pred = equal_to<Key>, 二元谓词，以两个Key类型的对象为参数，返回一个bool值
 *			如果第一个参数等价为第二个参数，该bool值为true,否则为false.默认值为std::equal_to
 *			class Alloc = allocator< pair<const Key, T> >
 *			容器内部用来管理内存分配及释放的内存分配器的类型
 * > class unordered_map; 
 *template<class Key> struct hash; 
 *
 */
/***********物体属性******/
class item
{
public:
	long value; //物体价值
	int weight; //物体重量
	item() : value(0), weight(0) {}
};
/***********包含物体数量以及背包容量的键值对象,映射的value为目前容量下所能获得的最优价值*************/
class Key
{
public:
	long weight;       //
	int number;
	Key(int _num, long _weight) :  number(_num), weight(_weight) {}
};

struct KeyHash   //自定义Hash函数
{
	size_t operator()(const Key& k) const
	{
		return std::hash<long>()(k.weight) ^
				(std::hash<int>()(k.number) << 1);
	}
};

struct KeyEqual  //自定义操作符号
{
	bool operator()(const Key& lhs, const Key& rhs) const
	{
		return lhs.number == rhs.number && lhs.weight == rhs.weight;
	}
};

class Knapsack
{
public:
/**************构造函数读取数据*************/
	Knapsack()
	{
		ifstream fin("knapsack_big.txt");
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
			long _weight;
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
		pre.resize(knapsack_size+1);
		current.resize(knapsack_size+1);
		items.resize(num_of_items+1);
	}
	void dynamic() //递归动态规划
	{
		long max_value = make(num_of_items, knapsack_size);
		cout << "max_value:" << max_value << endl;
	}
	
	long max(long a, long b)
	{
		return a > b ? a : b;
	}


	long make(int i_item, long weight)
	{

		for(int i = 0; i <= knapsack_size; i++)
		{
			pre[i] = 0;
			current[i] = 0;
		}
		
		for(int i = 1; i <= num_of_items; i++)
		{
			for(int j = 0; j <= knapsack_size; j++)
			{
				if( (j - items[i].weight) >= 0)
				{
					current[j] = max(pre[j], pre[j-items[i].weight]+items[i].value);
				} else {
					current[j] =  pre[j];
				}
			}
			pre.insert(pre.begin(), current.begin(), current.end());
		}
		return current[knapsack_size];
	}

	void constru()
	{
		for(int i = 1; i <= num_of_items; i++)
		{
			for(int j = knapsack_size; j>=0; j--)
			{
				if(current[j] > pre[j])
				{
					set.push_back(i);
				} else {
					i--;
				}
			}
		}
	}

	void write()
	{
		ofstream fout2;
		fout2.open("out2.txt");
		for(int j = knapsack_size; j >= 0; j--)
		{
			fout2 << "(" << j << "," << 10 << "):" << current[j] << endl;
		}
	
	}

	
/*
	long make(int i_item, long weight)
	{
		Key find_key(i_item, weight);
		unordered_map<Key, long, KeyHash, KeyEqual >::iterator got = map.find(find_key);
		if( got != map.end())
		{
			return got->second;
		} 
		long value1 = 0;
		long value2 = 0;
		if(i_item == 0)  //0件物品的情况
		{

			Key temp_key(0, weight);
			map.insert(make_pair(temp_key, 0));
			return 0;
		}
		value1 = make(i_item-1, weight);
		
		if(weight >= items[i_item].weight)
		{
			
			value2 = make(i_item-1, weight-items[i_item].weight) + items[i_item].value;
			if(value2 > value1)
			{
				Key temp_key(i_item, weight);
				map.insert(make_pair(temp_key, value2));
			} else {
				Key kk(i_item, weight);
				map.insert(make_pair(kk, value1));
				cout << value1 << endl;
			}
			
		} else {
			Key temp_key(i_item, weight);
			map.insert(make_pair(temp_key, value1));
		}
		return value1 > value2 ? value1 : value2;
	}*/
/**************根据hash表存储的值找到最优解集合****************/
	void reconstruct()
	{
		int i = num_of_items;
		int weight = knapsack_size;
		while(i > 0)
		{
			Key k1(i, weight);
			Key k2(i-1, weight);
			unordered_map<Key, long, KeyHash, KeyEqual>::iterator got1 = map.find(k1);
			unordered_map<Key, long, KeyHash, KeyEqual>::iterator got2 = map.find(k2);
			
			if(got1 != map.end() && got2 != map.end())
			{
				if( (got1->second > got2->second) && (weight-items[i].weight >=0) )
				{
					set.push_back(i);
					weight -= items[i].weight;
					i--;
				} else {
					i--;
				}
			}
		}
	}
	
/********打印看结果*********/
	void print()
	{
		ofstream fout1;
		fout1.open("out1.txt");
		for(int i = 0; i < set.size(); i++)
		{
			fout1  << set[i] << " " ;
		}
		fout1 << endl;

		
	}


public:
	vector<long> pre;
	vector<long> current;
	vector<item> items; 
	int num_of_items;    //物品个数
	long knapsack_size;  //背包容量
	unordered_map<Key, long, KeyHash, KeyEqual> map;  //自定义哈希键值
	vector<int> set; //存储最优解
};


int main()
{
	clock_t start, end;
	start = clock();
	Knapsack knapsack;
	knapsack.dynamic();
//	knapsack.reconstruct();
//	knapsack.print();
	knapsack.write();
	end = clock();
	cout << "running time:" << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;
	return 0;
}



