#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <algorithm>

using namespace std;

const int population_size=20;
int n,m; //n - liczba operacji, m - liczba maszyn
vector <int*> population;
int** machines,** optime;
const int T=6000, tsize=10, ksize=4, czymut=60;
int start,koniec;
bool czyTailard=0;

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
    }
}

void ReadTailard(char *a){
    string slowo;
    fstream plik;
    plik.open(a,ios::in);

    if(plik.good()==false)
    {
        cout<<"Nie uda³o siê wczytaæ danych!";
        exit(0);
    }
    plik >> n >> m;
    machines=new int*[n];
    optime=new int* [n];
    for(int i=0; i<n; i++){
            machines[i]=new int [m];
            optime[i]=new int [m];
    }
    do
        plik >> slowo;
    while(slowo!="Times");
    for(int i=0; i<n; i++)
    for(int j=0;j<m; j++){
            plik >> optime[i][j];
    }
    do
        plik >> slowo;
    while(slowo!="Machines");
    for(int i=0; i<n; i++)
    for(int j=0;j<m; j++){
            plik >> machines[i][j];
            machines[i][j]--;
    }
    plik.close();
}

void ReadBeasley(char *a){
    fstream plik;
    plik.open(a,ios::in);

    if(plik.good()==false)
    {
        cout<<"Nie uda³o siê wczytaæ danych!";
        exit(0);
    }
    plik >> n >> m;
    machines=new int*[n];
    optime=new int* [n];
    for(int i=0; i<n; i++){
            machines[i]=new int [m];
            optime[i]=new int [m];
    }
    for(int i=0; i<n; i++)
    for(int j=0;j<m; j++){
            plik >> machines[i][j] >> optime[i][j];
    }
    plik.close();
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
        if(ktop[j]==m)continue;
        if(optime[j][ktop[j]]==0)ktop[j]++;
        for(int k=n*machines[j][ktop[j]]; k<n*machines[j][ktop[j]]+n; k++){
          if(chr[k]==ktozad[machines[j][ktop[j]]])break;
          if(chr[k]==j){
          ktozad[machines[j][ktop[j]]]=j;
          break;
          }
         }
    }

    for(int i=0; i<m; i++){
        if(ktozad[i]==-1)continue;
        tabm[i]=max(tabm[i], tabo[ktozad[i]])+optime[ktozad[i]][ktop[ktozad[i]]];
        tabo[ktozad[i]]=tabm[i];
        ktop[ktozad[i]]++;
    }

 }
 int maks=0;
 for(int i=0; i<m; i++)
    if(tabm[i]>maks)maks=tabm[i];
 return maks;
}

void Result(int* chr){
int result[n][m];
for(int i=0; i<n; i++)
        for(int j=0; j<m; j++)
            result[i][j]=0;
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
        if(ktop[j]==m)continue;
        if(optime[j][ktop[j]]==0)ktop[j]++;
        for(int k=n*machines[j][ktop[j]]; k<n*machines[j][ktop[j]]+n; k++){
          if(chr[k]==ktozad[machines[j][ktop[j]]])break;
          if(chr[k]==j){
          ktozad[machines[j][ktop[j]]]=j;
          break;
          }
         }
    }

    for(int i=0; i<m; i++){
        if(ktozad[i]==-1)continue;
        result[ktozad[i]][ktop[ktozad[i]]]=max(tabm[i], tabo[ktozad[i]]);
        tabm[i]=max(tabm[i], tabo[ktozad[i]])+optime[ktozad[i]][ktop[ktozad[i]]];
        tabo[ktozad[i]]=tabm[i];
        ktop[ktozad[i]]++;
    }
 }
 int maks=0;
 for(int i=0; i<m; i++)
    if(tabm[i]>maks)maks=tabm[i];
 cout<<maks<<'\n';
 for(int i=0; i<n; i++){
        for(int j=0; j<m; j++)
            cout<<result[i][j]<<' ';
        cout<<'\n';
        }
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
            }
            chromosome[j*n+r]=k;
        }
  chromosome[n*m]=Time(chromosome); // n*m-ty element przechowuje wynik funkcji celu
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

void Mutacja(int* chr){
int gdziemasz=rand()%m, a=rand()%n, b=rand()%n;
int temp=chr[gdziemasz*n+a];
chr[gdziemasz*n+a]=chr[gdziemasz*n+b];
chr[gdziemasz*n+b]=temp;
}

void GeneticAlgorithm(){
    start=clock();
    srand(time(NULL));
    GeneratePop();
    while(koniec<T)
    {
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
        for(i=0;i<ksize-1;i+=2)
        {
            int* chromosome1=new int[n*m+1];
            int* chromosome2=new int[n*m+1];
            int miejsce;
            miejsce=(rand() % m)*n;
            for(int j=0;j<miejsce;j++)
            {
                chromosome1[j]=population[knames[i]][j];
                chromosome2[j]=population[knames[i+1]][j];
            }
            for(int j=miejsce;j<n*m;j++)
            {
                chromosome1[j]=population[knames[i+1]][j];
                chromosome2[j]=population[knames[i]][j];
            }
            int mut=rand()%100;
            if(mut>=czymut)
                Mutacja(chromosome1);
            mut=rand()%100;
            if(mut>=czymut)
                Mutacja(chromosome2);
            chromosome1[n*m]=Time(chromosome1);
            chromosome2[n*m]=Time(chromosome2);
            population.push_back(chromosome1);
            population.push_back(chromosome2);
        }
        sort(population.begin(), population.end(), comp);
        for(int i=population_size; i<population.size(); i++)
            delete(population[i]);
        population.erase(population.begin()+population_size,population.end());
        koniec=clock()-start;

    }
    Result(population[0]);
}

void ZapiszWynik(char* a){
    fstream plik;
    plik.open(a,ios_base::app);
    plik << " " << population[0][100];
    plik.close();
}

int main(int argc, char* argv[]){
    if(argc>1){
        if(argv[1][0]=='t' && argv[1][1]=='a' && argv[1][2]=='i')
        {
            //cout << "Weszlem";
            ReadTailard(argv[1]);
            czyTailard=1;
        }
        else
            ReadBeasley(argv[1]);
    GeneticAlgorithm();
    Delete1();
    }
    else{
        cout<<"Nie ma parametru\n";
    }
    //ZapiszWynik(argv[1]);
 return 0;
}
