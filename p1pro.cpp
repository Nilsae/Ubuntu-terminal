
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
//#include<readline/readline.h>
//#include<readline/history.h>
#include<stdlib.h>
//#include <bits/stdc++.h>
//#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include<curses.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <dirent.h>
#include "myeditor.h"
//using namespace std;
FILE *source;
//source = fopen(write_it, "r");
int usercount=0;
int currentuser_num=0;
int login_validation=1;
time_t minutes_start;
time_t minutes_now;
FILE *userdata_fp;
struct userdata{
    char name[15];
    char username [15];
    char passwd[40];
    unsigned int passprcntg;
    unsigned int usernum;
    int access_level;
    int error_num;
    int time_limitation;
    int time_started;
}user[100];

void allow_use();
int takeInput(char*);
void createuser(FILE * );
void display(FILE *);
void cd();
void pwd();
void mkdir();
void ls();
void changepass(FILE *);
void su(struct userdata[]);
long int filesize(FILE *fp);
void read_users_from_fp(FILE *fp);
void save_changes(FILE *fp);
void accelerate_access(FILE *);
void do_op1(char *);
void do_op2(char *);
void rm();
void cat();
void login();
void cp();
void mv();
void exif(char *);
struct userdata check_pass_security(struct userdata);
int main(){
    //printf("%d",user[0].usernum);
    //printf("%ld\n",sizeof(struct userdata));
    login();
    while(login_validation==0){
        printf("Sorry, username and password did not match.Try again.\n");
        login();
    }


        minutes_start = time(NULL)/60;
        //printf("Welcome %s :)\n",user[currentuser_num].username);
        user[currentuser_num].time_started=minutes_start;
        chdir(user[currentuser_num].username);
        allow_use();


}

void allow_use(){
//printf("%d",currentuser_num);
    char cmd[1000];

    if(user[currentuser_num].access_level==1){
        printf("\n(admin)$%s>>>>>",user[currentuser_num].username);

    }
    else{
    printf("\n$%s>>>>>",user[currentuser_num].username);
}
    scanf("%s",cmd);
    while(strcmp(cmd,"exit")){
        //printf("time left:%d",user[currentuser_num].time_limitation-minutes_now+minutes_start);
        minutes_now = time(NULL)/60;
        if(user[currentuser_num].time_limitation<minutes_now-user[currentuser_num].time_started){
            printf("Time is up.\nLogin Please.");
            login_validation=0;
            while(!login_validation){
            login();
        }
        printf("\nYou can continue. :)");
        }
        bool has_op=false;
        int op_c=0;
        for(unsigned int i=0;i<strlen(cmd);i++){
        if(cmd[i]=='>'){
            has_op=true;
            op_c++;
            }
        }
    if(has_op){
        if(op_c==1){
            //printf("in do op 1\n");
            do_op1(cmd);
        }
        if(op_c==2){
            do_op2(cmd);
        }


    }
    if(strcmp(cmd,"create")==0){
        createuser(userdata_fp);
    }
    else if(strcmp(cmd,"display")==0){
        display(userdata_fp);
    }
    else if(strcmp(cmd,"passwd")==0){
        changepass(userdata_fp);
    }
    else if(strcmp(cmd,"cd")==0){
        if(user[currentuser_num].access_level==1)
        cd();
        else{
            printf("\n  You do not have the permission\n");
        }
    }
    else if(strcmp(cmd,"cat")==0){
        system("cat");
    }
    else if(strcmp(cmd,"cp")==0){
        cp();
    }
    else if(strcmp(cmd,"myeditor")==0){
        //maini();
    }
    else if(strcmp(cmd,"chgr")==0){
        accelerate_access(userdata_fp);
    }
    else if(strcmp(cmd,"pwd")==0){
        pwd();
    }
    else if(strcmp(cmd,"ls")==0){
        ls();
    }
    else if(strcmp(cmd,"su")==0){
        su(user);
    }
    else if(strstr(cmd,"exif")){
        exif(cmd);
    }
    else if(strcmp(cmd,"diff")==0||strcmp(cmd,"wc") == 0){
        char args[1000];
        fgets(args ,1000, stdin);
        char com[1000];
        sprintf(com, "%s %s", cmd, args);
        system(com);
    }
    else if(strcmp(cmd,"rm")==0){
        rm();
    }
    else if(strcmp(cmd,"mkdir")==0){
        char name[100];
        scanf("%s",name);
        char command[1000];
        sprintf(command, "mkdir %s", name);
        system(command);
    }
    else if(strcmp(cmd,"mv")==0){
        mv();
    }
    else {
        printf("Command not found.\n" );
        user[currentuser_num].error_num++;
    }
    //save_changes(userdata_fp);
    printf("\n$%s>>>>>",user[currentuser_num].username);
    scanf("%s",cmd);

}
save_changes(userdata_fp);
fclose(userdata_fp);
}
/*int takeInput(char* str)
{
	char* buf;
    printf("\n%s>>> ",user[currentuser_num].username);
	buf = readline("");
	if (strlen(buf) != 0) {
		add_history(buf);
		strcpy(str, buf);
		return 0;
	} else {
		return 1;
	}
}
*/
void createuser(FILE *fp ){

    struct userdata user_to_c ={"","","",0,0,0,0};
    user_to_c.usernum=usercount;
    puts("Enter name:\n");
    scanf("%s",user_to_c.name);
    scanf("%s",user_to_c.name);
    puts("Enter username:\n");
    scanf("%s",user_to_c.username);
    puts("Enter password:\n");
    scanf("%s",user_to_c.passwd);
    user_to_c=check_pass_security(user_to_c);
    fseek(fp,(usercount)*sizeof(struct userdata),SEEK_SET);
    fwrite(&user_to_c,sizeof(struct userdata),1,fp);
    usercount++;
    printf("Signed up successfully\n");
    chdir("..");
    mkdir(user_to_c.username, 0777) ;
    printf("\nDirectory created\n");
    chdir(user[currentuser_num].username);
    //save_changes(fp);
    return;

}

void display(FILE *fp){
    rewind(fp);
    for(int i=0;i<usercount;i++){
        struct userdata user ={"","","",0,0,0};
        fread(&user,sizeof (struct userdata),1,fp);
        printf("number:%d\nname:%s\nusername:%s\npassword:%s\npasspercentage:%u\naccess level: %d\n",user.usernum,user.name,user.username,user.passwd,user.passprcntg,user.access_level);
    }
}
void cd(){
    char s[100];
    char directory[20];
    scanf("%s",directory);
    chdir(directory);
    printf("%s", getcwd(s, 100));
}
void pwd(){
    char s[100];
    printf("%s\n", getcwd(s, 100));
}
void mkdir(){
    char directory[20];
    scanf("%s",directory);
    mkdir(directory, 0777) ;
        printf("\nDirectory created\n");

}
long int filesize(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    long int res = ftell(fp);
    return res;
}

void su(struct userdata user[]){
    int login_validation=0;
    int founds=0;
    char user_to_go_to[15];
    scanf("%s",user_to_go_to);
    for(int i=0;i<usercount;i++){
        //printf("TTTTT");
        if(strcmp(user_to_go_to,user[i].username)==0){
            founds=1;
            //printf("TTTTT\n");
            if(user[currentuser_num].access_level==0){
                char passwd[40];
                scanf("%s",passwd);
            if(strcmp(passwd,user[i].passwd)==0){
                login_validation=1;
                currentuser_num=user[i].usernum;
                break;
            }
        }
        else{
            login_validation=1;
            currentuser_num=user[i].usernum;
            break;
        }
    }

}
if(founds==0){
    printf("User with the given username not found.\n");
    return;
}
if(login_validation==0){
    printf("Sorry, username and password did not match.Try again.\n");
}
else{
    time_t minutes_start;
    minutes_start = time(NULL)/60;
    printf("Logged in as %s :)\n",user_to_go_to);
    user[currentuser_num].time_started=minutes_start;

    allow_use();

}
}
void changepass(FILE * fp){
    if(user[currentuser_num].access_level==1){
        char order [15];
        scanf("%s",order );
        if(strcmp(order,"-l")==0){
            //blah blah blah
            int utime,success=0;
            char usernamet[15];
            scanf("%d %s",&utime,usernamet);
            for(int i=0;i<usercount;i++){
                if(strcmp(usernamet,user[i].username)==0){
                    user[i].time_limitation=utime;
                    success=1;
                    printf("Use time limit of user %s changed to %d\n",user[i].username,user[i].time_limitation);
                    }
                }
            if(success==0){
                printf("User not found.\n");
            }
        }
        else{
            //printf(" sjdh");
            //char ch_pass_user[15];
            //scanf("%s",ch_pass_user );
            for(int i=0;i<usercount;i++){
                if(strcmp(order,user[i].username)==0){
                    //printf("\nuser found\n");
                    printf("Enter new password:\n");
                    //fgets(passwd,40,stdin);
                    char newpassword[40];
                    scanf("%s",newpassword);
                    strcpy(user[i].passwd,newpassword);
                    printf("\n%s password changed.\n",user[i].username);
                    }
                }
            }

    }
    else{
        printf("you can only change your own password.\nEnter new password.\n");
        char newpassword[40];
        scanf("%s",newpassword);
        strcpy(user[currentuser_num].passwd,newpassword);
        printf("\n%s password changed.\n",user[currentuser_num].username);
    }
    save_changes(fp);
    return;
}
void read_users_from_fp(FILE *fp){
    usercount=filesize(fp)/sizeof(struct userdata);
    printf("usercount:%d\n",usercount);
    rewind(fp);
    for(int i=0;i<usercount;i++){
        fread(&user[i],sizeof (struct userdata),1,fp);
    }
}
void save_changes(FILE *fp){
    usercount=filesize(fp)/sizeof(struct userdata);
    rewind(fp);
    for(int i=0;i<usercount;i++){
        fwrite(&user[i],sizeof (struct userdata),1,fp);
    }
}
void accelerate_access(FILE *fp){
    if(user[currentuser_num].access_level==0){
        printf("You are not admin.Prohibited.\n");
        return;
    }
    else{
        char usernamet[15];
        int found=0;
        scanf("%s",usernamet);
        for(int i=0;i<usercount;i++){
            if(strcmp(usernamet,user[i].username)==0){
                found=1;
                if(user[i].access_level==1){
                    printf("User is already admin.\n");
                    return;
                }
                if(user[i].passprcntg<=75){
                    printf("Password security of the user is not enough.\n");
                    return;
                }
                if(user[i].error_num>10){
                    printf("Number of user faults too high!\n");
                    return;
                }
                user[i].access_level=1;
                save_changes(fp);
                printf("%s is now admin.\n",user[i].username);
                return;

            }
        }
        if(found==0){
            printf("User not found.\n");
            return;
        }
    }
}
struct userdata check_pass_security(struct userdata user){
    user.passprcntg=0;
    for(int i=0;user.passwd[i]!='\0';i++){
        if(user.passprcntg==100)
        break;
        if(user.passwd[i]>=97&&user.passwd[i]<=122)
            user.passprcntg+=2;
        else if(user.passwd[i]>=65&&user.passwd[i]<=90)
            user.passprcntg+=4;
        else if(user.passwd[i]>=48&&user.passwd[i]<=57)
            user.passprcntg+=3;
        else if(user.passwd[i]>=33&&user.passwd[i]<=64)
            user.passprcntg+=8;
        else if(user.passwd[i]==' ')
            user.passprcntg+=1;
        }
        while(user.passprcntg<=33){
            printf("Password security is not enough.\nPlease reEnter password.\n");
            scanf("%s",user.passwd);

            user.passprcntg=0;
            for(int i=0;user.passwd[i]!='\0';i++){
                if(user.passprcntg==100)
                break;
                if(user.passwd[i]>=97&&user.passwd[i]<=122)
                    user.passprcntg+=2;
                else if(user.passwd[i]>=65&&user.passwd[i]<=90)
                    user.passprcntg+=4;
                else if(user.passwd[i]>=48&&user.passwd[i]<=57)
                    user.passprcntg+=3;
                else if(user.passwd[i]>=33&&user.passwd[i]<=64)
                    user.passprcntg+=8;
                else if(user.passwd[i]==' ')
                    user.passprcntg+=1;
                }
        }
        return user;
}
void do_op1(char cmd[] ){

        char *write_it = strtok(cmd, ">");
            char *write_in_it = strtok(NULL, ">");

         bool is_file=false;
         for(unsigned int i=0;i<strlen(write_it);i++){
             if(write_it[i]=='.'){
                 is_file=true;
             }
         }

               char ch;// source_file[20], target_file[20];
               FILE *target;
               target = fopen(write_in_it, "w");
               /*if (target == NULL) {
                  fclose(source);
                  printf("target failure\n");
                  exit(EXIT_FAILURE);
              }*/
              if(is_file){
                  // printf("|%s|\n",write_it);
                 FILE *source;
               source = fopen(write_it, "r");
               if (source == NULL) {
                  printf("source failure\n");
                  //exit(EXIT_FAILURE);
              }

               while ((ch = fgetc(source)) != EOF)
                  fputc(ch, target);
               printf("File copied successfully.\n");
               fclose(source);
               fclose(target);
               return ;
                }
                else{

                        fwrite(write_it,strlen(write_it),1,target);

                    printf("string copied successfully.\n");
                    //fclose(source);
                    fclose(target);
                    return ;

                }


    }
    void do_op2(char cmd[] ){
        //printf("00");

            char *write_it = strtok(cmd, ">");
                char *write_in_it = strtok(NULL, ">");

             bool is_file=false;
             for(unsigned int i=0;i<strlen(write_it);i++){
                 if(write_it[i]=='.'){
                     is_file=true;
                 }
             }

                   char ch;// source_file[20], target_file[20];
                   FILE *target;
                   target = fopen(write_in_it, "a");
                   if (target == NULL) {
                      //fclose(source);
                      printf("target failure\n");
                    //  exit(EXIT_FAILURE);
                  }
                 if(is_file){
                     // printf("|%s|\n",write_it);
                    //  FILE *source;
                   source = fopen(write_it, "r");
                   if (source == NULL) {
                      printf("source failure\n");
                      //exit(EXIT_FAILURE);
                  }

                   while ((ch = fgetc(source)) != EOF)
                      fputc(ch, target);
                   printf("File copied successfully.\n");
                   fclose(source);
                   fclose(target);
                   return ;
                    }
                    else{

                            fwrite(write_it,strlen(write_it),1,target);

                        printf("string copied successfully.\n");

                        fclose(target);
                        return ;

                    }


        }
void ls(){


        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                printf("%s\n", dir->d_name);
            }
            closedir(d);
        }
        return;


}
void cat(){

    char directory[20];
    scanf("%s",directory);
    chdir(directory);
    DIR *d;
    struct dirent *dir;
    d = opendir(".");

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);
        }
    }
    chdir("..");
    return;
}
void rm(){
    char input[20];
    scanf("%s",input);
    if(strcmp(input,"-r")==0){
        //blah
        char dir[20];
        scanf("%s",dir);
        rmdir(dir);
        printf("Directory deleted successfully\n");
    }
    else{
        if (remove(input) == 0)
      printf("File deleted successfully\n");
   else
      printf("Unable to delete the file\n");

   return ;
    }
}
void cp(){
    char dir[100];
    scanf("%[^\n]%*c",dir);
    char cp[]="cp ";
    char *res=strcat(cp,dir);
    //puts(res);
    system(res);
}
void mv(){
    char dir1[50];
    char dir2[50];
    scanf("%s",dir1 );
    scanf("%s",dir2 );
    char *dir=strcat(dir1," ");
    dir=strcat(dir,dir2);
    char cp[]="cp ";
    char *res=strcat(cp,dir);
    //puts(res);
    system(res);
    remove(dir1);
}
void exif(char *filenameexif){
    char filename[strlen(filenameexif)-4];
    unsigned int i;
    for(i=0;i<strlen(filenameexif)-4;i++){
        filename[i]=filenameexif[i];
    }
    filename[i]='\0';
    //printf("****\n%s\n****",filename);
    struct stat stats;
if (stat(filename, &stats) == 0)
    {
        struct tm dt;

        // File permissions
        printf("\nFile access: ");
        if (stats.st_mode & R_OK)
            printf("read ");
        if (stats.st_mode & W_OK)
            printf("write ");
        if (stats.st_mode & X_OK)
            printf("execute");

        // File size
        printf("\nFile size: %lu", stats.st_size);

        // Get file creation time in seconds and
        // convert seconds to date and time format
        dt = *(gmtime(&stats.st_ctime));
        printf("\nCreated on: %d-%d-%d %d:%d:%d", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,
                                                  dt.tm_hour, dt.tm_min, dt.tm_sec);

        // File modification time
        dt = *(gmtime(&stats.st_atime));
        printf("\nLast Modified on: %d-%d-%d %d:%d:%d\n", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,
                                                  dt.tm_hour, dt.tm_min, dt.tm_sec);

    }
    else
    {
        printf("Unable to get file properties.\n");
        printf("Please check whether '%s' file exists.\n", filename);
    }
    for(int j=0;j<=strlen(filename);j++){
        filename[j]=' ';
    }
    filename[0]='\0';
    for(int j=0;j<=strlen(filename)+4;j++){
        filenameexif[j]=' ';
    }
    filenameexif[0]='\0';

}
void login(){

        char  username[15], passwd[40];

        userdata_fp=fopen("userdata.bin","r+b");
        //fseek(userdata_fp, 0, SEEK_END);
        //long int res = ftell(userdata_fp);
        //printf("%ld\n",res );
        rewind(userdata_fp);
        read_users_from_fp(userdata_fp);
        user[0].access_level=1;
        user[0].time_limitation=300;
        //printf("1:%s\n2:%s\n",user[0].username,user[1].username);
        //while(!(login_validation)){
        printf("Username:\n");
        scanf("%s",username);

        //scanf("%[^\n]%*c",passwd);
        for(int i=0;i<usercount;i++){
            if(strcmp(username,user[i].username)==0){
                printf("\nuser found\n"a;
                printf("Password:\n");
                //fgets(passwd,40,stdin);
                scanf("%s",passwd);
                if(strcmp(passwd,user[i].passwd)==0&&strlen(passwd)==strlen(user[i].passwd)){
                    login_validation=1;
                    printf("Welcome %s :)\n",username);
                    currentuser_num=user[i].usernum;
        //            break;
        //        }
            }
        }
}
}
