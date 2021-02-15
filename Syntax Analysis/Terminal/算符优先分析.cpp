#include<iostream>
#include<fstream>
using namespace std;

char pri_rela[20][20];	//������ȹ�ϵ data
char sym_stack[100];	//����ջ s
char terminal[20];		//�ս���� label
char input_str[100];		//����ķ��Ŵ� input
char str_analysis[20][10];		//���ڸ������봮�ķ��� string

int gram_num;		//�ķ������� r
int _gram_num_;	//ת�����ķ�������� r1
char grammar[10][30];		//�ķ����� st
char content[20][20];
char firstvt[10][10];		//�ķ����ս��FIRSTVT�� first
char lastvt[10][10];		//�ķ����ս��LASTVT�� last
bool first_flag[10] = { 0 };		//��־��i�����ս����FIRSTVT���Ƿ��Ѿ����
bool last_flag[10] = { 0 };		//��־��i�����ս����LASTVT���Ƿ��Ѿ����

/*�ж��Ƿ����ռ���*/
bool is_terminal(char c)
{
	// �����ռ�������
	for (int i = 0; terminal[i] != '\0'; i++)
	{
		if (c == terminal[i])
			return true;
	}
	return false;
}

/*��ȡ�ռ����������*/
int index(char c)
{
	// �����ռ�������
	for (int i = 0; terminal[i] != '\0'; i++)
	{
		if (c == terminal[i])
			return i;
	}
	return -1;
}

/*��ӡ����ջ*/
void print_stack(int j, int k, char*s)
{
	// ��ʼλ������Ϊj������λ������Ϊk
	// ��ӡs[j]��s[k]������
	int n = 0;
	for (int i = j; i <= k; i++)
	{
		cout << s[i];
		n++;
	}
	for (; n < 15; n++)
		cout << ' ';
}

/*����FIRSTVT����*/
void CreateFirstVT(char c)
{
	int i, j, k;
	int m, n;
	// �ҵ����ռ���cλ���ײ����ķ�
	for (i = 0; i < gram_num; i++)
	{
		if (grammar[i][0] == c)
			break;
	}
	// �����ǰ���ռ�����FIRSTVT��û�н��������
	// �����ظ�����
	if (first_flag[i] == false)
	{
		// nΪFIRSTVT�����У���ǰ�����λ��
		n = firstvt[i][0] + 1;
		// mΪ������ռ�����Ӧ�ķ����ʱ�õ�������
		m = 0;
		do
		{
			// m=2���������ֵ�һ������ʽ
			// ���� | ����˵������������һ������ʽ
			if (m == 2 || grammar[i][m] == '|')
			{
				// m+1���ķ������ĳ������ʽ�ĵ�һ����������
				// ����ʽ�ĵ�һ������λ�ռ������Ϳ��Խ�������FIRSTVT
				if (is_terminal(grammar[i][m + 1]))
				{
					firstvt[i][n] = grammar[i][m + 1];
					n++;
				}
				// ����ʽ�ĵ�һ�����Ų����ռ���������к����ж�
				else
				{
					// �����һ������������ռ�����
					// ��ɽ������FIRSTVT
					if (is_terminal(grammar[i][m + 2]))
					{
						firstvt[i][n] = grammar[i][m + 2];
						n++;
					}
					// �����һ��������һ����c��ͬ�ķ��ռ���
					// ����Ҫ������FIRSTVT�������䲢�뵱ǰ�����FIRSTVT
					if (grammar[i][m + 1] != c)
					{
						// �ݹ����һ������ͬ�ķ��ռ�����FIRSTVT
						CreateFirstVT(grammar[i][m + 1]);
						// Ϊ�˷��ϼ��ϻ�����
						// ���أ���ֹ�ظ�Ԫ�ؼ���FIRSTVT
						for (j = 0; j < gram_num; j++)
						{
							if (grammar[j][0] == grammar[i][m + 1])
								break;
						}
						// ���ݹ���õ�FIRSTVT���뵱ǰ�����FIRSTVT
						for (k = 0; k < firstvt[j][0]; k++)
						{
							int t = 0;
							// �ҵ�����λ�õ�����
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
		// ȷ������Ԫ�ظ���
		firstvt[i][0] = --n;
		// ��ǰ���ռ���c��FIRSTVT����������ظ���
		first_flag[i] = true;
	}
}

/*����LASTVT����*/
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

/*�������ȹ�ϵ��*/
void CreatePriorityTable()
{
	
	int x = 0, y = 0;
	int m, n, k;
	
	for (int i = 0; i < gram_num; i++)
	{
		/*��ÿ������ĵ�һ�����ŵ�FIRSTVT*/
		CreateFirstVT(grammar[i][0]);
		/*��ÿ������ĵ�һ�����ŵ�FIRSTVT*/
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
	cout << "�ķ�ת����Ϊ��" << endl;
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

/*������ȷ���*/
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
			cout << "��Լ" << endl;
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
							cout << "��Լ" << endl;
							sym_stack[k] = content[p][0];
							print_stack(1, k, sym_stack);
							cout << a;
							print_stack(i + 1, z, input_str);
						}
					}
				}

				cout << "����" << endl;
				cout << "�����ķ�����" << endl;
				return 1;
			}
		}
		else
			if (pri_rela[x][y] == '<' || pri_rela[x][y] == '=')
			{
				print_stack(1, k, sym_stack);
				cout << a;
				print_stack(i + 1, z, input_str);
				cout << "�ƽ�" << endl;
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
	/*cout << "�����ķ����������";
	cin >> gram_num;
	cout << "�����ķ�����" << endl;
	for (int i = 0; i < gram_num; i++)
	{
		cin >> grammar[i];
		//FIRTSTVT��LASTVT���ϵĵ�һλ��ʾ����Ԫ�ظ���
		//��ʼ��Ϊ0
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

	cout << "�ķ�����" << endl;
	for (int i = 0; i < gram_num; i++)
		cout << grammar[i] << endl;
	cout << endl;

	/*�ж��ķ��Ƿ�Ϸ�*/
	for (int i = 0; i < gram_num; i++)
	{
		for (int j = 0; grammar[i][j] != '\0'; j++)
		{
			if (grammar[i][0]<'A' || grammar[i][0]>'Z')
			{
				cout << "��������ķ�" << endl;
				return -1;
			}
			if (grammar[i][j] >= 'A'&&grammar[i][j] <= 'Z')
			{
				if (grammar[i][j + 1] >= 'A'&&grammar[i][j + 1] <= 'Z')
				{
					cout << "��������ķ�" << endl;
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

	
	cout << "���ռ�����FIRSTVT��Ϊ��" << endl;
	for (int i = 0; i < gram_num; i++)
	{
		cout << grammar[i][0] << "��";
		for (int j = 0; j < firstvt[i][0]; j++)
			cout << firstvt[i][j + 1] << ' ';
		cout << endl;
	}
	cout << endl;

	cout << "���ռ�����LASTVT��Ϊ��" << endl;
	for (int i = 0; i < gram_num; i++)
	{
		cout << grammar[i][0] << "��";
		for (int j = 0; j < lastvt[i][0]; j++)
			cout << lastvt[i][j + 1] << ' ';
		cout << endl;
	}
	cout << endl;

	cout << "������ȷ�����" << endl;
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

	cout << "������Ŵ�����#������";
	cin >> input_str;

	OperatorPriorityAnalyse();

	return 0;
}