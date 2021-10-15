#include <stdio.h>
#include <stdlib.h>//use of malloc
#include <ncurses.h>
#include "./functions/functions.h"


// run command 1: gcc main.c ./functions/functions.c ./lib/lib.c -lncurses -o main.exe
// run command 1: ./main.exe

int main () {
    Nav homeNav;
    //define nav info
    homeNav.titles = malloc( sizeof(char*) * 4 );
    homeNav.titles[0] = "1. Register";
    homeNav.titles[1] = "2. Show all";
    homeNav.titles[2] = "3. Sort after";
    homeNav.titles[3] = "4. Search after";
    homeNav.totalItems = 4;
    homeNav.currentIndex = 0;
    homeNav.navName = "HomeNav";
    
    Data data;
    //initial data;
    data.length = 0;
    for (int i = 0; i < 1000; i++)
    {
        data.substances[i].index = i;
        data.substances[i].Htid.value[0]= '\0';
        data.substances[i].Htid.length= 0;
        data.substances[i].name.value[0]= '\0';
        data.substances[i].name.length= 0;
        data.substances[i].massNr.value[0]= '\0';
        data.substances[i].massNr.length= 0;
    }
    

    initscr();
    noecho();
    timeout(0);
    int navOutPut = -2;
    while (1)
    {   

        if(navOutPut == -2){
            move(0,0);
            printw("Home\n\n");
            printw("Use Arrow keys to navigate!\n\n");
            showMeny(homeNav);
        }
        //handleNavInput return index of the nav user had chosen, -2 if nav updated, else return -99;
        navOutPut = handleArrowKeys(homeNav.totalItems, &homeNav.currentIndex);
        
        if(navOutPut != -99 && navOutPut != -2){
            switch (navOutPut)
            {
            case 0:
                Register(&data);
                navOutPut = -2;
                erase();
                break;
            case 1:
                showAll(&data);
                navOutPut = -2;
                erase();
                break;
            case 2:
                sortAfter(&data);
                navOutPut = -2;
                erase();
                break;
            case 3:
                handleSearch(&data);
                navOutPut = -2;
                erase();
                break;
            default:
                break;
            }
        }
    }

    free(homeNav.titles);
    return 0;
}
