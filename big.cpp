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
 *			class Hash = hash<Key>, ：一元，以一个key类型对象为参数
 *			返回一个基于该对象的size_t类型的唯一值,类模板内部，使用其别名为hasher的成员类型
 *			class Pred = equal_to<Key>, 二元谓词，以两个Key类型的对象为参数，返回一个bool值
 *			如果第一个参数等价为第二个参数，该bool值为true,否则为false.默认值为std::equal_to
 *			class Alloc = allocator< pair<const Key, T> >
 *			容器内部用来管理内存分配及释放的内存分配器的类型
 * > class unordered_map; 
 *template<class Key> struct hash; 
 *
 */

class item
{
public:
	long value;
	int weight;
	item() : value(0), weight(0) {}
};

class Key
{
public:
	long weight;
	int index;
	Key(int _index, long _weight) : index(_index), weight(_weight) {}
};

struct KeyHash 
{
	size_t operator()(const Key& k) const
	{
		return std::hash<long>()(k.weight) ^
				(std::hash<int>()(k.index) << 1);
	}
};

struct KeyEqual
{
	bool operator()(const Key& lhs, const Key& rhs) const
	{
		return lhs.index == rhs.index && lhs.weight == rhs.weight;
	}
};

class Knapsack
{
public:
	Knapsack()
	{
		ifstream fin("1.txt");
		string line;
		stringstream stream;
		if(getline(fin, line))
		{
			stream.clear();
			stream << line;
			stream >> knapsack_size;
			stream >> num_of_items;
	//		cout << "num:" << num_of_items << endl;
		} 
		int cnt = 0;
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
		items.resize(num_of_items);
//		residual.resize(num_of_items);
	}
	void dynamic()
	{
		long max_value = make(num_of_items-1, knapsack_size);
//		Key k(num_of_items-1, knapsack_size);
//		map.insert(make_pair(k, max_value));
		cout << "max_value:" << max_value << endl;
	}
	

	long make(int i_item, long weight)
	{
		Key find_key(i_item, weight);
		unordered_map<Key, long, KeyHash, KeyEqual >::iterator got = map.find(find_key);
		if( got != map.end())
		{
			cout << "find()" << endl;
			return got->second;
		} 
		long value1 = 0;
		long value2 = 0;
		cout << "make()" << endl;
			
		if(i_item == 0)
		{
			if(weight >= items[i_item].weight)
			{
				Key temp_key(0, weight);
				map.insert(make_pair(temp_key, items[0].value));
				cout << items[0].value << endl;
				return items[i_item].value;
			} else {
				Key temp_key(0, weight);
				map.insert(make_pair(temp_key, 0));
				return 0;
			}
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
	}
	
	void reconstruct()
	{
		int i = num_of_items-1;
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
//			i--;
		}
		if(i == 0)
		{
			if(weight >= items[0].weight)
			{
				set.push_back(0);
			}
		}

	}
	

	void print()
	{
		ofstream fout1;
		fout1.open("out1.txt");
		for(int i = 0; i < set.size(); i++)
		{
			cout  << set[i]+1 << " " ;
		}
		cout << endl;

	/*	ofstream fout2;
		fout2.open("out2.txt");
		for(int j = 0; j < values.size(); j++)
		{
			cout << " " << values[j] << " " << endl;
		}*/
		
	}


public:
	vector<item> items;
	int num_of_items;
	long knapsack_size;
	unordered_map<Key, long, KeyHash, KeyEqual> map;
//	vector<long> residual_weight;
	vector<int> set;
};


int main()
{
	clock_t start, end;
	start = clock();
	Knapsack knapsack;
	knapsack.dynamic();
	knapsack.reconstruct();
	knapsack.print();
	end = clock();
	cout << "running time:" << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;
	return 0;
}



