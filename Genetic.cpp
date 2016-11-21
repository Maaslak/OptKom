#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <algorithm>

using namespace std;

const int population_size=100;
int n,m; //n - liczba operacji, m - liczba maszyn
vector <int*> population;
int** machines,** optime;
const int T=3000, tsize=10, ksize=10;
int start,koniec;

void Read1(){
 scanf("%d %d", &n, &m);
 machines=new int*[n];
 optime=new int* [n];
 for(int i=0; i<n; i++){
  machines[i]=new int [m];
  optime[i]=new int [m];
 }
 for(int i=0; i<n; i++)
    for(int j=0;j<m; j++){
            scanf("%d %d ", &machines[i][j],&optime[i][j]);
 //printf("%d %d ",machines[i][j],optime[i][j]);
    }
}

void Delete1(){
    for(int i=0; i<n; i++){
        delete(machines[i]);
        delete(optime[i]);
    }
    delete(machines);
    delete(optime);
    int* poi;
    while(!population.empty()){
        poi=population.back();
        population.pop_back();
    }
}

void GeneratePop(){
  for(int i=0;i<population_size;i++){
  int* chromosome=new int[n*m];
  for(int j=0;j<m*n;j++)
    chromosome[j]=-1;
  for(int j=0;j<m;j++)
        for(int k=0;k<n;k++){
                int r=rand()%n;
            while (chromosome[j*n+r]!=-1){
            r=rand()%n;
            //cout<<k<<" chuj\n";
            }
            chromosome[j*n+r]=k;
        }
  population.push_back(chromosome);
  }
}

int Time(int* chr){
 int tabm[m];
 int tabo[n];
 int ktop[n];
 for(int i=0; i<m; i++)
    tabm[i]=0;
 for( int i=0; i<n; i++){
    tabo[i]=0;
    ktop[i]=0;
 }
 while(1){
    bool czy=true;
    for(int i=0; i<n; i++){
            if(ktop[i]!=m){
             czy=false;
             break;
            }
    }
    if(czy==true)break;
    for(int i=0; i<m; i++){
        int op=-1;
        //cout<<"\n\n"<<i<<"\n\n";
        for(int j=0; j<n; j++){
                //cout<<j<<'\n';
        if(ktop[j]==m)continue;
        if(machines[j][ktop[j]]!=i)continue;
        for(int k=n*i; k<n*i+n; k++){
            //cout<<"K: "<<k<<" J: "<<j<<'\n';
          if(chr[k]==op)break;
          if(chr[k]==j){
          op=j;
          break;
          }
         }
        }
        //cout<<"Op "<<op<<'\n';
        if(op==-1)continue;
        tabm[i]=max(tabm[i], tabo[op])+optime[op][i];
        tabo[op]=tabm[i];
        ktop[op]++;
        //cout<<op<<'\n';
    }
 }
 int maks=0;
 for(int i=0; i<m; i++)
    if(tabm[i]>maks)maks=tabm[i];
 return maks;
}

bool comp(int* l, int* r)
{
    return ((Time(l)) <= (Time(r)));
}

void Wyswietl(){
 for(int i=0; i<population_size; i++)
        cout<<Time(population[i])<<'\n';
}

int Turniej(){
    int wylosowani[tsize];
    for(int i=0;i<tsize;i++)
        wylosowani[i]=rand()%population_size;
    int wygr=0, temp;
    int maks=Time(population[wylosowani[0]]);
    for(int i=1; i<tsize; i++){
     if((temp=Time(population[wylosowani[i]]))>maks){
            maks=temp;
            wygr=i;
     }
    }
    return wylosowani[wygr];
}

void GeneticAlgorithm(){
    start=clock();
    srand(time(NULL));
    Read1();
    GeneratePop();
    //int i=0;
    while(koniec<T)
    {
        //i++;
        int knames[ksize];
        int i=0;
        while(i<ksize)
        {
            int temp=Turniej();
            bool czy=false;
            for(int j=0;j<i;j++){
             if(temp==knames[j]){
              czy=true;
             break;
             }
            }
            if(czy==false){
                knames[i]=temp;
                i++;
            }
        }




        // Krzyzowanie cdn...
        for(i=0;i<ksize;i++)
        {
            int* chromosome1=new int[n*m];
            int* chromosome2=new int[n*m];
            //cout << "miash\n";
            int miejsce;
            do
            miejsce=(rand()%10)*10;
            while(miejsce==0);
            for(int j=0;j<miejsce;j++)
            {
                chromosome1[j]=population[i][j];
                chromosome2[j]=population[i+1][j];
            }
            for(int j=miejsce;j<n*m;j++)
            {
                chromosome1[j]=population[i+1][j];
                chromosome2[j]=population[i][j];
            }
            population.push_back(chromosome1);
            population.push_back(chromosome2);
            i++;
        }



        sort(population.begin(), population.end(), comp);
        //Wyswietl();
        cout << Time(population[0]) << endl;
        population.erase(population.begin()+population_size-1,population.end());
        koniec=clock()-start;

        //cout << koniec;
    }
    //cout<<i<<'\n';
}

int main(){
    srand(time(NULL));
    Read1();
    GeneratePop();
    for (int i=0; i<population.size();i++)
    {
        cout << Time(population[i]) << " " ;
    }
 //GeneticAlgorithm();
 Delete1();

 return 0;
}
