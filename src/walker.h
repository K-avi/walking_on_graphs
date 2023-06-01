#ifndef WALKER_H
#define WALKER_H 

#include "common.h"
#include "graph.h"
#include <stdint.h>
#include <stdio.h>

//might be irrelevant 


typedef struct s_walker{
    uint32_t id; 
    uint32_t curgen;
    //what does a walker need to know ? good question. 
}Walker;

typedef struct s_warray{
    
    uint32_t size; 
    Walker *array;

}WalkerArray ; //where the actual walkers will be stored and initialised and so on


typedef struct s_walker_table_entry{
    uint32_t capa; 
    uint32_t curr_in; 
    Walker ** walkers; //references to the walker guys maybe I dunno 
}WalkerTableEntry ; //deletion w swap ????
//confused as to how to update this

typedef struct s_walker_table{
//table indexed by node key containing the walkers at a specific key

    uint32_t size; //fixed size array cuz the size is the number of entries (possibly mod something ?)
    WalkerTableEntry * table;

}WalkerTable;



uint8_t initWalkerArray( WalkerArray * wArray, uint32_t size);
void freeWalkerArray(WalkerArray * wArray);



uint8_t initWalkerTable( WalkerTable * walkerTab,  uint32_t size);
void freeWalkerTable( WalkerTable * walkerTab);

void printWalkerTable( WalkerTable * walkerTab, FILE * stream);

uint8_t initPos( WalkerTable * walkerTab , WalkerArray * walkerArr );

#endif