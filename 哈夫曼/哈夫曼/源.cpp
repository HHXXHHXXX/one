#include<iostream>
#include<fstream>

using namespace std;

#define  PATH       "C:\\Users\\25626\\Desktop\\数据结构试验\\哈夫曼\\Pic.bmp"
#define  NPATH      "C:\\Users\\25626\\Desktop\\数据结构试验\\哈夫曼\\Pic.bmp.huf"
#define  TEXTPATH   "C:\\Users\\25626\\Desktop\\数据结构试验\\哈夫曼\\Pic1.bmp"

int huffman[511][4];   //huffmantree
char huffmancode[256][256];  //存放huffman的编码
char usehuffmancode[257];    //将fuffmancode正序存放时的中间变量

//用于构建haffumantree时记录最小数
typedef struct Min
{
	int tmp;
	int num;
}Min;





//工具函数，比较两个数，让他们从小到大排列
void cmpmin(Min &min1, Min &min2)
{
	Min min;
	if (min1.num <= min2.num)   return;
	else
	{
		min = min1;
		min1 = min2;
		min2 = min;
	}
}




//将huffman都初始化为0
void sethHuffman()
{
	int i;
	for (i = 0; i < 511; i++)
	{
		huffman[i][0] = 0;
		huffman[i][1] = -1;   //-1表示没有左节点
		huffman[i][2] = -1;   //-1表示没有右节点
		huffman[i][3] = -1;   //-1表示没有被遍历，0表示被遍历了
	}
}





//统计每个字节的出现频率
int gettheWord()
{
	fstream isf;
	unsigned char a;
	int n = 0;   //统计字节数


	//打开文件
	isf.open(PATH, ios::binary | ios::in);
	if (!isf)
	{
		cout << "erro!";
		return 0;
	}

	//统计
	while (!isf.eof())
	{
		isf.read((char*)&a, sizeof(a));
		huffman[a][0]++;
		n++;
	}
	isf.close();


	//输出权重和字节数
	cout << "字节数为：" << n << endl;
	for (int i = 0; i < 256; i++)
		cout << i << ":  " << huffman[i][0] << endl;


	return n;
}



//构建huffman树
void buildHuffmantree()
{
	int t = 0;  //t表示建立的新的节点
	int i;
	Min min1, min2;   //min1存储最小的，min2存储第二小的

	//初始化min1,min2
	min1.num = -1;
	min2.num = -1;


	while (t + 256 != 511)
	{
		//找到还没有父母节点的最小的两个
		for (i = 0; i < 256 + t; i++)
		{
			//第一次给min1赋值
			if (min1.num == -1 && huffman[i][3] == -1)
			{
				min1.num = huffman[i][0];
				min1.tmp = i;
			}
			else
			{
				//第一次给min2赋值
				if ((min2.num == -1 && huffman[i][3] == -1))
				{
					min2.num = huffman[i][0];
					min2.tmp = i;
					//排列min1，min2的顺序
					cmpmin(min1, min2);
				}
				//一般性比较
				else if (min2.num > huffman[i][0] && huffman[i][3] == -1)
				{
					min2.num = huffman[i][0];
					min2.tmp = i;
					//排列min1，min2的顺序
					cmpmin(min1, min2);
				}
			}
		}


		//建立新的节点
		huffman[256 + t][0] = min1.num + min2.num;
		huffman[256 + t][1] = min1.tmp;
		huffman[256 + t][2] = min2.tmp;
		//说明节点已被遍历
		huffman[min1.tmp][3] = 0;
		huffman[min2.tmp][3] = 0;
		t++;

		//将min1.min2的值置为初值，用于下一次寻找
		min1.num = min2.num = -1;

	}


	//输出查看
	cout << "huffman表" << endl;
	for (i = 0; i < 511; i++)
	{
		cout << i << "   " << huffman[i][0] << "   " << huffman[i][1] << "   " << huffman[i][2] << "   " << huffman[i][3] << endl;
	}
}



//获得huffman编码
void getthepath(int n, int t)  //t记录已经存放的编码的长度，用于临时存放,n用于表示在查询的节点
{
	int i, j = 0;
	//如果是根节点则将其顺序存放在huffmancode中
	if (huffman[n][1] == -1)
	{
		for (i = 1; i <= t; i++)
		{
			huffmancode[n][j] = usehuffmancode[i];
			j++;
		}
		huffmancode[n][j] = '\0';
	}
	else   //如果不是根节点则先遍历左边再遍历右边
	{
		usehuffmancode[t + 1] = '0';
		getthepath(huffman[n][1], t + 1);
		usehuffmancode[t + 1] = '1';
		getthepath(huffman[n][2], t + 1);
	}


}




//压缩文件
void gettheSmallf(int num)   //n表示源文件的字节数
{
	fstream osf, isf;
	unsigned char a = 0, b = 0, c = 0;  //a用来读，b用来存,c用来中间计算
	int startx = 0, overx = 1, left = 8, tx, start;//startx表示可以读的huffmancode下标
	int i, t = 0, tn = 0;


	//打开文件
	osf.open(NPATH, ios::binary | ios::trunc | ios::out);
	if (!osf)
	{
		cout << "error!";
		return;
	}
	isf.open(PATH, ios::binary | ios::in);
	if (!isf)
	{
		cout << "erro!";
		return;
	}


	//首先放入源文件的字节数
	osf.write((char*)&num, sizeof(num));



	//压缩
	while (!isf.eof())
	{
		if (overx)
		{
			isf.read((char*)&a, sizeof(a));
			tx = strlen(huffmancode[a]);
			overx = 0;  //表示有字符串,可去，添加只是为了保险起见
		}
		if (left == tx - startx)   //如果正好凑成一个字节
		{
			c = 0;
			for (i = 1; i <= left; i++)
			{
				if (huffmancode[a][startx] == '0')
				{
					c = c * 2;
				}
				else if (huffmancode[a][startx] == '1')
				{
					c = c * 2 + 1;
				}
				startx++;
			}
			b = b + c;


			//存入文件中
			osf.write((char*)&b, sizeof(b));
			t++;
			tn++;


			//设置标志
			b = 0;
			overx = 1;
			startx = 0;
			left = 8;
		}
		else
		{
			if (left > tx - startx) //如果放不完一个字节
			{
				start = startx;
				c = 0;
				for (i = 1; i <= tx - start; i++)
				{
					if (huffmancode[a][startx] == '0')
					{
						c = c * 2;
					}
					else if (huffmancode[a][startx] == '1')
					{
						c = c * 2 + 1;
					}
					startx++;
				}
				for (i = 1; i <= left - tx + start; i++)
					c = c * 2;

				b = b + c;


				//设置标志
				overx = 1;
				left = left - tx + start;
				startx = 0;

				t++;
			}
			else if (left < tx - startx)  //表示放不下
			{
				c = 0;
				for (i = 1; i <= left; i++)
				{
					if (huffmancode[a][startx] == '0')
					{
						c = c * 2;
					}
					else if (huffmancode[a][startx] == '1')
					{
						c = c * 2 + 1;
					}
					startx++;
				}

				b = b + c;

				//写入
				osf.write((char*)&b, sizeof(b));
				tn++;



				//设置标志
				b = 0;
				overx = 0;
				left = 8;
			}

		}

	}


	//判断是否是放不完情况
	if (overx == 1 && left != 8)
	{
		osf.write((char*)&b, sizeof(b));
		tn++;
	}
	else if (overx == 0)  //判断是不是放不下情况，此时tx不变，overx没有用了
	{
		while (1)
		{
			if (left >= tx - startx)
			{
				start = startx;
				c = 0;
				for (i = 1; i <= tx - start; i++)
				{
					if (huffmancode[a][startx] == '0')
					{
						c = c * 2;
					}
					else if (huffmancode[a][startx] == '1')
					{
						c = c * 2 + 1;
					}
					startx++;
				}
				for (i = 1; i <= left - tx + start; i++)
					c = c * 2;

				b = b + c;


				osf.write((char*)&b, sizeof(b));
				t++;
				tn++;
				break;
			}
			else if (left < tx - startx)  //表示放不下
			{
				c = 0;
				for (i = 1; i <= left; i++)
				{
					if (huffmancode[a][startx] == '0')
					{
						c = c * 2;
					}
					else if (huffmancode[a][startx] == '1')
					{
						c = c * 2 + 1;
					}
					startx++;
				}

				b = b + c;

				//写入
				osf.write((char*)&b, sizeof(b));
				tn++;


				//设置标志
				b = 0;
				left = 8;
			}
		}
	}

	//关闭文件
	isf.close();
	osf.close();


	cout << "总放入字节数:" << t << endl;
	cout << "压缩后为：" << tn << endl;
}






//解压缩
void openthef()
{
	fstream osf, isf;
	unsigned char a = 0, b = 0, c = 0;  //a用来读，b用来存,c用来中间计算
	int overx = 1, left = 8, n = 510;
	int num, t = 0;   //num表示需要读的字节数,t表示已经读了的字节数
	int i;
	unsigned char j;


	//打开文件
	osf.open(TEXTPATH, ios::binary | ios::trunc | ios::out);
	if (!osf)
	{
		cout << "error!";
		return;
	}
	isf.open(NPATH, ios::binary | ios::in);
	if (!isf)
	{
		cout << "erro!";
		return;
	}


	//读取需要读的字节数
	isf.read((char*)&num, sizeof(num));
	cout << num << endl;


	//解压缩文件
	while (t != num)
	{
		if (overx)   //可以读取下一个字节
		{
			isf.read((char*)&a, sizeof(a));
			overx = 0;
		}
		while (left != 0 && huffman[n][1] != -1)   //表示遍历完一个字节或者找到了一个叶子节点
		{
			j = 1;
			if (left != 1)
			{
				for (i = 1; i < left; i++)
					j = j * 2;
			}
			c = a / j;
			a = a - c * j;
			if (c == 0)
				n = huffman[n][1];
			if (c == 1)
				n = huffman[n][2];
			left--;
		}
		if (huffman[n][1] == -1 && left == 0)  //表示遍历完一个字节同时找到了一个叶子节点
		{
			b = (unsigned char)n;
			osf.write((char*)&b, sizeof(b));
			t++;

			//设置标志
			n = 510;
			left = 8;
			overx = 1;
		}
		if (huffman[n][1] == -1 && left != 0)   //表示找到了一个叶子节点但是没有遍历完
		{
			b = (unsigned char)n;
			osf.write((char*)&b, sizeof(b));
			t++;

			//设置标志
			n = 510;
			overx = 0;
		}
		if (huffman[n][1] != -1 && left == 0)    //表示没有找到叶子节点但是遍历完了
		{
			//设置标志
			overx = 1;
			left = 8;
		}
	}

	cout << t << endl;


	//关闭文件
	isf.close();
	osf.close();
}






//主函数
int main()
{
	char a;
	int n;



	sethHuffman();
	n = gettheWord();
	buildHuffmantree();
	getthepath(510, 0);

	//输出
	cout << "huffmancode" << endl;
	for (int i = 0; i < 256; i++)
		cout << i << ":" << huffmancode[i] << endl;

	gettheSmallf(n);
	openthef();



	cout << "end!" << endl;
	cin >> a;
	return 0;
}
