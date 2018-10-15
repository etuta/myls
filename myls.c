/*
* myls.c
*
* Tenzin Dolma Gyalpo & Eliza-Teodora Tuta
*
* usage: ./myls options directory/file
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <pwd.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>

//#define CURRENT_DIR "."

/*print permission*/
void mode_to_letter(mode_t mode, char* str);
/*print permission and other info for files and directories*/
void options(DIR* root,char* CURRENT_DIR,int use_a, int use_l, int reg_file);
void file_features(char* FileName, DIR* root, int use_l);
void features(char* FileName, struct dirent *pDirent);

int
main(int argc, char*argv[]){
  char* CURRENT_DIR="."; /* The default current dir */
  int use_a =0; /* to check if 'a' was written or not */
  int use_l=0; /* to check if 'l' was written or not */
  int reg_file =0; /* to check if it is a file or not*/
  int opt; /* takes the input of getopt*/
  DIR* root;
  struct stat buf;
  opterr=0;

  while((opt=getopt(argc, argv, "al"))!=-1){
    switch(opt){
      case 'a':
      use_a = 1;
      break;
      case 'l':
      use_l=1;
      break;
      case '?':
      printf("Option -%c is not an accepted argument.\n", optopt);
      exit(3);
    }
  }

  if(argv[optind]==NULL){
    root = opendir(CURRENT_DIR);
    if(root==NULL){
      perror("opendir");
      exit(1);
    }
    else{
      options(root, CURRENT_DIR, use_a, use_l, reg_file);
    }
  }

  for(int i = optind; i<argc; i++){
    printf("%s:\n",argv[optind]);
    CURRENT_DIR = argv[optind];
    stat(CURRENT_DIR, &buf);
    if(S_ISDIR(buf.st_mode)){
      root = opendir(CURRENT_DIR);
      if(root==NULL){
        perror("opendir");
        exit(1);
      }
    }
    else{
      root = opendir(".");
      reg_file = 1;
    }
    options(root, CURRENT_DIR, use_a, use_l, reg_file);
    optind++;
  }

  /* close directory */
  closedir(root);
}

void
mode_to_letter(mode_t mode, char *str){
  str[0] = '-';
  // judge file type
  if(S_ISDIR(mode)){ str[0] = 'd';} /*directory*/
  if(S_ISCHR(mode)){ str[0] = 'c';}
  if(S_ISBLK(mode)){ str[0] = 'b';}
  // judge permission for owner
  if((mode & S_IRUSR)== S_IRUSR) { str[1] = 'r';}
  else { str[1] = '-';}
  if((mode & S_IWUSR) == S_IWUSR){ str[2] = 'w';}
  else { str[2] = '-';}
  if((mode & S_IXUSR) ==S_IXUSR ){ str[3] = 'x';}
  else { str[3] = '-';}
  // judge permission for owner group
  if((mode & S_IRGRP) ==S_IRGRP){ str[4] = 'r';}
  else{ str[4] = '-';}
  if((mode & S_IWGRP) == S_IWGRP){ str[5] = 'w';}
  else{ str[5] = '-';}
  if((mode & S_IXGRP) ==S_IXGRP){ str[6] = 'x';}
  else{ str[6] = '-';}
  // judge permission for others
  if((mode & S_IROTH) == S_IROTH){ str[7] = 'r';}
  else{ str[7] = '-';}
  if((mode & S_IWOTH) ==S_IWOTH){ str[8] = 'w';}
  else{ str[8] = '-';}
  if((mode & S_IXOTH) == S_IXOTH ){ str[9] = 'x';}
  else{ str[9] = '-';}
  str[10] = '\0';
}

void
options(DIR* root,char* CURRENT_DIR, int use_a,int use_l, int reg_file){
  char file_name[256]; /*check if the first character in the string is '-'*/
  struct dirent *pDirent; /*pointer to the struct*/
  char new_str[1000]; /*place in heap to store the path, to not to
                               change the CURRENT_DIR*/
  char* fileN; /*the absolute path to the directory*/
  char* fileD; /*use to change the absolute path*/

  /*if it is file than go to file()*/
  if(reg_file==1){
    file_features(CURRENT_DIR,root,use_l);
    reg_file=0; /*change is back so that it can read other files/directory*/
  }
  else{
    while((pDirent = readdir(root))!=NULL){
      strncpy(file_name, pDirent->d_name, 256);
      /*check the first character of the string - hidden file or not */
      if((file_name[0]==46)){
        /* only option 'a' was entered */
        if(use_a==1 && use_l!=1){
          printf ("%s \n", pDirent->d_name);
        }
        /*both option 'a' and 'l' was entered*/
        else if(use_l==1 && use_a==1){
          strcpy(new_str,CURRENT_DIR);
          fileD=strcat(new_str,"/");
          /*the absolute path for the directory*/
          fileN=strcat(fileD,pDirent->d_name);
          features(fileN,pDirent);
        }
      }
      else{
        /*not hidden files, only entered option 'l'*/
        if(use_l==1){
          strcpy(new_str,CURRENT_DIR);
          fileD=strcat(new_str,"/");
          fileN=strcat(fileD,pDirent->d_name);
          features(fileN,pDirent);
        }
        /*if no option was entered*/
        else{
          printf ("%s \n", pDirent->d_name);
        }
      }
    }
  }
}

void
file_features(char* FileName, DIR *root, int use_l){
  struct dirent *pDirent;
  pDirent = readdir(root); /*initialize the struct*/
  strcpy(pDirent->d_name,FileName);
  /*entered option 'l' */
  if(use_l==1){
    features(FileName, pDirent);
  }
  /* if no option was entered */
  else{
    printf ("%s \n", pDirent->d_name);
  }
}

void
features(char* fileN, struct dirent *pDirent){
  char str[12];
  struct passwd *pwd;
  struct group *grp;
  struct tm *tm;
  struct stat statbuf;
  char datestring[256];

  if(stat(fileN, &statbuf)==-1){
    perror("stat");
    exit(1);
  }
  /* print features */
  mode_to_letter(statbuf.st_mode,str);
  //file type and features
  printf("%s ",str);
  printf("%ld ", statbuf.st_nlink);
  /* print out owner's name */
  if((pwd = getpwuid(statbuf.st_uid))!= NULL){
    printf("%s ", pwd->pw_name);
  }
  else{
    printf("%d ", statbuf.st_uid);
  }
  /*Print out group name*/
  if((grp = getgrgid(statbuf.st_gid))!=NULL){
    printf("%s ", grp->gr_name);
  }
  else{
    printf("%d ", statbuf.st_gid);
  }
  /*Print out fileq size*/
  printf("%6d ", (int)statbuf.st_size);
  /*Print the local time*/
  tm = localtime(&statbuf.st_mtime);
  strftime(datestring, sizeof(datestring),"%b %d %H:%M", tm);
  printf(" %s %s\n", datestring, pDirent->d_name);

}
