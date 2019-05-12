#include "bookHeaders.h"

static void sortBubble(int *arrID, int loop)
{
  int tmp;
  for(int c = 1; c < loop; c++)
    for (int i = 0; i < loop - c; i++)
      {
	if (arrID[i] > arrID[i+1])
	  {
	    tmp = arrID[i];
	    arrID[i] = arrID[i+1];
	    arrID[i+1] = tmp;
	  }
      }
  return;
}
/*  
    extern FILE *delRecords(FILE *fp, int *arrID, int loop, char *name)
    {
    rename(name, "tempIndex");
    FILE *newfp = fopen(name, "w+");
    char tmpArr[recSize +1];
    sortBubble(arrID, loop);
    int advLoop = 1;
    
    int j = 0;
    fseek(fp, 0, SEEK_SET);
    for(int i = 0; i < loop; i++)
    {
    for (j = advLoop; (j < arrID[i]) && (j < tailID - loop); j++)
    {
    fread((void*)tmpArr, 1, recSize, fp);
    fwrite(tmpArr, 1, recSize, newfp);
	    
    }
    fseek(fp, recSize, SEEK_CUR);
    advLoop = ++j;
    }
    for (j = advLoop; (j <= tailID -1) ; j++)
    {
    fread((void*) tmpArr, 1, recSize, fp);
    fwrite(tmpArr, 1, recSize, newfp);
    }

    fread((void*) tmpArr, 1, recSize, fp);
    fwrite(tmpArr, 1, recSize, newfp);

    closeBooks();
    fclose(fp);
    fclose(newfp);
    remove("tempIndex");
    FILE *retFile = fopen(name, "r+");
    initializeBooks(retFile);
    return retFile;  
    }
*/

//an ugly way to handle deletion of records. This function takes an array of integers with ID's tagged for deletion. NOTE: arrID should always have the size of arrID[loop] due to internal assignments.
extern FILE *delRecords(FILE *fp, int *arrID, int loop, char *name)
{
  rename(name, "tempIndex");
  FILE *newfp = fopen(name, "w+");
  char tmpArr[recSize +1];
  fseek(fp, 0, SEEK_SET);
  int countID = 1;
  int arrCount = 0;
  sortBubble(arrID, loop);
  arrID[loop] = -1;
  if (arrID[loop-1] != tailID)
    {
      while(countID != tailID)
	{
	  if(countID == arrID[arrCount])
	    {
	      arrCount++;
	      fseek(fp, recSize, SEEK_CUR);
	      countID++;
	      continue;
	    }
	  fread((void*) tmpArr, 1, recSize, fp);
	  fwrite(tmpArr, 1, recSize, newfp);
	  countID++;
	}
      fread((void*) tmpArr, 1, recSize -1, fp);
      fwrite(tmpArr, 1, recSize -1, newfp);
    }
  else
    {
      int freeslot = 0;
      if (loop == 1)
	freeslot = arrID[0] -1;
      for (int i = loop - 1; i > 0; i--)
	{
	  if (arrID[i] - arrID[i-1] > 1)
	    {
	      freeslot = arrID[i] -1;
	      break;
	    }
	  if(i == 1)
	    freeslot = arrID[0] -1;
	   }
      
	if (!freeslot)
	{
	  closeBooks();
	  fclose(fp);
	  fclose(newfp);
	  remove("tempIndex");
	  FILE *retFile = fopen(name, "r+");
	  initializeBooks(retFile);
	  return retFile;
	}

      while(countID != (freeslot))
	{
	  if(countID == arrID[arrCount])
	    {
	      arrCount++;
	      fseek(fp, recSize, SEEK_CUR);
	      countID++;
	      continue;
	    }
	  fread((void*) tmpArr, 1, recSize, fp);
	  fwrite(tmpArr, 1, recSize, newfp);
	  countID++;
	}
      fread((void*) tmpArr, 1, recSize -1, fp);
      fwrite(tmpArr, 1, recSize -1, newfp);
    }
  
    closeBooks();
    fclose(fp);
    fclose(newfp);
    remove("tempIndex");
    FILE *retFile = fopen(name, "r+");
    initializeBooks(retFile);
    return retFile;
  
}
