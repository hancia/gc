#include <iostream>
#include <fstream>
#include <list>
using namespace std;

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

void print(verticle T[], int n)
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

int greedy(verticle T[], int n)
{
    bool colors[n];
    for(int i=0; i<n; i++)
    {
        for(int a=0; a<n; a++)
            colors[a]=false;

        if(T[i].color==-1)
        {
            for(auto it = T[i].neighbour.begin();it!=T[i].neighbour.end();it++)
            {
                if((*it)->color!=(-1)) colors[(*it)->color]=true;
            }
            for(int a=0; a<n; a++)
                if(colors[a]==false)
                {
                    T[i].color = a;
                    break;
                }
        }
    }

    int max=T[0].color;
    for(int a=1; a<n; a++)
    {
        if(T[a].color>max) max=T[a].color;
    }
    return max+1;
}

int main() {
    ifstream dane;
    dane.open("graf.txt");
    int n,a,b;
    dane>>n;
    verticle T[n];
    for(int i=0; i<n; i++)
    {
        T[i].set_id(i);
    }
    while(!dane.eof())
    {
        dane >> a >> b;
        T[a-1].neighbour.push_back(&T[b-1]);
        T[b-1].neighbour.push_back(&T[a-1]);
    }
    dane.close();
    cout<<"Potrzeba: "<<greedy(T,n)<<" kolorow";
    return 0;
}