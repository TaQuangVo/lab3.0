#ifndef LIB_H
#define LIB_H
#include "../functions/functions.h"

int handleNameInput (String * name);
int handleMassnrInput (String * massNr);
int handleHtidInput (String * hTid);
double checkHTidFormat (String hTid);
int handleChooseSubstance(int listLength, int * currentIndex);
void showData(Data data, int cursorIndex);
void handleModifyData(Data * data, int index);
void sortAfterField(Data * data, int sortField);
void searchByString(Data * data, int searchField);

#endif