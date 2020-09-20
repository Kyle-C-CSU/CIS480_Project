#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define numofstrings 100

void upper(char *inst){     //passed by reference so should update without having to return
    //upper cases all char value in instruction
    while(*inst){
        *inst = toupper((unsigned char) *inst);
        inst++;
    }
}

int *decimalToBinary_n(int dec, int *binMSB, int n){
    int binLSB[n],i,j; //i is for lsb count and j is for msb count
    j=0;
    i=0;
    if(i== 0){
           for(int k = 0; k < n;k++)
               binMSB[k]=0;
    }
    for(i = 0; i<n; i++){binLSB[i]=0;}//set all values to 0
    //convert n to binaryLSB
    for(i=0;dec>0;i++){
        binLSB[i]=dec%2;
        dec=dec/2;
        j++; //j counts how many elements entered
    }
    j = n-j; //j repersents the MSB index
    //reverse order of LSB to MSB
    for(i= i-1;i>=0;i--){
        binMSB[j++] = binLSB[i];
    }
    
    //return pointer of dynamic array
   
    return binMSB;
}

//instruction convertor
int ins_conv(char *ins){
    char *tab[] = {"ADD","SUB","SLL","SRL","SLT", //symbol table
                   "ADDI","LUI","ORI","LW","SW","LA"};
    int index;
    upper(ins);
    
    
    for(int i = 0;i<11;i++){
        if(strcmp(tab[i],ins)==0){
            index = ++i;
            break;
        }
    }
    
    return index;
}


char *binconv(int n, int bits){
    int c, d, count;
    char *pointer;
    
    count = 0;
    pointer = (char *)malloc(32+1);
    
    if (pointer == NULL)
        exit(EXIT_FAILURE);
    
    for(c = bits - 1; c >=0; c--){
        d = n >> c;
        
        if(d&1)
            *(pointer+count) = 1 + '0';
        else
            *(pointer+count) = 0 + '0';
        count++;
    }
    *(pointer+count) = '\0';
    return pointer;
    
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
    bool data_sec =false,text_sec =false;
    int opcode,rd,rs,rt,imm,input; //binary buffer vals
    int lab_count = 0;
    int text_bits = 0;
    int data_bits = 0;
   
    
    char **lab_name, **lab_val;
    lab_name = malloc(numofstrings*sizeof(char*));
    int *lab_add = malloc(100*sizeof(int*));
    lab_val = malloc(numofstrings*sizeof(char*));
   


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
    while( fgets (str33, 33, fp)!=NULL ) {
        /* print content to stdout */
        int len = strlen(str33);
        //trim off the /n
        if( str33[len-1] == '\n' )
            str33[len-1] = 0;
        puts(str33);
        
        char* pPosition = strchr(str33, ':');
        if (strcmp(str33, "\t.data")==0){
           
            data_sec = true;
            data_bits = 0;

        }
        if(strcmp(str33, "\t.text")==0){
           
            data_sec = false;
            text_bits = 512;

        }
        
        
        
        if(data_sec){
        /*.data section first 512 bits*/
            
            
            
           if(str33[0]=='\t')
                strcpy(str33,"\0");
           else{
               if(pPosition){//check for label
                   sscanf( str33, "%s %s %s", lab, dir, op );
                   lab[strlen(lab)-1]='\0';
               }
               else
                sscanf (str33,"%s %s", dir, op);
                //store our label with adress associated
                
                if(strcmp(dir,".space")==0){//check dir type
                   //.space
                    
                   imm = atoi(op);
                    lab_name[lab_count] = (char*)malloc(sizeof(char*) * strlen(lab));
                    strcpy(lab_name[lab_count],lab);
                    lab_add[lab_count] = data_bits;
                    lab_count++;
                    //fill data with value 0's
                    for(int j = 0; j<imm; j++){
                        strcpy(str33,binconv(0,8));
                        if (fwrite(str33,sizeof(str33), 1, fpnew )!=1) {
                            perror("Error writing file"); return(-1);
                        }
                        data_bits += 8;
                        
                    }
                    
                        
                }
                    
           
                else{ //.word
                    //find how many ops
                    char *tok = strtok(op,",");
                        *(lab_val+0) = tok;
                    int i = 1;
                    while(tok != NULL){
                        tok = strtok(NULL, ",");
                            *(lab_val+i) = tok;
                        if (tok != NULL)
                            i++;
                    }
                    for(int j = lab_count; j < i; j++){
                        imm = atoi(lab_val[j]);
                        strcpy(str33,binconv(imm,32));
                        if (fwrite(str33,sizeof(str33), 1, fpnew )!=1) {
                            perror("Error writing file"); return(-1);
                        }
                        //printf("%s\n",str33);
                        lab_name[j] = (char*)malloc(sizeof(char*) * strlen(lab));
                        if(pPosition){
                            strcpy(lab_name[j],lab);
                            lab_add[j] = data_bits;
                            lab_count++;
                        }
                        data_bits +=32;
                        
                    }
                }
           }
        }

        else{
        /*.text section last 512 bits*/
            if(pPosition){//check for label
                sscanf( str33, "%s %s %s",lab,ins,op);
                lab[strlen(lab)-1]='\0';
                lab_name[lab_count] = (char*)malloc(sizeof(char*) * strlen(lab));
                strcpy(lab_name[lab_count],lab);
                lab_add[lab_count] = text_bits;
                lab_count++;
                
            }
            else if(str33[0] == '\t')
                input = 12;
            else{
                sscanf( str33, "%s %s",ins, op );
            }
            if(str33[0] != '\t'){
                input = ins_conv(ins);
            }
            

            switch(input){
                case 1: //add
                    sscanf( op, "$%d,$%d,$%d", &rd, &rs, &rt );
                    opcode = 0;
                    strcpy(str33,binconv(opcode,6));
                    text_bits+=6;
                    
                    strcat(str33,binconv(rs,5));
                    text_bits+=5;
                   
                    strcat(str33,binconv(rt,5));
                    text_bits+=5;
                    
                    strcat(str33,binconv(rd,5));
                    text_bits+=5;
                   
                    strcat(str33,binconv(0,5));
                   
                    text_bits+=5;
                    strcat(str33,binconv(32,6));
                    //printf("%s\n",str33);
                    text_bits+=6;
                    //s = strtol(str33,NULL,2);
                    if (fwrite(str33,sizeof(str33), 1, fpnew )!=1) {
                        perror("Error writing file"); return(-1);
                    }
                    break;
                    
                case 2: //sub
                    sscanf( op, "$%d,$%d,$%d", &rd, &rs, &rt );
                    opcode = 0;
                    strcpy(str33,binconv(opcode,6));
                    text_bits+=6;
                    strcat(str33,binconv(rs,5));
                    text_bits+=5;
                    strcat(str33,binconv(rt,5));
                    text_bits+=5;
                    strcat(str33,binconv(rd,5));
                    text_bits+=5;
                    strcat(str33,binconv(0,5));
                    text_bits+=5;
                    strcat(str33,binconv(34,6));
                    //printf("%s\n",str33);
                    text_bits+=6;
                    if (fwrite(str33,sizeof(str33), 1, fpnew )!=1) {
                        perror("Error writing file"); return(-1);
                    }
                    break;
                case 3: //sll
                    sscanf( op, "$%d,$%d,%d", &rd, &rs, &rt );
                    opcode = 0;
                    strcpy(str33,binconv(opcode,6));
                    text_bits+=6;
                    strcat(str33,binconv(0,5));
                    text_bits+=5;
                    strcat(str33,binconv(rt,5));
                    text_bits+=5;
                    strcat(str33,binconv(rd,5));
                    text_bits+=5;
                    strcat(str33,binconv(rs,5)); //shamt
                    text_bits+=5;
                    strcat(str33,binconv(0,6));
                    //printf("%s\n",str33);
                    text_bits+=6;
                    if (fwrite(str33,sizeof(str33), 1, fpnew )!=1) {
                        perror("Error writing file"); return(-1);
                    }
                    break;
                case 4: //srl
                    sscanf( op, "$%d,$%d,%d", &rd, &rs, &rt );
                    opcode = 0;
                    strcpy(str33,binconv(opcode,6));
                    text_bits+=6;
                    strcat(str33,binconv(0,5));
                    text_bits+=5;
                    strcat(str33,binconv(rt,5));
                    text_bits+=5;
                    strcat(str33,binconv(rd,5));
                    text_bits+=5;
                    strcat(str33,binconv(rs,5)); //shamt
                    text_bits+=5;
                    strcat(str33,binconv(2,6));
                    //printf("%s\n",str33);
                    text_bits+=6;
                    if (fwrite(str33,sizeof(str33), 1, fpnew )!=1) {
                        perror("Error writing file"); return(-1);
                    }
                    break;
                case 5: //slt
                    sscanf( op, "$%d,$%d,$%d", &rd, &rs, &rt );
                    opcode = 0;
                    strcpy(str33,binconv(opcode,6));
                    text_bits+=6;
                    strcat(str33,binconv(rs,5));
                    text_bits+=5;
                    strcat(str33,binconv(rt,5));
                    text_bits+=5;
                    strcat(str33,binconv(rd,5));
                    text_bits+=5;
                    strcat(str33,binconv(0,5));
                    text_bits+=5;
                    strcat(str33,binconv(42,6));
                    //printf("%s\n",str33);
                    text_bits+=6;
                    if (fwrite(str33,sizeof(str33), 1, fpnew )!=1) {
                        perror("Error writing file"); return(-1);
                    }
                    break;
                
                /*need to figure out how to reference labels for imm*/

                case 6: //addi
                    sscanf( op, "$%d,$%d,%d", &rt, &rs, &imm );
                    opcode = 8;
                    strcpy(str33,binconv(opcode,6));
                    text_bits+=6;
                    strcat(str33,binconv(rs,5));
                    text_bits+=5;
                    strcat(str33,binconv(rt,5));
                    text_bits+=5;
                    strcat(str33,binconv(imm,16));
                    //printf("%s\n",str33);
                    text_bits+=16;
                    if (fwrite(str33,sizeof(str33), 1, fpnew )!=1) {
                        perror("Error writing file"); return(-1);
                    }
                    break;
                case 7: //lui
                    
                    sscanf( op, "$%d,%s", &rt, lower);
                    for(int i=0;i<lab_count;i++){
                        if(strcmp(lab_name[i],lower)==0)
                            imm = lab_add[i];
                    }
                    //store the upper 16
                    strcpy(temp,binconv(imm,32));
                    temp[16] = '\0';
                    opcode = 15;
                    strcpy(str33,binconv(opcode,6));
                    text_bits+=6;
                    strcat(str33,binconv(0,5));
                    text_bits+=5;
                    strcat(str33,binconv(rt,5));
                    text_bits+=5;
                    strcat(str33,temp); //make this upper
                    printf("%s\n",str33);
                    text_bits+=16;
                    if (fwrite(str33,sizeof(str33), 1, fpnew )!=1) {
                        perror("Error writing file"); return(-1);
                    }
                    break;
                case 8: //ori
                    sscanf( op, "$%d,%s", &rt,lower);
                    for(int i=0;i<lab_count;i++){
                        if(strcmp(lab_name[i],lower)==0)
                            imm = lab_add[i];
                    }
                    opcode = 13;
                    strcpy(str33,binconv(opcode,6));
                    text_bits+=6;
                    strcat(str33,binconv(rt,5));
                    text_bits+=5;
                    strcat(str33,binconv(rt,5));
                    text_bits+=5;
                    strcat(str33,binconv(imm,16)); //lower 16
                    //printf("%s\n",str33);
                    text_bits+=16;
                    if (fwrite(str33,sizeof(str33), 1, fpnew )!=1) {
                        perror("Error writing file"); return(-1);
                    }
                    break;
                case 9: //lw
                    sscanf( op, "$%d,%d($%d)", &rt, &imm, &rs);
                    opcode = 35;
                    strcpy(str33,binconv(opcode,6));
                    text_bits+=6;
                    strcpy(str33,binconv(rs,5));;
                    text_bits+=5;
                    strcat(str33,binconv(rt,5));
                    text_bits+=5;
                    strcat(str33,binconv(imm,16));
                    //printf("%s\n",str33);
                    text_bits+=16;
                    if (fwrite(str33,sizeof(str33), 1, fpnew )!=1) {
                        perror("Error writing file"); return(-1);
                    }
                    break;
                case 10: //sw
                    sscanf( op, "$%d,%d($%d)", &rt, &imm, &rs);
                    opcode = 43;
                    strcpy(str33,binconv(opcode,6));
                    text_bits+=6;
                    strcat(str33,binconv(rs,5));;
                    text_bits+=5;
                    strcat(str33,binconv(rt,5));
                    text_bits+=5;
                    strcat(str33,binconv(imm,16));
                    //printf("%s\n",str33);
                    text_bits+=16;
                    if (fwrite(str33,sizeof(str33), 1, fpnew )!=1) {
                        perror("Error writing file"); return(-1);
                    }
                    break;
                    
                case 11: //LA
                    sscanf( op, "$%d,%s", &rt, lower);
                    
                    for(int i=0;i<lab_count;i++){
                        if(strcmp(lab_name[i],lower)==0){
                            imm = lab_add[i];
                            
                        }
                    }
                    strcpy(temp,binconv(imm,32));
                    temp[16] = '\0';
                    opcode = 15;
                    strcpy(str65,binconv(opcode,6));
                    text_bits+=6;
                    strcat(str65,binconv(0,5));
                    text_bits+=5;
                    strcat(str65,binconv(rt,5));
                    text_bits+=5;
                    strcat(str65,temp); //make this upper //make this upper
                    //printf("%s\n",str65);
                    text_bits+=16;
                    
                    //ori
                    
                    opcode = 13;
                    strcat(str65,binconv(opcode,6));
                    text_bits+=6;
                    strcat(str65,binconv(rt,5));
                    text_bits+=5;
                    strcat(str65,binconv(rt,5));
                    text_bits+=5;
                    strcat(str65,binconv(imm,16)); //make this lower
                    //printf("%s\n",str33);
                    text_bits+=16;
                    if (fwrite(str65,sizeof(str65), 1, fpnew )!=1) {
                        perror("Error writing file"); return(-1);
                    }
                    break;
                    
                    
                    

                case 12:
                    //directive do nothing
                    break;

                     
                default:
                    printf("Error: value passed not between 1-10\n");
            }
        }
            
            
    }
        
            int n = data_bits;
            while(n<512){
                s = strtol("00000000", NULL, 2);
                if (fwrite(&s,sizeof(s), 1, fpnew )!=1) {
                    perror("Error writing file"); return(-1);
                }
                n++;
            }
        
        
            //text section complete
            //fill 512-bitcount with 0
            n = text_bits;
            while(n<1024){
                s = strtol("00000000", NULL, 2);
                if (fwrite(&s,sizeof(s), 1, fpnew )!=1) {
                    perror("Error writing file"); return(-1);
                }
                n++;
            }
        
        printf("bitcount: %d\n",text_bits);
    

        /*close pointers to files*/
    
        fclose(fp);
        fclose(fpnew);

    

        return(0);
}



