#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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
    for(int i = 0; i<n; i++){binLSB[i]=0;}//set all values to 0
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
    for(int i=offset;i<n;i++){
        b[i] = a[i];
    }
    return b;
}

//instruction convertor
int ins_conv(char *ins){
    char *tab[] = {"ADD","SUB","SLL","SRL","SLT" //symbol table
                   "ADDI","LUI","ORI","LW","SW"};
    int index;
    upper(ins);
    
    for(int i = 0;i<10;i++){
        if(strcmp(tab[i],ins)==0){
            index = ++i;
            break;
        }
    }
    
    return index;
}

void print_array(int a[], int n){
    for (int i = 0; i < n; ++i)
    {
        printf("%d\t",a[i] );
        if (i == n-1) printf("\n");
    }
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
    char str[60],lab[20], dir[20], ins[20], op[20]; //buffer vals
    ins[0] = '\0';
    bool data_sec;
    int bitcount, opcode,rd,rs,rt,imm,input, reg16[16], data[512], text[512]; //binary buffer vals

    
    int reg6[] = {0,0,0,0,0,0};
    int reg5[] = {0,0,0,0,0,0};

    for (int i = 0; i < 16; ++i)
        reg16[i]= 0;    
    for(int i = 0; i < 512; i++)
        data[i]=0;



   /* opening file for reading */
    fp = fopen(argv[1] , "r");
    if(fp == NULL) {
        perror("Error opening file");
        return(-1);
    }

    /*create file to write to new (bin)file*/
    FILE *fpnew;
    fpnew = fopen( "new1.txt" , "w" );

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
            bitcount = 0;

        }
            
        if(data_sec){
        /*.data section first 512 bits*/
            
            if(pPosition){//check for label
                sscanf( str, "%s: %s %s", lab, dir, op );
            }
            else
            sscanf( str, "%s %s",dir, op );
        }

        else{
        /*.text section last 512 bits*/
            if(pPosition){//check for label
                printf("entered label check in .text\n");
                sscanf( str, "%s:\t%s\t%s",lab,ins,op);
                printf("%s\n",str);
                printf("%s\t%s\t%s\n",lab,ins,op);
            }
            else if(str[0] == '\t')
                input = 12;
            else{
                sscanf( str, "%s %s",ins, op );
            }
            if(str[0] != '\t'){
                upper(ins);
                input = ins_conv(ins);
            }
            

            switch(input){
                case 1: //add
                    sscanf( op, "$%d,$%d,$%d", &rd, &rs, &rt );
                    opcode = 0;
                    decimalToBinary_n(opcode, reg6, 6);
                    arrypush(reg6,data,bitcount,6);
                    bitcount+=6;
                    printf("reg6 of opcode contains: \n");
                    print_array(reg6,6);
                    decimalToBinary_n(rs, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    printf("reg5 of rs contains: \n");
                    print_array(reg5,5);
                    decimalToBinary_n(rt, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    printf("reg5 of rt contains: \n");
                    print_array(reg5,5);
                    decimalToBinary_n(rd, reg5, 5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    printf("reg5 of rd contains: \n");
                    print_array(reg5,5);
                    decimalToBinary_n(0,reg5,5);
                    arrypush(reg5,data,bitcount,5);
                    bitcount+=5;
                    printf("reg5 of shamt contains: \n");
                    print_array(reg5,5);
                    decimalToBinary_n(32, reg6, 6);
                    arrypush(reg6, data,bitcount,6);
                    bitcount+=6;
                    printf("reg6 of func contains: \n");
                    print_array(reg5,5);
                    printf("data contains: \n");
                    print_array(data, 32);
                    
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
                    sscanf( op, "$%d,%s", &rt, &imm);
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
                    decimalToBinary_n(imm, reg16, 16);
                    arrypush(reg16,data,bitcount,16);
                    bitcount+=16;
                    break;
                case 8: //ori
                    sscanf( op, "$%d,$%d,%s", &rt,&rs,&imm);
                    opcode = 13;
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


