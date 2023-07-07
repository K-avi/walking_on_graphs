#include "search.h"
#include "common.h"
#include "graph_table.h"
#include "misc.h"
#include <stdint.h>

static uint8_t init_dyn_arr(s_dynamic_array * arr, uint32_t size, uint8_t type){
    /*
    init function for dynamic arrays in search.c ; 
    supports init for STACKs
     and group arrays 
    */
    if(!arr){
        if(type==GA_TYPE){
            report_err("init_GROUP_ARR", GA_NULL); 
            return GA_NULL;
        }else{
            report_err("init_GROUP_ARR", STACK_NULL); 
            return GA_NULL;
        }
    }
    arr->elem = calloc( size, sizeof(uint32_t));
    if(!arr->elem){
        
        if(type == GA_TYPE ){
            report_err("init_GROUP_ARR", GA_MALLOC); 
            return GA_MALLOC;
        }else{
            report_err("init_GROUP_ARR", STACK_MALLOC); 
            return GA_NULL;
        }
    }

    arr->cur_in = 0; 
    arr->size = size;

    return ERRFLAG_OK;
}//not tested ; watch out for flags and shit

static void free_dyn_arr(s_dynamic_array * arr){
    /*
    frees the array in a non null dynamic array ; 
    doesn't free the dynamic array pointer passed 
    as argument.
    */
    if(arr)if(arr->elem)free(arr->elem);
}//ok


static uint8_t app_dyn_arr( GROUP_ARR * arr, uint32_t value , uint8_t type){
    /*
    implicit "cast" of the structure as a group array for the sake of simplicity
    adds a node index to a group arr; 
    handles realloc ; reports errors and stuff
    */
    if(!arr){
        if(type== GA_TYPE){report_err("app_dyn_arr", GA_NULL); return GA_NULL;
        }else {report_err("app_dyn_arr", STACK_NULL); return STACK_NULL;}
    }

    if(arr->cur_in == arr->size ){
        uint32_t old_capa = arr->size ; 
        arr->size = GROW_CAPACITY(arr->size);
        arr->elem = GROW_ARRAY(uint32_t, arr->elem, old_capa, arr->size);
        if(!arr->elem){
            if(type== GA_TYPE){report_err("app_dyn_arr", GA_REALLOC); return GA_REALLOC;
            }else {report_err("app_dyn_arr", STACK_REALLOC); return STACK_REALLOC;}
        }
    }
    arr->elem[arr->cur_in++] = value;
    return ERRFLAG_OK;
}//not tested ; prolly ok 

static uint8_t pop_dyn_arr(S_STACK * arr , uint32_t * ret){
    /*
    implicit "cast" of the structure as a stack
    */
    if(!arr){report_err("pop_dyn_arr", STACK_NULL); return STACK_NULL;}
    if(!arr){report_err("pop_dyn_arr", ERRFLAG_NULL); return ERRFLAG_NULL;}

    *ret =  arr->elem[arr->cur_in];
    (arr->cur_in)--;

    return STACK_OK;
}///not tested

static uint8_t get_group_nodes(GraphTable * gt, GROUP_ARR * arr, uint32_t * sum){
    /*
    given a graph table returns the nodes that are in 
    the same "group" a group is defined by a set 
    of connected nodes from gt that each contain at 
    least one walker.
    the GROUP_ARR pointer given should store the 
    adress of an already initialised array 
    */
    if(!gt){report_err("get_group_nodes", GT_NULL); return GT_NULL;}
    if(!arr){report_err("get_group_nodes", GA_NULL);return GA_NULL;}

    int64_t start = -1; 
    for(uint32_t i = 0 ; i<gt->wkcn->size; i++){
        if(gt->wkcn->cur_num[i]){
            start = i; 
            break;
        }
    }
    if(start == -1){report_err("get_group_nodes", GA_NOWK);return GA_NOWK;}

    S_STACK stack; 
    uint8_t failure = init_stack(&stack);
    if(failure){report_err("get_group_nodes", failure);return failure;}

    stack_darr(&stack, start);
    gt->seen_array[start]=1;

    while(stack.cur_in){

        uint32_t cur_node;
        uint8_t failure = pop_stack(&stack, &cur_node);
        if(failure){ report_err("get_group_nodes", failure); return failure;}

        *sum+= gt->wkcn->cur_num[cur_node];
        app_ga_arr(arr, cur_node);

        for(uint32_t i =0 ; i < gt->entries[cur_node].neighboor_num; i++){//appends neighbors not already 
        //seen AND with walkers to the stack
            
            uint32_t neighbor_index = (gt->entries[cur_node].first_neighboor_ref+i)->node_index;
            if(gt->wkcn->cur_num[neighbor_index] && !gt->seen_array[neighbor_index]){ 
                gt->seen_array[neighbor_index]=1; 
                stack_darr(&stack, neighbor_index);
            }
        }
    }
    free_stack(&stack);
    return GA_OK;
}//not tested; 
//error prone  


uint8_t one_gp_check( GraphTable * gt, bool * check ){
    /*
    checks wether every Walker in the graphs 
    passed as argument are in the same group

    O(2*t) where t is the size of the group found; 
    likely very small in the graphs used; 
    could easily be reduced to O(t)
    */

    //sanity checks
    if(!gt){report_err("one_gp_check", GT_NULL);return GT_NULL;}
    if(!check){report_err("one_gp_check", ERRFLAG_NULL);return ERRFLAG_NULL;}

    //initialises a group array 
    GROUP_ARR ga; 
    uint8_t failure = init_gp_arr(&ga); 
    if(failure){report_err("one_gp_check", failure); return failure;}
    uint32_t sum;

    // retrieves a group from gt to store it in ga and
    //checks wether the number of walkers in the group is
    //the number of walkers in the graph
    get_group_nodes(gt, &ga , &sum);
    //actually I'm not sure I don't even NEED ga but whatever

    *check = (sum == gt->warray->size);

    free_gp_arr(&ga);
    return GA_OK;
}//not tested ; 
//if get_gp_nodes is ok 
//so is this guy