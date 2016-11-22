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
            printf("%d %d ",machines[i][j],optime[i][j]);
    }
 //cout<<"ReadEnd\n";
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
int Time(int* chr){
  int tabm[m];
 int tabo[n];
 int ktop[n], ktozad[m];
 for(int i=0; i<m; i++)
    tabm[i]=0;
 for( int i=0; i<n; i++){
    tabo[i]=0;
    ktop[i]=0;
 }
 while(1){
    for(int i=0; i<m; i++)
        ktozad[i]=-1;
    bool czy=true;
    for(int i=0; i<n; i++){
            if(ktop[i]!=m){
             czy=false;
             break;
            }
    }
    if(czy==true)break;
    for(int j=0; j<n; j++){
                //cout<<j<<'\n';
        if(ktop[j]==m)continue;
        for(int k=n*machines[j][ktop[j]]; k<n*machines[j][ktop[j]]+n; k++){
            //cout<<"Chr k: "<<chr[k]<<" J: "<<j<<" Ktozad[j]: "<<ktozad[machines[j][ktop[j]]]<<'\n';
          if(chr[k]==ktozad[machines[j][ktop[j]]])break;
          if(chr[k]==j){
          ktozad[machines[j][ktop[j]]]=j;
          break;
          }
         }
    }
    //cout<<"OK\n";

    for(int i=0; i<m; i++){
        //cout<<"Ktzad["<<i<<"]= "<<ktozad[i]<<'\n';
        //cout<<"OK\n";
        if(ktozad[i]==-1)continue;
        //if(ktozad[ktop[i]]>n)continue;
        //if(ktop[i]==n)continue;
        tabm[i]=max(tabm[i], tabo[ktozad[i]])+optime[ktozad[i]][ktop[ktozad[i]]];
        //cout<<tabm[i]<<'\n';
        tabo[ktozad[i]]=tabm[i];
        ktop[ktozad[i]]++;
    }
    /*for(int i=0; i<m; i++)
        cout<<"Ktoop["<<i<<"]= "<<ktop[i]<<'\n';
    for(int i=0; i<n; i++)
        cout<<"czasy: "<<tabm[i]<<'\n';
        //cout<<op<<'\n';*/

 }
 int maks=0;
 for(int i=0; i<m; i++)
    if(tabm[i]>maks)maks=tabm[i];
 return maks;
}

void GeneratePop(){
  for(int i=0;i<population_size;i++){
  int* chromosome=new int[n*m+1];
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
  chromosome[n*m]=Time(chromosome);
  population.push_back(chromosome);
  }
}



bool comp(int* l, int* r)
{
    return ((l[n*m]) < (r[n*m]));
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
    //cout<<"ok"<<'\n';
    //int i=0;
    while(koniec<T)
    {
        //i++;
        //cout<<"ok\n";
        int knames[ksize];
        int i=0;
        while(i<ksize)
        {
            //cout<<"ok\n";
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
        for(i=0;i<ksize-1;i++)
        {
            int* chromosome1=new int[n*m+1];
            int* chromosome2=new int[n*m+1];
            //cout << "miash\n";
            int miejsce;
            //cout<<m<<' ';
            miejsce=(rand() % m)*n;
            //cout<<miejsce<<' ';
            for(int j=0;j<miejsce;j++)
            {
                chromosome1[j]=population[knames[i]][j];
                chromosome2[j]=population[knames[i+1]][j];
            }
            //cout<<"spoko";
            for(int j=miejsce;j<n*m;j++)
            {
                chromosome1[j]=population[knames[i+1]][j];
                chromosome2[j]=population[knames[i]][j];
            }
            chromosome1[n*m]=Time(chromosome1);
            chromosome2[n*m]=Time(chromosome2);
            population.push_back(chromosome1);
            population.push_back(chromosome2);
        }
        //cout<<"spoko";
        sort(population.begin(), population.end(), comp);
        //Wyswietl();
        cout << Time(population[0]) << endl;
        for(int i=population_size; i<population.size(); i++)
            delete(population[i]);
        population.erase(population.begin()+population_size,population.end());
        koniec=clock()-start;

        //cout << koniec;
    }
    //cout<<i<<'\n';
}

int main(){
    GeneticAlgorithm();
    /*srand(time(NULL));
    Read1();
    GeneratePop();
    sort(population.begin(), population.end(), comp);
    Wyswietl();*/
 //GeneticAlgorithm();
 Delete1();

 return 0;
}
