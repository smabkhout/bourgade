#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "resource.h"
#include <string.h>

static const char *txt[MAX_RESOURCES] = {
  "Nothing", "Corn", "Fish", "Wood", "Stone", "Gold", "Ruby", "Silver", "Sapphire", "Emerald"
};


const char* resource_to_string(enum resource_t r){
  if (r >= 0 && r < MAX_RESOURCES)
    return txt[r];
  return "UNKNOWN";    
}

static const char *small_txt[MAX_RESOURCES] = {
  "N", "C", "F", "W", "S", "G", "R", "V", "P", "E"
};


const char* resource_to_short_string(enum resource_t r){
  if (r >= 0 && r < MAX_RESOURCES)
    return small_txt[r];
  return "U";    
}


const char* resource_color_start(enum resource_t r);





int main(int argc, char* argv[]){
    return 0;
}
