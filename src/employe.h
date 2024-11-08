#include "resource.h"

#ifndef MAX_WORKERS_PER_PLAYER
    #define MAX_WORKERS_PER_PLAYER 6
#endif

struct employe_t {
    char* nom;
    enum resource_t* cout;
};

const char* name_employe(struct employe_t worker){
    return worker.nom;
}

enum resource_t* cost_employe(struct employe_t worker){
    return worker.cout;
}
