
#include "bookHeaders.h"

//Limitations: username and password should not exceed 50  characters. Note that userdata location should also be less than 50 chars (uSize).
     

extern int checkUsername(char *username, FILE *fp)
{
  fseek(fp, 0, SEEK_SET);
  char tmpArr[uSize+1] = {0};
  char garbage[uSize+2] = {0};
  int count = 0;
  while((fscanf(fp, "%s %s\n", tmpArr, garbage)) != EOF)
    {
      count++;
      fgets(garbage, uSize+1, fp); 
      if(strcmp(tmpArr, username) == 0)
	{
	  return count;
	}
    }
  return 0;
}

extern FILE *delUser(char *username, FILE *fp, char *dbName)
{
  int id = checkUsername(username, fp);
  rename(dbName, "tempUser");
  FILE *newfile = fopen(dbName, "w+");
  char tmpArr[uSize+2] = {0};
  char garbage[uSize + 2] = {0};
  fseek(fp, 0, SEEK_SET);
  if(id > 1)
    {
      for(int i = 0; i < id -1; i++)
	{
	  fgets(tmpArr, uSize+1, fp);
	  fputs(tmpArr, newfile);
	  fgets(tmpArr, uSize+1, fp);
	  fputs(tmpArr, newfile);
	}
      fgets(garbage, uSize + 1, fp);
      fgets(garbage, uSize + 1, fp);
      tmpArr[0] = 0; 
      while(!feof(fp))
	{
	  fgets(tmpArr, uSize+1, fp);
	  fputs(tmpArr, newfile);
	  fgets(tmpArr, uSize+1, fp);
	  fputs(tmpArr, newfile);
	  tmpArr[0] = 0;
	}
    }

  fclose(fp);
  fclose(newfile);
  remove("tempUser");
  FILE *retFile = fopen(dbName, "r+");
  return retFile;
}

extern int userCount(FILE *fp)
{
  int count = 0;
  fseek(fp, 0, SEEK_SET);
  char garbage[uSize + 2] = {0};
  while(!feof(fp))
    {
      fgets(garbage, uSize, fp);
      count++;
    }
  count /= 2;
  return count;
}
    

extern void addusr(char *username, char *pass, FILE *fp, char *dbLocation)
{ 
  fseek(fp, 0, SEEK_END);
  char tmpArr[52];
  tmpArr[0] = '\0';
  strcpy(tmpArr, username);
  strcat(tmpArr, " ");
  strcat(tmpArr, pass);
  strcat(tmpArr, "\n");
  fwrite((void*) tmpArr, 1, strlen(tmpArr), fp);
  fputs(dbLocation, fp);
  fputc('\n', fp);
  FILE * ftmp = fopen(dbLocation, "w+");
  fclose(ftmp);
  return; 
}

extern int begin_auth(char *username, char *pass, FILE *fp, char *retArr)
{

  fseek(fp, 0, SEEK_SET);
  char tmpArr[strlen(username) + strlen(pass) + 1];
  tmpArr[0] = '\0';
  strcpy(tmpArr, username);
  strcat(tmpArr, " ");
  strcat(tmpArr, pass);
  strcat(tmpArr, "\n");
  char fileArr[uSize +2];
  char *check;
  do
    {
      check = fgets(fileArr, uSize+1, fp);
      if(strcmp(fileArr, tmpArr) == 0)
	{
	  fgets(fileArr, uSize+1, fp);
	  fileArr[strlen(fileArr) -1] = 0;
	  strcpy(retArr, fileArr);
	  return 0;
	}
    } while(check);

  retArr[0] = 0;
  return 1;  
}

