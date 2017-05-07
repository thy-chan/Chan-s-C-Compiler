#ifndef _LEXANALYSIS_H  
#define _LEXANALYSIS_H  
  
//关键字  
#define AUTO 1  
#define BREAK 2  
#define CASE 3  
#define CHAR 4  
#define CONST 5  
#define CONTINUE 6  
#define DEFAULT 7  
#define DO 8  
#define DOUBLE 9  
#define ELSE 10  
#define ENUM 11  
#define EXTERN 12  
#define FLOAT 13  
#define FOR 14  
#define GOTO 15  
#define IF 16  
#define INT 17  
#define LONG 18  
#define REGISTER 19  
#define RETURN 20  
#define SHORT 21  
#define SIGNED 22  
#define SIZEOF 23  
#define STATIC 24  
#define STRUCT 25  
#define SWITCH 26  
#define TYPEDEF 27  
#define UNION 28  
#define UNSIGNED 29  
#define VOID 30  
#define VOLATILE 31  
#define WHILE 32  
#define KEY_DESC "关键字"  
  
//标志符  
#define IDENTIFER 40  
#define IDENTIFER_DESC "标志符"  
  
//常量  
#define INT_VAL 51 //整形常量  
#define CHAR_VAL 52 //字符常量  
#define FLOAT_VAL 53 //浮点数常量  
#define STRING_VAL 54 //双精度浮点数常量  
#define MACRO_VAL 55 //宏常量  
#define CONSTANT_DESC "常量"  
  
//运算符  
#define NOT 61   // !  
#define BYTE_AND 62 //&  
#define COMPLEMENT 63 // ~  
#define BYTE_XOR  64 // ^  
#define MUL 65 // *  
#define DIV 66// /  
#define MOD 67 // %  
#define ADD 68 // +  
#define SUB 69 // -  
#define LES_THAN 70 // <  
#define GRT_THAN 71 // >  
#define ASG 72 // =  
#define ARROW 73 // ->  
#define SELF_ADD 74 // ++  
#define SELF_SUB 75 // --  
#define LEFT_MOVE 76 // <<  
#define RIGHT_MOVE 77 // >>  
#define LES_EQUAL 78 // <=  
#define GRT_EQUAL 79 // >=  
#define EQUAL 80 // ==  
#define NOT_EQUAL 81 // !=  
#define AND 82 // &&  
#define OR 83 // ||  
#define COMPLETE_ADD 84 // +=  
#define COMPLETE_SUB 85 // -=  
#define COMPLETE_MUL 86 // *=  
#define COMPLETE_DIV 87 // /=  
#define COMPLETE_BYTE_XOR 88 // ^=  
#define COMPLETE_BYTE_AND 89 // &=  
#define COMPLETE_COMPLEMENT 90 // ~=  
#define COMPLETE_MOD 91 //%=  
#define BYTE_OR 92 // |  
  
#define OPE_DESC "运算符"  
  
//限界符  
#define LEFT_BRA 100 // (  
#define RIGHT_BRA 101 // )  
#define LEFT_INDEX 102 // [  
#define RIGHT_INDEX 103 // ]  
#define L_BOUNDER 104 //  {  
#define R_BOUNDER 105 // }  
#define POINTER 106 // .  
#define JING 107 // #  
#define UNDER_LINE 108 // _  
#define COMMA 109 // ,  
#define SEMI 110 // ;  
#define SIN_QUE 111 // '  
#define DOU_QUE 112 // "  
  
#define CLE_OPE_DESC "限界符"  
  
#define NOTE1 120 // "/**/"注释  
#define NOTE2 121 // "//"注释  
#define NOTE_DESC "注释"  
  
  
#define HEADER 130 //头文件  
#define HEADER_DESC "头文件"  
  
//错误类型  
#define FLOAT_ERROR "float表示错误"  
#define FLOAT_ERROR_NUM 1  
#define DOUBLE_ERROR "double表示错误"  
#define DOUBLE_ERROR_NUM 2  
#define NOTE_ERROR "注释没有结束符"  
#define NOTE_ERROR_NUM 3  
#define STRING_ERROR "字符串常量没有结束符"  
#define STRING_ERROR_NUM 4  
#define CHARCONST_ERROR "字符常量没有结束符"  
#define CHARCONST_ERROR_NUM 5  
#define CHAR_ERROR "非法字符"  
#define CHAR_ERROR_NUM 6  
#define LEFT_BRA_ERROR "'('没有对应项"  
#define LEFT_BRA_ERROR_NUM 7  
#define RIGHT_BRA_ERROR "')'没有对应项"  
#define RIGHT_BRA_ERROR_NUM 8  
#define LEFT_INDEX_ERROR "'['没有对应项"  
#define LEFT_INDEX_ERROR_NUM 9  
#define RIGHT_INDEX_ERROR "']'没有对应项"  
#define RIGHT_INDEX_ERROR_NUM 10  
#define L_BOUNDER_ERROR "'{'没有对应项"  
#define L_BOUNDER_ERROR_NUM 11  
#define R_BOUNDER_ERROR "'}'没有对应项"  
#define R_BOUNDER_ERROR_NUM 12  
#define PRE_PROCESS_ERROR "预处理错误" //头文件或者宏定义错误  
#define PRE_PROCESS_ERROR_NUM  13  
  
#define _NULL "无"  
  
#define DESCRIBE 4000  
#define TYPE 4001  
#define STRING 4002  
#define DIGIT 4003  
  
struct NormalNode  
{  
    char content[30];//内容  
    char describe[30];//描述  
    int type;//种别码  
    int addr;//入口地址  
    int line;//所在行数  
    NormalNode * next;//下一个节点  
};  
  
void initKeyMapping();  
void initOperMapping();  
void initLimitMapping();  
  
void initNode();  
void createNewNode(char * content,char *descirbe,int type,int addr,int line);  
void createNewError(char * content,char *descirbe,int type,int line);  
int createNewIden(char * content,char *descirbe,int type,int addr,int line);  
void printNodeLink();  
void printErrorLink();  
void printIdentLink();  
int mystrlen(char * word);  
void preProcess(char * word,int line);  
void close();  
int seekKey(char * word);  
void scanner();  
void BraMappingError();  
  
  
#endif 
