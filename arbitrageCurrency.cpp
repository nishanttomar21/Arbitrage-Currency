#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

#define NUMBER_OF_CURRENCIES 158
#define INPUT_FILE_PATH "exchange_rates.txt"
#define OUTPUT_FILE_PATH "result.txt"

using namespace std;


const double MAX_LEN = INT_MAX;


typedef struct Graph
{
    
    int predecessor;
    double cur_dist;
    string name;
    double from[NUMBER_OF_CURRENCIES];

}Graph;



void relax(Graph *graph, int i)
{
    
    double *from = graph[i].from;
    
    for(int j = 0; j < NUMBER_OF_CURRENCIES; j++)
    {
        
        double compare = graph[j].cur_dist + from[j];
        
        if(graph[i].cur_dist > compare)
        {
            
            graph[i].cur_dist = compare;
            graph[i].predecessor = j;
        
        }

    }

}


void Bellman_Ford(Graph *graph)
{
    
    graph[0].cur_dist = 0;
    
    for(int i = 0; i < NUMBER_OF_CURRENCIES; i++)
        graph[i].predecessor = -1;
    
    for(int i = 1; i < NUMBER_OF_CURRENCIES; i++)
        graph[i].cur_dist = MAX_LEN;
    
    for(int i = 0; i < NUMBER_OF_CURRENCIES-1; i++)
    {
        
        for(int j = 0; j < NUMBER_OF_CURRENCIES; j++)
        {
            
            relax(graph, j);
        
        }
        
    }
    
}



void Edge_length(Graph* graph, int from, int to, double len)
{
    
    graph[to].from[from] = len;

}



bool Initialize_Graph(Graph *graph)
{
    
    ifstream file;
    file.open(INPUT_FILE_PATH);
    
    string from , to;
    double rate;
    
    for(int i = 0; i < NUMBER_OF_CURRENCIES; i++)
    {
        
        for(int j = 0; j < NUMBER_OF_CURRENCIES; j++)
        {
            
            file>>from;
            file>>to;
            file>>rate;
            
            if(i == 0)
                graph[j].name = to;
            
            if(i == j)
                Edge_length(graph, i, j, 0);
            
            else
                Edge_length(graph, i, j, -log(rate));
            
        }
        
    }
    
    file.close();
    
    return true;
    
}



int Find_Cycle_Start(Graph *graph, int Node)
{
    
    bool visited[NUMBER_OF_CURRENCIES];
    
    while( !visited[Node] )
    {
        
        visited[Node] = true;
        Node = graph[Node].predecessor;
        
    }
    
    return Node;

}



void print_negative_cycle(Graph* graph, int Node)
{
    
    int Cycle_Start = Find_Cycle_Start(graph, Node);
    int Cur_Node = Cycle_Start , Last_Node = Cycle_Start;
    float amount, total_amount;
        
    cout<<endl<<"\nArbitrage Cycle Detected!\n\n"<<endl;
        
    double Profit = 0;
        
    do
    {
            
        Last_Node = Cur_Node;
        Cur_Node = graph[Cur_Node].predecessor;
        Profit = Profit + graph[Last_Node].from[Cur_Node];
            
        cout<<graph[Cur_Node].name<<" -> "<<graph[Last_Node].name<<" "<<exp(-graph[Last_Node].from[Cur_Node])<<endl;
            
    } while(Cur_Node != Cycle_Start);
        
    total_amount = amount*(exp(-Profit)-1);
    
    cout<<"\nENTER THE AMOUNT: ";
    cin>>amount,
    cout<<"\n\nPROFIT: "<<amount*(exp(-Profit)-1)<<"\n\n\n";
    
    exit(0);

}



void Find_Negative_Cycle(Graph *graph)
{

    bool found = false;
    
    for(int i = 0; i < NUMBER_OF_CURRENCIES; i++)
    {
        
        for(int j = 0; j < NUMBER_OF_CURRENCIES; j++)
        {
            
            if(graph[i].cur_dist > (graph[j].cur_dist + graph[i].from[j]) )
            {
                found = true;
                print_negative_cycle(graph, i);
            }
            
        }
        
    }
    
    if( !found )
        cout<<"No Arbitrage Cycle Found!!"<<"\n\n";

}


int main()
{
    
    Graph graph[NUMBER_OF_CURRENCIES];

    Initialize_Graph(graph);

    Bellman_Ford(graph);
    
    Find_Negative_Cycle(graph);
    
    return 0;

}