#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "resource.h"
#include "color.h"


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
  case NOTHING: //Void
    return color_start(BLACK);
  case CORN: //Yellow
    return color_start(YELLOW);
  case FISH: //Blue
    return color_start(BLUE);
  case WOOD: //Brown
    return color_start(BROWN);
  case STONE: //Grey
    return color_start(WHITE);
  case GOLD: //Gold
    return color_start(YELLOW);
  case RUBY: //Red
    return color_start(RED);
  case SILVER: //Silver
    return color_start(BLACK);
  case SAPPHIRE: //Cyan
    return color_start(BLUE);
  case EMERALD: //Green
    return color_start(GREEN);
  default:
    return color_start(11);
  }
}

const char* resource_color_stop(){
    return color_stop();
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

void resource_sub(unsigned int r1[NUM_RESOURCES],
                  unsigned int r2[NUM_RESOURCES],
                  unsigned int res[NUM_RESOURCES]){
  int i=0;
  while (i<NUM_RESOURCES){
    res[i] = r1[i] - r2[i];
    ++i;
  }
}