#define _CRT_SECURE_NO_WARNINGS 1

#include"Heap.h"


//注意：這个树要依照文件压缩项目进行升级
template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;
	T _data;                        //這里的data实际上是权重(weight)
	HuffmanTreeNode(const T& d)
		:_data(d)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
	{
	}

};
template<class T>
struct WeitLessThan
{
	bool operator()(/*const*/ HuffmanTreeNode<T>* _left, /*const*/ HuffmanTreeNode<T>* _right) /*const */
	{
		return _left->_data < _right->_data;
	}
};

template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
private:
	Node* _root;
public:
	HuffmanTree()  //无参构造
		:_root(NULL)
	{
	}
	HuffmanTree(T* a, size_t n ,T invalid)         //传上来一个数组   //在文件压缩项目里面加上T类型的invalid非法值
	{
		//template<class T>                                            //刚开始我写模板参数列表的出错了，说模板声明只能再全局命名空间或者类范围内。 后来想我写一个内置类，还需要什么模板 就是Node*类型的
		//struct WeitLessThan         //這个结构体一定要放在這里吗？ 其实也可以放在上面
		//{
		//	bool operator()(const Node* _left, const Node* _right) const
		//	{
		//		return _left->_data < _right->_data;
		//	}
		//};

		//利用小堆选出数组中两个最小的数
		//建堆
		Heap<Node*, WeitLessThan<T>> minHeap;
		for (size_t i = 0; i < n; i++)
		{
			if (a[i] != invalid) 
			{
				minHeap.Push(new Node(a[i]));
			}
		}
		//建huffman树
		while (minHeap.Size()>1)                  //注意這里循环的条件 为什么是>1
		{
			                                    //选出权值最小的两个结点
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();
			                                     //构造父结点然后三点相链
			Node* parent = new Node(left->_data + right->_data);
			parent->_left = left;
			left->_parent = parent;                //parent指针
			parent->_right = right;
			right->_parent = parent;               //链接好之后再放回去比较 // 注意：放回去比较没啥特别的，要知道是放回到数组里面，链接好的不会产生什么影响
			minHeap.Push(parent);
		}
		_root = minHeap.Top();                       //注意：为什么這里是min.Top();
	}

	Node* GetRoot()
	{
		return _root;
	}
};

//void test()
//{
//	int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//	HuffmanTree<int> h1(arr, sizeof(arr) / sizeof(arr[0]));
//
//}
//int main()
//{
//	test();
//	return 0;
//}