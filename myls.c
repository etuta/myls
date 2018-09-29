/*
* myls.c
*
* Tenzin Dolma Gyalpo & Eliza-Teodora Tuta
*
* usage: ./myls root-directory
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


struct dirent *pDirent;
DIR *root;
struct stat statbuf;
struct passwd *pwd;
struct group *grp;
struct tm *tm;
char datestring[256];
char str[12];
char file_name[256];
char* dir_name[256];
char* current_dir=".";
char* name;
struct stat buffer;

int current = 0;
int status;
int use_a =0;
int use_l=0;
int opt,x=0,y=0;
int diropt[256];
int info_opt[256];
int j;


/*print permission*/
void
mode_to_letter(mode_t mode, char* str);

/*print permission and other info*/
void
options(void);

/*the main function*/
int
main(int argc, char*argv[])
{

  opterr=0;


  while((opt=getopt(argc, argv, "al"))!=-1)
  {
    switch(opt)
    {
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

  printf("this is the optind %d\n\n\n", optind);

if(argv[optind]==NULL){
  //printf("what is in %d = NULL YES\n", optind );
  current =1;
  root = opendir(current_dir);
  if(root==NULL)
  {
    perror("opendir");
    exit(1);
  }

  else
  {
    options();
  }

}


  for(int i = optind; i<argc; i++){
    printf("this is optind %d\n", optind);
    printf("what is in %d = %s\n", optind,argv[optind]);
current_dir = argv[optind];
   root = opendir(current_dir);
    if(root==NULL)
    {
      perror("opendir");
      exit(1);
    }
    else{
      options();
    }
    optind++;
  }

  /* close directory */
  closedir(root);
}

void
mode_to_letter(mode_t mode, char *str)
{
  //printf("is the permission being printed\n");
  str[0] = '-';
  // judge file type
  printf("directory %x \n",S_ISDIR(mode) );
  if(S_ISDIR(mode)){ str[0] = 'd';}
  printf("c %x \n",S_ISCHR(mode));
  if(S_ISCHR(mode)){ str[0] = 'c';}
  printf("b %x \n",S_ISBLK(mode) );
  if(S_ISBLK(mode)){ str[0] = 'b';}

  // judge permission for owner
  printf("mode= %d S_IRUSR = %d \n", mode, S_IRUSR);
  printf("mode & sth = %d \n", mode & S_IRUSR );
  if((mode & S_IRUSR)== S_IRUSR) { str[1] = 'r';}
  else { str[1] = '-';}

  printf("mode= %d S_IWUSR = %d \n", mode, S_IWUSR);
  printf("mode & sth = %d \n", mode & S_IWUSR);
  if((mode & S_IWUSR) == S_IWUSR){ str[2] = 'w';}
  else { str[2] = '-';}

  printf("mode= %d S_IXUSR = %d \n", mode, S_IXUSR);
  printf("mode & sth = %d \n", mode & S_IXUSR);
  if((mode & S_IXUSR) ==S_IXUSR ){ str[3] = 'x';}
  else { str[3] = '-';}

  printf("mode= %d S_IRGRP = %d \n", mode, S_IRGRP);
  printf("mode & sth = %d \n", mode & S_IRGRP);
  // judge permission for owner group
  if((mode & S_IRGRP) ==S_IRGRP){ str[4] = 'r';}
  else{ str[4] = '-';}

  printf("mode= %d S_IWGRP = %d \n", mode, S_IWGRP);
  printf("mode & sth = %d \n", mode &S_IWGRP);
  if((mode & S_IWGRP) == S_IWGRP){ str[5] = 'w';}
  else{ str[5] = '-';}

  printf("mode= %d S_IXGRP = %d \n", mode, S_IXGRP);
  printf("mode & sth = %d \n", mode & S_IXGRP);
  if((mode & S_IXGRP) ==S_IXGRP){ str[6] = 'x';}
  else{ str[6] = '-';}

  printf("mode= %d S_IROTH = %d \n", mode, S_IROTH);
  printf("mode & sth = %d \n", mode & S_IROTH);
  // judge permission for others
  if((mode & S_IROTH) == S_IROTH){ str[7] = 'r';}
  else{ str[7] = '-';}

  printf("mode= %d S_IWOTH= %d \n", mode, S_IWOTH);
  printf("mode & sth = %d \n", mode & S_IWOTH);
  if((mode & S_IWOTH) ==S_IWOTH){ str[8] = 'w';}
  else{ str[8] = '-';}

  printf("mode= %d S_IXOTH = %d \n", mode, S_IXOTH);
  printf("mode & sth = %d \n", mode & S_IXOTH);
  if((mode & S_IXOTH) == S_IXOTH ){ str[9] = 'x';}
  else{ str[9] = '-';}

  str[10] = '\0';
}

void
options(void)
{
  while((pDirent = readdir(root))!=NULL)
  {
    strncpy(file_name, pDirent->d_name, 256);
    if((file_name[0]==46))
    {
      //printf("\nthe file has a flag \n");
      if(use_a==1 && use_l!=1)
      {
        //printf("\nOnly -a no -l\n");
        printf ("%s \n", pDirent->d_name);
      }
      else if(use_l==1 && use_a==1)
      {
        //status = stat("/home", &buffer);
        //printf("the status of stat = %d \n", status);
      //  printf("\n-l and -a\n");
    // printf("\nsomething 2 = %d \n",stat(pDirent->d_name, &statbuf) );
    if(current == 1){
     if(stat(pDirent->d_name, &statbuf) == -1){
    //   perror("stat");
     continue;
   }
 }
 else {
   if(stat(current_dir, &statbuf) == -1){
  //   perror("stat");
   continue;
 }
 }
        /* print permissions */
        printf("the stat.mode = %d\n", statbuf.st_mode);

        mode_to_letter(statbuf.st_mode,str);
      //  printf("%10.10s", sperm (statbuf.st_mode));
          //    printf("%4d", statbuf.st_nlink);

        //file type and permissions
        printf("%s ",str);
        printf("%ld ", statbuf.st_nlink);

        /* print out owner's name */
        if((pwd = getpwuid(statbuf.st_uid))!= NULL)
        {
          printf("%s ", pwd->pw_name);
        }
        else
        {
          printf("%d ", statbuf.st_uid);
        }
        /*Print out group name*/
        if((grp = getgrgid(statbuf.st_gid))!=NULL)
        {
          printf("%s ", grp->gr_name);
        }
        else
        {
          printf("%d ", statbuf.st_gid);
        }
        /*Print out fileq size*/
        printf("%6d ", (int)statbuf.st_size);
        /*Print the local time*/
        tm = localtime(&statbuf.st_mtime);
        strftime(datestring, sizeof(datestring),"%b %d %H:%M", tm);
        printf(" %s %s\n", datestring, pDirent->d_name);
      }
    }

    else
    {
      if(use_l==1)
      {
        //printf("\nonly -l \n");
      //  printf("the file name %s \n", pDirent->d_name);
       //printf("\nsomething %d \n",stat(pDirent->d_name, &statbuf) );
       //printf("\n this is different %d \n", stat(current_dir, &statbuf));
       printf("the name of the file = %s \n", pDirent->d_name);


      if(current == 1){
       if(stat(pDirent->d_name, &statbuf) == -1){
         printf("okay something is wrong with 1");
      //   perror("stat");
       continue;
     }
   }
   else {
     if(stat(current_dir, &statbuf) == -1){
        printf("okay something is wrong with 2");
    //   perror("stat");
     continue;
   }
   }
        //printf("something is being printed here\n");
        /* print permissions */
          printf("the stat.mode = %d\n", statbuf.st_mode);

          //mode_to_letter(statbuf.st_mode,str);
        mode_to_letter(statbuf.st_mode,str);
      //  printf("%10.10s", sperm (statbuf.st_mode));
        //      printf("%4d", statbuf.st_nlink);

        //file type and permissions
        printf("%s ",str);
        printf("%ld ", statbuf.st_nlink);
        /* print out owner's name */
        if((pwd = getpwuid(statbuf.st_uid))!= NULL)
        {
          printf("%s ", pwd->pw_name);
        }
        else
        {
          printf("%d ", statbuf.st_uid);
        }
        /*Print out group name*/
        if((grp = getgrgid(statbuf.st_gid))!=NULL)
        {
          printf("%s ", grp->gr_name);
        }
        else
        {
          printf("%d ", statbuf.st_gid);
        }
        /*Print out fileq size*/
        printf("%6d ", (int)statbuf.st_size);
        /*Print the local time*/
        tm = localtime(&statbuf.st_mtime);
        strftime(datestring, sizeof(datestring),"%b %d %H:%M", tm);
        printf(" %s %s\n", datestring, pDirent->d_name);
      }
      else
      {
        printf ("%s \n", pDirent->d_name);
      }
    }
  }

}
