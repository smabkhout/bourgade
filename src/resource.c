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
  case 0:
    return "Void";
  case 1:
    return "Yellow";
  case 2:
    return "Blue";
  case 3:
    return "Brown";
  case 4:
    return "Grey";
  case 5:
    return "Gold";
  case 6:
    return "Red";
  case 7:
    return "Silver";
  case 8:
    return "Cyan";
  case 9:
    return "Green";
  default:
    return "Unknown";
  }
}



