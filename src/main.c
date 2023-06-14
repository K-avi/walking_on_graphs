#include "common.h"
#include "graph_table.h"
#include "misc.h"
#include "movement.h"
#include "walker.h"
#include "tactics.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc , char ** argv){

    int8_t c;
    uint8_t helpset=0 , dumpset=0;
    char * trace_name =NULL;
    while ((c = getopt(argc, argv, "hd:")) != -1) {
        
        switch (c) {
        case 'h':
            helpset=1;
            break;
 
        case 'd':
            dumpset=2;
            trace_name = optarg;
            break;
        case '?':
            if(optopt=='d'){
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            }else{
                fprintf(stderr, "Unknown option character `\\x%x'.", optopt);
                exit(ERRFLAG_INVALIDOPT);
            }
            break;
        default:
            abort();
        }
    }
    if(helpset){ //prints help
        fprintf(stdout, "usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n \
        check out the docu directory for more informations\n");
        exit(0);
    }

    if(argc < 4){ //checks that the number of args is ok
        fprintf(stderr, "1usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n");
        return ERRFLAG_NOFILE;
    }
    char * path = argv[1+dumpset]; 
    char * end=argv[2+dumpset];

    //parses number of walkers
    double walker_coeff=  (double) strtod( argv[2+dumpset], &end );
    if(end == argv[2+dumpset]){
        fprintf(stderr, "2usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n");
        return ERRFLAG_INVALID_ARG;
    }

    //parses number of iterations
    end= argv[3+dumpset];
    uint32_t iteration_num = (uint32_t ) strtol( argv[3+dumpset], &end , 10);
    fprintf(stderr, "args : %s %s\n", argv[5], argv[4]);
    if(end== argv[3+dumpset]){
        fprintf(stderr, "3usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n");     
        return ERRFLAG_INVALID_ARG;
    }

    //init timer
    time_t timer;
    time( &timer);
    srand(timer);

    //init graph 
    GraphTable gtable; 
    uint8_t failure = loadGraphTab(&gtable, path, walker_coeff, 0);
    if(failure){ report_err("in main loadGraphTab call", failure); exit(failure);}


    //init tactics
    Tactics tactics; 
    failure= initTactics(&tactics, DEFAULT_CAPA_TACTICS);
    if(failure) {report_err("in main loadGraphTab call", failure); exit(failure);}

    //tries to parse the tactics arg if they are present 
    //if the program is called without it ; simply uses the rand rule
    if(argc>4){
        failure=parse_args(&tactics, argc-4-dumpset, (argv+4+dumpset));
        if(failure){ report_err("in main loadGraphTab call", failure); exit(failure);}
    }else{//hmmm
        failure = parse_args(&tactics, 0, NULL);
         if(failure){ report_err("in main loadGraphTab call", failure); exit(failure);}
    }
    
    //starts the simulation 

    failure= init_pos(&gtable);
    if(failure){report_err("in main init_pos call", failure); exit(failure);}

    if(!dumpset){
        failure=iterate_ntimes(&gtable, &tactics, iteration_num);
        if(failure){report_err("in main iterate_ntimes call", failure); exit(failure);}
    }else{

        failure=iterate_ntimes_dump(&gtable, &tactics, iteration_num, trace_name);
        if(failure){report_err("in main iterate_ntimes_dump call", failure); exit(failure);}

    }

    //frees memory
    freeGraphTab(&gtable);
    freeTactics(&tactics);
    
    return 0;
}