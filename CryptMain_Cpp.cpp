//==================================================================
//    Edit by zq @2016-12-19
//    All right reserved
//==================================================================
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <string>  //enable c++ class string
#include "sys/stat.h"
#include "MemAllot_Cpp.h"
#include "AES_Cpp.h"
using namespace std;
using namespace OP_ZQs_tools;

//========================================================
//    MainFunction: 
//        Crypt & DeCrypt File
//    return:
//        0 success
//        1 command syntax wrong
//        2 password length not same
//        3 password not same
//==========================================================
int main(int argc, char *argv[])
{
    unsigned char PW1[1024]={0};//支持最长1024Bytes密码
    unsigned char PW2[1024]={0};//验证密码
    int PWLen1=0;
    int PWLen2=0;
    char *pOutF;
    CAES o_AES;
    if(argc==2) {
        if(!strcmp(argv[1],"-v")){
            printf("crypt 2.1s\nsupport 3AES256-CBC \nwrited by zq\ncopyright 2015.\n");
            return 0; 
        }
        else {
            printf("Error: Wrong Syntax.\n./Wright Syntax: crypt [ -e filename | -ed directory | -d filename | -dd directory | -v ]\n");
            return 1;//Error 1: 命令行语法错误
        }
    }
    else if(argc==3){
        if(!strcmp(argv[1],"-e")){
            printf("Please input password for encrypt:");
            system("stty -echo");
            while( (PW1[PWLen1++]=getchar())!='\n'){
                printf("*");
                if(PWLen1==1024){
                    break;
                }
            }
            system("stty echo");
            printf("\nPlease input password for encrypt again:");
            system("stty -echo");
            while( (PW2[PWLen2++]=getchar())!='\n'){
                printf("*");
                if(PWLen2==1024){
                    break;
                }
            }
            system("stty echo");
            printf("\n");
            if(PWLen1!=PWLen2){
                printf("Error: password length not same.\n");
                return 2;
            }
            PWLen1--;//\n is also counted.
            PWLen2--;
            do{
                PWLen1--;
                if(PW1[PWLen1]!=PW2[PWLen1]){
                    printf("Error: password not same.\n");
                    return 3; 
                }
            }while(PWLen1>0);

            o_AES.SetMem(8,4,14);
            o_AES.SetKey((char *)PW1,PWLen2,768);//because PWLen1 is change, so here use PWLen2.
            o_AES.NAESCBCFilE(argv[2]);
//-------------test line---------------------
//printf("\nThis is a test line: PWLen1 is %d.\n",PWLen1);
//printf("\nThis is a test line: PWLen2 is %d.\n",PWLen2);
//for(int xx=0;xx<PWLen2;xx++){
//printf("\nThis is a test line: %d.\n",PW1[xx]);
//}
//---------------------------------------
        }
        else if(!strcmp(argv[1],"-d")){	
            printf("Please input password for decrypt:");
            system("stty -echo");
            while( (PW1[PWLen1++]=getchar())!='\n'){
                printf("*");
                if(PWLen1==1024){
                    break;
                }
            }
            system("stty echo");
            printf("\n");
            PWLen1--;
            o_AES.SetMem(8,4,14);
            o_AES.SetKey((char *)PW1,PWLen1,768);
            o_AES.NAESCBCFilD(argv[2]);
//-------------test line---------------------
//printf("\nThis is a test line: PWLen1 is %d.\n",PWLen1);
//printf("\nThis is a test line: PWLen2 is %d.\n",PWLen2);
//for(int xx=0;xx<PWLen1;xx++){
//printf("\nThis is a test line: %d.\n",PW1[xx]);
//}
//---------------------------------------
        }
        else if(!strcmp(argv[1],"-ed")){
            printf("Please input password for encrypt:");
            system("stty -echo");
            while( (PW1[PWLen1++]=getchar())!='\n'){
                printf("*");
                if(PWLen1==1024){
                    break;
                }
            }
            system("stty echo");
            printf("\nPlease input password for encrypt again:");
            system("stty -echo");
            while( (PW2[PWLen2++]=getchar())!='\n'){
                printf("*");
                if(PWLen2==1024){
                    break;
                }
            }
            system("stty echo");
            printf("\n");
            if(PWLen1!=PWLen2){
                printf("Error: password length not same.\n");
                return 2;
            }
            PWLen1--;//\n is also counted.
            PWLen2--;
            do{
                PWLen1--;
                if(PW1[PWLen1]!=PW2[PWLen1]){
                    printf("Error: password not same.\n");
                    return 3; 
                }
            }while(PWLen1>0);
            DIR *d;
            struct dirent *dirfile;
            struct stat sb;
            char *tmpStr;
            tmpStr=(char *)malloc(512*sizeof(char));
            if(!(d = opendir(argv[2]))){
                printf("Error: Opendir %s failed.\n",argv[2]);
                return 0;
            }		
            while((dirfile = readdir(d)) != NULL){			
                if(strncmp(dirfile->d_name, ".", 1) == 0 || strncmp(dirfile->d_name, "..", 1) == 0 ) continue;//dont process file ., which indicate local
                strncpy(tmpStr,argv[2],512);
                tmpStr[512]='\0';
                strncat(tmpStr,dirfile->d_name,256);	
                if( stat(tmpStr,&sb)==0 && (S_ISREG(sb.st_mode)) ){
                    o_AES.SetMem(8,4,14);
                    o_AES.SetKey((char *)PW1,PWLen2,768);//because PWLen1 is change, so here use PWLen2.
                    o_AES.NAESCBCFilE(tmpStr);
                }
            }
            free(tmpStr);
            closedir(d);
        }
        else if(!strcmp(argv[1],"-dd")){	
            printf("Please input password for decrypt:");
            system("stty -echo");
            while( (PW1[PWLen1++]=getchar())!='\n'){
                printf("*");
                if(PWLen1==1024){
                    break;
                }
            }
            system("stty echo");
            printf("\n");
            PWLen1--;
            DIR *d;
            struct dirent *dirfile;
            struct stat sb;
            char *tmpStr;
            tmpStr=(char *)malloc(512*sizeof(char));
            if(!(d = opendir(argv[2]))){
                printf("Error: Opendir %s failed.\n",argv[2]);
                return 0;
            }	
            while((dirfile = readdir(d)) != NULL){
                if(strncmp(dirfile->d_name, ".", 1) == 0) continue;
                strncpy(tmpStr,argv[2],512);
                tmpStr[512]='\0';
                strncat(tmpStr,dirfile->d_name,256);	
                if( stat(tmpStr,&sb)==0 && (S_ISREG(sb.st_mode)) ){
                    o_AES.SetMem(8,4,14);
                    o_AES.SetKey((char *)PW1,PWLen1,768);
                    o_AES.NAESCBCFilD(tmpStr);
                }
            }
            free(tmpStr);
            closedir(d);
        }
        else{
            printf("Error: Wrong Syntax.\n./crypt_b [ -e filename | -ed directory | -d filename | -dd directory | -v ]\n");
            return 1;
        }
    }
    else {
        printf("Error: Wrong Syntax.\n./crypt_b [ -e filename | -ed directory | -d filename | -dd directory | -v ]\n");
        return 1;
    }
}

