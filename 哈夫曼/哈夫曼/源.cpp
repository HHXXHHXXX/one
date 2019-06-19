#include<iostream>
#include<fstream>

using namespace std;

#define  PATH       "C:\\Users\\25626\\Desktop\\���ݽṹ����\\������\\Pic.bmp"
#define  NPATH      "C:\\Users\\25626\\Desktop\\���ݽṹ����\\������\\Pic.bmp.huf"
#define  TEXTPATH   "C:\\Users\\25626\\Desktop\\���ݽṹ����\\������\\Pic1.bmp"

int huffman[511][4];   //huffmantree
char huffmancode[256][256];  //���huffman�ı���
char usehuffmancode[257];    //��fuffmancode������ʱ���м����

//���ڹ���haffumantreeʱ��¼��С��
typedef struct Min
{
	int tmp;
	int num;
}Min;





//���ߺ������Ƚ��������������Ǵ�С��������
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




//��huffman����ʼ��Ϊ0
void sethHuffman()
{
	int i;
	for (i = 0; i < 511; i++)
	{
		huffman[i][0] = 0;
		huffman[i][1] = -1;   //-1��ʾû����ڵ�
		huffman[i][2] = -1;   //-1��ʾû���ҽڵ�
		huffman[i][3] = -1;   //-1��ʾû�б�������0��ʾ��������
	}
}





//ͳ��ÿ���ֽڵĳ���Ƶ��
int gettheWord()
{
	fstream isf;
	unsigned char a;
	int n = 0;   //ͳ���ֽ���


	//���ļ�
	isf.open(PATH, ios::binary | ios::in);
	if (!isf)
	{
		cout << "erro!";
		return 0;
	}

	//ͳ��
	while (!isf.eof())
	{
		isf.read((char*)&a, sizeof(a));
		huffman[a][0]++;
		n++;
	}
	isf.close();


	//���Ȩ�غ��ֽ���
	cout << "�ֽ���Ϊ��" << n << endl;
	for (int i = 0; i < 256; i++)
		cout << i << ":  " << huffman[i][0] << endl;


	return n;
}



//����huffman��
void buildHuffmantree()
{
	int t = 0;  //t��ʾ�������µĽڵ�
	int i;
	Min min1, min2;   //min1�洢��С�ģ�min2�洢�ڶ�С��

	//��ʼ��min1,min2
	min1.num = -1;
	min2.num = -1;


	while (t + 256 != 511)
	{
		//�ҵ���û�и�ĸ�ڵ����С������
		for (i = 0; i < 256 + t; i++)
		{
			//��һ�θ�min1��ֵ
			if (min1.num == -1 && huffman[i][3] == -1)
			{
				min1.num = huffman[i][0];
				min1.tmp = i;
			}
			else
			{
				//��һ�θ�min2��ֵ
				if ((min2.num == -1 && huffman[i][3] == -1))
				{
					min2.num = huffman[i][0];
					min2.tmp = i;
					//����min1��min2��˳��
					cmpmin(min1, min2);
				}
				//һ���ԱȽ�
				else if (min2.num > huffman[i][0] && huffman[i][3] == -1)
				{
					min2.num = huffman[i][0];
					min2.tmp = i;
					//����min1��min2��˳��
					cmpmin(min1, min2);
				}
			}
		}


		//�����µĽڵ�
		huffman[256 + t][0] = min1.num + min2.num;
		huffman[256 + t][1] = min1.tmp;
		huffman[256 + t][2] = min2.tmp;
		//˵���ڵ��ѱ�����
		huffman[min1.tmp][3] = 0;
		huffman[min2.tmp][3] = 0;
		t++;

		//��min1.min2��ֵ��Ϊ��ֵ��������һ��Ѱ��
		min1.num = min2.num = -1;

	}


	//����鿴
	cout << "huffman��" << endl;
	for (i = 0; i < 511; i++)
	{
		cout << i << "   " << huffman[i][0] << "   " << huffman[i][1] << "   " << huffman[i][2] << "   " << huffman[i][3] << endl;
	}
}



//���huffman����
void getthepath(int n, int t)  //t��¼�Ѿ���ŵı���ĳ��ȣ�������ʱ���,n���ڱ�ʾ�ڲ�ѯ�Ľڵ�
{
	int i, j = 0;
	//����Ǹ��ڵ�����˳������huffmancode��
	if (huffman[n][1] == -1)
	{
		for (i = 1; i <= t; i++)
		{
			huffmancode[n][j] = usehuffmancode[i];
			j++;
		}
		huffmancode[n][j] = '\0';
	}
	else   //������Ǹ��ڵ����ȱ�������ٱ����ұ�
	{
		usehuffmancode[t + 1] = '0';
		getthepath(huffman[n][1], t + 1);
		usehuffmancode[t + 1] = '1';
		getthepath(huffman[n][2], t + 1);
	}


}




//ѹ���ļ�
void gettheSmallf(int num)   //n��ʾԴ�ļ����ֽ���
{
	fstream osf, isf;
	unsigned char a = 0, b = 0, c = 0;  //a��������b������,c�����м����
	int startx = 0, overx = 1, left = 8, tx, start;//startx��ʾ���Զ���huffmancode�±�
	int i, t = 0, tn = 0;


	//���ļ�
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


	//���ȷ���Դ�ļ����ֽ���
	osf.write((char*)&num, sizeof(num));



	//ѹ��
	while (!isf.eof())
	{
		if (overx)
		{
			isf.read((char*)&a, sizeof(a));
			tx = strlen(huffmancode[a]);
			overx = 0;  //��ʾ���ַ���,��ȥ�����ֻ��Ϊ�˱������
		}
		if (left == tx - startx)   //������ôճ�һ���ֽ�
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


			//�����ļ���
			osf.write((char*)&b, sizeof(b));
			t++;
			tn++;


			//���ñ�־
			b = 0;
			overx = 1;
			startx = 0;
			left = 8;
		}
		else
		{
			if (left > tx - startx) //����Ų���һ���ֽ�
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


				//���ñ�־
				overx = 1;
				left = left - tx + start;
				startx = 0;

				t++;
			}
			else if (left < tx - startx)  //��ʾ�Ų���
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

				//д��
				osf.write((char*)&b, sizeof(b));
				tn++;



				//���ñ�־
				b = 0;
				overx = 0;
				left = 8;
			}

		}

	}


	//�ж��Ƿ��ǷŲ������
	if (overx == 1 && left != 8)
	{
		osf.write((char*)&b, sizeof(b));
		tn++;
	}
	else if (overx == 0)  //�ж��ǲ��ǷŲ����������ʱtx���䣬overxû������
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
			else if (left < tx - startx)  //��ʾ�Ų���
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

				//д��
				osf.write((char*)&b, sizeof(b));
				tn++;


				//���ñ�־
				b = 0;
				left = 8;
			}
		}
	}

	//�ر��ļ�
	isf.close();
	osf.close();


	cout << "�ܷ����ֽ���:" << t << endl;
	cout << "ѹ����Ϊ��" << tn << endl;
}






//��ѹ��
void openthef()
{
	fstream osf, isf;
	unsigned char a = 0, b = 0, c = 0;  //a��������b������,c�����м����
	int overx = 1, left = 8, n = 510;
	int num, t = 0;   //num��ʾ��Ҫ�����ֽ���,t��ʾ�Ѿ����˵��ֽ���
	int i;
	unsigned char j;


	//���ļ�
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


	//��ȡ��Ҫ�����ֽ���
	isf.read((char*)&num, sizeof(num));
	cout << num << endl;


	//��ѹ���ļ�
	while (t != num)
	{
		if (overx)   //���Զ�ȡ��һ���ֽ�
		{
			isf.read((char*)&a, sizeof(a));
			overx = 0;
		}
		while (left != 0 && huffman[n][1] != -1)   //��ʾ������һ���ֽڻ����ҵ���һ��Ҷ�ӽڵ�
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
		if (huffman[n][1] == -1 && left == 0)  //��ʾ������һ���ֽ�ͬʱ�ҵ���һ��Ҷ�ӽڵ�
		{
			b = (unsigned char)n;
			osf.write((char*)&b, sizeof(b));
			t++;

			//���ñ�־
			n = 510;
			left = 8;
			overx = 1;
		}
		if (huffman[n][1] == -1 && left != 0)   //��ʾ�ҵ���һ��Ҷ�ӽڵ㵫��û�б�����
		{
			b = (unsigned char)n;
			osf.write((char*)&b, sizeof(b));
			t++;

			//���ñ�־
			n = 510;
			overx = 0;
		}
		if (huffman[n][1] != -1 && left == 0)    //��ʾû���ҵ�Ҷ�ӽڵ㵫�Ǳ�������
		{
			//���ñ�־
			overx = 1;
			left = 8;
		}
	}

	cout << t << endl;


	//�ر��ļ�
	isf.close();
	osf.close();
}






//������
int main()
{
	char a;
	int n;



	sethHuffman();
	n = gettheWord();
	buildHuffmantree();
	getthepath(510, 0);

	//���
	cout << "huffmancode" << endl;
	for (int i = 0; i < 256; i++)
		cout << i << ":" << huffmancode[i] << endl;

	gettheSmallf(n);
	openthef();



	cout << "end!" << endl;
	cin >> a;
	return 0;
}
