#include<iostream>
#include<fstream>
using namespace std;

char pri_rela[20][20];	//算符优先关系 data
char sym_stack[100];	//符号栈 s
char terminal[20];		//终结符集 label
char input_str[100];		//输入的符号串 input
char str_analysis[20][10];		//用于辅助输入串的分析 string

int gram_num;		//文法规则数 r
int _gram_num_;	//转化后文法规则个数 r1
char grammar[10][30];		//文法规则 st
char content[20][20];
char firstvt[10][10];		//文法非终结符FIRSTVT集 first
char lastvt[10][10];		//文法非终结符LASTVT集 last
bool first_flag[10] = { 0 };		//标志第i个非终结符的FIRSTVT集是否已经求出
bool last_flag[10] = { 0 };		//标志第i个非终结符的LASTVT集是否已经求出

/*判断是否是终极符*/
bool is_terminal(char c)
{
	// 遍历终极符集合
	for (int i = 0; terminal[i] != '\0'; i++)
	{
		if (c == terminal[i])
			return true;
	}
	return false;
}

/*获取终极符索引序号*/
int index(char c)
{
	// 遍历终极符集合
	for (int i = 0; terminal[i] != '\0'; i++)
	{
		if (c == terminal[i])
			return i;
	}
	return -1;
}

/*打印符号栈*/
void print_stack(int j, int k, char*s)
{
	// 起始位置索引为j，结束位置索引为k
	// 打印s[j]到s[k]的内容
	int n = 0;
	for (int i = j; i <= k; i++)
	{
		cout << s[i];
		n++;
	}
	for (; n < 15; n++)
		cout << ' ';
}

/*生成FIRSTVT集合*/
void CreateFirstVT(char c)
{
	int i, j, k;
	int m, n;
	// 找到非终极符c位于首部的文法
	for (i = 0; i < gram_num; i++)
	{
		if (grammar[i][0] == c)
			break;
	}
	// 如果当前非终极符的FIRSTVT还没有建立则继续
	// 否则不重复操作
	if (first_flag[i] == false)
	{
		// n为FIRSTVT集合中，当前填入的位置
		n = firstvt[i][0] + 1;
		// m为处理非终极符对应文法语句时用到的索引
		m = 0;
		do
		{
			// m=2代表即将出现第一个产生式
			// 遇到 | 符号说明即将出现下一个产生式
			if (m == 2 || grammar[i][m] == '|')
			{
				// m+1是文法语句中某个产生式的第一个符号索引
				// 产生式的第一个符号位终极符，就可以将它加入FIRSTVT
				if (is_terminal(grammar[i][m + 1]))
				{
					firstvt[i][n] = grammar[i][m + 1];
					n++;
				}
				// 产生式的第一个符号不是终极符，则进行后续判断
				else
				{
					// 再向后一个符号如果是终极符，
					// 则可将其加入FIRSTVT
					if (is_terminal(grammar[i][m + 2]))
					{
						firstvt[i][n] = grammar[i][m + 2];
						n++;
					}
					// 如果第一个符号是一个与c不同的非终极符
					// 则需要求它的FIRSTVT，并将其并入当前所求的FIRSTVT
					if (grammar[i][m + 1] != c)
					{
						// 递归求第一个不相同的非终极符的FIRSTVT
						CreateFirstVT(grammar[i][m + 1]);
						// 为了符合集合互异性
						// 查重，防止重复元素加入FIRSTVT
						for (j = 0; j < gram_num; j++)
						{
							if (grammar[j][0] == grammar[i][m + 1])
								break;
						}
						// 将递归求得的FIRSTVT并入当前所求的FIRSTVT
						for (k = 0; k < firstvt[j][0]; k++)
						{
							int t = 0;
							// 找到插入位置的索引
							for (t = 0; t < n; t++)
							{
								if (firstvt[i][t] == firstvt[j][k + 1])
									break;
							}
							if (t == n)
							{
								firstvt[i][n] = firstvt[j][k + 1];
								n++;
							}
						}
					}
				}
			}
			m++;
		} while (grammar[i][m] != '\0');

		firstvt[i][n] = '\0';
		// 确定集合元素个数
		firstvt[i][0] = --n;
		// 当前非终极符c的FIRSTVT已求出，不重复求
		first_flag[i] = true;
	}
}

/*生成LASTVT集合*/
void CreateLastVT(char c)
{
	int i, j, k;
	int m, n;
	for (i = 0; i < gram_num; i++)
	{
		if (grammar[i][0] == c)
			break;
	}
	if (last_flag[i] == false)
	{
		n = lastvt[i][0] + 1;
		m = 0;
		do
		{
			if (grammar[i][m + 1] == '\0' || grammar[i][m + 1] == '|')
			{
				if (is_terminal(grammar[i][m]))
				{
					lastvt[i][n] = grammar[i][m];
					n++;
				}
				else
				{
					if (is_terminal(grammar[i][m - 1]))
					{
						lastvt[i][n] = grammar[i][m - 1];
						n++;
					}
					if (grammar[i][m] != c)
					{
						CreateLastVT(grammar[i][m]);
						for (j = 0; j < gram_num; j++)
						{
							if (grammar[j][0] == grammar[i][m])
								break;
						}
						for (k = 0; k < lastvt[j][0]; k++)
						{
							int t;
							for (t = 0; t < n; t++)
							{
								if (lastvt[i][t] == lastvt[j][k + 1])
									break;
							}
							if (t == n)
							{
								lastvt[i][n] = lastvt[j][k + 1];
								n++;
							}
						}
					}
				}
			}
			m++;
		} while (grammar[i][m] != '\0');
		lastvt[i][n] = '\0';
		lastvt[i][0] = --n;
		last_flag[i] = true;
	}
}

/*生成优先关系表*/
void CreatePriorityTable()
{
	
	int x = 0, y = 0;
	int m, n, k;
	
	for (int i = 0; i < gram_num; i++)
	{
		/*求每个规则的第一个符号的FIRSTVT*/
		CreateFirstVT(grammar[i][0]);
		/*求每个规则的第一个符号的FIRSTVT*/
		CreateLastVT(grammar[i][0]);
	}
	for (int i = 0; i < gram_num; i++)
	{
		content[x][y] = grammar[i][0];
		y++;
		for (int j = 1; grammar[i][j] != '\0'; j++)
		{
			if (grammar[i][j] == '|')
			{
				content[x][y] = '\0';
				x++;
				y = 0;
				content[x][y] = grammar[i][0];
				y++;
				content[x][y++] = '-';
				content[x][y++] = '>';
			}
			else
			{
				content[x][y] = grammar[i][j];
				y++;
			}
		}
		content[x][y] = '\0';
		x++;
		y = 0;
	}
	_gram_num_ = x;
	cout << "文法转化后为：" << endl;
	for (int i = 0; i < _gram_num_; i++)
		cout << content[i] << endl;
	cout << endl;

	for (int i = 0; i < _gram_num_; i++)
	{
		int l = 0;
		int j = 0;
		str_analysis[i][0] = content[i][0];
		for ( j = 3, l = 1; content[i][j] != '\0'; j++, l++)
			str_analysis[i][l] = content[i][j];
		str_analysis[i][l] = '\0';
	}

	for (int i = 0; i < _gram_num_; i++)
	{
		for (int j = 1; str_analysis[i][j + 1] != '\0'; j++)
		{
			if (is_terminal(str_analysis[i][j]) && is_terminal(str_analysis[i][j + 1]))
			{
				m = index(str_analysis[i][j]);
				n = index(str_analysis[i][j+1]);
				pri_rela[m][n] = '=';
			}
			if (str_analysis[i][j + 2] != '\0'&&is_terminal(str_analysis[i][j]) && is_terminal(str_analysis[i][j + 2]) && !is_terminal(str_analysis[i][j + 1]))
			{
				m = index(str_analysis[i][j]);
				n = index(str_analysis[i][j + 2]);
				pri_rela[m][n] = '=';
			}
			if (is_terminal(str_analysis[i][j]) && !is_terminal(str_analysis[i][j + 1]))
			{
				for (k = 0; k < gram_num; k++)
				{
					if (grammar[k][0] == str_analysis[i][j + 1])
						break;
				}
				m = index(str_analysis[i][j]);
				for (int t = 0; t < firstvt[k][0]; t++)
				{
					n = index(firstvt[k][t + 1]);
					pri_rela[m][n] = '<';
				}
			}
			if (!is_terminal(str_analysis[i][j]) && is_terminal(str_analysis[i][j + 1]))
			{
				for (k = 0; k < gram_num; k++)
				{
					if (grammar[k][0] == str_analysis[i][j])
						break;
				}
				n = index(str_analysis[i][j + 1]);
				for (int t = 0; t < lastvt[k][0]; t++)
				{
					m = index(lastvt[k][t + 1]);
					pri_rela[m][n] = '>';
				}
			}
		}
	}
	m = index('#');
	for (int t = 0; t < firstvt[0][0]; t++)
	{
		n = index(firstvt[0][t + 1]);
		pri_rela[m][n] = '<';
	}
	n = index('#');
	for (int t = 0; t < lastvt[0][0]; t++)
	{
		m = index(lastvt[0][t + 1]);
		pri_rela[m][n] = '>';
	}
	pri_rela[n][n] = '=';
}

/*算符优先分析*/
int OperatorPriorityAnalyse()
{
	int i, j;
	int x, y;
	int z;
	int k = 1;
	char a, q;
	sym_stack[k] = '#';
	for (i = 0; input_str[i] != '\0';i++);
	z = i--;
	i = 0;
	while ((a = input_str[i]) != '\0')
	{
		if (is_terminal(sym_stack[k]))
			j = k;
		else
			j = k - 1;
		x = index(sym_stack[j]);
		y = index(a);
		if (pri_rela[x][y] == '>')
		{
			print_stack(1, k, sym_stack);
			cout << a;
			print_stack(i + 1, z, input_str);
			cout << "规约" << endl;
			do
			{
				q = sym_stack[j];
				if (is_terminal(sym_stack[j - 1]))
					j = j - 1;
				else
					j = j - 2;
				x = index(sym_stack[j]);
				y = index(q);
			} while (pri_rela[x][y] != '<');
			int m, n, N;
			for (m = j + 1; m <= k; m++)
			{
				for(N=0;N<_gram_num_;N++)
					for (n = 1; str_analysis[N][n] != '\0'; n++)
					{
						if (!is_terminal(sym_stack[m]) && !is_terminal(str_analysis[N][n]))
						{
							if (is_terminal(sym_stack[m + 1]) && is_terminal(str_analysis[N][n + 1]) && sym_stack[m + 1] == str_analysis[N][n + 1])
							{
								sym_stack[j + 1] = str_analysis[N][0];
								break;
							}
						}
						else
							if(is_terminal(sym_stack[m]))
								if (sym_stack[m] == str_analysis[N][n])
								{
									sym_stack[j + 1] = str_analysis[N][0];
									break;
								}
					}
			}
			k = j + 1;
			if (k == 2 && a == '#')
			{
				print_stack(1, k, sym_stack);
				cout << a;
				print_stack(i + 1, z, input_str);

				while (sym_stack[k] != grammar[0][0])
				{
					for (int p = 0; p < _gram_num_; p++)
					{
						int q = 0;
						for (q = 0; content[p][q] != '\0'; q++);
						if (q <= 4 && content[p][q-1]== sym_stack[k])
						{
							cout << "规约" << endl;
							sym_stack[k] = content[p][0];
							print_stack(1, k, sym_stack);
							cout << a;
							print_stack(i + 1, z, input_str);
						}
					}
				}

				cout << "结束" << endl;
				cout << "符合文法定义" << endl;
				return 1;
			}
		}
		else
			if (pri_rela[x][y] == '<' || pri_rela[x][y] == '=')
			{
				print_stack(1, k, sym_stack);
				cout << a;
				print_stack(i + 1, z, input_str);
				cout << "移进" << endl;
				k++;
				sym_stack[k] = a;
				i++;
			}
			else
			{
				cout << endl << "Failed" << endl;
				return 0;
			}
	}
	cout << endl << "Failed" << endl;
	return 0;
}

int main()
{
	/*cout << "输入文法规则个数：";
	cin >> gram_num;
	cout << "输入文法规则：" << endl;
	for (int i = 0; i < gram_num; i++)
	{
		cin >> grammar[i];
		//FIRTSTVT和LASTVT集合的第一位表示集合元素个数
		//初始化为0
		firstvt[i][0] = 0;
		lastvt[i][0] = 0;
	}*/

	ifstream fin;
	fin.open("grammar.txt",ios::in);
	if (!fin.is_open())
		return -1;
	gram_num =0;
	while (!fin.eof())
	{
		fin.getline(grammar[gram_num], 30);
		firstvt[gram_num][0] = 0;
		lastvt[gram_num][0] = 0;
		gram_num++;
	}
	fin.close();

	cout << "文法规则：" << endl;
	for (int i = 0; i < gram_num; i++)
		cout << grammar[i] << endl;
	cout << endl;

	/*判断文法是否合法*/
	for (int i = 0; i < gram_num; i++)
	{
		for (int j = 0; grammar[i][j] != '\0'; j++)
		{
			if (grammar[i][0]<'A' || grammar[i][0]>'Z')
			{
				cout << "不是算符文法" << endl;
				return -1;
			}
			if (grammar[i][j] >= 'A'&&grammar[i][j] <= 'Z')
			{
				if (grammar[i][j + 1] >= 'A'&&grammar[i][j + 1] <= 'Z')
				{
					cout << "不是算符文法" << endl;
					return -1;
				}
			}
		}
	}

	int k = 0;
	for (int i = 0; i < gram_num; i++)
	{
		for (int j = 0; grammar[i][j] != '\0'; j++)
		{
			if (grammar[i][j] == '|')
				continue;
			if (grammar[i][j] == '-'&&grammar[i][j+1] == '>')
				continue;
			if (grammar[i][j] == '>'&&grammar[i][j - 1] == '-')
				continue;
			if (grammar[i][j]<'A'&&grammar[i][j]>'Z')
				continue;
			if (grammar[i][j] >= 'A'&&grammar[i][j] <= 'Z')
				continue;
			terminal[k++] = grammar[i][j];
		}
	}
	terminal[k] = '#';
	terminal[k + 1] = '\0';
	CreatePriorityTable();

	
	cout << "各终极符的FIRSTVT集为：" << endl;
	for (int i = 0; i < gram_num; i++)
	{
		cout << grammar[i][0] << "：";
		for (int j = 0; j < firstvt[i][0]; j++)
			cout << firstvt[i][j + 1] << ' ';
		cout << endl;
	}
	cout << endl;

	cout << "各终极符的LASTVT集为：" << endl;
	for (int i = 0; i < gram_num; i++)
	{
		cout << grammar[i][0] << "：";
		for (int j = 0; j < lastvt[i][0]; j++)
			cout << lastvt[i][j + 1] << ' ';
		cout << endl;
	}
	cout << endl;

	cout << "算符优先分析表：" << endl;
	for (int i = 0; terminal[i] != '\0'; i++)
		cout << '\t' << terminal[i];
	cout << endl;
	for (int i = 0; i < k + 1; i++)
	{
		cout << terminal[i] << '\t';
		for (int j = 0; j < k + 1; j++)
			cout << pri_rela[i][j] << '\t';
		cout << endl;
	}
	cout << endl;

	cout << "输入符号串，以#结束：";
	cin >> input_str;

	OperatorPriorityAnalyse();

	return 0;
}