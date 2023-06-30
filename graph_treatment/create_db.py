#this script will call autorun w different parameters to 
#generate a LOT of stats 
import autorun_simul as ars
import graph_generator as gg 
import data_analysis as dt
import random as r
import time as t
import numpy as np
import networkx as nx 
import sim_plot as sp
import itertools as it 


def parse_seen_cpt(path): 
    """
    str -> set , num
    """
    cpt=0
    ret= set()
    with open(path, "r") as file:
        
        for line in file: 
            if not line.strip():
                continue
            s = line.split(",")[1::]
            l = []
            for field in s: 
                l.append(int( float(field.split(":")[1])*10))
            if(len(l)!=4):
                print("line:\n",line)
                print("error in parse seen")
                exit(-1)
            ret.add(tuple(l))
            cpt+=1
        
        
    return ret, cpt


 
def gen_db_var(graph_path, nb_it):
    """ 
    graphml_path , nb_it -> db of simul results
    
    generates the db w iterations instead of 
    randomly pulling graphs
    """

    s = [i for i in it.combinations_with_replacement(np.linspace(0,10,11), r=4) if sum(i) == 10]
    f = sum([ list(set(it.permutations(j, r=len(j)))) for j in s ],[])
    
    seen, cpt = parse_seen_cpt("index_base.csv")
    print(seen)
    for a,b,c,d in f: 
        
        if(a,b,c,d) in seen: 
            continue 
        
        print(f"simul running with par :\nrand:{a} attra:{b} align:{c} propu:{d}")
        ars.run_simul_nth(4,4, graph_path, 1 , nb_it,
                                      f"rand:{a} attco:{b} align:{c} propu:{d}",
                                      "trace", f"{cpt}_run", nb_it - 10)
        dt.mean_results( f"{cpt}_run", f"{cpt}_res")
        dt.clean_results( f"{cpt}_run")
        
        with open("index_base.csv", "a") as f1:
            f1.write(f"{cpt},rand:{a},attra:{b},align:{c},propu:{d}\n")
        cpt+=1  

    for a,b,c,d in f:             
        
        print(f"simul running with par :\nrand:{a} attco:{b} align:{c} propu:{d}")
        ars.run_simul_nth(4,4, graph_path, 1 , nb_it, f"rand:{a} attco:{b} align:{c} propu:{d}",
                                      "trace", f"{cpt}_run", nb_it - 10)
        dt.mean_results( f"{cpt}_run", f"{cpt}_res")
        dt.clean_results( f"{cpt}_run")
        
        with open("index_base.csv", "a") as f1:
            f1.write(f"{cpt},rand:{a},attra:{b},align:{c},propu:{d}\n")
        cpt+=1  
    
    close(index)

def get_simpar(index_path):
    """
    index base path -> array of strings
    """
    index_strings = []
    with open(index_path, "r") as f: 
        for line in f:
            if not line.strip():
                continue
            index = (int(line.split(",")[0]))
            string = ("".join([ i+" " for i in line.split(",")[1::] ]))
            index_strings.append([index,string])
    return index_strings

def gen_newflux(graph_path, index_path, nb_it):
    """
    generate new flux data and so on
    """
    para_strings = get_simpar(index_path)
    for i in para_strings:

        print(f"simul running with par :{i[1]}")
        ars.run_simul_nth_flux(4,4, graph_path, 1 , nb_it, i[1], "trace", f"{i[0]}_run", nb_it - 10)
        dt.mean_results_flux( f"{i[0]}_run", f"base/{i[0]}_res")
        dt.clean_results( f"{i[0]}_run")

def query(query, db_name):
    """
    parses a simple query string 
    currently not done 
    placeholder
    """
    if query.startwith("or_select"):
        
        ret=""
        args = query.split(" ")[1::]
        
        with open(f"index_base.csv") as index:
            for line in index : 
               if any( arg in line for arg in args ):
                   ret+= f"{line}\n"
        print("sim matching query are :\n",ret)
    
    elif query.startwith("and_select"): 
        
        ret=""    
        args = query.split(" ")[1::]
        
        with open(f"index_base.csv") as index:
            for line in index : 
               if all( arg in line for arg in args ):
                   ret+= f"{line}\n"
        print("sim matching query are :\n",ret)
   
    elif string.startwith("plot"):   
        
        ret=[]
        for file in os.listdir(db_name):
            args = query.split(" ")[1::]
            if(file.split("_")[1] in  args):
                ret.append(file)
        print(ret)
        sp.plot_list(ret)
        
    else : 
        print("invalid query please print a valid query")
        
    return 0 
    
        
if __name__=='__main__':
    gen_newflux("paris_5k_10m.csv","index_base.csv",2000)
