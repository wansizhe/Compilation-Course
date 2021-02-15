#ifndef LEX_ANA_H
#define LEX_ANA_H

#include <string>
using namespace std;

/*单字符转成单字符的字符串*/
string char_to_str(const char &c)
{
    string s;
    char t[2];
    t[0] = c;
    t[1] = '\0';
    s = t;
    return s;
}
struct item
{
    string info;
    int code;
    string value;
    item* next = NULL;
};
/*保留字符串*/
struct res_word
{
    string info;
    int code;
};
/*保留字符*/
struct res_symb
{
    char info;
    int code;
};
/*所有保留关键字*/
res_word std_tab[] = {
    {"main",1 },			{"if",2},
    {"else",3},			{"while",4},
    {"do",5},				{"for",6},
    {"switch",7},		{"case",8},
    {"default",9},		{"break",10},
    {"continue",11},	{"int",12},
    {"char",13},			{"float",14},
    {"double",15},		{"void",16},
    {"bool",17},			{"struct",18},
    {"class",19},			{"const",20},
    {"unsigned",21},	{"return",22},
    {"include",59},	{"define",60},
    {"NULL",63}
};
/*所有非前缀运算符、定界符*/
res_symb syb_tab[] = {
    {'#',23},		{'*',26},
    {'/',27},		{'%',28},
    {':',32},			{'?',33},
    {'~',45},		{'^',46},
    {';',47},			{'(',50},
    {')',51},			{'{',52},
    {'}',53},			{'[',54},
    {']',55},			{'.',58},
    {39,61},		{34,62},
    {92,64},		{',',65}
};
/*空格、制表符处理*/
void GetBC(string &s)
{
    string t ;
    int j = 0;
    int i = 0;
    bool flag1 = false;
    bool flag2 = false;
    for (; i < s.length(); i++)
        if (s[i] != ' '&&s[i] != '\t')
            break;
    t = char_to_str(s[i]);
    i++;
    for (; i < s.length(); i++)
    {
        if (flag1||flag2)
        {
            t = t + s[i];
            j++;
        }
        else
        {
            if (s[i] == ' ' || s[i] == '\t')
            {
                if (t[j] != ' ')
                {
                    t = t + ' ';
                    j++;
                }
            }
            else
            {
                if (s[i] == 39 && flag2 == false)
                    flag1 = flag1 ? false : true;
                if (s[i] == 34 && flag1 == false)
                    flag2 = flag2 ? false : true;
                t = t + s[i];
                j++;
            }
        }
    }
    s = t;
}
/*字符串后拼接字符*/
void Concat(string &s, const char &c)
{
    s = s + c;
}
/*判断是否是字母*/
bool is_Letter(const char &c)
{
    if (c >= 65 && c <= 90)
        return true;
    else if (c >= 97 && c <= 122)
        return true;
    else
        return false;
}
/*判断是否是数字*/
bool is_Digit(const char &c)
{
    if (c >= 48 && c <= 57)
        return true;
    else
        return false;
}
/*判断是否是非前缀符*/
int is_Symbol(const char&c)
{
    int n = sizeof(syb_tab) / sizeof(res_symb);
    for (int i = 0; i<n; i++)
    {
        if (c == syb_tab[i].info)
            return syb_tab[i].code;
    }
    return -1;
}
/*查询保留字*/
int Reserve(const string &s)
{
    int n= sizeof(std_tab) / sizeof(res_word);
    for (int i=0;i<n;i++)
    {
        if (s == std_tab[i].info)
            return std_tab[i].code;
    }
    return 56;
}
/*处理//型注释*/
void ign_Comment(string &s)
{
    string t="";
    int i;
    for (i = 0; i < s.length(); i++)
    {
        if (s[i] == '/'&&s[i + 1] == '/')
        {
            //t = s.substr(0, i);
            //s = t;
            break;
        }
        t = t + s[i];
    }
    s = t;
}
/*词法分析表插入项*/
void insert_item(item *&end, item* newone)
{
    end->next = newone;
    end=newone;
}
/*新建项*/
void new_item(item *t, const int &code,const string &info,const string &value)
{
    t->code = code;
    t->info = info;
    t->value = value;
    t->next = NULL;
}
/*查询编码*/
int get_code(const string &s)
{
    if (s == "main")
        return 1;
    else if (s == "if")
        return 2;
    else if (s == "else")
        return 3;
    else if (s == "while")
        return 4;
    else if (s == "do")
        return 5;
    else if (s == "for")
        return 6;
    else if (s == "switch")
        return 7;
    else if (s == "case")
        return 8;
    else if (s == "default")
        return 9;
    else if (s == "break")
        return 10;
    else if (s == "continue")
        return 11;
    else if (s == "int")
        return 12;
    else if (s == "char")
        return 13;
    else if (s == "float")
        return 14;
    else if (s == "double")
        return 15;
    else if (s == "void")
        return 16;
    else if (s == "bool")
        return 17;
    else if (s == "struct")
        return 18;
    else if (s == "class")
        return 19;
    else if (s == "const")
        return 20;
    else if (s == "unsigned")
        return 21;
    else if (s == "return")
        return 22;
    else if (s == "#")
        return  23;
    else if (s == "+")
        return 24;
    else if (s == "-")
        return 25;
    else if (s == "*")
        return 26;
    else if (s == "/")
        return 27;
    else if (s == "%")
        return 28;
    else if (s == "=")
        return 29;
    else if (s == ">")
        return 30;
    else if (s == "<")
        return 31;
    else if (s == ":")
        return 32;
    else if (s == "?")
        return 33;
    else if (s == "==")
        return 34;
    else if (s == ">=")
        return 35;
    else if (s == "<=")
        return 36;
    else if (s == "++")
        return 37;
    else if (s == "--")
        return 38;
    else if (s == "!=")
        return 39;
    else if (s == "&&")
        return 40;
    else if (s == "||")
        return 41;
    else if (s == "&")
        return 42;
    else if (s == "|")
        return 43;
    else if (s == "!")
        return 44;
    else if (s == "~")
        return 45;
    else if (s == "^")
        return 46;
    else if (s == ";")
        return 47;
    else if (s == "<<")
        return 48;
    else if (s == ">>")
        return 49;
    else if (s == "(")
        return 50;
    else if (s == ")")
        return 51;
    else if (s == "{")
        return 52;
    else if (s == "}")
        return 53;
    else if (s == "[")
        return 54;
    else if (s == "]")
        return 55;
    else if (s == ".")
        return 58;
    else if (s == "include")
        return 59;
    else if (s == "define")
        return 60;
    else if (s == "'")
        return 61;
    else if (s == "\"")
        return 62;
    else if (s == "NULL")
        return 63;
    else if (s == "\\")
        return 64;
    else if (s == ",")
        return 65;

    else
        return 0;
}
/*销毁输出链表*/
void Destory(item* word_tab)
{
        item* p = word_tab,*q;
        while (p != NULL)
        {
            q = p->next;
            delete p;
            p = q;
        }
}

#endif // LEX_ANA_H
