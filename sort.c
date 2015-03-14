#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LINESIZE 1024
//These are constants used through out the program.
int num_flag=0;
int fold_flag=0;
int reverse =0;
int num_sort=0;
int neg_flag=0;
int line_count;
char *rest;
char *word;
struct line *line_array;
int space_count;

//Out line object, has a char* word which is the array of chars
//the num, which is only set if the input has a numeric value
//and an int spacey, which is a count for how much leading whitespace
//is read in and eliminated for that line - in order to preserve the format.
struct line{
	char* word;
	long num;
	char* rest;
	
};

//This prints out a helpful instructional statement and exits with non-zero value.
void print_help(){	
	printf("%s\n","That is not a valid flag, please run the program again and enter either no flags, -f, -n, -r or any cumulative combination of the former.");	
	exit(1);
}

//This is a comparison fuction that takes in two pointers and 
//returns strcmp to compare and order strings
int cmp(const void* a, const void* b){
    const struct line* l1=a;
    const struct line* l2=b;
    return strcmp(l1->word,l2->word);
}

//This comparison function takes in two pointers
//and returns strcasecmp to compare strings via case insensitivity.
int cmp1(const void* a, const void* b){
      const struct line* l1=a;
      const struct line* l2=b;
	return strcasecmp(l1->word,l2->word);
 }

//This comparison function takes in pointers
//and returns a value based on numeric comparisons.
//However, if the two numbers ordered are equal
//this function then orders either by strcmp if
//the fold flag is NOT set, or strcasecmp if the
//fold flag is set.
int cmp2(const void* a, const void* b){
	const struct line* l1=a;
	const struct line* l2=b;
	if((l1->num - l2->num)==0){
		if(fold_flag!=0){
			return strcasecmp(l1->rest, l2->rest);
		}
		else{
			return strcmp(l1->rest, l2->rest);
		}
	}
	else{
		return (l1->num - l2->num);
	}	
}
//This function is an implementation of atoi/strtol.
//It takes in a pointer to the start of the input
//Skips over leading whitespace
//Attempts to convert the chars into longs
//and returns the number as a long
//As soon as it encounters a char that is not a digit
//It assigns rest to point to the rest of that string input.
long mystrtol(char *start, char **rest){
	int count=0;
	int num=0;
	*rest=start;
	neg_flag=0;
	while(isspace(start[count])){
		count++;
	}

	if(!isdigit(start[count])&& start[count]!='-'){
		*rest=start;
		return 0;
	}
	if(start[count]=='-'){
		neg_flag=1;
		count++;
	}
	if(!isdigit(start[count])){
		*rest=start;
		return 0;
	}
	if(isdigit(start[count])){
		num=start[count]-'0';
		count++;
	}
	while(isdigit(start[count])){	    
		num=num*10;
		num=num+(start[count]-'0');
		count++;
	}
	*rest=start+count;
	if(neg_flag==1){
		num=num*-1;
	}

	
	return (long)num;
}



//This function calls the appropraite comparison/sorting functions
//based on the flags that are specified by user input.
void format(){
	long numcheck=0;
	
	if(num_sort!=0){
	    for(int i=0; i<line_count; i++){
		numcheck=mystrtol(line_array[i].word, &rest);
		line_array[i].rest=rest;
		line_array[i].num=numcheck;
		}
	    qsort(line_array, line_count, sizeof(struct line), cmp2);
	}
	else if(fold_flag!=0){
		qsort(line_array, line_count, sizeof(struct line), cmp1);

	}
	else{

		qsort(line_array, line_count, sizeof(struct line), cmp);
	}

}


//This function reads in input from the file specified line by line.
//When it reads in, it eliminates whitespace and keeps track of
//how much whitespace per line read in.
//It returns that line it read in.
char* getLine(){
	int ch=0;
	char* ret_val;
	char* word;
	word=malloc(LINESIZE*sizeof(char));
	ret_val=word;
	//reading in the first word ch by ch as long as its not a space or newline
	while((ch=getchar())!=EOF && ch!='\n'){
		*word=ch;
		word++;

	}
	
	//if it a new line, unget it
	*word='\0';
	return ret_val;	
}





//This function continuously calls getLine until it 
//has reached the end of file.
//It stores the returned line of input and assigns it to
//a structs word field.
//It then stores each struct into a line_array.
void getInput(){	
	line_count=0; //how many words we read in 
	char* line_read;	//declare struct
	struct line LINE;
	int ch;
	line_array = (struct line*)malloc(LINESIZE*LINESIZE*sizeof(struct line));
	while((ch=getchar())!=EOF){
		ungetc(ch, stdin);
		line_read=getLine();
		LINE.word=line_read;
		line_array[line_count]=LINE;
		line_count++;
	}
	
	format();

	if(reverse!=0){
		for(int j=line_count-1; j>=0; j--){
			printf("%s\n",line_array[j].word);
		}
	}

	else{
		for(int k=0; k<line_count; k++){
			printf("%s\n", line_array[k].word);
		}
	}
	for(int c=0; c<line_count; c++){
		free(line_array[c].word);
	}
	free(line_array);

}

//This main function gets user argumenst, sets flags, and calls getInput.
//It also frees up space at the end. 
int main(int argc, const char* argv[]){
	line_count=0;
	for(int k=1; k<argc; k++){
		if(argv[k][0] == '-'){
			if( argv[k][1] == 'f'){
				fold_flag=1;
			}
			else if(argv[k][1]=='n'){
				num_sort=1;
			}
			else if(argv[k][1] == 'r'){
				reverse=1;
			}
			else if(argv[k][1]=='h' || argv[k][1]=='?'){
				print_help();
			}
			else{
				print_help();
			}
		}
		else if((argv[k][0] !='-') && (isalnum(argv[k][0]))){
			print_help();
		}
	}
	getInput();
	return 0;
}

