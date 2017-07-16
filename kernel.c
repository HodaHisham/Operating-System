void printString(char*);
void readString(char*);
void readSector(char*,int);
void readFile(char*,char*);
void writeSector(char*, int);
int mod(int,int);
int div(int,int);
void handleInterrupt21(int,int,int,int);
void executeProgram(char*);
void terminate();
void deleteFile(char*);
void writeFile(char*, char*, int);
void handleTimerInterrupt(int, int);
void killProcess(int);

typedef struct pair {
   int active;
   int stackPointer;
} pair;  

pair process_table [8];
int currentProcess;
int counter;
main(){
 int i = 0; 
 counter = 1;
 currentProcess = 0;
 for(i=0; i < 8; i++){
	 process_table[i].active = 0;
	 process_table[i].stackPointer = 0xFF00;
	 }

 makeTimerInterrupt();
 makeInterrupt21();
 
 interrupt(0x21, 4, "shell\0", 0, 0);
 interrupt(0x21, 4, "hello1\0", 0, 0);
interrupt(0x21, 4, "hello2\0", 0, 0);
interrupt(0x21, 4, "phello\0", 0, 0);


 // interrupt(0x21, 4, "hello1\0", 0, 0);
 // interrupt(0x21, 4, "hello2\0", 0, 0);
//  interrupt(0x21, 4, "shell\0", 0x2000, 0);
 while(1); /*hang up*/



}

void killProcess(int proc){

// counter= 101;
	setKernelDataSegment();
	
	process_table[proc].active = 0;

	restoreDataSegment();
	// while(1);
}

void handleTimerInterrupt(int segment, int sp){
	

	int found = 0;
	int i = currentProcess + 1;
	int active = currentProcess;
	int new_segment = 0;
	int new_sp = 0;

	currentProcess = segment / 0x1000 -2;
	active= currentProcess;
	i= currentProcess +1;
	
if(counter == 101){
	counter = 1;
process_table[currentProcess].stackPointer = sp;
for(i=currentProcess+1; i< 8;i++){
	if(process_table[i].active == 1){
		found = 1;
		active = i;
		break;
	}
}
if(found == 0){
	for(i=0;i < currentProcess;i++){
		if(process_table[i].active == 1){
		found = 1;
		active = i;
		break;
	}
	}
}
if(found == 1){
currentProcess = active;
new_segment = (active+2);
new_segment = new_segment*0x1000;
new_sp = process_table[active].stackPointer;
returnFromTimer(new_segment, new_sp);
}
else{
returnFromTimer(segment,sp);
}}
else{ 
	counter++;
	returnFromTimer(segment,sp);
}
}



void writeFile(char* name, char* buffer, int secNum)
{
	char directory [512];
	char map [512];
	char buffer4 [512];
	int sectors [26];
	int i = 0;
	int j = 0;
	int foundDir = -1;
	int foundSec = 0;

	readSector (map,1);
	readSector(directory,2);

	if(secNum > 26)
	{
		printString("Cannot write file!\0");
		return;
	}

	for (i = 0; i < 512; i += 32) {
  	 	if(directory[i] == 0)
  	 	{
  	 		foundDir = i;
  	 		break;
  	 	}
	}
	for (i = 0; i < 256 && foundSec < secNum; ++i)
	{
		if(map[i] == 0)
  	 	{
  	 		sectors[foundSec++] = i;

  	 	}
	}
	if(foundDir == -1 || foundSec < secNum)
	{
		printString("Cannot write file!\0");
		return;
	}

	i = foundDir;

	for (j = 0; j < 6; j++)
	{
		if(name[j] == '\0')
			break;
		directory[i+j] = name[j];
	}

	for (; j < 6; j++)
	{
		directory[i+j] = 0;
	}

	for (j = 0; j < 26 && j < secNum; j++)
	{
		directory[i+j+6] = sectors[j];
		map[sectors[j]] = 0xFF;
		writeSector(buffer+512*j, sectors[j]);
		
		

	}

	for (; j < 26; j++)
	{
		directory[i+j+6] = 0;
	}
 	
	writeSector(map,1);
 	writeSector(directory,2);
}

void deleteFile(char* name){

	
	char directory [512];
	char map [512];
	int found = 0; 
 	int i = 0;
 	int j = 0;
 	int count=0;
    char badCommand [18];


	badCommand[0]='I';
	badCommand[1]='n';
	badCommand[2]='e';
	badCommand[3]='x';
	badCommand[4]='i';
	badCommand[5]='s';
	badCommand[6]='t';
	badCommand[7]='i';
	badCommand[8]='n';
	badCommand[9]='g';
	badCommand[10]=' ';
	badCommand[11]='F';
	badCommand[12]='i';
	badCommand[13]='l';
	badCommand[14]='e';
	badCommand[15]='\n';
	badCommand[16]=0xd;
	badCommand[17]='\0';




	readSector (map,1);
	readSector(directory,2);

 
 	
 	for (i = 0; i < 512 ; i += 32 ) {
  	 	found = 1;
   		for(j = 0; j < 6 ; j++ ){
      	 if(name[j] != directory[j+i] ){
	 		found=0;
			 break;      
			}
    
   		}
   		if(found == 1)
		break;
	}

	
if(found == 0){
    printString(badCommand);
    return;
}

	directory[i]=0;
	while(count < 26){
  		if(directory[i+6+count] == 0)
			break;
		map[directory[i+count+6]]=0;
   		count++;
 		}
 	writeSector(map,1);
 	writeSector(directory,2);


}



void terminate(){
 	setKernelDataSegment();
	process_table[currentProcess].active = 0;
	while(1);
	
}

void executeProgram(char* name){

	char array [13312];
	int i=0;
	int segment = 0;
	setKernelDataSegment();
	for(i = 0;i <8; i++){
		if(process_table[i].active == 0){
			process_table[i].active = 1;
			segment = (i+2)*0x1000;
			break;
		}
	}
	restoreDataSegment();
	readFile(name,array);
	
	for(i=0; i<13312;i++){

		putInMemory (segment,i,array[i]);	
		
			
	}
	initializeProgram(segment);

}


void printString(char* chars){

	int i=0;

	while(chars[i]!='\0'){
	
	interrupt(0x10, 0xE*256+chars[i], 0, 0, 0);

	i++;
    }

}



void readFile(char* fileName, char* addressBuffer){
 int found = 0; 
 int i = 0;
 int j = 0;
 char buffer[512];
 int count=0;
 char badCommand [18];


badCommand[0]='I';
badCommand[1]='n';
badCommand[2]='e';
badCommand[3]='x';
badCommand[4]='i';
badCommand[5]='s';
badCommand[6]='t';
badCommand[7]='i';
badCommand[8]='n';
badCommand[9]='g';
badCommand[10]=' ';
badCommand[11]='F';
badCommand[12]='i';
badCommand[13]='l';
badCommand[14]='e';
badCommand[15]='\n';
badCommand[16]=0xd;
badCommand[17]='\0';

 readSector(buffer,2);
 for (i = 0; i < 512 ; i += 32 ) {
   found = 1;
   for(j = 0; j < 6 ; j++ ){
       if(fileName[j] != buffer[j+i] ){
	 found=0;
	 break;      
	}
    
   }
   if(found == 1)
	break;
}


if(found == 0){
    printString(badCommand);
    return;
}
while(count < 26){
  if(buffer[i+6+count] == 0)
	break;
  readSector(addressBuffer+512*count , buffer[i+count+6]);
  count++;
 }
}
void readString(char* chars){

	int i=0;
	int c=0;
	//chars[i++]='\n';
	while(1){
				
		
		c=interrupt(0x16, 0, 0, 0, 0);
		if (c == 13)
       		 interrupt(0x10,0xe*0x100+10,0x0,0x0,0x0);

				if(c==0x8){
				if(i!=0){
					interrupt(0x10, 0xE*256+0x8, 0, 0, 0);
					interrupt(0x10, 0xE*256+' ', 0, 0, 0);
					interrupt(0x10, 0xE*256+0x8, 0, 0, 0);
					i--;
				}
				
			}
			else{
				chars[i++]= c;
				interrupt(0x10, 0xE*256+chars[i-1], 0, 0, 0);
			}
			
	if(c == 0xd){
			
			chars[i++]= 0xa;
			chars[i]=0x0;
			break;
		}
	
	
	}




		}





int div(int dividend, int divisor){ 

	int res = 0;
	while(dividend >= divisor){
		res++;
		dividend = dividend - divisor;
	}
	return res;
}



int mod(int n,int d)
{

	int q = div(n,d);

	return n-d*q;
}	

void readSector(char* buffer, int sector){// interrupt 
	int track = div(sector,36);
	int cl = mod(sector,18) + 1;
	int dh = div(sector,18);
	dh= mod(dh,2);
	interrupt(0x13, 2*256+1, buffer, track*256+cl, dh*256);
}


void writeSector(char* buffer, int sector){

	int track= div(sector, 36);
	int cl = mod (sector, 18) +1;
	int dh = div(sector, 18);
	dh= mod(dh, 2);
	interrupt(0x13, 3*256+1, buffer, track*256+cl, dh*256);
}




void handleInterrupt21(int ax, int bx, int cx, int dx)
{ 

	switch (ax){

	case 0: printString(bx);  	  break;
	case 1: readString(bx);  	  break;
	case 2: readSector(bx,cx);    break;
	case 3: readFile(bx,cx);	  break;
	case 4: executeProgram(bx);   break;
	case 5: terminate();		  break;
	case 6: writeSector(bx,cx);   break;
	case 7: deleteFile(bx);		  break;
	case 8: writeFile(bx,cx,dx);  break;
	case 9: killProcess(bx);      break;
	default: printString("ERROR INTRUDER ALERT INTRUDER ALERT");

	}
	
	
}
