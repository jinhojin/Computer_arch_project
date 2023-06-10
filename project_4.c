#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int L1_cache_capacity;
int L1_cache_associativity;
int L2_cache_capacity;
int L2_cache_associativity;
int block_size;
int set_num_1;
int set_num_2;
int block_num_1;
int block_num_2;
int L2_miss = 0;
int L1_miss = 0;
int Cache_count = 0;

// Cahche_replacement 0:lru 1:random
int Cache_replacement;

struct L1_cache{
    char* state;
    unsigned long long address;
    int count;
};

struct L2_cache{
    char* state;
    unsigned long long address;
    int count;
};



int Cache_L1(char* state, char* address, struct L1_cache** L1){
    int L1_hit = 0;
    int RorW = 0;
    if(strcmp(state,"W") == 0){
        RorW = 1;
    }else{
        RorW = 2;
    }

    for(int i =0; i < set_num_1; i++){
        for(int j = 0; j < L1_cache_associativity; j++){
            if(L1[i][j].address == address){
                L1_hit++;
                if(RorW == 2){
                    Cache_count++;
                    L1[i][j].count = Cache_count;
                }
                break;
            }
        }
    }

    return L1_hit;
}

int Cache_L2(int check_L1, char* state, char* address, struct L2_cache** L2){
    int L2_hit = 0;
    int RorW = 0;
    if(strcmp(state,"W") == 0){
        RorW = 1;
    }else{
        RorW = 2;
    }

    for(int i =0; i < set_num_2; i++){
        for(int j = 0; j < L2_cache_associativity; j++){
            if(L2[i][j].address == address){
                L2_hit++;
                break;
            }
        }
    }

    if(check_L1 == 0 && L2_hit == 1){
        return 3;
    }

    if(L2_hit == 0 && RorW == 1){
        L2_miss++;
        if(Cache_replacement == 1){
            srand(time(NULL));
            int random_1 = rand()%set_num_2;
            srand(time(NULL));
            int random_2 = rand()%L2_cache_associativity;
            L2[random_1][random_2].address = address; 
        }else{
            //LRU algorithm needed
        }
    }


    


    return L2_hit;
}

void main(int argc, char *argv[]){
    char* readfile;
    char line[200];
    char* state;
    char* address;
    
    for(int i = 1; i < argc; i++){
        if(strcmp("-c" , argv[i]) == 0 ){
            L2_cache_capacity = atoi(argv[i+1]);
        }
        else if(strcmp("-a", argv[i]) == 0){
            L2_cache_associativity = atoi(argv[i+1]);
        }        
        else if(strcmp("-b", argv[i]) == 0){
            block_size = atoi(argv[i+1]);
        }
        else if(strcmp("-lru",argv[i]) == 0){
            Cache_replacement = 0;
        }
        else if(strcmp("-random", argv[i]) == 0){
            Cache_replacement = 1;
        }
    }

    L1_cache_capacity = L2_cache_capacity / 4;
    L1_cache_associativity = L2_cache_associativity / 4;

    block_num_1 = L1_cache_capacity / block_size;
    block_num_2 = L2_cache_capacity / block_size;

    set_num_1 = block_num_1 / L1_cache_associativity;
    set_num_2 = block_num_2 / L2_cache_associativity;

    struct L1_cache L1[set_num_1][L1_cache_associativity];
    struct L2_cache L2[set_num_2][L2_cache_associativity];



    FILE *fp = fopen(argv[argc-1],"r");

    while(!feof(fp)){
        readfile = fgets(line, sizeof(line), fp);
        readfile = strtok(readfile,"\n");

        char* temp;
        temp = strtok(readfile," ");

        for(int i=0; i<2; i++){
            if(i == 0){
                state = temp;
            }else{
                address = temp;
            }
            temp = strtok(NULL," ");
        }

        int check_L1 = Cache_L1(state, address, L1);
        if(check_L1 == 0){
            Cache_L2(check_L1, state, address, L2);
        }


    }

}