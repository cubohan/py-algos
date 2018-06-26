#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#define _XOPEN_SOURCE
#include <unistd.h>

static int len;

char *genpass(char password1[], int i){
    const char *alphabets  = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    password1[i] = '\0';
    for(int j=0; j<52; j++){
        password1[i-1] = alphabets[j];

    }
    if(i>0){
        return &password1[i-1];
    }
    else{
        return password1;
    }
}

char *checkhash(char password[],char hash[],char salt[], int i){
    int jj = 0;
    char *hashnew;
    for(jj=0; jj<i; jj++){
        password[jj] = *genpass(&password[jj], i);
        hashnew = crypt(password, salt);
        if(strcmp(hashnew , hash) == 0){
            break;
        }
    }
     return password;
}

int main(int argc, char *argv[]){
    while(argc != 2){
        printf("ERROR");
        return 1;
    }
    char salt[3], password[6], hash[strlen(argv[1])];
    for(i=0; i<2; i++){
       salt[i] = argv[1][i];
       
    }
    salt [2] = '\0';
    password[5] = '\0';
    printf("\n%s\n", salt);
    for (int i=1; i<6; i++){
        checkhash(password, argv[1], salt, i);
    }
    printf("%s\n",password);
    return 0;
}
