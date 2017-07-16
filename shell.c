void check(char *);
int div(int, int);
int div1(int,int);
int mod(int, int);
char convert(int);
main(){
	
	
	char buffer[13312];
	
	interrupt(0x21, 0, "Type Command:>\0", 0x2000, 0);
	interrupt(0x21, 1, buffer, 0x2000, 0);
	check(buffer);
	



}


void check(char * buffer){
int i = 0;
int j = 0;
int count = 0;
int unit = 0;
int ten = 0;
int sectors = 0;
int process=0;
char poss [5] ;
char poss2 [8] ;
char poss3 [7];
char poss4 [5];
char poss5 [4];
char poss6 [7];
char poss7 [5];


char buffer1[13312];
char filename [7] ;
char filename2 [7];
char filename3 [12];
char badCommand [14];
char inexistantFile [14];

int view = 1;
int exec = 1;
int del = 1;
int copy = 1;
int dir = 1;
int create = 1;
int kill= 1;

badCommand[0]='B';
badCommand[1]='a';
badCommand[2]='d';
badCommand[3]=' ';
badCommand[4]='C';
badCommand[5]='o';
badCommand[6]='m';
badCommand[7]='m';
badCommand[8]='a';
badCommand[9]='n';
badCommand[10]='d';
badCommand[11]='\n';
badCommand[12]=0xd;
badCommand[13]='\0';


poss[0]= 'v';
poss[1]= 'i';
poss[2]= 'e';
poss[3]= 'w';
poss[4]= ' ';


poss2[0] = 'e';
poss2[1] = 'x';
poss2[2] = 'e';
poss2[3] = 'c';
poss2[4] = 'u';
poss2[5] = 't';
poss2[6] = 'e';
poss2[7] = ' ';


poss3[0] = 'd';
poss3[1] = 'e';
poss3[2] = 'l';
poss3[3] = 'e';
poss3[4] = 't';
poss3[5] = 'e';
poss3[6] = ' ';

poss4[0] = 'c';
poss4[1] = 'o';
poss4[2] = 'p';
poss4[3] = 'y';
poss4[4] = ' ';

poss5[0] = 'd';
poss5[1] = 'i';
poss5[2] = 'r';
poss5[3] = 0xd;

poss6[0] = 'c';
poss6[1] = 'r';
poss6[2] = 'e';
poss6[3] = 'a';
poss6[4] = 't';
poss6[5] = 'e';
poss6[6] = ' ';

poss7[0]='k';
poss7[1]='i';
poss7[2]='l';
poss7[3]='l';
poss7[4]=' ';


for( i=0; i < 5 ; i++){
	if(buffer[i]=='\0'){
	view=0;
	break;
	}

	if(buffer[i] != poss[i]){
  		view = 0;
 	    break;
}
}

for(i=0; i < 8 ; i++){
	if(buffer[i]=='\0'){
	exec=0;
	break;
	}

	if(buffer[i] != poss2[i]){
  		exec = 0;
  		break;
	}
}

for(i=0; i < 7; i++){
 if(buffer[i]=='\0'){
	del = 0;
	break;
	}
  if(buffer[i] != poss3[i]){
  		del = 0;
  		break;
	}
}

for(i=0; i < 5; i++){
  if(buffer[i]=='\0'){
	copy = 0;
	break;
	}
  if(buffer[i] != poss4[i]){
  		copy = 0;
  		break;
	}
}	

for(i=0; i < 4; i++){
  if(buffer[i]=='\0'){
	dir = 0;
	break;
	}
  if(buffer[i] != poss5[i]){
  		dir = 0;
  		break;
	}
}	



for(i=0; i < 7; i++){
 if(buffer[i]=='\0'){
	create = 0;
	break;
	}
  if(buffer[i] != poss6[i]){
  		create = 0;
  		break;
	}
}

for(i=0; i < 5; i++){
 if(buffer[i]=='\0'){
	kill = 0;
	break;
	}
  if(buffer[i] != poss7[i]){
  		kill = 0;
  		break;
	}
}



if(view){
	for( i = 5;i < 11 ;i++)
	   filename[i-5]= buffer[i];

	filename[6]='\0';
	buffer1[0]= '\0';
	interrupt(0x21, 3, filename, buffer1, 0); /*read the file into buffer*/
	if(buffer1[0] != '\0' )
	interrupt(0x21, 0, buffer1, 0, 0); /*print out the file*/
}
else if(exec){
	for( i = 8;i <14 ; i++)
		filename[i-8] = buffer[i];

	filename[6]='\0';
	interrupt(0x21, 4, filename, 0x2000, 0);
}
else if(del){
	for( i = 7;i <14 ; i++)
		filename[i-7] = buffer[i];

	filename[6]='\0';
	interrupt(0x21, 7, filename, 0, 0); 
	
}
else if(copy){
   for(i = 5; i<11;i++){
	if(buffer[i]== ' ')
		break;
   	filename[i-5] = buffer[i];
   }
   filename[6]= '\0';
   interrupt(0x21, 3, filename, buffer1, 0); /*read the file into buffer*/
  
  
  i++;
   for( j = 0; j<6; j++){
  	filename2[j] = buffer[i+j];
   }

   filename2[6]= '\0';
	
 for(i = 0;i < 13312; i++){
	 if(buffer1[i] == 0)
	 break;
	 sectors++;
 }
 sectors = div(sectors, 512); 
interrupt(0x21, 8, filename2, buffer1, sectors); /*write the file from the buffer*/
 }
 else if(dir){

    interrupt(0x21, 2, buffer1, 2, 0); /*read the sector into buffer*/
   
   for (i = 0; i < 512 ; i += 32 ) {
	   sectors = 0;
	if(buffer1[i]!= 0){
		for(j = 0; j<6;j++)
   		  filename3[j] = buffer1[i+j];
		 
		 filename3[6]= ' ';// filname = filname + space+ number_of_sectors

		 for(j=6; j<32;j++){
			 if(buffer1[i+j] == 0)
			 break;
			 sectors++;
		 }

		unit = mod(sectors, 10);
		ten = div1(sectors, 10);
		filename3[7] = convert(ten) ;
		filename3[8] = convert(unit) ;
		filename3[9]='\n';
		filename3[10]=0xd;
		filename3[11]='\0';


		interrupt(0x21, 0, filename3, 0, 0); /*print out the file*/

	}

   }

	 
 }
else if(create){
	for( i = 7;i <14 ; i++)
		filename[i-7] = buffer[i];
	
	 filename[6]='\0';
	
	while(1){
	
	interrupt(0x21, 1, buffer, 0x2000, 0);
	 if(buffer[0]==0xd)
		break;
		i = 0;
	if(count< 13311){
	while(1){
		if(buffer[i]=='\0'){
			break;
		}
		buffer1[count++] = buffer[i++];
	}
	buffer1[count]='\0';
	}
	}
 
 sectors = div(count, 512); 
interrupt(0x21, 8, filename, buffer1,sectors); /*write the file from the buffer*/


}
else if(kill){
	process =buffer[5]-'0';
	if(process>7 || process <0)
		interrupt(0x21, 0, badCommand, 0, 0);
	else
		interrupt(0x21, 9, process, 0,0);
}
else {

	interrupt(0x21, 0, badCommand, 0, 0);
}




}

char convert(int x){
char res =' ';
switch(x){
    case 0: res = 48;  	  break;
	case 1: res = 49;  	  break;
	case 2: res = 50;    break;
	case 3: res = 51;	  break;
	case 4: res = 52;  break;
	case 5: res = 53;		  break;
	case 6: res = 54;   break;
	case 7: res = 55;		  break;
	case 8: res = 56;  break;
    case 9: res = 57;  break;
}
return res;
}

int div(int dividend, int divisor){ 

	int res = 0;
	while(dividend > 0){
		res++;
		dividend = dividend - divisor;
	}
	return res;
}


int div1(int dividend, int divisor){ 

	int res = 0;
	while(dividend >= divisor){
		res++;
		dividend = dividend - divisor;
	}
	return res;
}



int mod(int n,int d)
{

	int q = div1(n,d);

	return n-d*q;
}	
