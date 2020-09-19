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

//push values from a to b
int *arrypush(int *a, int *b, int offset, int n ){
    for(int i=offset;i<n+offset;i++){
        b[i] = a[i];
    }
    return b;
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

void print_array(int *a, int n){
    for (int i = 0; i < n; ++i)
    {
        printf("%d",a[i] );
        if (i == n-1) printf("\n");
    }
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
    char str[60],lab[20], dir[20], ins[20], op[20], lower[20]; //buffer vals
    ins[0] = '\0';
    bool data_sec;
    int bitcount, opcode,rd,rs,rt,imm,input, reg8[8],reg16[16], reg32[32], data[512], text[512]; //binary buffer vals
    int lab_count = 0;
    int reg6[] = {0,0,0,0,0,0};
    int reg5[] = {0,0,0,0,0,0};
    
    char **lab_name, **lab_val;
    lab_name = malloc(numofstrings*sizeof(char*));
    int *lab_add = malloc(100*sizeof(int*));
    lab_val = malloc(numofstrings*sizeof(char*));
    int x = 0;
    

    for (int i = 0; i < 16; ++i)
        reg16[i]= 0;
    for(int i = 0; i < 512; i++)
        data[i]=0;
    for (int i = 0; i < 32; ++i)
        reg32[i]= 0;



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
    while( fgets (str, 60, fp)!=NULL ) {
        /* print content to stdout */
        int len = strlen(str);
        //trim off the /n
        if( str[len-1] == '\n' )
            str[len-1] = 0;
        puts(str);
        
        char* pPosition = strchr(str, ':');
        if (strcmp(str, "\t.data")==0){
            data_sec = true;
            bitcount = 0;

        }
        if(strcmp(str, "\t.text")==0){
            data_sec = false;
            x = bitcount;
            bitcount = 0;

        }
        
        
        
        if(data_sec){
        /*.data section first 512 bits*/
            
            
            
           if(str[0]=='\t')
                strcpy(str,"\0");
           else{
               if(pPosition){//check for label
                   sscanf( str, "%s %s %s", lab, dir, op );
                   lab[strlen(lab)-1]='\0';
               }
               else
                sscanf (str,"%s %s", dir, op);
                //store our label with adress associated
                
                if(strcmp(dir,".space")==0){//check dir type
                   //.space
                    for (int i = 0; i < 8; ++i)
                        reg8[i]= 0;
                   imm = atoi(op);
                    lab_name[lab_count] = (char*)malloc(sizeof(char*) * strlen(lab));
                    strcpy(lab_name[lab_count],lab);
                    lab_add[lab_count] = bitcount;
                    lab_count++;
                    //fill data with value 0's
                    for(int j = 0; j<imm; j++){
                        arrypush(reg8, text, bitcount, 8);
                        bitcount += 8;
                        
                    }
                    
                        
                }
                    
           
                else{ //.word
                    //find how many ops
                    char *tok = strtok(op,",");
                    *(lab_val+0) = tok;
                    int i = 1;
                    while(tok != NULL){
                        tok = strtok(NULL, op);
                        *(lab_val+i) = tok;
                        if (tok != NULL)
                            i++;
                    }
                    for(int j = 0; j < i; j++){
                        imm = atoi(lab_val[j]);
                        decimalToBinary_n(imm, reg32, 32);
                        //print_array(reg32, 32);
                        arrypush(reg32, text, bitcount, 32);
                        lab_name[j] = (char*)malloc(sizeof(char*) * strlen(lab));
                        strcpy(lab_name[j],lab);
                        lab_add[j] = bitcount;
                        bitcount +=32;
                        lab_count++;
                    }
                }
           }
        }

        else{
        /*.text section last 512 bits*/
            if(pPosition){//check for label
                sscanf( str, "%s %s %s",lab,ins,op);
                lab[strlen(lab)-1]='\0';
                lab_name[lab_count] = (char*)malloc(sizeof(char*) * strlen(lab));
                strcpy(lab_name[lab_count],lab);
                lab_add[lab_count] = x;
                lab_count++;
                
            }
            else if(str[0] == '\t')
                input = 12;
            else{
                sscanf( str, "%s %s",ins, op );
            }
            if(str[0] != '\t'){
                input = ins_conv(ins);
            }
            

            switch(input){
                case 1: //add
                    sscanf( op, "$%d,$%d,$%d", &rd, &rs, &rt );
                    opcode = 0;
                    //decimalToBinary_n(opcode, reg6, 6);
                    //arrypush(reg6,data,bitcount,6);
                    strcpy(str,binconv(opcode,6));
                    bitcount+=6;
                    printf("reg6 of opcode contains: \n");
                    printf("%s\n",str);
                    //print_array(reg6,6);
                    //decimalToBinary_n(rs, reg5, 5);
                    //arrypush(reg5,data,bitcount,5);
                    strcat(str,binconv(rs,5));
                    bitcount+=5;
                    //printf("reg5 of rs contains: \n");
                    printf("%s\n",str);
                    //print_array(reg5,5);
                    //decimalToBinary_n(rt, reg5, 5);
                    //arrypush(reg5,data,bitcount,5);
                    strcat(str,binconv(rt,5));
                    bitcount+=5;
                    //printf("reg5 of rt contains: \n");
                    printf("%s\n",str);
                    //print_array(reg5,5);
                    //decimalToBinary_n(rd, reg5, 5);
                    //arrypush(reg5,data,bitcount,5);
                    strcat(str,binconv(rd,5));
                    bitcount+=5;
                    //printf("reg5 of rd contains: \n");
                    printf("%s\n",str);
                    //print_array(reg5,5);
                    //decimalToBinary_n(0,reg5,5);
                    //printf("reg5 of shamt contains: \n");
                    strcat(str,binconv(0,5));
                    printf("%s\n",str);
                    //print_array(reg5,5);
                    //arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    //decimalToBinary_n(32, reg6, 6);
                    //printf("reg6 of func contains: \n");
                    //print_array(reg6,6);
                    strcat(str,binconv(32,6));
                    printf("%s\n",str);
                    
                    //arrypush(reg6, data,bitcount,6);
                    bitcount+=6;
                    if (fwrite(str,sizeof(str), 1, fpnew )!=1) {
                        perror("Error writing file"); return(-1);
                    }
                    //printf("data contains: \n");
                    //print_array(data, 32);
                    
                    break;
                    
                case 2: //sub
                    sscanf( op, "$%d,$%d,$%d", &rd, &rs, &rt );
                    opcode = 0;
                    decimalToBinary_n(opcode, reg6, 6);
                    arrypush(reg6,data,bitcount,6);
                    bitcount+=6;
                    decimalToBinary_n(rs, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rt, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rd, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(0,reg5,5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(34, reg6, 6);
                    arrypush(reg6, data,bitcount,6);
                    bitcount+=6;
                    break;
                case 3: //sll
                    sscanf( op, "$%d,$%d,%d", &rd, &rs, &rt );
                    opcode = 0;
                    decimalToBinary_n(opcode, reg6, 6);
                    arrypush(reg6,data,bitcount,6);
                    bitcount+=6;
                    decimalToBinary_n(0, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rt, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rd, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rs,reg5,5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(0, reg6, 6);
                    arrypush(reg6, data,bitcount,6);
                    bitcount+=6;
                    break;
                case 4: //srl
                    sscanf( op, "$%d,$%d,%d", &rd, &rs, &rt );
                    opcode = 0;
                    decimalToBinary_n(opcode, reg6, 6);
                    arrypush(reg6,data,bitcount,6);
                    bitcount+=6;
                    decimalToBinary_n(0, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rt, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rd, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rs,reg5,5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(2, reg6, 6);
                    arrypush(reg6, data,bitcount,6);
                    bitcount+=6;
                    break;
                case 5: //slt
                    sscanf( op, "$%d,$%d,$%d", &rd, &rs, &rt );
                    opcode = 0;
                    decimalToBinary_n(opcode, reg6, 6);
                    arrypush(reg6,data,bitcount,6);
                    bitcount+=6;
                    decimalToBinary_n(rs, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rt, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rd, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(0,reg5,5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(42, reg6, 6);
                    arrypush(reg6, data,bitcount,6);
                    bitcount+=6;
                    break;
                
                /*need to figure out how to reference labels for imm*/

                case 6: //addi
                    sscanf( op, "$%d,$%d,%d", &rt, &rs, &imm );
                    opcode = 8;
                    decimalToBinary_n(opcode, reg6, 6);
                    arrypush(reg6,data,bitcount,6);
                    bitcount+=6;
                    decimalToBinary_n(rs, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rt, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(imm, reg16, 16);
                    arrypush(reg16,data,bitcount,16);
                    bitcount+=16;
                    break;
                case 7: //lui
                    
                    sscanf( op, "$%d,%s", &rt, lower);
                    for(int i=0;i<lab_count;i++){
                        if(strcmp(lab_name[i],lower)==0)
                            imm = lab_add[i];
                    }
                    decimalToBinary_n(imm, reg32, 32);
                    for(int i=0;i<16;i++)
                        reg16[i]=reg32[i];
                    opcode = 15;
                    decimalToBinary_n(opcode, reg6, 6);
                    arrypush(reg6,data,bitcount,6);
                    bitcount+=6;
                    decimalToBinary_n(0, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rt, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    arrypush(reg16,data,bitcount,16);
                    bitcount+=16;
                    break;
                case 8: //ori
                    sscanf( op, "$%d,%s", &rt,lower);
                    for(int i=0;i<lab_count;i++){
                        if(strcmp(lab_name[i],lower)==0)
                            imm = lab_add[i];
                    }
                    decimalToBinary_n(imm, reg32, 32);
                    for(int i=16;i<32;i++)
                        reg16[i] = reg32[i];
                    opcode = 13;
                    decimalToBinary_n(opcode, reg6, 6);
                    arrypush(reg6,data,bitcount,6);
                    bitcount+=6;
                    decimalToBinary_n(rt, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    arrypush(reg16,data,bitcount,16);
                    bitcount+=16;
                    break;
                case 9: //lw
                    sscanf( op, "$%d,%d($%d)", &rt, &imm, &rs);
                    opcode = 35;
                    decimalToBinary_n(opcode, reg6, 6);
                    arrypush(reg6,data,bitcount,6);
                    bitcount+=6;
                    decimalToBinary_n(rs, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rt, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(imm, reg16, 16);
                    arrypush(reg16,data,bitcount,16);
                    bitcount+=16;
                    break;
                case 10: //sw
                    sscanf( op, "$%d,%d($%d)", &rd, &imm, &rs);
                    opcode = 43;
                    decimalToBinary_n(opcode, reg6, 6);
                    arrypush(reg6,data,bitcount,6);
                    bitcount+=6;
                    decimalToBinary_n(rs, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rt, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(imm, reg16, 16);
                    arrypush(reg16,data,bitcount,16);
                    bitcount+=16;
                    break;
                    
                case 11: //LA
                    sscanf( op, "$%d,%s", &rt, lower);
                    for(int i=0;i<lab_count;i++){
                        if(strcmp(lab_name[i],lower)==0){
                            imm = lab_add[i];
                            break;
                        }
                    }
                    decimalToBinary_n(imm, reg32, 32);
                    for(int i=0;i<16;i++)
                        reg16[i]=reg32[i];
                    opcode = 15;
                    decimalToBinary_n(opcode, reg6, 6);
                    arrypush(reg6,data,bitcount,6);
                    bitcount+=6;
                    decimalToBinary_n(0, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    decimalToBinary_n(rt, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    arrypush(reg16,data,bitcount,16);
                    bitcount+=16;
                    
                    //ori
                    decimalToBinary_n(imm, reg32, 32);
                    for(int i=16;i<32;i++)
                        reg16[i] = reg32[i];
                    opcode = 13;
                    decimalToBinary_n(opcode, reg6, 6);
                    arrypush(reg6,data,bitcount,6);
                    bitcount+=6;
                    decimalToBinary_n(rt, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    arrypush(reg16,data,bitcount,16);
                    bitcount+=16;
                    break;
                    
                    
                    

                case 12:
                    //directive do nothing
                    break;

                    //need case for LA
                     
                default:
                    printf("Error: value passed not between 1-10\n");
            }
            //read all text section into string
            //parse into op, rs, rt, rd, shamt, func
            //op

            //convert into binary
            //append all remaing bits to 0
        }
            
        /*scan data into place holders for bin data to be converted*/
        /*write binary to new file(binfile)*/
        if (fwrite(str,sizeof(str), 1, fpnew )!=1) {
            perror("Error writing file"); return(-1);
        }
        
    }

    /*close pointers to files*/
    fclose(fp);
    fclose(fpnew);

    

    return(0);
}


