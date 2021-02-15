#ifndef OPERATORPRIORITY_H
#define OPERATORPRIORITY_H

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

/*_______生成优先关系表*/

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

    /*
    cout << "文法转化后为：" << endl;
    for (int i = 0; i < _gram_num_; i++)
        cout << content[i] << endl;
    cout << endl;
    */

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




#endif // OPERATORPRIORITY_H
