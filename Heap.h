#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<string>
#include<vector>
#include<assert.h>
#include<algorithm>
using namespace std;



template<class T>
struct LessThan                                  //使用仿函数  
{
	T left;
	T right;
	bool operator()(const T& left, const T& right)
	{
		return left < right;
	}
};

template<class T>
struct MoreThan
{
	T left;
	T right;                                        //這里的成员变量不写也没什么  类里面也可以只有成员函数完成一定的操作，不一定要有成员变量
	bool operator()(const T& left, const T& right)
	{
		return left > right;
	}
};

//template<class T>
//struct WeitLessThan
//{
//	bool operator()(const )
//
//};

 
template<class T, class Compare = LessThan<T>>   //默认小堆
class Heap
{
private:
	vector<T> _a;                 //堆的本质实际上是对vector的操作
public:
	Heap()     //无参构造函数 实例化对象vector自己调用缺省的构造函数
	{
	}
	Heap(T* a, size_t n)    //构造函数，把数组的值一一给vector
	{
		_a.reserve(n);
		for (size_t i = 0; i < n; i++)
		{
			_a.push_back(a[i]);
		}

		//    建堆    
		for (int i = (_a.size() - 2) / 2; i >= 0; i--)     
		{
			AdjustDown(i);   
		}

	}

	void AdjustDown(int  root)          //[*]  
	{
		Compare con;                  //注意要实例化   刚开始忘掉了 
		int parent = root;
		int child = parent * 2 + 1;     
		while (child<_a.size())                  
		{
			
			if (child != _a.size() - 1 && con(_a[child + 1], _a[child]))
			{
				child++;
			}

			if (con(_a[child], _a[parent]))
			{
				swap(_a[parent], _a[child]);
				parent = child;
				child = child * 2 + 1;
			}
			else
			{
				break;
			}
		}

	}

	void AdjustUp(int root)   //向上调整，针对于尾部插入一个数据，调整为大堆 (针对于尾部插入)
	{
		Compare con;
		int child = root;
		int parent = (child - 1) / 2;                //不论是左子树还是右子树，（子树-1）/2就是等于跟结点 我一开始以为（左子树-1）/2的值和左子树/2是一样的 都是父结点，以为对于右子树也是一样，但是发现如果（右子树/2）答案不是跟结点
		while (parent >= 0)
		{
			if (con(_a[child],_a[parent]))
			{
				swap(_a[child], _a[parent]);          //這里的swap会存在问题吗？
				child = parent;
				parent = (parent - 1) / 2;
			}
			else
				break;
		}
	}


	void Push(const T& d)
	{
		_a.push_back(d);
		AdjustUp(_a.size() - 1);              //把最后一个结点进行向上调整

	}

	void Pop()         //删除最顶上结点
	{
		swap(_a[_a.size() - 1], _a[0]);
		_a.pop_back();
		AdjustDown(0);
	}

	T& Top()
	{
		return _a[0];
	}
	size_t Size()
	{
		return _a.size();
	}

};
