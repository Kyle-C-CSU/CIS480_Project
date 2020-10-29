#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#define numofstrings 100

char **lab_name;
int *lab_add;

int pc_addr_data(dir,op,pc_addr){
    int op_count=0;
    char *tok = strtok(op, ",")
    while(tok != NULL){
        tok = strtok(NULL,",");
        op_count++;
    }
    if(!strcmp(dir,".word"))
        pc_addr = op_count*32;
    else //.space
        pc_addr = op*8
    return pc_addr;
                
}

int pc_addr_text(ins,pc_addr){
    if(!strcmp(upper(ins),"LA")
        pc_addr += 2*32;
    else
        pc_addr += 32;
    return pc_addr;
    
}

void lab_name_addr(lab,pc_addr){
    lab[strlen(lab)-1]='\0';
    lab_name[lab_count] = (char*)malloc(sizeof(char*) * strlen(lab));
    strcpy(lab_name[lab_count],lab);
    lab_add[lab_count] = pc_addr;
}


int main(int argc, char *argv[]) {




    /*To do:
    *1. find solution to identify labels and store accordingly
    *2. find solution to reference labels (symbol table)
    *3. fix arraypush method: not storing correct values by reference
    *4. finish .data condition
    *5.*done* Finish formating strings for each instruction case                                  *done*
    *6. implement LA instruction
    *7. Create 1024 array to store data + txt feilds and find a solution to concatinate
    */

    /*Bugs:
    * Segmentation fault because instruction is being read into label
    * then trying to pass gibberish to method that is expecting string
    * once 1 is taken care of on to do list. This should be managed.

    */





    FILE *fp; //pointer for read file
    char str65[65],str33[33],lab[20], dir[20], ins[20], op[20], lower[20]; //buffer vals
    char temp[33] = "/0";
    
    long s;
    ins[0] = '\0';
    bool data_sec =false,text_sec =false, word_sec=false;
    int opcode,rd,rs,rt,imm,input; //binary buffer vals
    int lab_count = 0;
    int text_bits = 0;
    int pc_addr = 0;
   
    
    char **lab_val, **ins_lines;
    int *ins_line_addr = malloc(numofstrings*sizeof(int*));
    lab_name = malloc(numofstrings*sizeof(char*));
    lab_add = malloc(numofstrings*sizeof(int*));
    lab_val = malloc(numofstrings*sizeof(char*));
    ins_lines = malloc(numofstrings*sizeof(char*));
   


   /* opening file for reading */
    fp = fopen(argv[1] , "r");
    if(fp == NULL) {
        perror("Error opening file");
        return(-1);
    }

    /*create file to write to new (bin)file*/
    FILE *fpnew;
    fpnew = fopen( "/Users/kylecorcoran/Dev/xcode/test.c/test.c/new.out" , "wb" );

   /*get line by line of read file until EOF*/
   int lines = 0;
    while( fgets (str33, 33, fp)!=NULL ) {
        /* print content to stdout */
        int len = strlen(str33);
        //trim off the /n
        if( str33[len-1] == '\n' )
            str33[len-1] = 0;
        //puts: write str33 to console 
        //puts(str33);
        //store instruction line in str array
        //printf("%s\n",str33);
        ins_lines[lines] = (char*)malloc(sizeof(char*) * strlen(str33));
        strcpy(ins_lines[lines],str33);
        
        //check conditions for text and data
        char* pPosition = strchr(str33, ':');
        if(!strcmp(str33,".data")){
            data_sec = true;
            pc_addr = 0;
            continue;
        }
        if(!strcmp(str33, ".text")){
            data_sec = false;
            pc_addr = 512;
            continue;
        }

        if(data_sec){
            //labels in .data
            if(pPosition){
                sscanf( str33, "%s %s %s", lab, dir, op );
                pc_addr = pc_addr_data(dir,op,pc_addr);
            
            }
            else{ //no labels in .data
                sscanf( str33, "%s %s",dir, op );
                pc_addr =  pc_addr_data(dir,op,pc_addr);        
            }

        }
        else{ //text section
            if(pPosition){ //labels in text
                sscanf(str33, "%s %s, %s"lab,ins,op);
                pc_addr = pc_addr_text(ins,pc_addr);
            }
            else{ //no labels in text
                sscanf(str33, "%s %s",ins,op);
                pc_addr = pc_addr_text(ins,pc_addr);
            }


        }

        //associate ins_lines with address 
        ins_lines_addr[lines] = pc_addr;

        //assigning label symbol table
        if(pPosition){//check for label
            if(data_sec){
                sscanf( str33, "%s %s %s", lab, dir, op );
                lab[strlen(lab)-1]='\0';

                lab_name[lab_count] = (char*)malloc(sizeof(char*) * strlen(lab));
                strcpy(lab_name[lab_count],lab);
                lab_add[lab_count] = pc_addr;
                char *tok = strtok(op,",");
                *(lab_val+0) = tok;
                int ops = 1;
                while(tok != NULL){
                    tok = strtok(NULL, ",");
                    *(lab_val+ops) = tok;
                if (tok != NULL)
                    ops++;
                }
                lab_count++;
                
            }
            else{ //text section
                sscanf( str33, "%s %s %s", lab, ins, op);
                lab[strlen(lab)-1]='\0';

                lab_name[lab_count] = (char*)malloc(sizeof(char*) * strlen(lab));
                strcpy(lab_name[lab_count],lab);
                lab_add[lab_count] = pc_addr;
                lab_count++;



            }
        }
        lines++;
        

    }

    printf("\nend\n");
        for(int i=0;i<lines;i++)
            printf("lines[%d]%s\n",i,ins_lines[i]);
    return 0;
}