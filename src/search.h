#ifndef SEARCH_H 
#define SEARCH_H
#include "common.h"
#include "graph_table.h"
#include <stdint.h>


#define GA_SIZE_DEF 8
#define STACK_SIZE_DEF 32


enum{ GA_TYPE, STACK_TYPE};


typedef struct dyn_arr{  //generic uint32 dynamic array 
    uint32_t size , cur_in; 
    uint32_t * elem; 

}s_dynamic_array;

typedef s_dynamic_array GROUP_ARR; //the gp array / stack are typedefs of dynamic arrays
typedef s_dynamic_array S_STACK;

bool size_gp_check( GraphTable * gt, bool * check ,  uint32_t threshold_size);

#define init_gp_arr(arr) (init_dyn_arr((arr), GA_SIZE_DEF , GA_TYPE))
#define free_gp_arr(arr) (free_dyn_arr((arr)))
#define app_ga_arr(arr,value)  (app_dyn_arr(  (arr), (value), GA_TYPE))

#define init_stack(arr) (init_dyn_arr((arr), STACK_SIZE_DEF , STACK_TYPE))
#define free_stack(arr) (free_dyn_arr((arr)));
#define stack_darr(arr,value) (app_dyn_arr((arr), (value) , STACK_TYPE))
#define pop_stack(arr,ret) (pop_dyn_arr((arr), (ret)))

#ifdef debug_mode 
#endif 

#endif