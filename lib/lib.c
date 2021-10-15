#include "lib.h"
#include "../functions/functions.h"
#include <ncurses.h>
#include <string.h>//strcmp
#include <stdlib.h>//use of malloc



int isNumber (int ascii){
    if(ascii >= 47 && ascii <= 57 ){
        return 1;
    }else {
        return 0;
    }
}
int isAlfabet (int ascii){
    if((ascii >= 97 && ascii <= 122) 
    || (ascii >= 65 && ascii <= 90)){
        return 1;
    }else{
        return 0;
    }
}
int charToNr (char character){
    return (int)character - 48;
}
char toLowerCase (char character){
    if(character <= 90){
        return character + 32;
    }
    return character;
}
int compareString(String str1, String str2){
    int minLength = str2.length;
    if(str1.length < str2.length){
        minLength = str1.length;
    }
    for (int i = 0; i < minLength; i++)
    {
        int str1Ascii = (int)toLowerCase(str1.value[i]);
        int str2Ascii = (int)toLowerCase(str2.value[i]);
        if(str1Ascii < str2Ascii){
            return -1;
        }else if(str1Ascii > str2Ascii){
            return 1;
        }else if(str1Ascii == str2Ascii && i == minLength - 1){
            if(str1.length < str2.length){
                return -1;
            }else if (str1.length > str2.length){
                return 1;
            }
        }
    }
    return 0;
}
int stringToNumber(String str){
    int number = 0;
    int exp = 1;
    for (int i = str.length - 1; i >= 0; i--)
    {
        int digit = (int)str.value[i] - 48;
        number = (digit * exp) + number;
        exp *= 10;
    }
    return number;
}
void swapSubstancesPlace(Substance *a, Substance *b) {
    Substance t = *a;
    *a = *b;
    *b = t;
}
int partition(Substance substances[], int low, int high, int fieldComparing) {
    Substance pivot = substances[high];

    //finding index if pivot, and reoder smaller to the left
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (fieldComparing == 0) {
            int compareToPivot = compareString(substances[j].name, pivot.name);
            if(compareToPivot == -1 || compareToPivot == 0){
                i++;
                swapSubstancesPlace(&substances[i], &substances[j]);
            }
        } else if(fieldComparing == 1){
            if(stringToNumber(substances[j].massNr) <= stringToNumber(pivot.massNr)){
                i++;
                swapSubstancesPlace(&substances[i], &substances[j]);
            }
        }else if(fieldComparing == 2){
            if(substances[j].HtidInSecond <= pivot.HtidInSecond){
                i++;
                swapSubstancesPlace(&substances[i], &substances[j]);
            }
        }
    }
    swapSubstancesPlace(&substances[i + 1], &substances[high]);
    return i + 1;
}
void quickSortSubstances(Substance substances[], int low, int high, int sortField) {
    if (low < high) {

        int pi = partition(substances, low, high, sortField);

        quickSortSubstances(substances, low, pi - 1, sortField);
        quickSortSubstances(substances, pi + 1, high, sortField);
    }
}
double checkHTidFormat (String hTid){
    //Function convert Halv-life-time string into double in second
    //return value in second or negative number if format wrong;
    //Example: 12.345E3 dagar

    int number = 0;
    int deximal = -1;
    int exponent = -1;
    char unit[10];
    int unitIndex = -1;

    //serparate string to calculatable parts
    //12.345e3 dagar
    //number:12, deximal:345, exponent:3, unit:"dagar"
    for (int i = 0; i < hTid.length; i++)
    {
        if(isNumber(hTid.value[i]) && unitIndex == -1){
            if(deximal == -1 && exponent == -1 && unitIndex == -1){
                number = (number * 10) + charToNr(hTid.value[i]);
            }
            else if(deximal != -1 && exponent == -1 && unitIndex == -1){
                number = (number * 10) + charToNr(hTid.value[i]);
                deximal = deximal * 10;
            }else if(exponent != -1 && unitIndex == -1){
                exponent = (exponent * 10) + charToNr(hTid.value[i]);
            }else{
                return -99;
            }
        }else if(isAlfabet(hTid.value[i]) && unitIndex != -1){
            unit[unitIndex] = toLowerCase(hTid.value[i]);
            unitIndex = unitIndex + 1;
            unit[unitIndex] = '\0';
        }else if(hTid.value[i] == '.'){
            if(deximal == -1 && exponent == -1 && unitIndex == -1){
                deximal = 1;
            }else {
                return -98;
            }
        }else if(hTid.value[i] =='e' || hTid.value[i] == 'E'){
            if(exponent == -1 && unitIndex == -1 && number > 0){
                exponent = 0;
            }else {
                return -97;
            }
        }else if(hTid.value[i] == ' '  && number > 0){
            if(unitIndex == -1){
                unitIndex = 0;
            }else {
                return -96;
            }
        }else {
            return -95;
        }
    }

    //turn number, deximal and exponent into one double
    //number:12345, deximal:1000, exponent:3  --> 12345.0
    double hTidInSecond = number;
    if(deximal != -1){
       hTidInSecond = hTidInSecond / deximal;
    }
    if(exponent != -1){
        for(int i = 0; i < exponent; i++){
            hTidInSecond = hTidInSecond * 10;
        }
    }

    //convert all unit to second and return.
    //12345.0 dagar --> 1100000000 seconds
    if(strcmp(unit,"sekund") == 0|| strcmp(unit,"sekunder") == 0 || strcmp(unit,"second") == 0 || strcmp(unit,"seconds") == 0){
        return hTidInSecond;
    }else if(strcmp(unit,"minut") == 0 || strcmp(unit,"minuter") == 0 || strcmp(unit,"minute") == 0 || strcmp(unit,"minutes") == 0){
        return hTidInSecond * 60;
    }else if(strcmp(unit,"timmar") == 0 || strcmp(unit,"timma") == 0 || strcmp(unit,"hour") == 0 || strcmp(unit,"hours") == 0){
        return hTidInSecond * 60 * 60;
    }else if(strcmp(unit,"dagar") == 0 || strcmp(unit,"dag") == 0 || strcmp(unit,"days") == 0 || strcmp(unit,"day") == 0){
        return hTidInSecond * 60 * 60 * 24;
    }else if(strcmp(unit,"ar") == 0 || strcmp(unit,"year") == 0 || strcmp(unit,"years") == 0){
        return hTidInSecond * 60 * 60 * 24 * 365;
    }else {
        return -94;
    }
    
}
int handleNameInput (String * name){
    int keyPressed = getch();
    if(keyPressed == 27){ //esc
        getch();
        keyPressed = getch();
        if(keyPressed == 68)//left arrow key
        {
            return -1;
        }else if(keyPressed == 67){
            return -67;
        }else if(keyPressed == 65){
            return -65;
        }else if(keyPressed == 66){
            return -66;
        }
    }else if(isAlfabet(keyPressed) && name->length < MAXSTRINGLENGTH - 1)
    {
        name->value[name->length] = (char)keyPressed;
        name->length = name->length + 1;
        name->value[name->length] = '\0';
        return -2;
    }else if(keyPressed == 127 && name->length > 0)//handle delete key
    {
        name->length = name->length - 1;
        name->value[name->length] = '\0';
        erase();
        return -2;
    }else if(keyPressed == 10)//enter key
    {
        return 1;
    }
    return 0;
}
int handleMassnrInput (String * massNr){
    int keyPressed = getch();
    if(isNumber(keyPressed) && massNr->length < MAXSTRINGLENGTH -1)//handle numberic char
    {
        massNr->value[massNr->length] = (char)keyPressed;
        massNr->length = massNr->length + 1;
        massNr->value[massNr->length] = '\0';
        return -2;
    }else if(keyPressed == 127 && massNr->length > 0)//handle delete key
    {
        massNr->length = massNr->length - 1;
        massNr->value[massNr->length] = '\0';
        return -2;
        erase();
    }else if(keyPressed == 10)//enter key
    {
        return 1;
    }if(keyPressed == 27){ //esc
        getch();
        keyPressed = getch();
        if(keyPressed == 68)//left arrow key
        {
            return -1;
        }else if(keyPressed == 67){
            return -67;
        }else if(keyPressed == 65){
            return -65;
        }else if(keyPressed == 66){
            return -66;
        }
    }
    return 0;
}
int handleHtidInput (String * hTid){
    int keyPressed = getch();
    if(isNumber(keyPressed) && hTid->length <= MAXSTRINGLENGTH -1){
        hTid->value[hTid->length] = (char)keyPressed;
        hTid->length = hTid->length + 1;
        hTid->value[hTid->length] = '\0';
    }else if ((keyPressed == 46 || keyPressed == 32) && hTid->length <= MAXSTRINGLENGTH){
        //  46:'.'   32:' ' 
        hTid->value[hTid->length] = (char)keyPressed;
        hTid->length = hTid->length + 1;
        hTid->value[hTid->length] = '\0';
    }else if (keyPressed == 127 && hTid->length > 0){
        //127: 'del'
        hTid->length = hTid->length - 1;
        hTid->value[hTid->length] = '\0';
        erase();
    }else if(isAlfabet(keyPressed) && hTid->length <= MAXSTRINGLENGTH)
    //handle alfabetic char
    {
        hTid->value[hTid->length] = (char)keyPressed;
        hTid->length = hTid->length + 1;
        hTid->value[hTid->length] = '\0';
    }else if(keyPressed == 10)//enter key
    {
        return 1;
    }if(keyPressed == 27){ //esc
        getch();
        keyPressed = getch();
        if(keyPressed == 68)//left arrow key
        {
            return -1;
        }else if(keyPressed == 67){
            return -67;
        }else if(keyPressed == 65){
            return -65;
        }else if(keyPressed == 66){
            return -66;
        }
    }
    return 0;

}
void showData(Data data, int cursorIndex){
        //Show max 9 substance at a time;
        int showMax = 2;
        int iMin, iMax;

        iMin =  cursorIndex - showMax;
        iMax = cursorIndex + showMax;
        if(iMin < 0 && iMax > data.length){
            iMin = 0;
            iMax = data.length;
        }else if(iMin < 0){
            iMax = iMax - iMin;
            iMin = 0;
            if(iMax > data.length){
                iMax = data.length;
            }
        }else if(iMax > data.length){
            iMin = iMin - (iMax - data.length);
            iMax = data.length;
            if(iMin < 0){
                iMin = 0;
            }
        }

        printw("\tName\t\tMassnr\tH-L-time\n");
        if(iMin == 0){
            printw("\t--------------------------------\n");
        }else{
            printw("\t--------------more--------------\n");
        }

        if(data.length == 0){
            printw("\t\tNo data To show\n");
        }
        for (int i = iMin; i < iMax; i++)
        {
            if(cursorIndex != i){
                printw("\t%s\t\t%s\t%s\n", data.substances[i].name, data.substances[i].massNr, data.substances[i].Htid);
            }else{
                printw("     ->\t%s\t\t%s\t%s\n", data.substances[i].name, data.substances[i].massNr, data.substances[i].Htid);

            }
        }
        if(iMax == data.length){
            printw("\t--------------------------------\n");
        }else{
            printw("\t--------------more--------------\n");
        }
    
}
void changeHLTime(Data * data, int index){
    String newData;
    newData.value[0] = '\0';
    newData.length = 0;
    erase();
    while (1)
    {
        move(0,0);
        printw("Home - Modify - Change H-l Time\n\n");
        printw("Change Substance H-l Time:\n\n");
        printw("\tOld Data: %s-%s   %s\n", data->substances[index].name.value, data->substances[index].massNr.value, data->substances[index].Htid.value);
        printw("\tNew Data: %s-%s   %s", data->substances[index].name.value, data->substances[index].massNr.value, newData.value);
        int output = handleHtidInput(&newData);

        if(output == -1){
            erase();
            break;
        }else if(output == 1){
            double hTidInSecond = checkHTidFormat(newData);
            if(hTidInSecond >= 0){
                data->substances[index].Htid = newData;
                data->substances[index].HtidInSecond = hTidInSecond;
                newData.length = 0;
                newData.value[0] = '\0';
                erase();
            }
        }
    }
    
}
void deleteSubstance(Data * data, int index){
    for (int i = index; i < data->length; i++){
        data->substances[i] = data->substances[i+1];
        data->substances[i].index = i;
    }
    data->length = data->length - 1;
}
void handleDeleteSubstance(Data * data, int index){
    int isDeleted = 0;
    int isRunning = 1;
    erase();
    while (isRunning)
    {
        if(!isDeleted){
            move(0,0);
            printw("Home - Modify - Delete Substance\n\n");
            printw("Substance To Be Detete: %s-%s\n\n", data->substances[index].name.value, data->substances[index].massNr.value);
            printw("This substance will be permantly deleted, \nPress 'x' to confirm!");
        }else{
            move(0,0);
            printw("Home - Modify - Delete Substance\n\n");
            printw("Substance Deleted Successfully, Left Arror To Quit!");
        }

        int keypressed = getch();
        switch (keypressed){
        case 68:
            erase();
            isRunning = 0;
            break;
        case (int)'x':
            if(!isDeleted){
                deleteSubstance(data, index);
                isDeleted = 1;
                erase();
            }
        default:
            break;
        }
    }
}
void handleModifyData(Data * data, int index){
    Nav modifyNav;
    modifyNav.totalItems = 2;
    modifyNav.currentIndex = 0;
    modifyNav.navName = "Modify data";
    modifyNav.titles = malloc(sizeof(char *) * 2);
    modifyNav.titles[0] = "Change H-l Time";
    modifyNav.titles[1] = "Delete Substance";

    int navOutput = -2;
    erase();
    int isRunning = 1;
    while (isRunning)
    {
        if(navOutput == -2){
            move(0,0);
            printw("Home - Modify\n\n");
            printw("Modifying:  %s-%s\n\n", data->substances[index].name, data->substances[index].massNr);
            showMeny(modifyNav);
        }else if(navOutput == -1){
            erase();
            break;
        }

        navOutput = handleArrowKeys(modifyNav.totalItems, &modifyNav.currentIndex);
        
        if(navOutput != -99){
            switch (navOutput)
            {
            case 0:
                changeHLTime(data, index);
                isRunning = 0;
                break;
            case 1:
                handleDeleteSubstance(data, index);
                isRunning = 0;
                break;
            default:
                break;
            }
        }
    }
    free(modifyNav.titles);
}
void sortAfterField(Data * data, int sortField){
    Data sortedData = *data;
    int cursorIndex = 0;
    int userAction = -2;
    quickSortSubstances(sortedData.substances, 0, sortedData.length - 1, sortField);
    erase();
    while (1)
    {
        if(userAction == -2){
            move(0,0);
            if(sortField == 0){
                printw("Home - Sort After - Name\n\n");
            }else if(sortField == 1){
                printw("Home - Sort After - MassNr\n\n");
            }else if(sortField == 2){
                printw("Home - Sort After - H-l Time\n\n");
            }
            printw("Choose a substance to make a change or delete!\n\n");
            showData(sortedData, cursorIndex);
        }else if(userAction == -1){
            erase();
            break;
        }

        userAction = handleArrowKeys(sortedData.length, &cursorIndex);

        if(userAction >= 0){
            handleModifyData(data, sortedData.substances[cursorIndex].index);
            if(data->length == sortedData.length){
                sortedData.substances[cursorIndex] = data->substances[sortedData.substances[cursorIndex].index];
            }else if(data->length == sortedData.length -1){
                deleteSubstance(&sortedData, cursorIndex);
            }
            userAction = -2;
        }
    }
}
void search(Data data, Data * searchReturn, String searchStr, int searchField){
    searchReturn->length = 0;

    if(searchField == 2){
        double HtidInS = checkHTidFormat(searchStr);
        if(HtidInS < 0){
            return;
        }
        for (int i = 0; i < data.length; i++)
        {
            if(data.substances[i].HtidInSecond == HtidInS){
                searchReturn->substances[searchReturn->length] = data.substances[i];
                searchReturn->length = searchReturn->length + 1;
            }
        }
        
    }else {
        for (int i = 0; i < data.length; i++){
            char * str1;
            if(searchField == 0){
                str1 = data.substances[i].name.value;
            }else if(searchField == 1){
                str1 = data.substances[i].massNr.value;
            }
            char * str2 = searchStr.value;
            if(strstr(str1, str2) != NULL){
                searchReturn->substances[searchReturn->length] = data.substances[i];
                searchReturn->length = searchReturn->length + 1;
            }
        }
    }

    
}
void searchByString(Data * data, int searchField){
    Data searchReturn;
    searchReturn = *data;
    String input;
    input.length = 0;
    input.value[0] = '\0';
    int cursorIndex = 0;
    int userAction = -2;

    erase();
    while (1)
    {
        //userAction = handleArrowKeys(searchReturn.length, &cursorIndex);
        if(searchField == 0){
            move(0,0);
            printw("Home - Search After - Name\n\n");
            printw("Type to search.\n\n");
            printw("Name: %s\n\n", input.value);
            showData(searchReturn, cursorIndex);
            userAction = handleNameInput(&input);
        }else if(searchField == 1){
            move(0,0);
            printw("Home - Search After - MassNr\n\n");
            printw("Type to search.\n\n");
            printw("MassNr: %s\n\n", input.value);
            showData(searchReturn, cursorIndex);
            userAction = handleMassnrInput(&input);
        }else if(searchField == 2){
            move(0,0);
            printw("Home - Search After - Hl Time\n\n");
            printw("Type And Press \"Enter\" to search.\n\n");
            printw("Hl Time: %s\n\n", input.value);
            showData(searchReturn, cursorIndex);
            userAction = handleHtidInput(&input);
        }
        
        if(userAction == -2 || userAction == 1){
            search(*data, &searchReturn,input,searchField);
            if(cursorIndex < 0){
                cursorIndex = searchReturn.length - 1;
            }else if(cursorIndex >= searchReturn.length){
                cursorIndex = 0;
            }
            erase();
        }else if(userAction == -1){
            erase();
            break;
        }else if(userAction == -65){
            cursorIndex = cursorIndex - 1;
            if(cursorIndex < 0){
                cursorIndex = searchReturn.length - 1;
            }
        }else if(userAction == -66){
            cursorIndex = cursorIndex + 1;
            if(cursorIndex >= searchReturn.length){
                cursorIndex = 0;
            }
        }else if(userAction == -67){
            handleModifyData(data, searchReturn.substances[cursorIndex].index);
            if(data->length == searchReturn.length){
                searchReturn.substances[cursorIndex] = data->substances[searchReturn.substances[cursorIndex].index];
            }else if(data->length == searchReturn.length -1){
                deleteSubstance(&searchReturn, cursorIndex);
            }
            userAction = -2;
        }
    }
}
