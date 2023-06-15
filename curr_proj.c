#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>

/*In this initial population, the 10's place represnts the room number and the 1's place represents the subject.*/

#define MAXFUN 1000000  //upper bound for number of function evaluations
                        // in case best fitness does'nt reach the desired value
#define MAXP 5       //population size (best if chosen between 100-150)
#define KIDS 1          //pool size of kids to be formed (use 2,3 or 4)
#define LIMIT 0
#define MINIMIZE 1      //set 1 to minimize and -1 to maximize
#define M 1             //M+2 is the number of parents participating in xover (use 1)
#define family 2        //number of parents to be replaced by good individuals(use 1 or 2)

int random_number_creator(int upper, int lower) //used to generate random numbers between lower and upper
{
    int n;
    n = rand() % (upper-lower)+ lower;
    return n;
}

int best,random,p1[5][9],p2[5][9],d1,d2,h1,h2,l;
int gen=(MAXFUN/KIDS);
int rp[family];


struct pop{            //population or one timetable consists of subjects, labs, rooms for the subjects, and days. Labarartory rooms have indices 7&8 respectively.
    int subjects[6];
    int labs[2];
    int rooms[3];
    int days[5][9];
    int fitness;     //this variable will calculate the fitness of a particular timetable, that is how good the timetable is.
}oldpop[MAXP], newpop[KIDS+family], bench; //since oldpop is 5, the number of random timetables generated will be 5.

void intialize_bench()// this will initialise the benchmark structure declared previously.
{
	int i,j;
 	for(i=0;i<5;i++)
    {
		for(j=0;j<9;j++)
         if(j!=5)
            bench.days[i][j]=0; //5 days all the spaces are left free for filling up.
         else
            bench.days[i][j]=11111;//this is the lunchbreak hence it is not free.
    }
    for(i=0;i<6;i++)
    {   
       if(i<4)
           bench.subjects[i]=3; // the first 3 subject require to be taught 3 time a week
       else 
           bench.subjects[i]=2;// the last 2 subjects need to be taught 2 times a week, it can be a elective.
    }
    for(i=0;i<2;i++)
    {
    	bench.labs[i]=3;// each lab is taught for 3 hours.
    }
    bench.rooms[0]=5;// room 0 can be only used 5 times a week
    bench.rooms[1]=7;// room 1 can be only use 7 times a week
    bench.rooms[2]=4;// room 2 can be only used 4 times a week
}
int subjects_to_allocate() // it counts the number of hours of subjects the entire timetable has to allocate.
{
	int i,cou=0;
    for(i=0;i<6;i++)
    { 
        cou=cou+ bench.subjects[i];
    }
	return cou;
}

int countappearance(int x[][9], int d, int n)
{
	int i,count=0;
	for(i=0;i<9;i++)
	{
		if(x[d][i]%10==n)
			count++;
	}
	return count;
}

int fitnesscalc(int x[5][9])
{
	int k,i,j,fitness=0,s,fac[6][5][9],cou;

		if(x[4][6]==77||x[4][6]==88)
			fitness+=10;
			
		for(i=0;i<5;i++)
		{
			for(j=0;j<6;j++)
			{
				cou=countappearance(x,i,j+1);
				if(cou>1)
				{
					fitness+=30;
				}
		 	}	
		}		
			
		FILE *myFile;
    	myFile = fopen("Faculty.txt", "r");

	    //read file into array
    	int numberArray[6][5][9];

	    if (myFile == NULL)
		{
    	    printf("Error Reading File\n");
        	exit (0);
    	}

	    for (i=0; i<6; i++)	
		{
    		for(j=0;j<5;j++)
    			for(k=0;k<9;k++)
	       			fscanf(myFile, "%d,", &numberArray[i][j][k] );
    	}

	    for (i=0; i<6; i++)
		{
    		for(j=0;j<5;j++)
    			for(k=0;k<9;k++)
       				 fac[i][j][k]=numberArray[i][j][k];
    	}

	    fclose(myFile);
	    
		for(i=0;i<5;i++)
		{
			for(j=0;j<9;j++)
			{
				int a;
				a=x[i][j]%10;
				if(a!=0)
				{
					if(fac[a-1][i][j]==1)
						fitness+=5;
				}
			}

		}
		return fitness;
			
}

void initpop() // function to intitalise the population/ timetables
{
    int i,j,s=1,d,h,stop=1,cou,coul=0,l,z,rl,r,objbest;
    for(z=0;z<MAXP;z++)
    {
    	oldpop[z].fitness=0;
   		intialize_bench();
		for(i=0;i<2;i++)
		coul+=bench.labs[i];//number of lab hours are calculated
		rl=random_number_creator(9,7);
		l=rl-7;//this is the room of the lab.
		do{                          //labs are allocated
			d=random_number_creator(5,0);
			h=6;//labs can only be done in the afternoon session
			if(bench.days[d][h]==0)
			{
				for(i=0;i<3;i++)
				{
					bench.days[d][h]=1111;
					bench.labs[l]--;
					oldpop[z].days[d][h]=(rl*10)+l+7;
					h++;
					coul--;
				}
				if(l==0)
				{
					l=1;
				}
				else
				{
					l=0;
				}
				if(rl==8)
				{
					rl=7;
				}
				else
				{
					rl=8;
				}
			}
			
		}while(coul!=0);
	    cou=subjects_to_allocate();
        do//subjects are allocated
        {   
    	    s=random_number_creator(6,0);//random subject is chosen
     	    do
       	    { 
         	   d=random_number_creator(5,0);//random day selected
 			   h=random_number_creator(9,0);//random hour selected
          	   if(bench.days[d][h]==0&&bench.subjects[s]!=0)
               {   
       			 int w=1;
	   	         do{
           		 	r=random_number_creator(3,0);
 		           	if(bench.rooms[r]==0)
 		           	{
       		 			continue;
          		 	}
          		 	else
          		 		break;
 		           }while(w=1);
                 oldpop[z].days[d][h]=(r+1)*10+s+1;// room number and subject number allocated
                 bench.days[d][h]=11111;
                 bench.rooms[r]--;
                 stop=0;
                 cou--;
                 bench.subjects[s]--;
               }
               else  
               {
                 break; 
               }
        	}while(stop!=0);
   		 }while(cou!=0);
   	 }
   	 for(i=0;i<MAXP;i++)
		oldpop[i].fitness=fitnesscalc(oldpop[i].days); //fitness is calculated of the initated population.
    objbest = oldpop[0].fitness;
 	best = 0;
 	for(i=1;i<MAXP;i++)
    {
      if (MINIMIZE * objbest > MINIMIZE * oldpop[i].fitness)
   	 {
		  objbest = oldpop[i].fitness;
	 	 best = i;
     }
    } 
}


int selectparent2()
{
	int r;
	r=random_number_creator(MAXP+1,0);
	return r;
	
}
int labelement()
{
	int i,j,t,s;
	for(i=0;i<5;i++)
	{
		for(j=0;j<9;j++)
		{
			if(p1[i][j]==77||p1[i][j]==88)
			{
				if(p1[i][j]==77)
				{
					d1=i;
					l=77;
					for(t=6;t<9;t++)
						p1[i][t]=0;
					for(s=0;s<5;s++)
						for(t=0;t<9;t++)
							if(p2[s][t]==77)
							{
								d2=s;
								break;
							}	
					for(t=6;t<9;t++)
						p2[d2][t]=0;
					return 1;
				}
				else
				{
					d1=i;
					l=88;
					for(t=6;t<9;t++)
						p1[i][t]=0;
					for(t=6;t<9;t++)
						p1[i][t]=0;
					for(s=0;s<5;s++)
						for(t=0;t<9;t++)
							if(p2[s][t]==88)
								d2=s;
					for(t=6;t<9;t++)
						p2[d2][t]=0;
					return 1;
					
				}	
			}
		
		}
	}
	return 0;
}

int subelement()
{
	int i,j,m1,m2,s,t,flag=0;
	for(i=0;i<5;i++)
	{
		for(j=0;j<9;j++)
		{
			if(p1[i][j]!=0)
			{
				d1=i;
				h1=j;
				l=p1[i][j];
				p1[i][j]=0;
				m1=l%10;
				flag=1;
				break;
				
			}
		}
		if(flag==1)
			break;
	}
	for(i=0;i<5;i++)
	{
		for(j=0;j<9;j++)
		{
			m2=p2[i][j]%10;
			if(m2==m1)
			{
				d2=i;
				h2=j;
				p2[i][j]=0;
				
				return 1;
			}
		}
	}
	return 0;

}

void generatenew()
{
	int count,i,j,k,r=selectparent2();
	random=r;
	for(i=0;i<5;i++)
	{
		for(j=0;j<9;j++)
		{
			p1[i][j]=oldpop[best].days[i][j];
		}
	}
	for(i=0;i<5;i++)
	{
		for(j=0;j<9;j++)
		{
			p2[i][j]=oldpop[r].days[i][j];
		}
	}	
	count=labelement();
	do
	{
		r=random_number_creator(101,1);
		if(newpop[0].days[d1][6]==0&&newpop[0].days[d2][6]==0)
		{
			if(r<70)
			{	
				for(k=6;k<9;k++)
					newpop[0].days[d1][k]=l;
				
			}	
			else
			{	
				for(k=6;k<9;k++)
					newpop[0].days[d2][k]=l;
			}
			
		}
		else if(newpop[0].days[d1][6]==0)
		{
			for(k=6;k<9;k++)
				newpop[0].days[d1][k]=l;
		}
		else
		{
			for(k=6;k<9;k++)
				newpop[0].days[d2][k]=l;
		}
		count=labelement();
	}while(count!=0);
	count=subelement();
	do{
		
		r=random_number_creator(101,1);
		if(newpop[0].days[d1][h1]==0&&newpop[0].days[d2][h2]==0)
		{
			if(r<70)
			{	
				newpop[0].days[d1][h1]=l;
			}
			else
			{
				newpop[0].days[d2][h2]=l;
			}
		}
		else if(newpop[0].days[d1][h1]==0)
		{
			newpop[0].days[d1][h1]=l;
		}
		else 
		{
			newpop[0].days[d2][h2]=l;
		}
		count=subelement();
	}while(count!=0);
	newpop[0].fitness=fitnesscalc(newpop[0].days);
	
}

void find_parents()
{
	int r,i,j,k;
	for(i=KIDS;i<(KIDS+family);i++)
	{
		r=random_number_creator(MAXP+1,0);
		rp[i-1]=r;
		for(j=0;j<5;j++)
			for(k=0;k<9;k++)
				newpop[i].days[j][k]=oldpop[r].days[j][k];	
		newpop[i].fitness=oldpop[r].fitness;			
	}	
}

void sort()
{
	int	i,j,a;
 	for (i=0; i > KIDS+family; ++i) 
	{
	  for (j=i + 1; j > KIDS+family; ++j) 
	  {
		if (newpop[i].fitness < newpop[j].fitness) 
		{
            a = newpop[i].fitness;
            newpop[i].fitness = newpop[j].fitness;
            newpop[j].fitness = a;
        }
      }
    }
}

void rep_parents()
{
	int i,j,k;
	for(i=0;i<family;i++)
	{
		for(j=0;j<5;j++)
			for(k=0;k<9;k++)
				oldpop[rp[i]].days[j][k]=newpop[i].days[j][k];
		oldpop[rp[i]].fitness=newpop[i].fitness;
		
	}	
}

main()  //main function
{
		int i,j,z,count,tempfit,objbest;
		initpop();//population is initated
   for(z=0;z<MAXP;z++)
    {
    	printf("\nFitness= %d\n",oldpop[z].fitness);
        for(i=0;i<5;i++)
   	    {
    	
    	if(i==0)
        printf("\nMon");
        if(i==1)
        printf("\nTue");
        if(i==2)
        printf("\nWed");
        if(i==3)
        printf("\nThu");
        if(i==4)
        printf("\nFri");
        for(j=0;j<9;j++)
            printf("\t%3d",oldpop[z].days[i][j]);
        }
        printf("\n");
    }

 //	tempfit=oldpop[best].fitness;
//    for(count=1;count<=2;count++)
  //  {
    	
		generatenew();
		printf("\nNewly Generated Timestable Using Crossover with Fitness= %d\n",newpop[0].fitness);
        for(i=0;i<5;i++)
   	    {
    	
    	if(i==0)
        printf("\nMon");
        if(i==1)
        printf("\nTue");
        if(i==2)
        printf("\nWed");
        if(i==3)
        printf("\nThu");
        if(i==4)
        printf("\nFri");
        for(j=0;j<9;j++)
            printf("\t%3d",newpop[0].days[i][j]);
   	    }
   	    printf("\n");   	    

		
		find_parents();  // form a pool from which a solution is to be 
	  
 		sort();          // sort the kids+parents by fitness
		
		rep_parents();   // a chosen parent is replaced by the child

/*		printf("\nenter\n");
		best=0;
		tempfit=oldpop[0].fitness;
		for(i=1;i<MAXP;i++)
	    	if(MINIMIZE * oldpop[i].fitness < MINIMIZE * tempfit)
     		{
     			tempfit=oldpop[i].fitness;
				best=i;
   			}   
		if ((count%100)==0||tempfit==LIMIT)
	    	printf("\nCurrent Count%d\nCurrent Best Fitness Value:\n",(count),tempfit);
    }
    printf("Best solution obtained after %d function evaluations: \n",count);*/
    printf("\nNew Population with Crossover Timetable entering Population \n");
    for(z=0;z<MAXP;z++)
    {
    	printf("\nFitness= %d",oldpop[z].fitness);
        for(i=0;i<5;i++)
   	    {
    	
    	if(i==0)
        printf("\nMon");
        if(i==1)
        printf("\nTue");
        if(i==2)
        printf("\nWed");
        if(i==3)
        printf("\nThu");
        if(i==4)
        printf("\nFri");
        for(j=0;j<9;j++)
            printf("\t%3d",oldpop[z].days[i][j]);

        }
        printf("\n");
    }
}