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


const char* resource_color_start(enum resource_t r){
  switch (r)
  {
  case 0: //Void
    return "\033[38;5;0m";
  case 1: //Yellow
    return "\033[38;5;11m";
  case 2: //Blue
    return "\033[38;5;12m";
  case 3: //Brown
    return "\033[38;5;94m";
  case 4: //Grey
    return "\033[38;5;8m";
  case 5: //Gold
    return "\033[38;5;172m";
  case 6: //Red
    return "\033[0;31m";
  case 7: //Silver
    return "\033[38;5;7m";
  case 8: //Cyan
    return "\033[38;5;14m";
  case 9: //Green
    return "\033[0;32m";
  default:
    break;
  }
}

const char* resource_color_stop(){
    return "\033[0m";
}

int resource_le_than(unsigned int r1[NUM_RESOURCES],
                     unsigned int r2[NUM_RESOURCES]){
  int i=0;
  while (i<NUM_RESOURCES){
    if  (r2[i] > r1[i])
      return 0;
    ++i;
  }
  return 1;
}

void resource_add(unsigned int r1[NUM_RESOURCES],
                  unsigned int r2[NUM_RESOURCES],
                  unsigned int res[NUM_RESOURCES]){
  int i=0;
  while (i<NUM_RESOURCES){
    res[i] = r1[i] + r2[i];
    ++i;
  }
}

void resource_add(unsigned int r1[NUM_RESOURCES],
                  unsigned int r2[NUM_RESOURCES],
                  unsigned int res[NUM_RESOURCES]){
  int i=0;
  while (i<NUM_RESOURCES){
    res[i] = r1[i] - r2[i];
    ++i;
  }
}
