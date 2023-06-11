#include "movement.h"
#include "common.h"
#include "graph_table.h"
#include "misc.h"
#include "walker.h"
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

uint8_t init_pos(GraphTable * gtable){
    /*inits position of walkers on the graphs; 
    
    warning : puts walker num in the NEXT_NUM array 
    watch out for compatibility with the prepare_ite fn
    */
    if(!gtable){ report_err("init_pos", GT_NULL); return GT_NULL;}

    for(uint32_t i=0; i<gtable->warray->size;i++){
        uint32_t randval= (uint32_t) rand()%gtable->table_size;
        
        gtable->warray->array[i].cur_entry=&gtable->entries[randval];
        gtable->wkcn->next_num[randval]++;
    }
    return MV_OK;
}//tested ; ok


uint8_t prepare_ite( GraphTable * gtable){
    /*
    prepartion fn to call before iterating; 
    flushes the previous values of flux and position of walkers arrays and 
    swaps the next and current ones 
    O(1)
    */
    if(!gtable){ report_err("prepare_ite", GT_NULL); return GT_NULL;}

    uint8_t failure= swapWalkerCurNext(gtable->wkcn);
    if(failure) { report_err("prepare_ite", failure); return failure;}

    failure= swap_flux_curnext(gtable->arrLine);
    if(failure) { report_err("prepare_ite", failure); return failure;}

    gtable->curgen++;
    return MV_OK;

}//tested ; ok

uint8_t iterate_once(GraphTable * gtable , Tactics * t){
    /*
    iteration function; 
    for every walker; chooses a node ; update relevant fields for next iteration 
    (flux, number of walkers in a node , walker position,... )
    warning : some of the fields are updated by the call to choose node 
    if the choose_node fn doesn't update flux correctly it might cause an issue. 
    While this is highly a questionnable choice, 
    it was also a convenient one
    O(w) where w is the number of walkers 
    */

    for(uint32_t i=0; i<gtable->warray->size;i++){

        uint8_t failure= choose_node(t, gtable,  gtable->warray->array[i].cur_entry - gtable->entries, i);
        if(failure)return failure;      
    }
    return MV_OK;
}//tested; seems ok
//new version relies on new implem of tactics which is being tested 

uint8_t iterate_ntimes( GraphTable * gtable, Tactics * tactics, uint32_t iter_num){
    /*
    O(w*i)
    the main iteration function ; simply iterates i times ; more of a wrapper around 
    iterate_once and prepare it
    */
    if(!gtable){ report_err("iterate_ntimes", GT_NULL); return GT_NULL;}
    if(!tactics){ report_err("iterate_ntimes", T_NULL); return T_NULL;}
    uint8_t failure;
    for(uint32_t i=0; i<iter_num; i++){
        
        failure= prepare_ite(gtable);
        if(failure){report_err("iterate_ntimes prepare ite call", failure); return failure;}
        failure= iterate_once(gtable, tactics);
        if(failure){report_err("iterate_ntimes iterate_once call", failure); return failure;}
    }

    return MV_OK;
}//tested; ok

uint8_t iterate_ntimes_dump( GraphTable * gtable, Tactics * tactics, uint32_t iter_num, char * trace_name){
    /*
    O(w*i)
    clone of iterate_ntimes that dumps the graph after each iteration
    */
    if(!gtable){ report_err("iterate_ntimes", GT_NULL); return GT_NULL;}
    if(!tactics){ report_err("iterate_ntimes", T_NULL); return T_NULL;}

    char * trace_human_readable = malloc( (5+ strnlen(trace_name,252)) * sizeof(char));
    //memset(trace_human_readable, 0,  5+ strnlen(trace_name, 252));
    snprintf(trace_human_readable, 256, "%s_hr", trace_name);

    char * trace_flux= malloc( (7+ strnlen(trace_name,250 )) * sizeof(char)); 
    snprintf(trace_flux, 256, "%s_flux", trace_name);

    char * trace_curnum=malloc( (9+ strnlen(trace_name,248 )) * sizeof(char)); ; 
    snprintf(trace_curnum, 256, "%s_curnum", trace_name);

    char * trace_walkerpos=malloc( (8+ strnlen(trace_name,249 )) * sizeof(char)); ; 
    snprintf(trace_walkerpos, 256, "%s_wkpos", trace_name);

    FILE * f_hr = fopen(trace_human_readable, "ab");
    FILE * f_flux = fopen(trace_flux, "ab");
    FILE * f_curnum = fopen(trace_curnum, "ab");
    FILE * f_wkpos = fopen(trace_walkerpos, "ab");
    
    free(trace_human_readable);free(trace_flux); 
    free(trace_curnum); free(trace_walkerpos);

    //dumps human readable info once
    if(!(f_hr && f_flux && f_curnum && f_wkpos)){
        //printf("%p %p %p %p\n%s %s %s %s\n", f_hr , f_flux , f_curnum , f_wkpos, trace_human_readable, trace_flux, trace_curnum , trace_walkerpos);
        report_err("iterate_ntimes_dump",  ERRGLAG_CANTWRITE);
        return ERRGLAG_CANTWRITE;
    }
    printGraphTab(gtable, f_hr);
    printWarray(gtable->entries, gtable->warray, f_hr);
    printWKCN(gtable->wkcn, f_hr);
    fclose(f_hr);

    uint8_t failure;

    for(uint32_t i=0; i<iter_num; i++){
        
        failure= prepare_ite(gtable);
        if(failure){report_err("iterate_ntimes prepare ite call", failure); return failure;}
        failure= iterate_once(gtable, tactics);
        if(failure){report_err("iterate_ntimes iterate_once call", failure); return failure;}
      
        dump_trace(gtable, f_curnum, f_flux, f_wkpos);
    }

    fclose(f_curnum); 
    fclose(f_flux);
    fclose(f_wkpos);

    return MV_OK;
}//done ; tested scaling ; seems constant (great) however 
//haven't tested outputs yet 