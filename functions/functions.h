#ifndef FUNCTION_H_
#define FUNCTION_H_

#define MAXSTRINGLENGTH 20

typedef struct {
    char ** titles;
    int currentIndex;
    int totalItems;
    char * navName;
}Nav;

typedef struct
{
    char value[MAXSTRINGLENGTH];
    int length;
} String;
typedef struct{
    int index;
    String name;
    String massNr;
    String Htid;
    double HtidInSecond;
}Substance;
typedef struct  {
    Substance substances [1000];
    int length;
}Data;



int handleArrowKeys(int listLength, int * currentIndex);
void showMeny(Nav nav);
void Register(Data * data);
void showAll(Data * data);
void sortAfter(Data * data);
void handleSearch(Data * data);

#endif