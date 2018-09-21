/*
  * ls-file.c
  *
  * Tenzin Dolma Gyalpo & Eliza-Teodora Tuta
  *
  * usage: ./ls-file root-directory
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
  char* current_dir=".";
  char* name;
  opterr=0;
  int use_a =0;
  int use_l=0;
  int opt,x=0,y=0;
  int diropt[256];
  int info_opt[256];

  void
  mode_to_letter(int mode, char* str);

  void
  information(void);

  int
  main(int argc, char*argv[])
  {

    for(int i =0; i < argc; i++){
      if(strncmp(argv[i],"-",1)==0)
      {
        info_opt[x] = i;
        x++;
        printf("the info %s \n", argv[i]);
      }
      else
      {
        diropt[y] = i;
        y++;
        printf("flag is %s \n", argv[i]);
      }
    }
    int dir_size = sizeof(diropt) / sizeof(diropt[0]);
    int info_size = sizeof(info_opt) / sizeof(info_opt[0]);

    printf( "size of dir %ld \n", dir_size);
    printf("size of info %ld \n", info_size);

    for(int k=0; k < 3; k++)
    {
      printf("the name of the directory: %d", diropt[k]);
      for(int l = 0; l <  3; l++)
      {
        printf("       the information: %d", info_opt[l]);
      }
    }

    //if(flag==1){
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
            /*  if(dir_flag==1){
              root=opendir(name);
            }
              else{
              root = opendir(current_dir);}*/

              root = opendir(current_dir);


              if(root==NULL)
              {
                perror("opendir");
                exit(1);
              }

              /* reads the files in the directory*/
              else
              {
                information();
              }



              /* close directory */
              closedir(root);
            }



            void
            mode_to_letter(int mode, char *str)
            {
              str[0] = '-';

              // judge file type
              if(S_ISDIR(mode)) str[0] = 'd';
              if(S_ISCHR(mode)) str[0] = 'c';
              if(S_ISBLK(mode)) str[0] = 'b';

              // judge permission for owner
              if(mode & S_IRUSR) str[1] = 'r';
              else str[1] = '-';
              if(mode & S_IWUSR) str[2] = 'w';
              else str[2] = '-';
              if(mode & S_IXUSR) str[3] = 'x';
              else str[3] = '-';

              // judge permission for owner group
              if(mode & S_IRGRP) str[4] = 'r';
              else str[4] = '-';
              if(mode & S_IWGRP) str[5] = 'w';
              else str[5] = '-';
              if(mode & S_IXGRP) str[6] = 'x';
              else str[6] = '-';

              // judge permission for others
              if(mode & S_IROTH) str[7] = 'r';
              else str[7] = '-';
              if(mode & S_IWOTH) str[8] = 'w';
              else str[8] = '-';
              if(mode & S_IXOTH) str[9] = 'x';
              else str[9] = '-';

              str[10] = '\0';
            }


            void
            information(void) {

              while((pDirent = readdir(root))!=NULL)
              {
                strncpy(file_name, pDirent->d_name, 256);
                if(file_name[0]==46)
                {
                  if(use_a==1)
                  {
                    printf ("%s \n", pDirent->d_name);
                  }
                }
                else
                {
                  if(use_l==1)
                  {
                    if(stat(pDirent->d_name, &statbuf) == -1)
                    continue;

                    //printf("%10.10d", sperm(statbuf.st_mode));

                    /* print permissions */
                    mode_to_letter(statbuf.st_mode,str);
                    //file type and permissions
                    printf("%s ",str);

                    printf("%ld ", statbuf.st_nlink);

                    /* print out owner's name */
                    if((pwd = getpwuid(statbuf.st_uid))!= NULL)
                    printf("%s ", pwd->pw_name);
                      else
                      printf("%d ", statbuf.st_uid);

                      /*Print out group name*/
                      if((grp = getgrgid(statbuf.st_gid))!=NULL)
                      printf("%s ", grp->gr_name);
                       else
                       printf("%d ", statbuf.st_gid);

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
