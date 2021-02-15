#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include "operatorpriority.h"
#include <QMessageBox>
#include <QString>
#include <string>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "dialog.h"
#include <QTime>
#include <QApplication>
using namespace std;

QString paintinfo;
int paintnum;

//延时停顿
void sleep(unsigned int ds)
{
    //先获取当前时间，再加上延时得到目标时刻
    QTime reachTime=QTime::currentTime().addMSecs(ds);

    //不断获取当前时间与目标时刻比对
    while(QTime::currentTime()<reachTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList fileNames;
QString filestr;

void MainWindow::selectFile()
{
    QFileDialog *fileDialog=new QFileDialog(this);
    fileDialog->setWindowTitle(tr("open file"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter(tr("Files(*.txt)"));
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);

    if(fileDialog->exec())
    {
        fileNames=fileDialog->selectedFiles();
    }
    for(auto tmp:fileNames)
    {
        QFile f(tmp);
        if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            filestr="Open failed.";
        }

        QTextStream txtinput(&f);
        int row=0;
        while(!txtinput.atEnd())
        {
            filestr = txtinput.readLine();
            string tempstr = filestr.toStdString();
            int cnt=0;
            for(cnt=0;cnt<filestr.length();cnt++)
            {
                grammar[row][cnt]=tempstr[cnt];
            }
            grammar[row][cnt]='\0';

            ui->GrammarOut->append(grammar[row]);
            row++;
        }
        gram_num = row;
        f.close();
    }
}

/*栈中内容*/
QString stackinfo(int j, int k, char*s, char a='\0')
{
    // 起始位置索引为j，结束位置索引为k
    // 打印s[j]到s[k]的内容
    string str="";
    QString qstr;
    //int n = 0;
    if(a!='\0')
    {
        str+=a;
    }
    for (int i = j; i <= k; i++)
    {
        //cout << s[i];
        //n++;
        str+=s[i];
    }
    //for (; n < 15; n++);
        //cout << ' ';

    qstr = QString::fromStdString(str);

    return qstr;
}

/*_______算符优先分析*/
int OperatorPriorityAnalyse(Dialog* win)
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
    int rownum=0;
    QString info;
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
            win->addTabRow(rownum);
            //print_stack(1, k, sym_stack);
            info = stackinfo(1,k,sym_stack);
            paintinfo = info;
            win->setTabItem(rownum,0,info);
            //cout << a;
            //print_stack(i + 1, z, input_str);
            info = stackinfo(i+1,z,input_str,a);
            win->setTabItem(rownum,1,info);
            //cout << "规约" << endl;
            info="规约";
            win->setTabItem(rownum,2,info);
            rownum++;
            win->update();
            sleep(300);
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
                win->addTabRow(rownum);
                //print_stack(1, k, sym_stack);
                info = stackinfo(1,k,sym_stack);
                paintinfo = info;
                win->setTabItem(rownum,0,info);
                //cout << a;
                //print_stack(i + 1, z, input_str);
                info = stackinfo(i+1,z,input_str,a);
                win->setTabItem(rownum,1,info);

                while (sym_stack[k] != grammar[0][0])
                {
                    for (int p = 0; p < _gram_num_; p++)
                    {
                        int q = 0;
                        for (q = 0; content[p][q] != '\0'; q++);
                        if (q <= 4 && content[p][q-1]== sym_stack[k])
                        {
                            //cout << "规约" << endl;
                            info = "规约";
                            win->setTabItem(rownum,2,info);
                            rownum++;
                            win->update();
                            sleep(300);

                            win->addTabRow(rownum);
                            sym_stack[k] = content[p][0];
                            //print_stack(1, k, sym_stack);
                            info = stackinfo(1,k,sym_stack);
                            paintinfo = info;
                            win->setTabItem(rownum,0,info);
                            //cout << a;
                            //print_stack(i + 1, z, input_str);
                            info = stackinfo(i+1,z,input_str,a);
                            win->setTabItem(rownum,1,info);
                            break;
                        }
                    }
                }

                //cout << "结束" << endl;
                info="结束";
                win->setTabItem(rownum,2,info);
                rownum++;
                win->update();
                //sleep(500);
                //cout << "符合文法定义" << endl;
                QMessageBox::about(win,QString("Succeeded"),QString("符合文法定义"));
                return 1;
            }
        }
        else
            if (pri_rela[x][y] == '<' || pri_rela[x][y] == '=')
            {
                win->addTabRow(rownum);

                info = stackinfo(1,k,sym_stack);
                paintinfo = info;
                win->setTabItem(rownum,0,info);
                //cout << a;

                info = stackinfo(i+1,z,input_str,a);
                win->setTabItem(rownum,1,info);

                info="移进";
                win->setTabItem(rownum,2,info);
                win->update();
                sleep(300);
                rownum++;
                k++;
                sym_stack[k] = a;
                i++;
            }
            else
            {
                //cout << endl << "Failed" << endl;
                QMessageBox::about(win,QString("Failed"),QString("分析失败"));
                return 0;
            }
    }
    //cout << endl << "Failed" << endl;
    QMessageBox::about(win,QString("Failed"),QString("分析失败"));
    return 0;
}

void MainWindow::on_pushButton_2_clicked()
{
    selectFile();
}

void MainWindow::on_pushButton_clicked()
{
    /*判断文法是否合法*/
        for (int i = 0; i < gram_num; i++)
        {
            for (int j = 0; grammar[i][j] != '\0'; j++)
            {
                if (grammar[i][0]<'A' || grammar[i][0]>'Z')
                {
                    //cout << "不是算符文法" << endl;
                    QMessageBox::about(this,QString("Failed"),QString("不是算符文法"));
                    return;
                }
                if (grammar[i][j] >= 'A'&&grammar[i][j] <= 'Z')
                {
                    if (grammar[i][j + 1] >= 'A'&&grammar[i][j + 1] <= 'Z')
                    {
                        //cout << "不是算符文法" << endl;
                        QMessageBox::about(this,QString("Failed"),QString("不是算符文法"));
                        return;
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


        //cout << "各终极符的FIRSTVT集为：" << endl;
        for (int i = 0; i < gram_num; i++)
        {
            QString tempstr = QString(grammar[i][0]);
            //cout << grammar[i][0] << "：";
            tempstr += "：";
            for (int j = 0; j < firstvt[i][0]; j++)
            {
                //cout << firstvt[i][j + 1] << ' ';
                tempstr += firstvt[i][j + 1];
                tempstr += ' ';
            }
            //cout << endl;
            ui->firstvt->append(tempstr);
        }
        //cout << endl;

        //cout << "各终极符的LASTVT集为：" << endl;
        for (int i = 0; i < gram_num; i++)
        {
            QString tempstr = QString(grammar[i][0]);
            //cout << grammar[i][0] << "：";
            tempstr += "：";
            for (int j = 0; j < lastvt[i][0]; j++)
            {
                //cout << lastvt[i][j + 1] << ' ';
                tempstr += lastvt[i][j + 1];
                tempstr += ' ';
            }
            //cout << endl;
            ui->lastvt->append(tempstr);
        }
        //cout << endl;


        //cout << "算符优先分析表：" << endl;
        QString tempstr ;
        ui->priority->setColumnCount(strlen(terminal)+1);

        ui->priority->insertRow(0);
        ui->priority->setColumnWidth(0,70);
        for (int i = 0; terminal[i] != '\0'; i++)
        {
            tempstr = terminal[i];
            ui->priority->setColumnWidth(i+1,70);
            ui->priority->setItem(0, i+1, new QTableWidgetItem(tempstr));
        }
        tempstr = "";
        for (int i = 0; i < k + 1; i++)
        {
            tempstr = terminal[i];
            ui->priority->insertRow(i+1);
            ui->priority->setItem(i+1, 0, new QTableWidgetItem(tempstr));
            for (int j = 0; j < k + 1; j++)
            {
                tempstr = pri_rela[i][j];
                ui->priority->setItem(i+1, j+1, new QTableWidgetItem(tempstr));
            }
        }
}

void MainWindow::on_pushButton_3_clicked()
{
    QString tempstr = ui->input->text();
    int cnt;
    for(cnt=0;cnt<tempstr.length();cnt++)
        input_str[cnt]=tempstr.toStdString()[cnt];
    input_str[cnt]='#';
    input_str[cnt+1]='\0';
    paintnum = cnt+2;
    /*
    cout << "输入符号串，以#结束：";
    cin >> input_str;
    */
    Dialog outwin;
    outwin.show();
    outwin.setTabColCount(3);
    outwin.setTabColWidth(0,220);
    outwin.setTabColWidth(1,220);
    outwin.setTabColWidth(2,220);
    OperatorPriorityAnalyse(&outwin);

    outwin.exec();
}


