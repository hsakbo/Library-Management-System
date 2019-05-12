#include "bookHeaders.h"

struct books
{
  int id; //starts from 0 and ascends by +1 along the list
  fpos_t  cLocation;
  char type;
  int rackNum;
  struct books *link;
}*bData;




//helper function for initializeBooks(fptr), this function adds the next to the current line of the cursor. If there isn't any new line it will return EOF in retC which initializeBooks will have to handly accordingly
static void nextline(FILE *fptr, char *retC, fpos_t *idc, char *tmpRack)
{
  while ((*retC = fgetc(fptr)) != EOF)
    {
      if (*retC == shinlf)
	break;
    }
  
  if (*retC == EOF)
    {
      fseek(fptr ,1, SEEK_END);
      return;
    }
  
  fgetpos(fptr, idc);
  *retC = fgetc(fptr); 
  return;
}

//helper function to get the rack number of successive records
 
//for starting the structure	 
extern void initializeBooks(FILE *fptr)
{
  fpos_t pos;
  char c;
  char tmpRack[6];
  fseek(fptr, 0, SEEK_SET);
  if ((c = fgetc(fptr)) == EOF)
    {
      bData = malloc(sizeof(*bData));
      head = bData;
      tailID = 0;
      return;
    }
  
  bData = malloc(sizeof(*bData));
  fseek(fptr, 0, SEEK_SET);
  fgetpos(fptr, &pos);
  (*bData).cLocation = pos;
  bData->type = c;
  bData->link = NULL;
  tail = head;
  bData->id = 1;
  head = bData;

  /*assigning first rackNum 303*/
  fseek(fptr, 303, SEEK_SET);
  
  for(int i = 0; i < 5; i++)
    tmpRack[i] = fgetc(fptr);
  tmpRack[5] = '\0';
  bData->rackNum = atoi(tmpRack);
  tailID = 1;
    
  //this while loop handles all successive records other than the first one. This also calls the above helper function
  while(true)
    {
      nextline(fptr, &c, &pos, tmpRack);
      if (c == EOF)
	break;
      
      //structure progression
      bData->link = malloc(sizeof(*bData));
      bData = bData->link;
      //

      //assigning rackNum
      fseek(fptr, 302, SEEK_CUR); //nextline()'s fgetc
      for (int i = 0; i < 5; i++)
	tmpRack[i] = fgetc(fptr);
      tmpRack[5] = '\0';
      bData->rackNum = atoi(tmpRack);
      //
      
      bData->cLocation = pos;
      bData->type = c;
      bData->link = NULL;      
      bData->id = ++tailID;
  
    }
  tail = bData;
   
}

//adding a record by giving the necessary arguments
extern void addRecord(FILE *fl, char c, char *auth, char *rack, char *name, char *descript)
{
  fseek(fl, 0, SEEK_END);
  if(tail != NULL)
    fputc(shinlf, fl);
  fpos_t pos;
  fgetpos(fl, &pos);
  fputc(c, fl);
  fputc(delim, fl);
  fwrite(auth, sizeof(char), strlen(auth), fl);

  //adding buffer characters if less than 300
  
  for(int i = 0; i < (300 - strlen(auth)); i++)
    fputc(6, fl);
  
  fputc(delim, fl);
  fwrite(rack, sizeof(char), strlen(rack), fl);

  //adding buffer characters if less than 5
  
  for(int i = 0; i < (5 - strlen(rack)); i++)
    fputc(6, fl);
  
  fputc(delim, fl);
  fwrite(name, sizeof(char), strlen(name), fl);

  //adding buffer characters if name is less than 300
 
  for (int i = 0; i < (300 - strlen(name)); i++)
    fputc(6, fl);

  if (descript)
    {
      fputc(delim, fl);
      fwrite(descript, sizeof(char), strlen(descript), fl);
      for (int i = 0; i < (1000 - strlen(descript)); i++)
	fputc(6, fl);
    }
  else
    {
      fputc(delim, fl);
    for (int i = 0; i < 1000; i++)
      fputc(6, fl);
    }

  int tmp;
  if (tail==NULL)
    {
      tmp = 0;
      bData = head;      
    }
  else
    {
      bData = tail;
      tmp = tail->id;  
      bData->link = malloc(sizeof(*bData));
      bData = bData->link;
    }
  bData->id = ++tmp;
  bData->cLocation = pos;
  bData->type = c;
  bData->link = NULL;
  tail = bData;
  bData->rackNum = atoi(rack);
  tailID++;
}

//helper function that returns the bStruct pointer to the corresponding id
extern b_struct *retIDbook(int theID)
{
  bData = head;
  while (true)
    {
      if(bData->id == theID)
	return bData;	
      bData = bData->link;
      if (bData == NULL)
	return NULL;	
    }
}

//edits record for category in the given id
extern void editCategory(int theID, FILE *fp, char c)
{
  bData = retIDbook(theID);
  bData->type = c;
  fsetpos(fp, &((*bData).cLocation));
  fputc(c, fp);
}

//edits name in the record of given id
extern void editName(int theID, FILE *fp, char *arr)
{
  bData = retIDbook(theID);  
  fsetpos(fp, &(bData->cLocation));
  fseek(fp, 2, SEEK_CUR);
  for (int i = 0; i < strlen(arr); i++)
    fputc(arr[i], fp);
  for (int i = 0; i < (300 - strlen(arr)); i++)
    fputc(6, fp);
}

//edits the rack no of a given record
extern void editRack(int theID, FILE *fp, char *arr)
{
  bData = retIDbook(theID);
  bData->rackNum = atoi(arr);  
  fsetpos(fp, &(bData->cLocation));
  fseek(fp, 303, SEEK_CUR);
  for (int i = 0; i < strlen(arr); i++)
    fputc(arr[i], fp);
  for (int i = 0; i < (5 - strlen(arr)); i++)
    fputc(6, fp);
}

//edits the title record of a given book
extern void editTitle(int theID, FILE *fp, char *arr)
{
  bData = retIDbook(theID);
  fsetpos(fp, &(bData->cLocation));
  fseek(fp, 309, SEEK_CUR);
  
  for (int i = 0; i < strlen(arr); i++)
    fputc(arr[i], fp);
  for (int i = 0; i < (300 - strlen(arr)); i++)
    fputc(6, fp);
 
}

//edits the description of a record
extern void editDes(int theID, FILE *fp, char *arr)
{
  
  bData = retIDbook(theID);
  fsetpos(fp, &(bData->cLocation));
  fseek(fp, 610, SEEK_CUR);

  if(arr)
    {
  for (int i = 0; i < strlen(arr); i++)
    fputc(arr[i], fp);
  for (int i = 0; i < (1000 - strlen(arr)); i++)
    fputc(6, fp);
    }
  else
    {
  for (int i = 0; i < 1000; i++)
    fputc(6, fp);
    }
}



//gives id of the index filtered by the book type
extern int list_id_category(char c, int *arr)
{
  int count = 0;
  if (head == NULL)
    {
      return 0;
    }
  int returnVal = 0;
  bData = head;
  do
    {
      if (c == bData->type)
	{
	  arr[count++] = bData->id;
	  returnVal++;
	}
      bData = bData->link;
    }while(bData != NULL);
  return returnVal;
}

//gives id in the index filtered by the rackNum
extern int list_id_rack(int rck, int *arr)
{
  int count = 0;
  if (head == NULL)
    {
      return 0;
    }
  int returnVal = 0;
  bData = head;
  do
    {
      if (rck == bData->rackNum)
	{
	  arr[count++] = bData->id;
	  returnVal++;
	}
      bData = bData->link;
    }while(bData != NULL);
  return returnVal;
}

//returns a record string tokenized by \n 
extern void returnRecord(FILE *fp, int theID, char *retArr)
{ 
  char tok[2] = {21, 0};
  char lf[2] = {shinlf, 0};
  char tmpArr[recSize + 1];
  char *segment;
  bData = retIDbook(theID);
  fpos_t pos = bData->cLocation;
  
  fsetpos(fp, &pos);
  fread((void*) tmpArr, sizeof(char), recSize, fp);
  //(char*) tmpArr;
  tmpArr[recSize] = '\0';
  //printf("%s\n", tmpArr);
  strtok(tmpArr,tok);
  strcpy(retArr, tmpArr);
  strcat(retArr, lf);

  

  int tmpCount;
  int counter = strlen(retArr);
  char c;
  for (int i = 0; i < 4; i++)
    {
      tmpCount = 0;
      segment = strtok(NULL, tok);
      c = segment[tmpCount];
      
      while((c != 6) && (c != 0) && (c != shinlf))
	{
	  retArr[counter++] = c;
	  c = segment[++tmpCount];
	}
      retArr[counter++] = shinlf;
    }
  retArr[counter -1] = '\0';
  return;
  
}

//frees the structure
extern void closeBooks()
{
  if(tailID == 0)     
    return;
    
  bData = head;
  tail = bData->link;
  free(bData);
  while(tail != NULL)
    {
      bData = tail;
      tail = tail->link;
      free(bData);
    }
}

//loop through the database to find records with matching author name with strstr
extern int searchName(FILE *fp, int *arrID, char *str)
{
  if (strlen(str) == 0)
    return 0;
  char *segment;
  char tmpArr[recSize];
  int retVal = 0;
  char lf[2] = {shinlf, 0};
  for(int i = 1; i <= tailID; i++)
    {
      returnRecord(fp, i, tmpArr);
      strtok(tmpArr, lf);
      segment = strtok(NULL, lf);
      if (strstr(segment, str) != NULL)
	{
	  arrID[retVal] = i;
	  retVal++;
	}
    }
  return retVal;
}

//loop through the database to find records with matching title with strstr
extern int searchTitle(FILE *fp, int *arrID, char *str)
{
  if (str == NULL)
    return 0;
  else if (strlen(str) == 0)
    return 0;
  
  char *segment;
  char tmpArr[recSize];
  int retVal = 0;
  char slf[2] = {shinlf, 0};
  for(int i = 1; i <= tailID; i++)
    {
      returnRecord(fp, i, tmpArr);
      strtok(tmpArr, slf);
      strtok(NULL, slf);
      strtok(NULL, slf);
      segment = strtok(NULL, slf);
      if (strstr(segment, str) != NULL)
	  arrID[retVal++] = i;	
    }
  return retVal;
}      


//loop through the database to find records with matching description with strstr
extern int searchDes(FILE *fp, int *arrID, char *str)
{
  if (str == NULL)
    return 0;
  else if (strlen(str) == 0)
    return 0;
  
  char *segment;
  char tmpArr[recSize];
  int retVal = 0;
  char slf[2] = {shinlf, 0};
  for(int i = 1; i <= tailID; i++)
    {
      returnRecord(fp, i, tmpArr);
      strtok(tmpArr, slf);
      strtok(NULL, slf);
      strtok(NULL, slf);
      strtok(NULL, slf);
      segment = strtok(NULL, slf);
      if (segment == NULL)
	continue;
      if (strstr(segment, str) != NULL)
	  arrID[retVal++] = i;	
    }
  return retVal;
} 

