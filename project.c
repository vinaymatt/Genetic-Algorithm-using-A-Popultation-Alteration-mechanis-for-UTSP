#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<ctype.h>
#include <time.h>


#define MAXP 10        //population size (best if chosen between 100-150)

struct pop{
	int days[5][9];
	int faculty;
	int students;
	int subjects[6];
	int labs[2];
	int rooms[9][5][9];
	int labaratory[9][2];
	int obj;
} old_pop[MAXP],benchmark;

int random_number_creator(int upper, int lower)
{
	int n;
    n = rand() % (upper-lower)+ lower;
    return n;
}
void initialize_days(struct pop b)
{
    int i,j;
	for(i=0;i<5;i++)
	{
		for(j=0;j<9;j++)
		 if(j!=5)
		  b.days[i][j]=0;
	     else
	      b.days[i][j]=11111;
	}
}
void initialize_benchmark(struct pop b)
{
	int i,j,k;
	for(i=0;i<5;i++)
	{
		for(j=0;j<9;j++)
		 if(j!=5)
		  b.days[i][j]=0;
	     else
	      b.days[i][j]=11111;
	}
	b.faculty=9;
	b.students=100;
	for(i=0;i<6;i++)
	{   
	   if(i<4)
 		b.subjects[i]=3;
	   else 
 		b.subjects[i]=2;
	}
	for(i=0;i<2;i++)
	{
		b.labs[i]=3;
	}
	for(i=0;i<9;i++)
	{   
         for(j=0;j<5;j++)
         	for(k=0;k<9;k++)
         		if(i!=5)
		      		 b.rooms[i][j][k]=0;
       
	}
	for(i=6;i<9;i++)
	{   
	  for(j=0;j<8;j++)
		    b.labaratory[i][j]=0;
    }
}
int check_avail_days(struct pop p, int b)
{   
    int j,count=0;
	for(j=0;j<9;j++)
    {  
       if(p.days[b][j]==0)
   	     count+=1;
   			
   	}
	return (count);
}
int check_avail_hours(struct pop b,int d)
{
	int i,count=0;
	for(i=0;i<9;i++)
	{
		if(b.days[d][i]==0)
		  count+=1;
	}
	return count;
	
}

int hours_alloter(struct pop b,int d)
{
	int i,j=0;
	int arr[9]={0,0,0,0,0,0,0,0,0};
	for(i=0;i<9;i++)
	{
		if(b.days[d][i]==0)
		{   
			arr[j]=i;
            j++;
		}
		
	}
	int n;
	n=random_number_creator(j,0);
	return arr[n];
}
int availble_room(struct pop b,int s, int r, int q)
{   
    if(b.rooms[s][r][q]==0)
	 return 1;
	else
	 return 0;
}
void initpop()
{
  int i,j,count=0, a=0;
    
  for(i=0;i<MAXP;i++)
    old_pop[i].obj=0;
  
  for(i=0;i<MAXP;i++)
    {
	 initialize_benchmark(benchmark);
	 initialize_days(old_pop[i]);
	 do{
	 	printf("\nloop 1 entery\n");
        
        int subject1,ss,fa=benchmark.faculty,h1;
        for(subject1=random_number_creator(6,0); ;benchmark.subjects[subject1]--)
        {
           printf("\nloop 2 entery\n");
		   int cou=1,r;
		   ss=subject1*10;
		   while(cou!=0&&benchmark.subjects[subject1]!=0&&fa!=0)
		   {  
		      printf("\nloop 3 entery\n");
			  int d=random_number_creator(6,0),hi;
		      cou=check_avail_hours(benchmark,d);
   			  hi=hours_alloter(benchmark,d);
   			  old_pop[i].days[d][h1]=ss;
   			  do
   			    {   
				    printf("\nloop 4 entery\n");
					r=random_number_creator(9,0);
					if(availble_room(benchmark,r,d,hi)==1)
    			   	{
					   benchmark.rooms[r][d][hi]=1;
    			   	   old_pop[i].days[d][hi]+=r;
   	 	      		   benchmark.faculty-=1;
  			           fa++;
    			   	   a++;
    			   	}
				    else
					    continue;
   			    }while(a==0);
	        }
            old_pop[i].faculty=fa;	
        }
 
	    for(i=0;i<6;i++)
	    {
    		count+=benchmark.subjects[i];
    	}
    }while(count!=0);
  }
}

int main()
{
	initpop();
    int i,j,k;
 	for(i=0;i<MAXP;i++)
    {   
       printf("\nPop %d\n",i);
       for(j=0;j<5;j++)
       {   printf("\n");
           for(k=0;k<9;k++)
 	       printf("   %d",old_pop[i].days[j][k]);
       }
 	 }
   return 0;
 }
 


 
