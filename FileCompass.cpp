#define _CRT_SECURE_NO_WARNINGS 1

#include"HuffmanTree.h"
 
typedef  long long LongType;
struct CharInfo               //這里可以形象的叫這结构体为字母包
{
	char _ch;
	LongType _Count;
	string  _HUffmanCode;
	//這里不写构造，因为赫夫曼编码刚开始的时候不适合生成  调用默认的构造函数

	bool operator !=(const CharInfo& ch)
	{  
		return this->_Count != ch._Count;
	}

	bool operator < (const CharInfo& ch)
	{
		return this->_Count < ch._Count;
	}

	CharInfo operator+(const CharInfo& ch)
	{
		CharInfo tmpInfo;
		tmpInfo._Count = this->_Count + ch._Count;
		return tmpInfo;
	}
	
};
class FileCompass
{
public:
	typedef HuffmanTreeNode<CharInfo> CharNode;
private:
	CharInfo Info[256];
public:
	FileCompass() //构造这256个字母的结构体   初始化
	{
		for (size_t i = 0; i < 256; i++)
		{
			Info[i]._ch = i;
			Info[i]._Count = 0;
		}
	}

	struct ConfigurationInformation                 //配置信息
	{
		char _ch;
		LongType _Count;
	};

	void Compass(const char* filename)//   压缩文件
	{
		assert(filename);
		FILE* fout = fopen(filename, "rb");        //   二进制读取
		assert(fout);

		//统计字符出现的次数
		size_t chcount = 0;
		char ch = fgetc(fout);                 //fgetc的用法
		while (!feof(fout))                      //EOF：文件结束标志    
		{
			Info[(unsigned char)ch]._Count++;   
			chcount++;
			ch = fgetc(fout);
		}
		cout << "原文件的字符总数：" << chcount << endl;

		                                                   //搭建出现字母的赫夫曼树，将文件中出现的字母进行建堆，没有出现就不需要建堆了 所以需要用的invalid
		CharInfo _invalidInfoNode;
		_invalidInfoNode._Count = 0;                          //出现次数为0的字母包
		HuffmanTree<CharInfo> _h(Info, 256,_invalidInfoNode);

		string code;
		GetHuffmanCodeII(_h.GetRoot(),code);


		//写配置信息（二进制方式）然后再压缩
	    //
		string compassfile = filename;                   //這里存在疑问   单参数构造函数支持强转
		compassfile = compassfile + ".huffman";
		FILE* fIn = fopen(compassfile.c_str(), "wb");      //注意這里的c_str().....    还有這里就会创建一个新的文件   //wb错误
		assert(fIn);

		//写入二进制信息
		ConfigurationInformation cinfo;
		for (size_t i = 0; i < 256; i++)
		{
			if (Info[i]._Count)
			{
				cinfo._ch = Info[i]._ch;
				cinfo._Count = Info[i]._Count;
				size_t size=fwrite(&cinfo, sizeof(ConfigurationInformation), 1, fIn);            //写入
				//cout << size << endl;
			}
		}
		cinfo._Count = 0;                                                        //這里相当于写一个结束标志方便后面解压的时候配合
		size_t size=fwrite(&cinfo, sizeof(ConfigurationInformation), 1, fIn);   //返回值是：size*count
		assert(size = sizeof(ConfigurationInformation));
		//
		char value = 0;
		int count = 0;                 //要注意這里的count的刚开始的值和最后结束的值
		int compasschcount = 0;
		fseek(fout, 0, SEEK_SET);     //fseek函数的返回值为整形，不是一个文件指针  fout指针指向文件首字母
		char ch1 = fgetc(fout);       //fgetc操作函数
		while (!feof(fout))   
		{
			string& code = Info[(unsigned char)ch1]._HUffmanCode;                 //使用unsigned char 强转的目的是为了什么？
			for (size_t i = 0; i < code.size(); i++)
			{
				value <<= 1;                      //注意移位并不会改变变量本身的值
				if (code[i] == '1')                //string支持了[]的重载
				{
					value |= 1;
				}
				else
				{
					value |= 0;
				}
				count++;

				if (count == 8)                   //满8位就写到压缩文件里面去  然后重新给8位0
				{
					fputc(value, fIn);            //fputc的使用      //文本方式写入？可以吗？打开的时候是用二进制打开的
					value = 0;
					count = 0;
				}
			}
			ch1 = fgetc(fout);
			compasschcount++;
	   }

		if (count != 0)                 //未满八位的位处理
		{
			value <<= (8 - count);
			fputc(value, fIn);                          
		}
		fclose(fIn);
		fclose(fout);
		cout << "压缩了的字符个数：" << compasschcount << endl;
	}
	//获取每一个字符的赫夫曼编码（递归，从底下往上跑）[*]
	//void GetHuffmanCode(CharNode* root)
	//{
	//	string _code;
	//	if (root == NULL)                               //返回条件
	//		return;
	//	if (root->_left == NULL&&root->_right == NULL)
	//	{
	//		CharNode* cur = root;
	//		CharNode* parent = cur->_parent;
	//		while (parent != NULL)
	//		{
	//		    if (cur == parent->_left)
	//			{
	//				_code.push_back('0');
	//			}
	//			if (cur == parent->_right)
	//			{
	//				_code.push_back('1');
	//			}
	//			cur = parent;
	//			parent = cur->_parent;
	//		}
	//		Info[(unsigned char)root->_data._ch]._HUffmanCode = _code;    //找到结点之后直接生成编码放进去
	//		reverse(_code.begin(), _code.end());
	//		return;
	//	}
	//	GetHuffmanCode(root->_left);
	//	GetHuffmanCode(root->_right);
	//}
	
	//第二种方法：  
	void GetHuffmanCodeII(CharNode* root, string code)   //从高处往下递归递归下去，遇到叶子结点就把编码放到字母包的赫夫曼编码里面去。
	{
		if (root == NULL)
			return;
		if (root->_left == NULL&&root->_right == NULL)
		{
			Info[(unsigned char)root->_data._ch]._HUffmanCode = code;     //注意這里不需要反转
			return;
		}
		GetHuffmanCodeII(root->_left, code + '0');     //注意：string是支持+的
		GetHuffmanCodeII(root->_right, code + '1');
	}



	//解压
	void UnCompass(const char* filename)
	{
		assert(filename);
		string uncompassfile = filename;
		size_t pos = uncompassfile.rfind('.');
		uncompassfile = uncompassfile.substr(0, pos);    //substr的用法
		//uncompassfile = uncompassfile + ".uncomphuffman";                //如果這样的话就会覆盖原文件进行写入     

		FILE* fin = fopen(uncompassfile.c_str(), "wb"); 
		assert(fin);

		FILE* fout = fopen(filename, "rb");        //压缩文件二进制写，则解压的时候使用二进制方式读
		assert(fout);


		//读取配置信息然后传件原文件里面相同的Info结构体最后建树
		while (1)
		{
			ConfigurationInformation cinfo;
			size_t size = fread(&cinfo, sizeof(ConfigurationInformation), 1, fout);          //从fout读到cinfo结构体里面
			assert(size);
			if (cinfo._Count)
			{
				Info[(unsigned char)cinfo._ch]._ch = cinfo._ch;
				Info[(unsigned char)cinfo._ch]._Count = cinfo._Count;
			}
			else
			{
				break;            //循环终止
			}
		}


		//重建huffman树       注意這里[*]
		CharInfo _invalidInfoNode;
		_invalidInfoNode._Count = 0;
		HuffmanTree<CharInfo> _h(Info, 256, _invalidInfoNode);

		CharNode* root = _h.GetRoot();
		size_t charCount = root->_data._Count;          //赫夫曼树根结点的权值的含义是？   可以记录原文件里面一共有多少字符
		

		//按位读取进行
		char value = fgetc(fout);
		CharNode* cur = root;
		size_t cCount = 0;
		while (!feof(fout))                    //如果這里是EOF  EOF的值相当于-1
		{
			for (int pos = 7; pos >= 0; pos--)         //别用size_t
			{
				if (value&(1 << pos))      //1       //位操作
				{
					cur = cur->_right;
				}
				else                       //0
				{
					cur = cur->_left;
				}
				if (cur->_left == NULL&&cur->_right == NULL)
				{
					fputc(cur->_data._ch, fin);
					cCount++;
					cur = root;
					if (cCount == charCount)               //该解压到多少字符就该停止了
					{   
						break;
					}
				}
			}
			value = fgetc(fout);      //为什么這里再第9个字母解压完成后是-1  ?
		}
		fclose(fin);
		fclose(fout);
		cout << "解压了的字符个数：" << cCount << endl;
	}
};



//********压缩测试用例*************
//压缩照片
void testPhotosCompass()
{
	//FileCompass fcps;
	//fcps.Compass("_MG_9344.JPG");
	
	FileCompass fcps2;
	fcps2.UnCompass("_MG_9344.JPG.huffman");

}

//压缩文档
void testDocumentCompass()
{
	//FileCompass fcps;
	//fcps.Compass("TestInput.txt");

	FileCompass fcps2;
	fcps2.UnCompass("TestInput.txt.huffman");     //配置信息已经写入

}

//压缩视频
void testVideoCompass()
{
	FileCompass fcps;
	fcps.Compass("8-29（String的各种操作）.wmv");

	FileCompass fcps2;
	fcps2.UnCompass("8-29（String的各种操作）.wmv.huffman");

}

int main()
{
	//testVideoCompass();
	//testDocumentCompass();
	testPhotosCompass();
	return 0;
}
