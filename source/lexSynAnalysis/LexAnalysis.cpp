#include <iostream>  
#include <fstream>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <vector>  
#include <iomanip>  
#include "LexAnalysis.h"  
  
using namespace std;  
  
int leftSmall = 0;//左小括号  
int rightSmall = 0;//右小括号  
int leftMiddle = 0;//左中括号  
int rightMiddle = 0;//右中括号  
int leftBig = 0;//左大括号  
int rightBig = 0;//右大括号  
int lineBra[6][1000] = {0};//括号和行数的对应关系，第一维代表左右6种括号  
int static_iden_number = 0;//模拟标志符的地址，自增  
//Token节点  
  
  
NormalNode * normalHead;//首结点  
  
//错误节点  
struct ErrorNode  
{  
    char content[30];//错误内容  
    char describe[30];//错误描述  
    int type;  
    int line;//所在行数  
    ErrorNode * next;//下一个节点  
};  
  
ErrorNode * errorHead;//首节点  
  
//标志符节点  
struct IdentiferNode  
{  
    char content[30];//内容  
    char describe[30];//描述  
    int type;//种别码  
    int addr;//入口地址  
    int line;//所在行数  
    IdentiferNode * next;//下一个节点  
};  
IdentiferNode * idenHead;//首节点  
  
vector<pair<const char *,int> > keyMap;  
vector<pair<const char *,int> > operMap;  
vector<pair<const char *,int> > limitMap;  
  
  
  
//初始化C语言的关键字的集合  
void initKeyMapping()  
{  
    keyMap.clear();  
    keyMap.push_back(make_pair("auto",AUTO));  
    keyMap.push_back(make_pair("break",BREAK));  
    keyMap.push_back(make_pair("case",CASE));  
    keyMap.push_back(make_pair("char",CHAR));  
    keyMap.push_back(make_pair("const",CONST));  
    keyMap.push_back(make_pair("continue",CONTINUE));  
    keyMap.push_back(make_pair("default",DEFAULT));  
    keyMap.push_back(make_pair("do",DO));  
    keyMap.push_back(make_pair("double",DOUBLE));  
    keyMap.push_back(make_pair("else",ELSE));  
    keyMap.push_back(make_pair("enum",ENUM));  
    keyMap.push_back(make_pair("extern",EXTERN));  
    keyMap.push_back(make_pair("float",FLOAT));  
    keyMap.push_back(make_pair("for",FOR));  
    keyMap.push_back(make_pair("goto",GOTO));  
    keyMap.push_back(make_pair("if",IF));  
    keyMap.push_back(make_pair("int",INT));  
    keyMap.push_back(make_pair("long",LONG));  
    keyMap.push_back(make_pair("register",REGISTER));  
    keyMap.push_back(make_pair("return",RETURN));  
    keyMap.push_back(make_pair("short",SHORT));  
    keyMap.push_back(make_pair("signed",SIGNED));  
    keyMap.push_back(make_pair("sizeof",SIZEOF));  
    keyMap.push_back(make_pair("static",STATIC));  
    keyMap.push_back(make_pair("struct",STRUCT));  
    keyMap.push_back(make_pair("switch",SWITCH));  
    keyMap.push_back(make_pair("typedef",TYPEDEF));  
    keyMap.push_back(make_pair("union",UNION));  
    keyMap.push_back(make_pair("unsigned",UNSIGNED));  
    keyMap.push_back(make_pair("void",VOID));  
    keyMap.push_back(make_pair("volatile",VOLATILE));  
    keyMap.push_back(make_pair("while",WHILE));  
  
    keyMap.push_back(make_pair("describe",DESCRIBE));  
    keyMap.push_back(make_pair("type",TYPE));  
    keyMap.push_back(make_pair("string",STRING));  
    keyMap.push_back(make_pair("digit",DIGIT));  
}  
void initOperMapping()  
{  
    operMap.clear();  
    operMap.push_back(make_pair("!",NOT));  
    operMap.push_back(make_pair("&",BYTE_AND));  
    operMap.push_back(make_pair("~",COMPLEMENT));  
    operMap.push_back(make_pair("^",BYTE_XOR));  
    operMap.push_back(make_pair("*",MUL));  
    operMap.push_back(make_pair("/",DIV));  
    operMap.push_back(make_pair("%",MOD));  
    operMap.push_back(make_pair("+",ADD));  
    operMap.push_back(make_pair("-",SUB));  
    operMap.push_back(make_pair("<",LES_THAN));  
    operMap.push_back(make_pair(">",GRT_THAN));  
    operMap.push_back(make_pair("=",ASG));  
    operMap.push_back(make_pair("->",ARROW));  
    operMap.push_back(make_pair("++",SELF_ADD));  
    operMap.push_back(make_pair("--",SELF_SUB));  
    operMap.push_back(make_pair("<<",LEFT_MOVE));  
    operMap.push_back(make_pair(">>",RIGHT_MOVE));  
    operMap.push_back(make_pair("<=",LES_EQUAL));  
    operMap.push_back(make_pair(">=",GRT_EQUAL));  
    operMap.push_back(make_pair("==",EQUAL));  
    operMap.push_back(make_pair("!=",NOT_EQUAL));  
    operMap.push_back(make_pair("&&",AND));  
    operMap.push_back(make_pair("||",OR));  
    operMap.push_back(make_pair("+=",COMPLETE_ADD));  
    operMap.push_back(make_pair("-=",COMPLETE_SUB));  
    operMap.push_back(make_pair("*=",COMPLETE_MUL));  
    operMap.push_back(make_pair("/=",COMPLETE_DIV));  
    operMap.push_back(make_pair("^=",COMPLETE_BYTE_XOR));  
    operMap.push_back(make_pair("&=",COMPLETE_BYTE_AND));  
    operMap.push_back(make_pair("~=",COMPLETE_COMPLEMENT));  
    operMap.push_back(make_pair("%=",COMPLETE_MOD));  
    operMap.push_back(make_pair("|",BYTE_OR));  
}  
void initLimitMapping()  
{  
    limitMap.clear();  
    limitMap.push_back(make_pair("(",LEFT_BRA));  
    limitMap.push_back(make_pair(")",RIGHT_BRA));  
    limitMap.push_back(make_pair("[",LEFT_INDEX));  
    limitMap.push_back(make_pair("]",RIGHT_INDEX));  
    limitMap.push_back(make_pair("{",L_BOUNDER));  
    limitMap.push_back(make_pair("}",R_BOUNDER));  
    limitMap.push_back(make_pair(".",POINTER));  
    limitMap.push_back(make_pair("#",JING));  
    limitMap.push_back(make_pair("_",UNDER_LINE));  
    limitMap.push_back(make_pair(",",COMMA));  
    limitMap.push_back(make_pair(";",SEMI));  
    limitMap.push_back(make_pair("'",SIN_QUE));  
    limitMap.push_back(make_pair("\"",DOU_QUE));  
}  
void initNode()  
{  
    normalHead = new NormalNode();  
    strcpy(normalHead->content,"");  
    strcpy(normalHead->describe,"");  
    normalHead->type = -1;  
    normalHead->addr = -1;  
    normalHead->line = -1;  
    normalHead->next = NULL;  
  
    errorHead = new ErrorNode();  
    strcpy(errorHead->content,"");  
    strcpy(errorHead->describe,"");  
    errorHead->line = -1;  
    errorHead->next = NULL;  
  
    idenHead = new IdentiferNode();  
    strcpy(idenHead->content,"");  
    strcpy(idenHead->describe,"");  
    idenHead->type = -1;  
    idenHead->addr = -1;  
    idenHead->line = -1;  
    idenHead->next = NULL;  
}  
  
void createNewNode(char * content,char *descirbe,int type,int addr,int line)  
{  
    NormalNode * p = normalHead;  
    NormalNode * temp = new NormalNode();  
  
    while(p->next!=NULL)  
    {  
        p = p->next;  
    }  
  
    strcpy(temp->content,content);  
    strcpy(temp->describe,descirbe);  
    temp->type = type;  
    temp->addr = addr;  
    temp->line = line;  
    temp->next = NULL;  
  
    p->next = temp;  
}  
void createNewError(char * content,char *descirbe,int type,int line)  
{  
    ErrorNode * p = errorHead;  
    ErrorNode * temp = new ErrorNode();  
  
    strcpy(temp->content,content);  
    strcpy(temp->describe,descirbe);  
    temp->type = type;  
    temp->line = line;  
    temp->next = NULL;  
    while(p->next!=NULL)  
    {  
        p = p->next;  
    }  
    p->next = temp;  
}  
//返回值是新的标志符的入口地址  
int createNewIden(char * content,char *descirbe,int type,int addr,int line)  
{  
    IdentiferNode * p = idenHead;  
    IdentiferNode * temp = new IdentiferNode();  
    int flag = 0;  
    int addr_temp = -2;  
    while(p->next!=NULL)  
    {  
        if(strcmp(content,p->next->content) == 0)  
        {  
            flag = 1;  
            addr_temp = p->next->addr;  
        }  
        p = p->next;  
    }  
    if(flag == 0)  
    {  
        addr_temp = ++static_iden_number;//用自增来模拟入口地址  
    }  
    strcpy(temp->content,content);  
    strcpy(temp->describe,descirbe);  
    temp->type = type;  
    temp->addr = addr_temp;  
    temp->line = line;  
    temp->next = NULL;  
    p->next = temp;  
    return addr_temp;  
}  
  
void printNodeLink()  
{  
    NormalNode * p = normalHead;  
    p = p->next;  
    cout<<"************************************分析表******************************"<<endl<<endl;  
    cout<<setw(30)<<"内容"<<setw(10)<<"描述"<<"\t"<<"种别码"<<"\t"<<"地址"<<"\t"<<"行号"<<endl;  
    while(p!=NULL)  
    {  
        if(p->type == IDENTIFER)  
        {  
            cout<<setw(30)<<p->content<<setw(10)<<p->describe<<"\t"<<p->type<<"\t"<<p->addr<<"\t"<<p->line<<endl;  
        }  
        else  
        {  
            cout<<setw(30)<<p->content<<setw(10)<<p->describe<<"\t"<<p->type<<"\t"<<"\t"<<p->line<<endl;  
        }  
        p = p->next;  
    }  
    cout<<endl<<endl;  
}  
/* 
错误种类： 
1.float表示错误 
2.double表示错误 
3.注释没有结束符 
4.字符串常量没有结束符 
5.字符常量没有结束符 
6.非法字符 
7.'('没有对应项 
8.预处理错误 
*/  
void printErrorLink()  
{  
    ErrorNode * p = errorHead;  
    p = p->next;  
    cout<<"************************************错误表******************************"<<endl<<endl;  
    cout<<setw(10)<<"内容"<<setw(30)<<"描述"<<"\t"<<"类型"<<"\t"<<"行号"<<endl;  
    while(p!=NULL)  
    {  
        cout<<setw(10)<<p->content<<setw(30)<<p->describe<<"\t"<<p->type<<"\t"<<p->line<<endl;  
        p = p->next;  
    }  
    cout<<endl<<endl;  
}  
//标志符表，有重复部分，暂不考虑  
void printIdentLink()  
{  
    IdentiferNode * p = idenHead;  
    p = p->next;  
    cout<<"************************************标志符表******************************"<<endl<<endl;  
    cout<<setw(30)<<"内容"<<setw(10)<<"描述"<<"\t"<<"种别码"<<"\t"<<"地址"<<"\t"<<"行号"<<endl;  
    while(p!=NULL)  
    {  
        cout<<setw(30)<<p->content<<setw(10)<<p->describe<<"\t"<<p->type<<"\t"<<p->addr<<"\t"<<p->line<<endl;  
        p = p->next;  
    }  
    cout<<endl<<endl;  
}  
int mystrlen(char * word)  
{  
    if(*word == '\0')  
    {  
        return 0;  
    }  
    else  
    {  
        return 1+mystrlen(word+1);  
    }  
}  
//预处理，处理头文件和宏定义  
void preProcess(char * word,int line)  
{  
    const char * include_temp = "include";  
    const char * define_temp = "define";  
    char * p_include,*p_define;  
    int flag = 0;  
    p_include = strstr(word,include_temp);  
    if(p_include!=NULL)  
    {  
        flag = 1;  
        int i;  
        for(i=7;;)  
        {  
            if(*(p_include+i) == ' ' || *(p_include+i) == '\t')  
            {  
                i++;  
            }  
            else  
            {  
                break;  
            }  
        }  
        createNewNode(p_include+i,HEADER_DESC,HEADER,-1,line);  
    }  
    else  
    {  
        p_define = strstr(word,define_temp);  
        if(p_define!=NULL)  
        {  
            flag = 1;  
            int i;  
            for(i=7;;)  
            {  
                if(*(p_define+i) == ' ' || *(p_define+i) == '\t')  
                {  
                    i++;  
                }  
                else  
                {  
                    break;  
                }  
            }  
            createNewNode(p_define+i,CONSTANT_DESC,MACRO_VAL,-1,line);  
        }  
    }  
    if(flag == 0)  
    {  
        createNewError(word,PRE_PROCESS_ERROR,PRE_PROCESS_ERROR_NUM,line);  
    }  
}  
  
void close()  
{  
    //delete idenHead;  
    //delete errorHead;  
    //delete normalHead;  
}  
  
int seekKey(char * word)  
{  
    for(int i=0; i<keyMap.size(); i++)  
    {  
        if(strcmp(word,keyMap[i].first) == 0)  
        {  
            return i+1;  
        }  
    }  
    return IDENTIFER;  
}  
  
void scanner()  
{  
    char filename[30];  
    char ch;  
    char array[30];//单词长度上限是30  
    char * word;  
    int i;  
    int line = 1;//行数  
  
  
    FILE * infile;  
    printf("请输入要进行语法分析的C语言程序：\n");  
    scanf("%s",filename);  
    infile = fopen(filename,"r");  
    while(!infile)  
    {  
        printf("打开文件失败！\n");  
        return;  
    }  
    ch = fgetc(infile);  
    while(ch!=EOF)  
    {  
  
        i = 0;  
        //以字母或者下划线开头,处理关键字或者标识符  
        if((ch>='A' && ch<='Z') || (ch>='a' && ch<='z') || ch == '_')  
        {  
            while((ch>='A' && ch<='Z')||(ch>='a' && ch<='z')||(ch>='0' && ch<='9') || ch == '_')  
            {  
                array[i++] = ch;  
                ch = fgetc(infile);  
            }  
            word = new char[i+1];  
            memcpy(word,array,i);  
            word[i] = '\0';  
            int seekTemp = seekKey(word);  
            if(seekTemp!=IDENTIFER)  
            {  
                createNewNode(word,KEY_DESC,seekTemp,-1,line);  
            }  
            else  
            {  
                int addr_tmp = createNewIden(word,IDENTIFER_DESC,seekTemp,-1,line);  
                createNewNode(word,IDENTIFER_DESC,seekTemp,addr_tmp,line);  
            }  
            fseek(infile,-1L,SEEK_CUR);//向后回退一位  
        }  
        //以数字开头，处理数字  
        else if(ch >='0' && ch<='9')  
        {  
            int flag = 0;  
            int flag2 = 0;  
            //处理整数  
            while(ch >='0' && ch<='9')  
            {  
                array[i++] = ch;  
                ch = fgetc(infile);  
            }  
            //处理float  
            if(ch == '.')  
            {  
                flag2 = 1;  
                array[i++] = ch;  
                ch = fgetc(infile);  
                if(ch>='0' && ch<='9')  
                {  
                    while(ch>='0' && ch<='9')  
                    {  
                        array[i++] = ch;  
                        ch = fgetc(infile);  
                    }  
                }  
                else  
                {  
                    flag = 1;  
                }  
  
                //处理Double  
                if(ch == 'E' || ch == 'e')  
                {  
                    array[i++] = ch;  
                    ch = fgetc(infile);  
                    if(ch == '+' || ch == '-')  
                    {  
                        array[i++] = ch;  
                        ch = fgetc(infile);  
                    }  
                    if(ch >='0' && ch<='9')  
                    {  
                        array[i++] = ch;  
                        ch = fgetc(infile);  
                    }  
                    else  
                    {  
                        flag = 2;  
                    }  
                }  
  
            }  
            word = new char[i+1];  
            memcpy(word,array,i);  
            word[i] = '\0';  
            if(flag == 1)  
            {  
                createNewError(word,FLOAT_ERROR,FLOAT_ERROR_NUM,line);  
            }  
            else if(flag == 2)  
            {  
                createNewError(word,DOUBLE_ERROR,DOUBLE_ERROR_NUM,line);  
            }  
            else  
            {  
                if(flag2 == 0)  
                {  
                    createNewNode(word,CONSTANT_DESC,INT_VAL,-1,line);  
                }  
                else  
                {  
                    createNewNode(word,CONSTANT_DESC,FLOAT_VAL,-1,line);  
                }  
            }  
            fseek(infile,-1L,SEEK_CUR);//向后回退一位  
        }  
        //以"/"开头  
        else if(ch == '/')  
        {  
            ch = fgetc(infile);  
            //处理运算符"/="  
            if(ch == '=')  
            {  
                createNewNode("/=",OPE_DESC,COMPLETE_DIV,-1,line);  
            }  
            //处理"/**/"型注释  
            else if(ch == '*')  
            {  
                ch =  fgetc(infile);  
                while(1)  
                {  
                    while(ch != '*')  
                    {  
                        if(ch == '\n')  
                        {  
                            line++;  
                        }  
                        ch = fgetc(infile);  
                        if(ch == EOF)  
                        {  
                            createNewError(_NULL,NOTE_ERROR,NOTE_ERROR_NUM,line);  
                            return;  
                        }  
                    }  
                    ch = fgetc(infile);  
                    if(ch == '/')  
                    {  
                        break;  
                    }  
                    if(ch == EOF)  
                    {  
                        createNewError(_NULL,NOTE_ERROR,NOTE_ERROR_NUM,line);  
                        return;  
                    }  
                }  
                createNewNode(_NULL,NOTE_DESC,NOTE1,-1,line);  
            }  
            //处理"//"型注释  
            else if(ch == '/')  
            {  
                while(ch!='\n')  
                {  
                    ch = fgetc(infile);  
                    if(ch == EOF)  
                    {  
                        createNewNode(_NULL,NOTE_DESC,NOTE2,-1,line);  
                        return;  
                    }  
                }  
                line++;  
                createNewNode(_NULL,NOTE_DESC,NOTE2,-1,line);  
                if(ch == EOF)  
                {  
                    return;  
                }  
            }  
            //处理除号  
            else  
            {  
                createNewNode("/",OPE_DESC,DIV,-1,line);  
            }  
        }  
        //处理常量字符串  
        else if(ch == '"')  
        {  
            createNewNode("\"",CLE_OPE_DESC,DOU_QUE,-1,line);  
            ch = fgetc(infile);  
            i = 0;  
            while(ch!='"')  
            {  
                array[i++] = ch;  
                if(ch == '\n')  
                {  
                    line++;  
                }  
                ch = fgetc(infile);  
                if(ch == EOF)  
                {  
                    createNewError(_NULL,STRING_ERROR,STRING_ERROR_NUM,line);  
                    return;  
                }  
            }  
            word = new char[i+1];  
            memcpy(word,array,i);  
            word[i] = '\0';  
            createNewNode(word,CONSTANT_DESC,STRING_VAL,-1,line);  
            createNewNode("\"",CLE_OPE_DESC,DOU_QUE,-1,line);  
        }  
        //处理常量字符  
        else if(ch == '\'')  
        {  
            createNewNode("\'",CLE_OPE_DESC,SIN_QUE,-1,line);  
            ch = fgetc(infile);  
            i = 0;  
            while(ch!='\'')  
            {  
                array[i++] = ch;  
                if(ch == '\n')  
                {  
                    line++;  
                }  
                ch = fgetc(infile);  
                if(ch == EOF)  
                {  
                    createNewError(_NULL,CHARCONST_ERROR,CHARCONST_ERROR_NUM,line);  
                    return;  
                }  
            }  
            word = new char[i+1];  
            memcpy(word,array,i);  
            word[i] = '\0';  
            createNewNode(word,CONSTANT_DESC,CHAR_VAL,-1,line);  
            createNewNode("\'",CLE_OPE_DESC,SIN_QUE,-1,line);  
        }  
        else if(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')  
        {  
            if(ch == '\n')  
            {  
                line++;  
            }  
        }  
        else  
        {  
            if(ch == EOF)  
            {  
                return;  
            }  
            //处理头文件和宏常量（预处理）  
            else if(ch == '#')  
            {  
                while(ch!='\n' && ch!=EOF)  
                {  
                    array[i++] = ch;  
                    ch = fgetc(infile);  
                }  
                word = new char[i+1];  
                memcpy(word,array,i);  
                word[i] = '\0';  
                preProcess(word,line);  
  
                fseek(infile,-1L,SEEK_CUR);//向后回退一位  
            }  
            //处理-开头的运算符  
            else if(ch == '-')  
            {  
                array[i++] = ch;  
                ch = fgetc(infile);  
                if(ch >='0' && ch<='9')  
                {  
                    int flag = 0;  
                    int flag2 = 0;  
                    //处理整数  
                    while(ch>='0' && ch<='9')  
                    {  
                        array[i++] = ch;  
                        ch = fgetc(infile);  
                    }  
                    //处理float  
                    if(ch == '.')  
                    {  
                        flag2 = 1;  
                        array[i++] = ch;  
                        ch = fgetc(infile);  
                        if(ch>='0' && ch<='9')  
                        {  
                            while(ch>='0' && ch<='9')  
                            {  
                                array[i++] = ch;  
                                ch = fgetc(infile);  
                            }  
                        }  
                        else  
                        {  
                            flag = 1;  
                        }  
                        //处理Double  
                        if(ch == 'E' || ch == 'e')  
                        {  
                            array[i++] = ch;  
                            ch = fgetc(infile);  
                            if(ch == '+' || ch == '-')  
                            {  
                                array[i++] = ch;  
                                ch = fgetc(infile);  
                            }  
                            if(ch >='0' && ch<='9')  
                            {  
                                array[i++] = ch;  
                                ch = fgetc(infile);  
                            }  
                            else  
                            {  
                                flag = 2;  
                            }  
                        }  
                    }  
                    word = new char[i+1];  
                    memcpy(word,array,i);  
                    word[i] = '\0';  
                    if(flag == 1)  
                    {  
                        createNewError(word,FLOAT_ERROR,FLOAT_ERROR_NUM,line);  
                    }  
                    else if(flag == 2)  
                    {  
                        createNewError(word,DOUBLE_ERROR,DOUBLE_ERROR_NUM,line);  
                    }  
                    else  
                    {  
                        if(flag2 == 0)  
                        {  
                            createNewNode(word,CONSTANT_DESC,INT_VAL,-1,line);  
                        }  
                        else  
                        {  
                            createNewNode(word,CONSTANT_DESC,FLOAT_VAL,-1,line);  
                        }  
                    }  
                    fseek(infile,-1L,SEEK_CUR);//向后回退一位  
                }  
                else if(ch == '>')  
                {  
                    createNewNode("->",OPE_DESC,ARROW,-1,line);  
                }  
                else if(ch == '-')  
                {  
                    createNewNode("--",OPE_DESC,SELF_SUB,-1,line);  
                }  
                else if(ch == '=')  
                {  
                    createNewNode("--",OPE_DESC,SELF_SUB,-1,line);  
                }  
                else  
                {  
                    createNewNode("-",OPE_DESC,SUB,-1,line);  
                    fseek(infile,-1L,SEEK_CUR);  
                }  
            }  
            //处理+开头的运算符  
            else if(ch == '+')  
            {  
                ch = fgetc(infile);  
                if(ch == '+')  
                {  
                    createNewNode("++",OPE_DESC,SELF_ADD,-1,line);  
                }  
                else if(ch == '=')  
                {  
                    createNewNode("+=",OPE_DESC,COMPLETE_ADD,-1,line);  
                }  
                else  
                {  
                    createNewNode("+",OPE_DESC,ADD,-1,line);  
                    fseek(infile,-1L,SEEK_CUR);  
                }  
            }  
            //处理*开头的运算符  
            else if(ch == '*')  
            {  
                ch = fgetc(infile);  
                if(ch == '=')  
                {  
                    createNewNode("*=",OPE_DESC,COMPLETE_MUL,-1,line);  
                }  
                else  
                {  
                    createNewNode("*",OPE_DESC,MUL,-1,line);  
                    fseek(infile,-1L,SEEK_CUR);  
                }  
            }  
            //处理按^开头的运算符  
            else if(ch == '^')  
            {  
                ch = fgetc(infile);  
                if(ch == '=')  
                {  
                    createNewNode("^=",OPE_DESC,COMPLETE_BYTE_XOR,-1,line);  
                }  
                else  
                {  
                    createNewNode("^",OPE_DESC,BYTE_XOR,-1,line);  
                    fseek(infile,-1L,SEEK_CUR);  
                }  
            }  
            //处理%开头的运算符  
            else if(ch == '%')  
            {  
                ch = fgetc(infile);  
                if(ch == '=')  
                {  
                    createNewNode("%=",OPE_DESC,COMPLETE_MOD,-1,line);  
                }  
                else  
                {  
                    createNewNode("%",OPE_DESC,MOD,-1,line);  
                    fseek(infile,-1L,SEEK_CUR);  
                }  
            }  
            //处理&开头的运算符  
            else if(ch == '&')  
            {  
                ch = fgetc(infile);  
                if(ch == '=')  
                {  
                    createNewNode("&=",OPE_DESC,COMPLETE_BYTE_AND,-1,line);  
                }  
                else if(ch == '&')  
                {  
                    createNewNode("&&",OPE_DESC,AND,-1,line);  
                }  
                else  
                {  
                    createNewNode("&",OPE_DESC,BYTE_AND,-1,line);  
                    fseek(infile,-1L,SEEK_CUR);  
                }  
            }  
            //处理~开头的运算符  
            else if(ch == '~')  
            {  
                ch = fgetc(infile);  
                if(ch == '=')  
                {  
                    createNewNode("~=",OPE_DESC,COMPLETE_COMPLEMENT,-1,line);  
                }  
                else  
                {  
                    createNewNode("~",OPE_DESC,COMPLEMENT,-1,line);  
                    fseek(infile,-1L,SEEK_CUR);  
                }  
            }  
            //处理!开头的运算符  
            else if(ch == '!')  
            {  
                ch = fgetc(infile);  
                if(ch == '=')  
                {  
                    createNewNode("!=",OPE_DESC,NOT_EQUAL,-1,line);  
                }  
                else  
                {  
                    createNewNode("!",OPE_DESC,NOT,-1,line);  
                    fseek(infile,-1L,SEEK_CUR);  
                }  
            }  
            //处理<开头的运算符  
            else if(ch == '<')  
            {  
                ch = fgetc(infile);  
                if(ch == '<')  
                {  
                    createNewNode("<<",OPE_DESC,LEFT_MOVE,-1,line);  
                }  
                else if(ch == '=')  
                {  
                    createNewNode("<=",OPE_DESC,LES_EQUAL,-1,line);  
                }  
                else  
                {  
                    createNewNode("<",OPE_DESC,LES_THAN,-1,line);  
                    fseek(infile,-1L,SEEK_CUR);  
                }  
            }  
            //处理>开头的运算符  
            else if(ch == '>')  
            {  
                ch = fgetc(infile);  
                if(ch == '>')  
                {  
                    createNewNode(">>",OPE_DESC,RIGHT_MOVE,-1,line);  
                }  
                else if(ch == '=')  
                {  
                    createNewNode(">=",OPE_DESC,GRT_EQUAL,-1,line);  
                }  
                else  
                {  
                    createNewNode(">",OPE_DESC,GRT_THAN,-1,line);  
                    fseek(infile,-1L,SEEK_CUR);  
                }  
            }  
            //处理|开头的运算符  
            else if(ch == '|')  
            {  
                ch = fgetc(infile);  
                if(ch == '|')  
                {  
                    createNewNode("||",OPE_DESC,OR,-1,line);  
                }  
                else  
                {  
                    createNewNode("|",OPE_DESC,BYTE_OR,-1,line);  
                    fseek(infile,-1L,SEEK_CUR);  
                }  
            }  
            else if(ch == '=')  
            {  
                ch = fgetc(infile);  
                if(ch == '=')  
                {  
                    createNewNode("==",OPE_DESC,EQUAL,-1,line);  
                }  
                else  
                {  
                    createNewNode("=",OPE_DESC,ASG,-1,line);  
                    fseek(infile,-1L,SEEK_CUR);  
                }  
            }  
            else if(ch == '(')  
            {  
                leftSmall++;  
                lineBra[0][leftSmall] = line;  
                createNewNode("(",CLE_OPE_DESC,LEFT_BRA,-1,line);  
            }  
            else if(ch == ')')  
            {  
                rightSmall++;  
                lineBra[1][rightSmall] = line;  
                createNewNode(")",CLE_OPE_DESC,RIGHT_BRA,-1,line);  
            }  
            else if(ch == '[')  
            {  
                leftMiddle++;  
                lineBra[2][leftMiddle] = line;  
                createNewNode("[",CLE_OPE_DESC,LEFT_INDEX,-1,line);  
            }  
            else if(ch == ']')  
            {  
                rightMiddle++;  
                lineBra[3][rightMiddle] = line;  
                createNewNode("]",CLE_OPE_DESC,RIGHT_INDEX,-1,line);  
            }  
            else if(ch == '{')  
            {  
                leftBig++;  
                lineBra[4][leftBig] = line;  
                createNewNode("{",CLE_OPE_DESC,L_BOUNDER,-1,line);  
            }  
            else if(ch == '}')  
            {  
                rightBig++;  
                lineBra[5][rightBig] = line;  
                createNewNode("}",CLE_OPE_DESC,R_BOUNDER,-1,line);  
            }  
            else if(ch == '.')  
            {  
                createNewNode(".",CLE_OPE_DESC,POINTER,-1,line);  
            }  
            else if(ch == ',')  
            {  
                createNewNode(",",CLE_OPE_DESC,COMMA,-1,line);  
            }  
            else if(ch == ';')  
            {  
                createNewNode(";",CLE_OPE_DESC,SEMI,-1,line);  
            }  
            else  
            {  
                char temp[2];  
                temp[0] = ch;  
                temp[1] = '\0';  
                createNewError(temp,CHAR_ERROR,CHAR_ERROR_NUM,line);  
            }  
        }  
        ch = fgetc(infile);  
    }  
}  
void BraMappingError()  
{  
    if(leftSmall != rightSmall)  
    {  
        int i = (leftSmall>rightSmall) ? (leftSmall-rightSmall) : (rightSmall - leftSmall);  
        bool  flag = (leftSmall>rightSmall) ? true : false;  
        if(flag)  
        {  
            while(i--)  
            {  
                createNewError(_NULL,LEFT_BRA_ERROR,LEFT_BRA_ERROR_NUM,lineBra[0][i+1]);  
            }  
        }  
        else  
        {  
            while(i--)  
            {  
                createNewError(_NULL,RIGHT_BRA_ERROR,RIGHT_BRA_ERROR_NUM,lineBra[1][i+1]);  
            }  
        }  
    }  
    if(leftMiddle != rightMiddle)  
    {  
        int i = (leftMiddle>rightMiddle) ? (leftMiddle-rightMiddle) : (rightMiddle - leftMiddle);  
        bool flag = (leftMiddle>rightMiddle) ? true : false;  
        if(flag)  
        {  
            while(i--)  
            {  
                createNewError(_NULL,LEFT_INDEX_ERROR,LEFT_INDEX_ERROR_NUM,lineBra[2][i+1]);  
            }  
        }  
        else  
        {  
            while(i--)  
            {  
                createNewError(_NULL,RIGHT_INDEX_ERROR,RIGHT_INDEX_ERROR_NUM,lineBra[3][i+1]);  
            }  
        }  
    }  
    if(leftBig != rightBig)  
    {  
        int i = (leftBig>rightBig) ? (leftBig-rightBig) : (rightBig - leftSmall);  
        bool flag = (leftBig>rightBig) ? true : false;  
        if(flag)  
        {  
            while(i--)  
            {  
                createNewError(_NULL,L_BOUNDER_ERROR,L_BOUNDER_ERROR_NUM,lineBra[4][i+1]);  
            }  
        }  
        else  
        {  
            while(i--)  
            {  
                createNewError(_NULL,R_BOUNDER_ERROR,R_BOUNDER_ERROR_NUM,lineBra[5][i+1]);  
            }  
        }  
    }  
}  
