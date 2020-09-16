/*#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

int main(){

	
	char *str[] = {"ADD","SUB","SLL","SRL","SLT"
				   "ADDI","LUI","ORI","LW","SW"};
	char input[]="ADDI";
	int index;
	for(int i = 0;i<10;i++){
		if(strcmp(str[i],input)==0){
			index = ++i;
			break;
		}
	}
	printf("%d\n",index );
}
*/

#include <ctype.h>
#include <stdio.h>
#include <math.h>

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


int main()
{
    /*.data section first 512 bits*/
    //read all data section into string
    char str[] ="start:\tADD\t$4,$0,$1";
    //if (strcmp(str, ".data")==0){
    int data[512], bit_count; bit_count = 0;
    //parse into op, rs, rt, rd, shamt, func
    char label[20], instruction[20], operand[20] = "/0";
    int  rs, rt, rd,ins_val;
    ins_val =1;
    sscanf( str, "%s\t%s\t%s", label, instruction, operand );
    //method to convert instruction into table value
    switch(ins_val){
        case 1: //add
            sscanf( operand, "$%d,$%d,$%d", rd, rs, rt );
            break;
            /*
        case 2: //sub
            break;
        case 3: //sll
            break;
        case 4: //srl
            break;
        case 5: //slt
            break;
        case 6: //addi
            break;
        case 7: //lui
            break;
        case 8: //ori
            break;
        case 9: //lw
            break;
        case 10: //sw
            break;
             */
        default:
            printf("Error: value passed not between 1-10\n");
            
            
    }
    printf("%s\t%s\t%s\n",label,instruction,operand);
    printf("rs: %d\trt: %d\trd: %d\n", rd,rs,rt);
            
            
            //convert into binary
            //append all remaing bits to 0
}
