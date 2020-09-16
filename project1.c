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



int main(int argc, char *argv[]) {

   FILE *fp; //pointer for read file 
   char str[60],label[20], instruction[20], operand[20]; //buffer vals 
   bool data_sec; 
   int bitcount, data[512], text[512]; //binary buffer vals 

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
        puts(str);

    
        if (strcmp(str, ".data")==0){
            data_sec = true;
            bitcount = 0;

        }
        if(strcmp(str, ".text")==0){
            data_sec = false;
            bitcount = 0;
        }
        
        
        if(data_sec){
        /*.data section first 512 bits*/
            sscanf( str, "%s %s %s", label, instruction, operand );
            //read all data section into string
            //parse into op, rs, rt, rd, shamt, func
            //convert into binary
            //append all remaing bits to 0
        }

        else{
        /*.text section last 512 bits*/
            sscanf( str, "%s\t%s\t%s\n", label, instruction, operand );
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


