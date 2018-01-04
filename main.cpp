#include <iostream>
#include <fstream>
#include <list>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;
int PopulationNumber=50;

class verticle
{
public:
    int v,color;
    list<verticle*> neighbour;
    void set_id(int v){
        this->v=v;
        color=-1;
    }
};

class chromosome
{
public:
    int fitness,factor;
    int* colours;
    chromosome(){fitness=0;}
};
void print(verticle T[], int n)  //wyswietla graf
{
    for(int i=0; i<n; i++)
    {
        cout<<i<<": ";
        for(auto it = T[i].neighbour.begin();it!=T[i].neighbour.end();it++)
        {
            cout<<(*it)->v<<", ";
        }
        cout<<endl;
    }
}

int greedy(verticle T[], int n) //algorytm zachlanny
{
    bool colors[n];
    for(int i=0; i<n; i++)
    {
        for(int a=0; a<n; a++) //zeruje tablice dostepnych kolorow
            colors[a]=false;

        if(T[i].color==-1) //jezeli wierzcholek nie ma jeszcze koloru
        {
            for(auto it = T[i].neighbour.begin();it!=T[i].neighbour.end();it++) //przechodzi po sasiadach wierzcholka
            {
                if((*it)->color!=(-1)) colors[(*it)->color]=true; //jezeli wierzcholek jest pokolorowany, zaznacz to w tablicy kolorow
            }
            for(int a=0; a<n; a++)
                if(colors[a]==false) //szuka dostepnych kolorow
                {
                    T[i].color = a;
                    break;
                }
        }
    }

    int max=T[0].color;
    for(int a=1; a<n; a++) //sprawdza, ile kolorow jest uzytych
    {
        if(T[a].color>max) max=T[a].color;
    }
    return max+1; //+1 bo kolory sa numerowane od 0
}

void generate_chromosomes(int n,int p, chromosome population[],int wynik)
{
    for(int i=p; i<PopulationNumber; i++)
    {
        for(int j=0; j<n; j++)
        {
            population[i].colours[j]=rand()%wynik;
        }
    }
}

void count_fitness(int n, chromosome population[], verticle T[])
{
    for(int i=0; i<PopulationNumber; i++)
    {
        for(int j=0; j<n; j++)
        {
            for(auto it: T[j].neighbour)
            {
                if(population[i].colours[j]==population[i].colours[it->v])
                    population[i].fitness++;

            }
        }
    }
}
void quicksort(chromosome P[], int lewy, int prawy)
{
    if(prawy == lewy) return;
    int i = lewy, j = prawy, pivot = P[(lewy + prawy)/2].factor;
    chromosome temp;
    while(1)
    {
        while(pivot > P[i].factor) i++;
        while(pivot < P[j].factor) j--;
        if(i <= j)
        {
            temp = P[i];
            P[i] = P[j];
            P[j] = temp;
            i++;j--;
        }
        else break;
    }
    if(j > lewy) quicksort(P, lewy, j);
    if(i < prawy) quicksort(P, i, prawy);

}
void parent_selection1(chromosome &parent1, chromosome &parent2,chromosome population[])
{
    chromosome temp1, temp2;
    int x;
    x=rand()%PopulationNumber;
    temp1=population[x];
    x=rand()%PopulationNumber;
    temp2=population[x];
    if(temp1.factor>temp2.factor) parent1=temp2;
    else parent1=temp1;
    x=rand()%PopulationNumber;
    temp1=population[x];
    x=rand()%PopulationNumber;
    temp2=population[x];
    if(temp1.factor>temp2.factor) parent2=temp2;
    else parent2=temp1;
}
void parent_selection2(chromosome &parent1, chromosome &parent2,chromosome population[])
{
    int bestfitness=population[0].factor;
    int last=0,index;
    do{
        last++;
    }while(population[last].factor==bestfitness);
    index=rand()%last;
    parent1=population[index];
    parent2=population[index];
}
void crossover(int n,chromosome &parent1, chromosome &parent2,chromosome &child)
{
    int x=rand()%n;
    for(int i=0; i<x; i++) child.colours[i]=parent1.colours[i];
    for(int i=x; i<n; i++) child.colours[i]=parent2.colours[i];
}

void mutation1(chromosome &child, verticle T[], int n, int wynik)
{
    int ValidColours[n];
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++) ValidColours[j]=0;
        for(auto it:T[i].neighbour) ValidColours[it->v]=1;
        for(auto it: T[i].neighbour)
            if(child.colours[i]==child.colours[it->v])
            {
                int x;
                do{
                    x=rand()%wynik;
                }while(ValidColours[x]!=0);
                child.colours[i]=x;
            }
    }
}
void mutation2(chromosome &child, verticle T[], int n)
{
    for(int i=0; i<n; i++)
    {
        for(auto it: T[i].neighbour)
            if(child.colours[i]==child.colours[it->v])
            {
                child.colours[i]=rand()%n;
            }
    }
}
void wisdomofArtificialCrowds(chromosome &chr,int n,verticle T[])
{
    int MostUsedColour[n];
    int Colours[n];
    int maxx;
    for(int i=0; i<n; i++)
    {
        for(int z=0; z<n; z++) Colours[z]=0;
        for(int j=0; j<PopulationNumber/2; j++)
        {
            for(int z=0; z<n; z++) Colours[z]++;
        }
        maxx=0;
        for(int z=1; z<n; z++) if(Colours[z]>Colours[maxx]) maxx=z;
        MostUsedColour[i]=maxx;
    }
    for(int i=0; i<n; i++)
    {
        for(auto it:T[i].neighbour)
            if(chr.colours[i]==chr.colours[it->v])
                chr.colours[i]=MostUsedColour[i];
    }
}
int counting_colours(int n,chromosome &chr)
{
    int existingColours[n];
    int optimum = 0;
    for (int i=0; i<n; i++) existingColours[i] = 0;
    for (int i=0; i<n; i++)
    {
        existingColours[chr.colours[i]]=1;
    }
    for (int i=0; i<n; i++) if(existingColours[i]) optimum++;
    return optimum;
}
void counting_factor(chromosome population[],int n)
{
    for(int i=0; i<PopulationNumber; i++)
    {
        int temp=counting_colours(n,population[i]);
        population[i].factor=temp*(population[i].fitness+1);
    }
}

void print_pop(chromosome population[], int n){
    for(int i=0; i<PopulationNumber; i++)
    {
        for(int j=0; j<n; j++)
            cout<<population[i].colours[j]<<" ";
        cout<<endl;
    }
    Sleep(500);
}
int genetic(verticle T[], int n, int wynik)
{
    chromosome population[PopulationNumber],pop[PopulationNumber];
    for(int i=0; i<PopulationNumber;i++)
    {
        population[i].colours=new int[n];
        pop[i].colours=new int[n];
    }
    generate_chromosomes(n,0, population,wynik);
    int count=0;
    chromosome parent1,parent2;
    parent1.colours= new int[n];
    parent2.colours= new int[n];
    do
    {
        generate_chromosomes(n,(PopulationNumber/2),population,wynik);

        count_fitness(n,population,T);
        counting_factor(population,n);
        quicksort(population,0,PopulationNumber-1);

        for(int i=0; i<PopulationNumber; i++){
            chromosome child;
            child.colours=new int [n];
            parent_selection1(parent1,parent2,population);
            crossover(n,parent1,parent2,child);
            mutation1(child,T,n,wynik);
            pop[i]=child;
        }
        for(int i=0; i<PopulationNumber; i++)
        {
            population[i]=pop[i];
        }
        count_fitness(n,population,T);
        counting_factor(population,n);
        quicksort(population,0,PopulationNumber-1);
        //print_pop(population,n);
        count++;
    }
    while(count!=10000);
    if(population[0].fitness!=0) wisdomofArtificialCrowds(population[0],n,T);
    int result=counting_colours(n,population[0]);
    return result;
}

int main() {
    srand(time(NULL));
    ifstream dane;
    dane.open("gc500.txt");
    int n,a,b;
    dane>>n;
    verticle T[n];
    for(int i=0; i<n; i++)
    {
        T[i].set_id(i);
    }
    while(!dane.eof()) //tworzy graf
    {
        dane >> a >> b;
        T[a-1].neighbour.push_back(&T[b-1]);
        T[b-1].neighbour.push_back(&T[a-1]);
    }
    dane.close();
    int wynik;
    wynik=greedy(T,n);
    cout<<"Greedy: "<<wynik<<" kolorow"<<endl;
    cout<<"Genetic: "<<genetic(T,n,wynik)<<" kolorow";
    return 0;
}
