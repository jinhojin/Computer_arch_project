#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int count1 =0;
int count2 =0;
int count3 =0;
int count_word = 0;
char*** order;
char*** label_address;
char **word_save;
int number = 0;

void eliminate (char *str, char ch) {
    if (!str) return;

    for(char* cp = str; 1 ; ) {
        if(*cp != ch)
            *str++ = *cp;
        if ('\0' == *cp++)
            break;
    }
}

void label_set(char* file, int address_count, int num2){
	char* ptr = strtok(file," ");
	if(strstr(ptr, "\n") != NULL){
		char* ptr =strtok(file,"\n");
	}
	eliminate(ptr,':');
	label_address[num2][0]=(char*)malloc(sizeof(char) * 50);
	strcpy(label_address[num2][0], ptr);
	label_address[num2][1]=(char*)malloc(sizeof(char) * 50);
	sprintf(label_address[num2][1], "%d", address_count);
}
void bin_to_hex(char* input, char* outputfile){
    
    int value = strtol(input, NULL, 2);
    char hexstring[31];
    sprintf(hexstring, "%x", value);
    FILE *fp;
    fp = fopen(outputfile, "a");
    fputs("0x",fp);
    fputs(hexstring,fp);
    fputs("\n",fp);
    fclose(fp);
}
char* dec_to_hex(int input, int size){
	char* result;
	size = size;
	char temp[50];
	result = (char*) malloc(sizeof(char)*size);
	char hexadecimal[32] = { 0, };
	sprintf(temp, "%x", input);
	for(int i= strlen(temp); i< size; i++){
	    if(i == strlen(temp)){
	        strcpy(result,"1");
	    }else{
	        strcat(result,"0");
	    }
	}
    strcat(result,temp);
	return result;
}

char* dec_to_bin(int input, int size){
	char leftover[32]={0,};
	int i;
	char* result;
	char* result_minus;
	int trigger = 0;
	result = (char*) malloc(sizeof(char)*size);
	result_minus = (char*) malloc(sizeof(char)*size);
	char temp[50];
	if(input<0){
	    trigger=1;
	    input = input-(2*input);
	}
	for(i=0; input>0; i++){
		leftover[i] = input%2;
		input = input/2;
	}

	for(i=size-1; i>=0; i--){
	    if(trigger == 1){
	        if(leftover[i] == 0){
	            sprintf(temp,"%d", 1);
	        }else{
	            sprintf(temp,"%d", 0);
	        }
	    }else{
		    sprintf(temp, "%d", leftover[i]);
	    }
		if(i == size-1){
		    strcpy(result,temp);
		}else{
		    strcat(result,temp);
		}
	}
	
	if(trigger == 1){
	    int value = strtol(result, NULL, 2);
	    int value_2 = value + 1;
	    result_minus = dec_to_bin(value_2, size);
	    return result_minus;
	}

	return result;
}

void binary_maker(char** input, int format, int j, char* outputfile){
	char* result;
	result = (char*) malloc(sizeof(char)*32);
	if(format == 1){
		char* op="000000";
		char* funct;
		char* shamt;
		char* rd;
		char* rs;
		char* rt;
		if(strcmp(input[0],"addu") == 0){
			funct="100001";
			shamt="00000";
			rd = dec_to_bin(atoi(input[1]),5);
			rs = dec_to_bin(atoi(input[2]),5);
			rt = dec_to_bin(atoi(input[3]),5);
		}
		if(strcmp(input[0],"and") == 0){
			funct="100100";
			shamt="00000";
			rd = dec_to_bin(atoi(input[1]),5);
			rs = dec_to_bin(atoi(input[2]),5);
			rt = dec_to_bin(atoi(input[3]),5);
		}
		if(strcmp(input[0],"jr") == 0){
			funct="001000";
			shamt="00000";
			rd = "00000";
			rs = dec_to_bin(atoi(input[1]),5);
			rt = "00000";
		}
		if(strcmp(input[0],"nor") == 0){
			funct="100111";
			shamt="00000";
			rd = dec_to_bin(atoi(input[1]),5);
			rs = dec_to_bin(atoi(input[2]),5);
			rt = dec_to_bin(atoi(input[3]),5);
		}
		if(strcmp(input[0],"or") == 0){
			funct="100101";
			shamt="00000";
			rd = dec_to_bin(atoi(input[1]),5);
			rs = dec_to_bin(atoi(input[2]),5);
			rt = dec_to_bin(atoi(input[3]),5);
		}
		if(strcmp(input[0],"sltu") == 0){
			funct="101011";
			shamt="00000";
			rd = dec_to_bin(atoi(input[1]),5);
			rs = dec_to_bin(atoi(input[2]),5);
			rt = dec_to_bin(atoi(input[3]),5);
		}
		if(strcmp(input[0],"sll") == 0){
			funct="000000";
			rs="00000";
			rd = dec_to_bin(atoi(input[1]),5);
			rt = dec_to_bin(atoi(input[2]),5);
			shamt = dec_to_bin(atoi(input[3]),5);
		}
		if(strcmp(input[0],"srl") == 0){
			funct="000010";
			rs="00000";
			rd = dec_to_bin(atoi(input[1]),5);
			rt = dec_to_bin(atoi(input[2]),5);
			shamt = dec_to_bin(atoi(input[3]),5);
		}
		if(strcmp(input[0],"subu") == 0){
			funct="100011";
			shamt="00000";
			rd = dec_to_bin(atoi(input[1]),5);
			rs = dec_to_bin(atoi(input[2]),5);
			rt = dec_to_bin(atoi(input[3]),5);
		}
		strcpy(result, op);
		strcat(result, rs);
		strcat(result, rt);
		strcat(result, rd);
		strcat(result, shamt);
		strcat(result, funct);
	    bin_to_hex(result, outputfile);
	}
	if(format == 2){
		char* op;
		char* imm;
		char* rs;
		char* rt;
		if(strcmp(input[0],"addiu") == 0){
			op="001001";
			
			if(strstr(input[3],"0x") != NULL){
		        int temp;
		        char temp2[50];
		        temp = strtol(input[3],NULL,16);
		        sprintf(temp2,"%d",temp);
		        imm = dec_to_bin(atoi(temp2),16);
		    }else{
			    imm=dec_to_bin(atoi(input[3]),16);
		    }
		    
			rs = dec_to_bin(atoi(input[2]),5);
			rt = dec_to_bin(atoi(input[1]),5);
		}
		if(strcmp(input[0],"andi") == 0){
			op="001100";
			if(strstr(input[3],"0x") != NULL){
		        int temp;
		        char temp2[50];
		        temp = strtol(input[3],NULL,16);
		        sprintf(temp2,"%d",temp);
		        imm = dec_to_bin(atoi(temp2),16);
		    }else{
			    imm=dec_to_bin(atoi(input[3]),16);
		    }
			rs = dec_to_bin(atoi(input[2]),5);
			rt = dec_to_bin(atoi(input[1]),5);
		}
		if(strcmp(input[0],"beq") == 0){
			op="000100";
			for(int i=0; i<count2; i++){
				if(strcmp(input[3],label_address[i][0]) == 0){
					imm = dec_to_bin(atoi(label_address[i][1]),16);
				}else{
					continue;
				}
			}
			rs = dec_to_bin(atoi(input[1]),5);
			rt = dec_to_bin(atoi(input[2]),5);
		}
		if(strcmp(input[0],"bne") == 0){
			op="000101";
			for(int i=0; i<count2; i++){
				if(strcmp(input[3], label_address[i][0]) == 0){
					imm = dec_to_bin((atoi(label_address[i][1]) - (4194304 + 4*j) - 4)/4,16);
				}else{
					continue;
				}
			}
			rs = dec_to_bin(atoi(input[1]),5);
			rt = dec_to_bin(atoi(input[2]),5);
		}
		if(strcmp(input[0],"lui") == 0){
			op="001111";
			if(strstr(input[2],"0x") != NULL){
		        int temp;
		        char temp2[50];
		        temp = strtol(input[2],NULL,16);
		        sprintf(temp2,"%d",temp);
		        imm = dec_to_bin(atoi(temp2),16);
		    }else{
			    imm=dec_to_bin(atoi(input[2]),16);
		    }
			rs = "00000";
			rt = dec_to_bin(atoi(input[1]),5);
		}
		if(strcmp(input[0],"lw") == 0){
			op="100011";
			if(strstr(input[2],"0x") != NULL){
		        int temp;
		        char temp2[50];
		        temp = strtol(input[2],NULL,16);
		        sprintf(temp2,"%d",temp);
		        imm = dec_to_bin(atoi(temp2),16);
		    }else{
			    imm=dec_to_bin(atoi(input[2]),16);
		    }
			rs = dec_to_bin(atoi(input[3]),5);
			rt = dec_to_bin(atoi(input[1]),5);
		}
		if(strcmp(input[0],"lb") == 0){
			op="100000";
			if(strstr(input[2],"0x") != NULL){
		        int temp;
		        char temp2[50];
		        temp = strtol(input[2],NULL,16);
		        sprintf(temp2,"%d",temp);
		        imm = dec_to_bin(atoi(temp2),16);
		    }else{
			    imm=dec_to_bin(atoi(input[2]),16);
		    }
			rs = dec_to_bin(atoi(input[3]),5);
			rt = dec_to_bin(atoi(input[1]),5);
		}
		if(strcmp(input[0],"ori") == 0){
			op="001101";
			if(strstr(input[3],"0x") != NULL){
		        int temp;
		        char temp2[50];
		        temp = strtol(input[3],NULL,16);
		        sprintf(temp2,"%d",temp);
		        imm = dec_to_bin(atoi(temp2),16);
		    }else{
			    imm=dec_to_bin(atoi(input[3]),16);
		    }
			rs = dec_to_bin(atoi(input[2]),5);
			rt = dec_to_bin(atoi(input[1]),5);
		}
		if(strcmp(input[0],"sltiu") == 0){
			op="001011";
			if(strstr(input[3],"0x") != NULL){
		        int temp;
		        char temp2[50];
		        temp = strtol(input[3],NULL,16);
		        sprintf(temp2,"%d",temp);
		        imm = dec_to_bin(atoi(temp2),16);
		    }else{
			    imm=dec_to_bin(atoi(input[3]),16);
		    }
			rs = dec_to_bin(atoi(input[2]),5);
			rt = dec_to_bin(atoi(input[1]),5);
		}
		if(strcmp(input[0],"sw") == 0){
			op="100011";
			if(strstr(input[2],"0x") != NULL){
		        int temp;
		        char temp2[50];
		        temp = strtol(input[2],NULL,16);
		        sprintf(temp2,"%d",temp);
		        imm = dec_to_bin(atoi(temp2),16);
		    }else{
			    imm=dec_to_bin(atoi(input[2]),16);
		    }
			rs = dec_to_bin(atoi(input[3]),5);
			rt = dec_to_bin(atoi(input[1]),5);
		}
		if(strcmp(input[0],"sb") == 0){
			op="100011";
			if(strstr(input[2],"0x") != NULL){
		        int temp;
		        char temp2[50];
		        temp = strtol(input[2],NULL,16);
		        sprintf(temp2,"%d",temp);
		        imm = dec_to_bin(atoi(temp2),16);
		    }else{
			    imm=dec_to_bin(atoi(input[2]),16);
		    }
			rs = dec_to_bin(atoi(input[3]),5);
			rt = dec_to_bin(atoi(input[1]),5);
		}
		strcpy(result, op);
		strcat(result, rs);
		strcat(result, rt);
		strcat(result, imm);
		bin_to_hex(result, outputfile);
	}
	if(format == 3){
		char* target;
		char* op;
		if(strcmp(input[0],"j") == 0){
			op="000010";
			for(int i=0; i<count2; i++){
			    if(strcmp(input[1],label_address[i][0]) == 0){
			        target = dec_to_bin(atoi(label_address[i][1])/4,26);
			    }
			}
		}
		if(strcmp(input[0],"jal") == 0){
			op="000011";
			for(int i=0; i<count2; i++){
			    if(strcmp(input[1],label_address[i][0]) == 0){
			        target = dec_to_bin(atoi(label_address[i][1])/4,26);
			    }
			}
		}
		strcpy(result,op);
		strcat(result,target);
		bin_to_hex(result, outputfile);
	}
	if(format == 4){
	    char* address_dec;
	    char* address_hex;
	    char* op;
		char* imm;
		char* rs;
		char* rt;
	    for(int i=0; i< count2; i++){
	        if(strcmp(input[2],label_address[i][0]) == 0){
	            address_dec = (char*) malloc(sizeof(char)*strlen(label_address[i][1]));
	            strcpy(address_dec,label_address[i][1]);
	        }else{
	            continue;
	        }
	    }
		
		op="001111";
		imm=dec_to_bin(4096 ,16);
		rs = "00000";
		rt = dec_to_bin(atoi(input[1]),5);
		strcpy(result, op);
		strcat(result, rs);
		strcat(result, rt);
		strcat(result, imm);
		bin_to_hex(result, outputfile);

	    if((atoi(address_dec) & 65535) != 0){
            op="001101";
			imm=dec_to_bin(atoi(address_dec),16);
			rs = dec_to_bin(atoi(input[1]),5);
			rt = dec_to_bin(atoi(input[1]),5);
			strcpy(result, op);
		    strcat(result, rs);
		    strcat(result, rt);
		    strcat(result, imm);
		    bin_to_hex(result, outputfile);
	    }
	    
	}
}


int Format_select(char* op){
	char* R_format[9] = {"addu", "and", "jr", "nor", "or", "sltu", "sll", "srl", "subu"};
	char* I_format[11] = {"addiu", "andi", "beq", "bne", "lui", "lw", "ori", "sltiu", "sw", "lb", "sb"};
	char* J_format[2] = {"j", "jal"};
	for(int i=0; i<9; i++){
		if(strcmp(op, R_format[i]) == 0){
			return 1; 
		}else{
			continue;
		}
	}

	for(int i=0; i<11; i++){
		if(strcmp(op, I_format[i]) == 0){
			return 2;
		}else{
			continue;
		}
	}

	for(int i=0; i<2; i++){
		if(strcmp(op, J_format[i]) == 0){
			return 3;
		}else{
			continue;
		}
	}
	
	if(strcmp(op,"la") == 0){
	    return 4;
	}

}

void makespace(){
	order = (char ***) malloc (sizeof(char**) * count1);
	label_address = (char ***) malloc (sizeof(char**) * count2);
	for(int i=0; i<count2; i++){
		label_address[i] = (char**) malloc ((sizeof(char*)+1) * 2 );
	}
}

void fillspace(int num, char* file){
	order[number]= (char**) malloc (sizeof(char*) * num);
	eliminate(file, '$');
	eliminate(file, ',');
	eliminate(file, '\n');
	eliminate(file, ')');
	char* ptr = strtok(file," ");
	if(strstr(ptr,":") != NULL){
	    ptr = strtok(NULL," ");
	}
	for(int i=0; i< num; i++){
		if(strstr(ptr,"(") != NULL){
			strtok(ptr,"(");
		}
		order[number][i] = (char*) malloc (sizeof(char)*100);
		strcpy(order[number][i], ptr);
		ptr = strtok(NULL, " ");
	}
	number++;
}


void savefile(char* filename){
	FILE *fp;
	char* readfile;
	char line[255];
	int num=0;
	int num2=0;
	int count_temp = 0;
	int address_count=-4;
	int fill_count = 0;
	int trigger =0;
	word_save = (char**) malloc (sizeof(char*)*count_word);
	fp = fopen(filename,"r");
	while(!feof(fp)){
		readfile = fgets(line, sizeof(line), fp);
		if(readfile == NULL){
			continue;
		}
		else if(strlen(readfile) == 1){
		    continue;
		}
		else if(strstr(readfile,":") != NULL){
			int temp = strlen(readfile);
			int temp_trigger = 0;
			char* readfile_save_2 = malloc(sizeof(char) * (temp+1));
			strcpy(readfile_save_2, readfile);
			char* readfile_save_3 = malloc(sizeof(char) * (temp+1));
			strcpy(readfile_save_3, readfile);
			char* readfile_save_4 = malloc(sizeof(char) * (temp+1));
			strcpy(readfile_save_4, readfile);
			if(strstr(readfile,".word") != NULL){
			    address_count = address_count+4;
			    word_save[count_temp] = (char*) malloc (sizeof(char)*50);
			    char* ptr = strtok(readfile_save_4," ");
			        while(ptr != NULL){
			            if(temp_trigger == 1){
			                strcpy(word_save[count_temp],ptr);
			            }
				        if(strstr(ptr,"word") != NULL){
				            temp_trigger = 1;
				        }
				        ptr = strtok(NULL, " ");
		            }
		        count_temp++;
			}
			if(strstr(readfile,".") == NULL){
			    char* ptr = strtok(readfile_save_3,":");
			    int temp2 = strlen(ptr);
			    if(temp > (temp2+2)){
			        size_t size = strlen(readfile);
			        char* readfile_save = malloc(sizeof(char) * (size+1));
			        strcpy(readfile_save, readfile);

			        char* ptr = strtok(readfile," ");
			        while(ptr != NULL){
				        if(strstr(ptr,"(") != NULL){
				            num++;
				        }
				        ptr = strtok(NULL, " ");
				        num++;
		            }
		            num--;
		            address_count = address_count - 4;
			        fillspace(num,readfile_save);
			    }
			    eliminate(readfile,'\n');
			    if(strcmp(readfile,"exit:") == 0){
		                address_count = address_count - 4;
		            }
			}
			if(trigger == 1){
			    if(fill_count != 0){
			        address_count = address_count + 4;
			    }
			    fill_count++;
			}
			label_set(readfile_save_2,address_count,num2);
			num2++;
			free(readfile_save_2);
			free(readfile_save_3);
		}else{
			if(strstr(readfile,".word") != NULL){
				address_count = address_count+4;
				int temp_trigger = 0;
				int temp = strlen(readfile);
				char* readfile_save_4 = malloc(sizeof(char) * (temp+1));
			    strcpy(readfile_save_4, readfile);
				word_save[count_temp] = (char*) malloc (sizeof(char)*50);
			    char* ptr = strtok(readfile_save_4," ");
			        while(ptr != NULL){
			            if(temp_trigger == 1){
			                strcpy(word_save[count_temp],ptr);
			            }
				        if(strstr(ptr,"word") != NULL){
				            temp_trigger = 1;
				        }
				        ptr = strtok(NULL, " ");
		            }
		        count_temp++;
				continue;
			}
			else if(strstr(readfile,".text")!=NULL){
			    address_count=4194304;
			    trigger = 1;
			    continue;
			}
			else if(strstr(readfile,".") != NULL){
			    continue;
			}else{
			    size_t size = strlen(readfile);
			    char* readfile_save = malloc(sizeof(char) * (size+1));
			    strcpy(readfile_save, readfile);

			    char* ptr = strtok(readfile," ");
			    while(ptr != NULL){
				    if(strstr(ptr,"(") != NULL){
				        num++;
				    }
				    ptr = strtok(NULL, " ");
				    num++;
		        }
		        address_count = address_count +4;
			    fillspace(num,readfile_save);
			    free(readfile_save);
			
	        }
        }
        num=0;
    }
    fclose(fp);
}

void countfile(char* filename){
	FILE *fp;
	char* readfile;
	char line[255];
	fp = fopen(filename,"r");
	while(!feof(fp)){
		readfile = fgets(line, sizeof(line), fp);
		if(readfile == NULL){
			continue;
		}
		else if(strcmp(readfile,"\n") == 0){
		    continue;
		}
		else if(strstr(readfile,":") != NULL){
			count2++;
			if(strstr(readfile,"word")){
				    count_word++;
			}
			if(strstr(readfile,".") == NULL){
			    size_t size = strlen(readfile);
			    int temp = strlen(readfile);
			    char* readfile_save = malloc(sizeof(char) * (size+1));
			    strcpy(readfile_save, readfile);
			    char* ptr = strtok(readfile_save,":");
			    int temp2 = strlen(ptr);
			    if(temp > (temp2+2)){
			        count1++;
			    }
			}
		}else{
			if(strstr(readfile,".") != NULL){
				if(strstr(readfile,"word")){
				    count_word++;
				}
				continue;
			}
			count1++;
		}
	}
	fclose(fp);
	makespace();
	savefile(filename);
}

void check_la(char** input){
    char* address_dec;
    for(int i=0; i< count2; i++){
        if(strcmp(input[2],label_address[i][0]) == 0){
            address_dec = (char*) malloc(sizeof(char)*strlen(label_address[i][1]));
            strcpy(address_dec,label_address[i][1]);
        }else{
            continue;
        }
    }
	if((atoi(address_dec) & 65535) != 0){
	    count3 = count1 + 1;
	    return;
	}else{
	    count3 = count1;
	    return;
	}
}

void main(int argc, char* argv[]){
	int format;
	char outputfile[50];
	countfile(argv[1]);
	char* temp_binary_1;
	char* temp_binary_2;
	char* temp_binary_3;
	strcpy(outputfile, argv[1]);
	strtok(outputfile,".");
	strcat(outputfile,".o");
	for(int k = 0 ; k<count1; k++){
	    if(strstr(order[k][0], "la") != NULL){
	        check_la(order[k]);
	    }
	}
	temp_binary_1 = dec_to_bin((count3)*4,10);
	bin_to_hex(temp_binary_1, outputfile);
	temp_binary_2 = dec_to_bin((count_word)*4,10);
	bin_to_hex(temp_binary_2, outputfile);
	for(int i=0; i<count1; i++){
		format = Format_select(order[i][0]);
		binary_maker(order[i], format,i,outputfile);
	}

	for(int j=0; j< count_word; j++){
	    if(strstr(word_save[j],"0x")){
	        FILE *fp;
            fp = fopen(outputfile, "a");
            fputs(word_save[j],fp);
            fclose(fp);
	    }else{
	        temp_binary_3 = dec_to_bin(atoi(word_save[j]),16);
	        bin_to_hex(temp_binary_3,outputfile);
	    }
	}
	count1 = 0;
    count2 = 0; 
    number = 0;
    free(order);
    free(label_address);
}
