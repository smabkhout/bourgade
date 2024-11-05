#include <stdio.h>

#include "color.h"


static const char *txt[MAX_COLORS] = {
    "Black", "White", "Orange", "Pink", "Violet", "Yellow", "Blue", "Brown", "Green","Red","Shiny"
};


const char* color_start(enum color_t c){
    switch (c)
    {
        case 0 : //Black
            return "\033[0;30m";
        case 1 : //White
            return "\033[0;37m";
        case 2 : //Orange
            return "\033[m";
        case 3 : //Pink
            return "\033[38;5;213m";
        case 4 : //Violet
            return "\033[38;5;129m";
        case 5 : //Yellow
            return "\033[0;33m";
        case 6 : //Blue
            return "\033[0;34m";
        case 7 : //Brown
            return "\033[38;5;94m";
        case 8 : //Green
            return "\033[0;32m";
        case 9 : //Red
            return "\033[0;31m";
        case 10 : //Shiny
            return "\033[1;97m";
        default :
            return "\033[0m";
    }
}

const char* color_stop(){
    return "\033[0m";
}

const char* color_to_string(enum color_t c){    
    if (c >= 0 && c < MAX_COLORS)
        return txt[c];
    return "UNKNOWN";
}

int main(int argc, char* argv[]){
    return 0;
}