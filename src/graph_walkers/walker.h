// walking_on_graphs: A simulation of .... walkers on graphs
// Copyright (C) 2023 Ivan Mulot-Radojcic 
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 or later.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details: https://www.gnu.org/licenses/

#ifndef WALKER_H
#define WALKER_H 

#include "../common.h"

//might be irrelevant 

typedef struct s_graph_table_entry GraphTableEntry;
typedef struct s_line Line;
struct s_graph_table_entry{

    Line * first_neighboor_ref;  //will replace by a 32 bit integer 
    //representing index to gain a bit of space 
    uint16_t neighboor_num; //Might store on one byte
};


typedef struct s_walker{
   
    uint32_t index_entry; //gte of the node where the where the walker currently is
    //make it into the index 
}Walker;

typedef struct s_warray{
    uint32_t size; 
    Walker *array;
    Walker *array_prev; 
}WalkerArray ; //where the actual walkers will be stored and initialised and so on

typedef struct s_walker_num_curnext{
    //structure to store the number of walker at each node ;updated to keep track of where 
    //walkers came in and stuff 
    uint32_t size; //size of number of nodes
    uint32_t *cur_num;
    uint32_t *next_num; 
}WalkerCurNext;

uint8_t initWalkerArray( WalkerArray * wArray, uint32_t size, uint8_t prevset);
void freeWalkerArray(WalkerArray * wArray);


uint8_t initWalkerCurNext(WalkerCurNext * wkcn , uint32_t size);
void freeWalkerCurNext(WalkerCurNext * wkcn);

uint8_t swapWalkerCurNext (WalkerCurNext * wkcn);

#ifdef debug_mode

void printWKCN( WalkerCurNext * wkcn, FILE * stream);
void printWarray(const GraphTableEntry * gt_ref,  WalkerArray *wArray, FILE * stream);

#endif

#endif