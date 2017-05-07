#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include "LexAnalysis.h"
#include "SynAnalysis.h"

using namespace std;

int main()
{
    //词法分析部分
    initKeyMapping();
    initOperMapping();
    initLimitMapping();
    initNode();
    scanner();
    BraMappingError();
    printNodeLink();

    printErrorLink();
    printIdentLink();

    //语法分析部分
    initGrammer();
    First();
    Follow();
    Select();
    MTable();
    Analysis();
    close();
    return 0;
}
