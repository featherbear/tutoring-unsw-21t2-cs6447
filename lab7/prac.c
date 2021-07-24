/*
    Heap Double Free & Use After Free
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>



char *clones[10];
char *viewclones[10];
int num_clones = 0;

struct clone{
    char name[8];
    int (*hint)();
};


void hint(){
    printf("You dont need a hint, you just need to overwrite me with win!\n ;)\n");
}

int getInput(){
    int input = getchar();
    if(input != '\n'){
        while(fgetc(stdin)!='\n');
    }
    return input;
}

int checkClones(){
    printf("Teleporting Clones...\n");
    printf("Clone ID: ");
    int index = getInput() - '0';
    if(index > 9 || index < 0){
        printf("Invalid Clone ID! (0-9)\n");
        return -1;
    }    
    if(index > 9 || index < 0){
        printf("Invalid Clone ID! (0-9)\n");
        return -1;
    }
    if(clones[index]==NULL){
        printf("Clone doesnt exist!\n");
        return -1;
    }    
    return index;
}

void create(){
    printf("Preparing to clone body...\n");
    if(num_clones>=10){
        printf("You cannot create any more clones!\n");
        return;
    }    
    printf("Clone ID: ");
    int index = getInput() - '0';
    if(index > 9 || index < 0){
        printf("Invalid Clone ID! (0-9)\n");
        return;
    }
    if(clones[index]!=NULL){
        printf("Clone %d already created!\n",index);
        return;
    }
    struct clone *clone = malloc(sizeof(struct clone));
    if(clone==NULL) return;
    clones[index] = clone;
    viewclones[index] = clone;
    clone->hint = *hint;
    
    num_clones++;
    printf("Creating Clone ID: %d\nEnter Name (max length 8): ",index);
    fgets(clone->name,8,stdin);
    if(!strchr(clone->name, '\n')){
		while(fgetc(stdin)!='\n');
	}
}

void kill(){
    printf("Charging ray gun...\n");
    if(num_clones<=0){
        printf("You cannot kill any more clones!\n");
        return;
    }     
    int index = checkClones();
    if(index == -1) return;
    free(clones[index]);
    printf("You just vaporised Clone ID: %d\n",index);
}


void name(){
    printf("Heating the branding irons...\n");
    printf("Clone ID: ");
    int index = getInput() - '0';
    if(index > 9 || index < 0){
        printf("Invalid Clone ID! (0-9)\n");
        return;
    }    
    if(index > 9 || index < 0){
        printf("Invalid Clone ID! (0-9)\n");
        return;
    }
    if(viewclones[index]==NULL){
        printf("Clone doesnt exist!\n");
        return;
    }
    printf("Enter Name (max length 8): ");
    fgets(viewclones[index],8,stdin);
    if(!strchr(viewclones[index], '\n')){
		while(fgetc(stdin)!='\n');
	}
    printf("Successfully Renamed Clone ID: %d\n",index);
}

void view(){
    printf("Teleporting Clones...\n");
    printf("Clone ID: ");
    int index = getInput() - '0';
    if(index > 9 || index < 0){
        printf("Invalid Clone ID! (0-9)\n");
        return;
    }    
    if(index > 9 || index < 0){
        printf("Invalid Clone ID! (0-9)\n");
        return;
    }
    if(viewclones[index]==NULL){
        printf("Clone doesnt exist!\n");
        return;
    }
    printf("Clone ID: %d\nName: %s\n",index,viewclones[index]);
}


void win(){
    printf("You just successfully exploited a double free bug!\n");
    execve("/bin/sh",NULL,NULL);
}

void givehint(){
    if(num_clones == 0){
        printf("Maybe try making a clone first\n");
        return;
    }
    if(num_clones >= 10){
        printf("You should probably start again\n");
        return;
    }
    printf("Which clone will you sacrifice to the gods?\n");
    int index = checkClones();
    if(index == -1) return;
    struct clone *c=clones[index];
    c->hint();
    free(clones[index]);
    clones[index] = NULL; 
}


int main(){
    setbuf(stdout, NULL);
    printf("\nWelcome hackers!\n\nMission:\n\nTake control of the heap\nExecute the win function!\n\n");
    int run = 1;
    while(run){
        printf("------------------\n");
        printf("| [A] Make Clone |\n");
        printf("| [B] Kill Clone |\n");
        printf("| [C] Name Clone |\n");
        printf("| [D] View Clone |\n");
        printf("| [H] Give Hint  |\n");
        printf("| [Q] Exit       |\n");
        printf("------------------\n");
        printf("Choice: ");
        int input = getInput();        
        switch (input)
        {
            case 'a':            
            case 'A':
                create();
                break;
            case 'b': 
            case 'B':
                kill();
                break;
            case 'c': 
            case 'C':
                name();
                break;
            case 'd': 
            case 'D':
                view();
                break;
            case 'h': 
            case 'H':
                givehint();
                break;

            case 'q': 
            case 'Q':
                run = 0;
                break;
            default:
                printf("That command is unknown\n");
                break;
        }

    }

    printf("Goodbye!\n");
    return 1;
}