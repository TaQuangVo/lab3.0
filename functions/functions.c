#include "functions.h"
#include <stdio.h>
#include <ncurses.h>
#include "../lib/lib.h"
#include <stdlib.h>//use of malloc



int handleArrowKeys(int listLength, int * currentIndex){
    int keypress = getch();

    if(keypress == 10){
        return *currentIndex;
    }else if(keypress == 27){
        getch();
        keypress = getch();
        switch (keypress)
        {
            case 68://left key
                return -1;
                break;
            case 67://right arrow
                if(listLength == 0){
                    return -99;
                }
                return *currentIndex;
            case 65: //up
                if(listLength == 0){
                    return -99;
                }
                *currentIndex = *currentIndex - 1;
                if(*currentIndex == -1){
                    *currentIndex = listLength -1;
                }
                return -2;
            case 66: //down
                if(listLength == 0){
                    return -99;
                }
                *currentIndex = *currentIndex + 1;
                if(*currentIndex == listLength){
                    *currentIndex = 0;
                }
                return -2;
            default:
                break;
        }
    }
    return -99;
}
void showMeny(Nav nav){
    for (int i = 0; i < nav.totalItems; i++)
    {
        if(nav.currentIndex != i){
            printw("\t%s\n",nav.titles[i]);
        }else{
            printw("     ->\t%s\n", nav.titles[i]);
        }
    }
}
void Register(Data * data){
    int fieldInputing = 0;//indicate which field user is entering; 0:name, 1:Massnr, 2:Htid
    int inputReturn; //-1:esc, 0:nting, 1:user pressed enter
    int isError = 0;
    int continues = 1;
    erase();
    while (continues)
    {
        //handle input & view
        move(0,0);
        printw("Home - Register\n\n");
        printw("Register a new substance #%d:\n\n", data->length + 1);
        if(fieldInputing == 0){
            printw("\tName: %s", data->substances[data->length].name.value);
            inputReturn = handleNameInput(&data->substances[data->length].name);
            if(isError){
                printw("\n\n\t%s","Input Can't Be Empty!!!");
            }
        }else if(fieldInputing == 1){
            printw("\tName: %s\n", data->substances[data->length].name.value);
            printw("\tMassnr: %s", data->substances[data->length].massNr.value);
            inputReturn = handleMassnrInput(&data->substances[data->length].massNr);
            if(isError){
                printw("\n\n\t%s","Input Can't Be Empty!!!");
            }
        }else if(fieldInputing == 2){
            printw("\tName: %s\n", data->substances[data->length].name.value);
            printw("\tMassnr: %s\n", data->substances[data->length].massNr.value);
            printw("\tHTid: %s", data->substances[data->length].Htid.value);
            inputReturn = handleHtidInput(&data->substances[data->length].Htid);
            if(isError){
                printw("\n\n%s","   !!! Input Formated IncorrectLy !!!");
            }
        }

        //handle inputreturn, if user entered a field?, entered a substance? or pressed esc to exit
        if(inputReturn == -1){
            break;
        }else if(inputReturn == 1){
            if( fieldInputing == 0){
                if(data->substances[data->length].name.length > 0){
                    fieldInputing = 1;
                    isError = 0;
                    erase();
                }else{
                    isError = 1;
                }
            }else if(fieldInputing == 1){
                if(data->substances[data->length].massNr.length > 0){
                    fieldInputing = 2;
                    isError = 0;
                    erase();
                }else{
                    isError = 1;
                }
            }else if (fieldInputing == 2){          
                double hTidInSecond = checkHTidFormat(data->substances[data->length].Htid);  
                if( hTidInSecond >= 0){
                    data->substances[data->length].HtidInSecond = hTidInSecond;
                    data->length = data->length + 1;
                    fieldInputing = 0;
                    isError = 0;
                    erase();
                }else{
                    isError = 1;
                }
            }  
        }
    }
}
void showAll(Data * data){
    int cursorIndex = 0;
    int userAction = -2;
    erase();
    while (1)
    {
        if(userAction == -2){
            move(0,0);
            printw("Home - Show All\n\n");
            printw("Choose a substance to make a change or delete!\n\n");
            showData(*data, cursorIndex);
        }else if (userAction == -1){
            break;
        }

        userAction = handleArrowKeys(data->length, &cursorIndex);

        if(userAction >= 0){
            handleModifyData(data , cursorIndex);
            userAction = -2;
        }
    }
}
void sortAfter(Data * data){
    Nav sortMeny;
    sortMeny.navName = "SortMeny";
    sortMeny.currentIndex = 0;
    sortMeny.totalItems = 3;
    sortMeny.titles = malloc(sizeof(char *) * sortMeny.totalItems);
    sortMeny.titles[0] = "Name";
    sortMeny.titles[1] = "MassNr";
    sortMeny.titles[2] = "H-l Time";

    int navOutPut = -2;
    erase();
    while (1)
    {
        if(navOutPut == -2){
            move(0,0);
            printw("Home - Sort After\n\n");
            printw("Choose What You Want To Sort After.\n\n");
            showMeny(sortMeny);
        }else if (navOutPut == -1){
            erase();
            break;
        }
        
        navOutPut = handleArrowKeys(sortMeny.totalItems, &sortMeny.currentIndex);

        if(navOutPut != -99){
            switch (navOutPut)
            {
            case 0:
                sortAfterField(data, 0);
                navOutPut = -2;
                break;
            case 1:
                sortAfterField(data, 1);
                navOutPut = -2;
                break;
            case 2:
                sortAfterField(data, 2);
                navOutPut = -2;
            default:
                break;
            }
        }
    }
}
void handleSearch(Data * data){
    Nav sortMeny;
    sortMeny.navName = "SearchMeny";
    sortMeny.currentIndex = 0;
    sortMeny.totalItems = 3;
    sortMeny.titles = malloc(sizeof(char *) * sortMeny.totalItems);
    sortMeny.titles[0] = "Name";
    sortMeny.titles[1] = "MassNr";
    sortMeny.titles[2] = "H-l Time";

    int navOutPut = -2;
    erase();
    while (1)
    {
        if(navOutPut == -2){
            move(0,0);
            printw("Home - Search After\n\n");
            printw("Choose What You Want To Search After.\n\n");
            showMeny(sortMeny);
        }else if (navOutPut == -1){
            erase();
            break;
        }
        
        navOutPut = handleArrowKeys(sortMeny.totalItems, &sortMeny.currentIndex);

        if(navOutPut != -99){
            switch (navOutPut)
            {
            case 0:
                searchByString(data, 0);
                navOutPut = -2;
                break;
            case 1:
                searchByString(data, 1);
                navOutPut = -2;
                break;
            case 2:
                searchByString(data, 2);
                navOutPut = -2;
            default:
                break;
            }
        }
    }
}
