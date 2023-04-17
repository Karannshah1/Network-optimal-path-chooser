#include <bits/stdc++.h>
using namespace std;


struct Edge {
	int v; 
	int flow; 

	int C; 
    int time;
	int rev; 
};

// Residual Graph
class Graph {
	int V; // number of vertex
	int* level; // stores level of a node
	vector<Edge>* adj;
public:
    vector<vector<int>> ad,fl;
	int var_time=0;
	Graph(int V)
	{
		adj = new vector<Edge>[V];
		this->V = V;
		level = new int[V];
        for(int i=0;i<V;i++)
        {
            vector<int> x(V,0);
            ad.push_back(x);
			fl.push_back(x);
        }
	}

	// add edge to the graph
	void addEdge(int u, int v, int C,int t)
	{
		// Forward edge : 0 flow and C capacity
		Edge a{ v, 0, C, t,(int)adj[v].size() };

		// Back edge : 0 flow and 0 capacity
		Edge b{ u, 0, 0, t,(int)adj[u].size() };

		adj[u].push_back(a);
		adj[v].push_back(b); // reverse edge
        ad[u][v]=t;
		fl[v][u]=C;
		fl[u][v]=C;

	}

	bool BFS(int s, int t);
	int sendFlow(int s, int flow, int t, int ptr[]);
	void DinicMaxflow(int s, int t);
    int Dijkstra(int src,int end);
    int dist(vector<int> a,vector<bool> b);
};

// Finds if more flow can be sent from s to t and assign level to nodes

bool Graph::BFS(int s, int t)
{
	for (int i = 0; i < V; i++)
		level[i] = -1;

	level[s] = 0; 

	list<int> q;
	q.push_back(s);

	vector<Edge>::iterator i;

	while (!q.empty()) {
		int u = q.front();
		q.pop_front();
		for (i = adj[u].begin(); i != adj[u].end(); i++) {

			Edge& e = *i;
			if (level[e.v] < 0 && e.flow < e.C) {
				// Level of current vertex is,
				// level of parent + 1
				level[e.v] = level[u] + 1;

				q.push_back(e.v);
			}
		}
	}

	return level[t] < 0 ? false : true;
}

//dfs as we have to find max path
int Graph::sendFlow(int u, int flow, int t, int start[])
{
	int pre=0;
	if (u == t)
		return flow;

	for (; start[u] < adj[u].size(); start[u]++) {
			Edge& e = adj[u][start[u]];

		if (level[e.v] == level[u] + 1 && e.flow < e.C) {// find minimum flow from u to t

			int curr_flow = min(flow, e.C - e.flow);
			
			int temp_flow
				= sendFlow(e.v, curr_flow, t, start);
			
			// int var_time =  Dijkstra(u,e.v);
			// var_time = max(pre,var_time);
			// pre = var_time;

			if (temp_flow > 0) {

				e.flow += temp_flow;

				// subtract flow from reverse edge of current edge
				adj[e.v][e.rev].flow -= temp_flow;
				var_time += ad[u][e.v];


				return temp_flow;
				
			}
		}

		
	}
	
	return 0;
}


void Graph::DinicMaxflow(int s, int t)
{

	int y=0;
	int total = 0; 

	while (BFS(s, t) == true) {
		int* start = new int[V+1]{0};
		
		while (int flow = sendFlow(s, INT_MAX, t, start))
			{total += flow;
			}
			
	}

	cout<<"Max flow :- " <<total<<endl;
	cout<<"time is :- "<<var_time<<endl;
}

int Graph::Dijkstra(int src,int end)
{
    vector<int> a(V,INT_MAX);
    vector<bool> b(V);
    int ans=0,f=INT_MAX,pre=0;
	int* start = new int[V+1]{0};
    a[src]=0;
    
    for(int i=0;i<V;i++)
    {
        int temp = dist(a,b);

        b[temp] = true;
        
		int  y= 0;
        for(int x=0;x<V;x++)
        {
            if(!b[x]&& ad[temp][x] && a[temp]!=INT_MAX &&a[temp]+ad[temp][x]<a[x])
            {a[x]=a[temp]+ad[temp][x];y=x;}
			f+=sendFlow(temp,INT_MAX,y,start);
        }

    }
    cout<<endl;
    for(auto x:a){ans+=x;}
    cout<<endl;
	cout<<"Maximum flow from this is: "<<f<<endl;
    cout<<"Minimum time :- " <<ans;
	return ans;
}

int Graph::dist(vector<int> a,vector<bool> b)
{
    int mini = INT_MAX,min_index=0;

    for(int v=0;v<V;v++)
    {
        if(b[v]==false && a[v]<=mini) mini = a[v],min_index = v;
    }
    return min_index;
}


int main()
{
	

    int n,e;
    cin>>n>>e;
    Graph g(n);
    for(int i=0;i<e;i++)
    {
        int u,v,flow,time;
        cin>>u>>v>>flow>>time;
        g.addEdge(u,v,flow,time);
    }

	/*Graph g(6);
	g.addEdge(0, 1, 10,100);
	g.addEdge(0, 2, 10,234);
	g.addEdge(1, 3, 4 ,234);
	g.addEdge(1, 4, 8,857 );
	g.addEdge(1, 2, 2,46 );
	g.addEdge(2, 4, 9,82);
	g.addEdge(3, 5, 10 ,118);
	g.addEdge(4, 3, 6 ,123);
	g.addEdge(4, 5, 10 ,121); */


	cout << "Based on flow : " <<endl<<endl;
	g.DinicMaxflow(0, n-1);
	cout<<endl<<endl;

    cout<< "Based on time : ";
	int temp = g.Dijkstra(0,n-1);

}
