#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#define SIZE 10240 //默认大小

int token;            // 当前标记
char *src;         // 指向源代码字符串指针
int poolsize;         // 默认文本/数据大小
int line;        // 源码行号

char *keywords[26]={
    "main",   "if",       "then",   "while",  "do",     "static", "defualt",
    "do",     "int",      "double", "struct", "break",  "else",   "long",
    "swtich", "case",     "typedf", "char",   "return", "const",  "float",
    "short",  "continue", "for",    "void",   "sizeof"
};
//能够支持的标记符
enum{
    Num = 128, Fun, Sys, Glo, Loc, Id,
    Char, Else, Enum, If, Int, Return, Sizeof, While,
    Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak,
    Var, Separator, Keyw, Other
};

typedef struct node{
    int key;
    int type;
    char value[12];
    struct node *next;
}node;

//用于词法分析，获取下一个标记，它将自动忽略空白字符。
struct node* next(struct node *p){
    int key=0,type,i,j;
    char value[12];
    for(i=0;i<12;i++){
        value[i]=0;
    }
    while(1){   //跳过不识别的字符以及处理空白字符
        token = *src;
        src++;
        if (token == '\n'){    //处理换行符,当前的行号加一
            ++line;
        }
        else if(token==' '){
            if(*src==' '){
                src++;
            }
        }
        else if (token == '#'){   //处理宏定义
        // skip macro, because we will not support it
            while (*src != 0 && *src != '\n'){
                src++;
            }
        }
        else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_')){
            // 解析标识符
            i = 0;
            value[i]=token;
            while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_')){
                value[i+1] = *src;
                src++;
                i++;
            }
            value[i+1] = '\0';
            //查询已有的标识符
            for (j = 0; j < 26; j++){
                if (strcmp(value, keywords[j]) == 0){
                    key = j + 1;
                    type=Var;
                    break;
                }
            }
            if(type!=Keyw){
                key=39;
                type=Var;
            }
            break;
        }
        //数字
        //在ASCII码中，字符a对应的十六进制值是 61, A是41，故通过 (token & 15) 可以得到个位数的值。
        else if (token >= '0' && token <= '9'){
            int num;
            num = token - '0';
            value[0]=token;
            if (num > 0){
                for (i=1;*src >= '0' && *src <= '9';i++){
                    value[i]=*src;
                    num = num * 10 + *src++ - '0';
                }
            }
            else{
                num='0';
            }
            key = 40;
            type = Num;
            value[i]='\0';
            break;
        }
        //注释
        else if (token == '/'){
            if (*src == '/'){
                // skip comments
                while (*src != 0 && *src != '\n'){
                    src++;
                }
            }
            else{
                // divide operator
                key=44;
                type = Div;
                value[0]=token;
                break;
            }
        }
        //其他
        else if (token == '+'){
            // parse '+' and '++'
            if (*src == '+'){
                src ++;
                key=45;
                type = Inc;
                value[0]=value[1]='+';
                break;
            }
            else{
                key=41;
                type = Add;
                value[0]='+';
                break;
            }
        }
        else if (token == '-'){
            // parse '-' and '--'
            if (*src == '-'){
                src ++;
                key=46;
                type = Dec;
                value[0]=value[1]='-';
                break;
            }
            else{
                key=42;
                type = Sub;
                value[0]='-';
                break;
            }
        }
        else if (token == '*'){
            key=43;
            type = Mul;
            value[0]='*';
            break;
        }
        else if (token == '='){
            // parse '==' and '='
            if (*src == '='){
                src ++;
                type = Eq;
                value[0]=value[1]='=';
                break;
            }
            else{
                key=47;
                type = Assign;
                value[0]='=';
                break;
            }
        }
        else if (token == '!'){
            // parse '!='
            if (*src == '='){
                src++;
                key=48;
                type = Ne;
                value[0]='!';
                value[1]='=';
                break;
            }
        }
        else if (token == '<'){
            // parse '<=', '<<' or '<'
            if (*src == '='){
                src ++;
                key=49;
                type = Le;
                value[0]='<';
                value[1]='=';
                break;
            }
            else if (*src == '<'){
                src ++;
                key=50;
                type = Shl;
                value[0]=value[1]='<';
                break;
            }
            else{
                key=51;
                type = Lt;
                value[0]='<';
                break;
            }
        }
        else if (token == '>'){
            // parse '>=', '>>' or '>'
            if (*src == '='){
                src ++;
                key=52;
                type = Ge;
                value[0]='>';
                value[1]='=';
                break;
            }
            else if (*src == '>'){
                src ++;
                key=53;
                type = Shr;
                value[0]=value[1]='>';
                break;
            }
            else{
                key=54;
                type = Gt;
                value[0]='>';
                break;
            }
        }
        else if (token == '|'){
            // parse '|' or '||'
            if (*src == '|'){
                src ++;
                key=55;
                type = Lor;
                value[0]=value[1]='|';
                break;
            }
            else{
                key=56;
                type = Or;
                value[0]='|';
                break;
            }
        }
        else if (token == '&'){
            // parse '&' and '&&'
            if (*src == '&'){
                src ++;
                key=58;
                type = Lan;
                value[0]=value[1]='&';
                break;
            }
            else{
                key=59;
                type = And;
                value[0]='&';
                break;
            }
        }
        else if (token == '^'){
            key=60;
            type = Xor;
            value[0]='^';
            break;
        }
        else if (token == '%'){
            key=61;
            type = Mod;
            value[0]='%';
            break;
        }
        else if (token == '['){
            key=62;
            type = Brak;
            value[0]='[';
            break;
        }
        else if (token == '?'){
            key=63;
            type = Cond;
            value[0]='?';
            break;
        }
        else if(token == '~' || token == ';' || token == '{' || token == '}' || token == '(' || token == ')' || token == ']' || token == ',' || token == ':'|| token == '"' || token =='\\'){
            key=64;
            type=Separator;
            value[0]=token;
            break;
        }
	else if(token==0)
	  break;
        else {
            printf("Error,第%d行出现错误!\n",line);
            key = 65;
            type =Other;
            value[0]=token;
            break;
        }
    }
    p->key = key;
    for(i=0;i<12;i++){
        p->value[i] = value[i];
    }
    p->type = type;
    node *q = (node *)malloc(sizeof(node));
    q->next=NULL;
    p->next = q;
    return p;
}

//语法分析的入口，分析整个 C 语言程序。
void program(struct node *p, FILE *fd2){
    printf("词法分析产生的词法二元式为: \n");
    p=next(p);                  // 获取下一个标记
    while (token > 0){
        printf("< %d , %s >\n", p->key, p->value);
        fprintf(fd2,"< %d , %s >\n", p->key, p->value);
        p=p->next;
        next(p);
    }
    printf("\n\n数据已写入output.txt\n\n");
}

//主函数
int main(void){
    int i;
    FILE *fd1,*fd2;
    char path[50];
    struct node *head;

    head = (node *)malloc(sizeof(node));
    head->next = NULL;

    printf("词法分析器(chan)\n"
           "请输入源语言文件绝对路径： ");
    scanf("%s", &path);

    if ((fd1 = fopen(path, "r")) == NULL){
        printf("could not open(%s)\n", path);
        return -1;
    }
    if ((fd2 = fopen(".\\output.txt", "w")) == NULL){
        printf("could not open(%s)\n", path);
        return -1;
    }

    if (!(src = (char *)malloc(SIZE))){
        printf("could not malloc(%d) for source area\n", poolsize);
        return -1;
    }
    // 读取源文件
    if ((i = fread(src,1,SIZE,fd1)) <= 0){
        printf("read() returned %d\n", i);
        return -1;
    }
    src[i] = 0; // 添加EOF字符
    fclose(fd1);
    program(head,fd2);
    fclose(fd2);
    return 0;
}
