#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QApplication>
#include <QProcess>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include "lex_ana.h"


QStringList fileNames;
int errorline=-1;
item* word_tab = NULL;
item* tab_end = word_tab;
int line_num=0;

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
        bool flag = false;
        bool in_str = false;
        bool in_char = false;
        line_num=0;
        errorline=-1;
        Destory(word_tab);
        word_tab = new item;
        tab_end = word_tab;
        QString filestr;
        QFile f(tmp);
        if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            filestr="Open failed.";
        }

        QTextStream txtinput(&f);

        string s,st;
        ui->original->clear();
        while(!txtinput.atEnd())
        {
            filestr = txtinput.readLine();
            ui->original->append(filestr);
            s=filestr.toStdString();
            line_num++;

            if(errorline!=-1)
                continue;

            if (line_num ==15 )
                    {
                        line_num++;
                        line_num--;
                    }

                    ign_Comment(s);
                    GetBC(s);
                    int i = 0;

                    int len = s.length();
                    s = s + '\0';

                    if (s[0] == '#')
                    {
                        item *t = new item;
                        new_item(t, 23, "#", "-");
                        insert_item(tab_end, t);
                        i++;
                        st = "";
                        while (i < len)
                        {
                            if (s[i] == ' ')
                            {
                                i++;
                                continue;
                            }
                            if (is_Letter(s[i]))
                            {
                                st = char_to_str(s[i]);
                                i++;
                                while (is_Letter(s[i]))
                                {
                                    Concat(st, s[i]);
                                    i++;
                                }
                                if (st == "include"|| st == "define")
                                {
                                    item *t = new item;
                                    new_item(t, Reserve(st), st, "-");
                                    insert_item(tab_end, t);
                                    break;
                                }
                                else
                                {
                                    //cerr << "Error:Line " << line_num << ". " << endl;
                                    errorline=line_num;
                                    break; ;
                                }
                            }
                        }

                        continue;
                    }

                    while (i < len)
                    {
                        st = "";
                        /*开启注释*/
                        if (s[i] == '/'&&s[i + 1] == '*')
                        {
                            flag = true;
                            i++;
                            continue;
                        }
                        if (flag == true && s[i] == '*'&&s[i + 1] == '/')
                        {
                            flag = false;
                            i += 2;
                            continue;
                        }
                        if (s[i] == ' ')
                        {
                            i++;
                            continue;
                        }
                        if (!flag)
                        {
                            if (is_Letter(s[i]))
                            {
                                st = char_to_str(s[i]);
                                i++;
                                while (is_Letter(s[i]) || is_Digit(s[i]) || s[i] == '_')
                                {
                                    Concat(st, s[i]);
                                    i++;
                                }
                                if (Reserve(st) != 56)
                                {
                                    item *t = new item;
                                    new_item(t, Reserve(st), st, "-");
                                    insert_item(tab_end, t);
                                    continue;
                                }
                                else
                                {
                                    item *t = new item;
                                    new_item(t, 56, "ID", st);
                                    insert_item(tab_end, t);
                                    continue;
                                }
                            }

                            if (is_Digit(s[i]))
                            {
                                st = char_to_str(s[i]);
                                i++;
                                while (is_Digit(s[i]))
                                {
                                    Concat(st, s[i]);
                                    i++;
                                }
                                if (is_Letter(s[i]))
                                {
                                    //cerr << "Error:Line " << line_num << ". " << endl;
                                    errorline=line_num;
                                    break; ;
                                }
                                else if (s[i] == '.')
                                {
                                    Concat(st, s[i]);
                                    i++;
                                    while (is_Digit(s[i]))
                                    {
                                        Concat(st, s[i]);
                                        i++;
                                    }
                                    if (is_Letter(s[i])||s[i]=='.')
                                    {
                                        errorline=line_num;
                                        //cerr << "Error:Line " << line_num << ". " << endl;
                                        break;;
                                    }
                                    else
                                    {
                                        item *t = new item;
                                        new_item(t, 57, "NUM", st);
                                        insert_item(tab_end, t);
                                        continue;
                                    }
                                }
                                else
                                {
                                    item *t = new item;
                                    new_item(t, 57, "NUM", st);
                                    insert_item(tab_end, t);
                                    continue;
                                }
                            }

                            if (s[i] == '+')
                            {
                                if (s[i + 1] == '+')
                                {
                                    item *t = new item;
                                    new_item(t, 37, "++", "-");
                                    insert_item(tab_end, t);
                                    i += 2;
                                }
                                else
                                {
                                    item *t = new item;
                                    new_item(t, 24, "+", "-");
                                    insert_item(tab_end, t);
                                    i ++;
                                }
                            }
                            else if (s[i] == '-')
                            {
                                if (s[i + 1] == '-')
                                {
                                    item *t = new item;
                                    new_item(t, 38, "--", "-");
                                    insert_item(tab_end, t);
                                    i += 2;
                                }
                                else
                                {
                                    item *t = new item;
                                    new_item(t, 25, "-", "-");
                                    insert_item(tab_end, t);
                                    i++;
                                }
                            }
                            else if (s[i] == '=')
                            {
                                if (s[i + 1] == '=')
                                {
                                    item *t = new item;
                                    new_item(t, 34, "==", "-");
                                    insert_item(tab_end, t);
                                    i+=2;
                                }
                                else
                                {
                                    item *t = new item;
                                    new_item(t, 29, "=", "-");
                                    insert_item(tab_end, t);
                                    i++;
                                }
                            }
                            else if (s[i] == '>')
                            {
                                if (s[i + 1] == '=')
                                {
                                    item *t = new item;
                                    new_item(t, 35, ">=", "-");
                                    insert_item(tab_end, t);
                                    i+=2;
                                }
                                else if (s[i + 1] == '>')
                                {
                                    item *t = new item;
                                    new_item(t, 49, ">>", "-");
                                    insert_item(tab_end, t);
                                    i+=2;
                                }
                                else
                                {
                                    item *t = new item;
                                    new_item(t, 30, ">", "-");
                                    insert_item(tab_end, t);
                                    i++;
                                }
                            }
                            else if (s[i] == '<')
                            {
                                if (s[i + 1] == '=')
                                {
                                    item *t = new item;
                                    new_item(t, 36, "<=", "-");
                                    insert_item(tab_end, t);
                                    i += 2;
                                }
                                else if (s[i + 1] == '<')
                                {
                                    item *t = new item;
                                    new_item(t, 48, "<<", "-");
                                    insert_item(tab_end, t);
                                    i += 2;
                                }
                                else
                                {
                                    item *t = new item;
                                    new_item(t, 31, "<", "-");
                                    insert_item(tab_end, t);
                                    i++;
                                }
                            }
                            else if (s[i] == '!')
                            {
                                if (s[i + 1] == '=')
                                {
                                    item *t = new item;
                                    new_item(t, 39, "!=", "-");
                                    insert_item(tab_end, t);
                                    i += 2;
                                }
                                else
                                {
                                    item *t = new item;
                                    new_item(t, 44, "!", "-");
                                    insert_item(tab_end, t);
                                    i++;
                                }
                            }
                            else if (s[i] == '&')
                            {
                                if (s[i + 1] == '&')
                                {
                                    item *t = new item;
                                    new_item(t, 40, "&&", "-");
                                    insert_item(tab_end, t);
                                    i += 2;
                                }
                                else
                                {
                                    item *t = new item;
                                    new_item(t, 42, "&", "-");
                                    insert_item(tab_end, t);
                                    i++;
                                }
                            }
                            else if (s[i] == '|')
                            {
                                if (s[i + 1] == '|')
                                {
                                    item *t = new item;
                                    new_item(t, 41, "||", "-");
                                    insert_item(tab_end, t);
                                    i += 2;
                                }
                                else
                                {
                                    item *t = new item;
                                    new_item(t, 43, "|", "-");
                                    insert_item(tab_end, t);
                                    i++;
                                }
                            }
                            else if (is_Symbol(s[i]) != -1)
                            {
                                if (s[i] == 39 && in_str == false)
                                    in_char = in_char ? false : true;
                                if (s[i] == 34&&in_char == false)
                                    in_str = in_str ? false : true;

                                item *t = new item;
                                new_item(t, is_Symbol(s[i]), char_to_str(s[i]), "-");
                                insert_item(tab_end, t);
                                i++;

                                if (in_char)
                                {
                                    st = "";
                                    if (s[i] != '\'')
                                        st = char_to_str(s[i]);
                                    item *t = new item;
                                    new_item(t, 57, "NUM",st );
                                    insert_item(tab_end, t);

                                    i++;
                                }
                                else if (in_str)
                                {
                                    st = "";
                                    if (s[i] != '"')
                                    {
                                        st = char_to_str(s[i]);
                                        i++;
                                    }
                                    while (s[i] != '"')
                                    {
                                        st += s[i];
                                        i++;
                                    }

                                    item *t = new item;
                                    new_item(t, 57, "NUM", st);
                                    insert_item(tab_end, t);
                                }


                            }

                        }
                        else
                            i++;
                    }


        }

        f.close();


    }


}

void MainWindow::on_pushButton_clicked()
{
    ui->compiled->clear();
    if(errorline==-1)
    {
        item* k = word_tab->next;
        while (k != NULL)
        {
            ui->compiled->append("【 "+QString::fromStdString(k->info)+" , "+QString::fromStdString(k->value)+" 】");
            //fout << "【 " << k->info << " , " << k->value << " 】" << endl;
            k = k->next;
        }
    }
    else
    {
        ui->compiled->append(" Error: Line"+QString::number(errorline)+'.');
    }
    //
}

void MainWindow::on_actionOpen_triggered()
{
    selectFile();
}

void MainWindow::on_actionHelp_triggered()
{
    Dialog mydialog;
    mydialog.show();
    mydialog.exec();

}
