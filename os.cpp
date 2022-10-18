#include <iostream>
#include <ctime>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <algorithm>
using namespace std;

pthread_mutex_t mutex;
void *dis(void *arg);
void *pro(void *arg);

int component[3]={}; // battery , aircraft , propeller
int com[3]={}; // battery , aircraft , propeller number
int total=0;
int num[3]={};

int main(int agrc,char* argv[])
{
pthread_mutex_init(&mutex,NULL);
srand(atoi(argv[2]));

if(atoi(argv[1])==0)
 {
 pthread_t th[4];
 if(pthread_create(&th[0],NULL,dis,NULL)!=0)
  cout<<"error"<<endl;
for(int i=1;i<4;i++)
  if(pthread_create(&th[i],NULL,pro,(void*)(intptr_t)i)!=0)
      cout<<"error"<<endl;
 for(int i=0;i<4;i++)
  pthread_join(th[i],NULL);
cout<<"battery : "<<com[0]<<endl;
cout<<"aircraft : "<<com[1]<<endl;
cout<<"proeller : "<<com[2]<<endl;
int a[3]={0,1,2};
 for(int i=0;i<3;i++)
 {
  for(int j=i+1;j<3;j++)
    {
     if(num[i]<num[j])
       swap(a[i],a[j]);
    }
  }
cout<<"producer"<<a[0]+1<<" : "<<num[a[0]]<<"drone(s)"<<endl;
cout<<"producer"<<a[1]+1<<" : "<<num[a[1]]<<"drone(s)"<<endl;
cout<<"producer"<<a[2]+1<<" : "<<num[a[2]]<<"drone(s)"<<endl;
  } 
  return 0;
}

void *dis(void *arg)
{
while(total<50)
{
int turn= -1;
while(component[turn]!=0)
 {
 turn=rand()%3;
 }
 if(total>=50)
 return 0;
component[turn]+=1;
com[turn]+=1;
if(turn==0)
  cout<<"dispatcher : battery"<<endl;
else if(turn==1)
   cout<<"dispatcher : aircraft"<<endl;
else if(turn==2)
   cout<<"dispatcher : propeller"<<endl; 
}
pthread_exit(0);
}

void *pro(void *arg)
{
pthread_mutex_lock(&mutex);
 int bat=0;
 int pro=0;
 int air=0;
 
 int t=(intptr_t)arg;
 string name= "null";
if(t==1)
 {
   air=50;
   name="producer 1 (aircraft) : ";
 }
 else if(t==2)
   name="producer 2 : ";
  else if(t==3)
    name="producer 3 : ";
if(total>=50)
 return 0;
while(total<50)
{
 if(bat==0&&component[0]>0)
  {
  bat+=1;
  component[0]-=1;
  cout<<name<<"get battery"<<endl;
  }
 else if(air==0&&component[1]>0)
 {
 air+=1;
 component[1]-=1;
 cout<<name<<"get aircraft"<<endl;
 }
 else if(pro==0&&component[2]>0)
 {
 pro+=1;
 component[2]-=1;
 cout<<name<<"get propeller"<<endl;
 }     
 if(bat>0&&air>0&&pro>0)
 {
 bat-=1;
 air-=1;
 pro-=1;
 num[t-1]+=1;
 total+=1;
 cout<<name<<"OK, "<<num[t-1]<<" drone(s)"<<endl;
 }
 pthread_mutex_unlock(&mutex);  
 }
 pthread_exit(0); 
}

