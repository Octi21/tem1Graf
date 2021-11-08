#include <bits/stdc++.h>

using namespace std;

/*
ifstream in("sortaret.in");
ofstream out("sortaret.out");
*/
ifstream in("biconex.in");
ofstream out("biconex.out");



class Graf
{
    int n , m;
    vector <vector <int>> adiacenta;
    vector <vector<int>> rev;

    void dfs(int nod,bool viz[]);

    int minim(int a, int b);
    void dfsCc(int nodp, vector<int>& prnt, vector<int>& desc, vector<int>& ret, stack<pair<int,int>>& st, int & cc ,vector <vector <int>>& compCon);

    void dfsNorm(int & nod, vector <bool>& viz, stack <int>& st);
    void  dfsNorm2(int & nod, vector <bool>& viz,  int & nrC ,vector <vector<int>>& solutii);

    void dfsMC(int nodp, vector<int>& prnt, vector<int>& desc, vector<int>& ret, stack <pair<int,int>>& st);
    public:
        Graf(int n, int m, vector <vector <int>> adiacenta);
        Graf(int n, int m, vector <vector <int>> adiacenta, vector <vector <int>> rev);

        vector<int> bfs(int nod); // prob1 bfs
        int compCon(); // prob2 comn con cu dfs
        void PA(); // prob3 compBicon
        void tareConexC(); // prob4
        void muchieC(); //prob 5 muchie critica
        void topologic (); // prob6 sortTopologica

};

Graf :: Graf(int n, int m, vector <vector <int>> adiacenta)
{
    this-> n = n;
    this-> m = m;
    this -> adiacenta = adiacenta ;
    /*for(int i = 0; i<= n; ++i)
    {
        this-> rev[i] = NULL;
    }*/

}
Graf :: Graf(int n, int m, vector <vector <int>> adiacenta, vector <vector<int>> rev)
{
    this-> n = n;
    this-> m = m;
    this -> adiacenta = adiacenta ;
    this -> rev = rev;

}


vector <int> Graf :: bfs(int nod)
{
    vector <int> dist;

    queue <int> coada;

    for(int i=0; i<=n; ++i)
    {
        dist.push_back(-1);

    }
    coada.push (nod);

    dist[nod] = 0;

    while(!coada.empty())
    {
        int p = coada. front();
        for(int i=0; i<adiacenta[p].size(); ++i)
        {
            if( dist[adiacenta[p][i]] == -1)
            {
                dist[adiacenta[p][i]] = dist[p] + 1;
                coada. push(adiacenta[p][i]);
            }
        }
        coada.pop();
    }
    return dist;
}

void p1()
{
    int n,m,nod, x, y;
    in>>n>>m>>nod;
    vector <vector <int>> adiacenta (n+1);



    for(int i=0;i<m; ++i)
    {
        in>>x>>y;
        adiacenta[x].push_back(y);
    }

    Graf g(n,m,adiacenta);
    vector<int> r = g.bfs(nod);

    for(int i = 1;i<=n; ++i)
        out<<r[i]<<" ";
}



void Graf :: dfs(int nod,bool viz[])
{
    viz[nod] = true;
    for(int i = 0; i <adiacenta[nod].size(); ++i)
    {
        if(viz[adiacenta[nod][i]] == false)
        {
            viz[adiacenta[nod][i]] = true;
            dfs(adiacenta[nod][i],viz);
        }
    }
}

int Graf :: compCon()
{
    int nrComp = 0;
    bool* viz = new bool[n+1];
    for(int i = 0; i<=n; ++i)
    {
        viz[i] = false;
    }
    for(int i = 1; i<= n; ++i)
    {
        if(viz[i] == false)
        {
            dfs(i, viz);
            nrComp++;
        }
    }
    return nrComp;

}

void p2()
{

    int n,m,x,y;
    in>>n>>m;
    vector <vector <int>> adiacenta (n+1);

    for(int i =0; i<m; ++i)
    {
        in>>x>>y;
        adiacenta[x].push_back(y);
        adiacenta[y].push_back(x);
    }
    Graf g1 = Graf(n,m,adiacenta);
    int r = g1.compCon();
    out<<r;

}



int Graf ::  minim(int a, int b)
{
    if(a>b)
        return b;
    return a;
}

void Graf :: dfsCc(int nodp, vector<int>& prnt, vector<int>& desc, vector<int>& ret , stack<pair<int,int>>& st, int & cc, vector <vector <int>>& compCon)
{
    static int timp = 1;
    desc[nodp] = timp;
    ret[nodp] = timp;
    timp ++;
    int nrc = 0;

    for(auto nodc: adiacenta[nodp])
    {
        if(desc[nodc] == -1)
        {
            nrc++;
            prnt[nodc] = nodp;
            st.push( make_pair (nodp,nodc));
            dfsCc(nodc,prnt,desc,ret,st,cc,compCon);

            ret[nodp] = minim(ret[nodp],ret[nodc]);

            if((prnt[nodp] == -1 && nrc > 1)  || (prnt[nodp] != -1 && desc[nodp]<= ret[nodc]))  // verific noduri sunt ap
            {

                while(st.top().first != nodp || st.top().second != nodc)
                {
                    compCon[cc].push_back(st.top().second);
                    st . pop();
                }
                compCon[cc].push_back(st.top().second);
                compCon[cc].push_back(st.top().first);

                st . pop();

                cc++;

            }
        }
        else if(nodc != prnt[nodp])
        {
            ret[nodp] = minim(ret[nodp], desc[nodc]);
        }
    }

}

void Graf :: PA()
{
    vector<int> prnt (n+1,-1), desc (n+1,-1), ret (n+1,-1);
    vector<bool> ap(n+1,false);
    stack<pair<int,int>> st;
    vector <vector <int>> compCon (n+1);
    int cc = 1;
    int chestie;

    for(int i = 1; i<= n; ++i)
    {
        if(desc[i] == -1)
        {
            dfsCc(i,prnt,desc,ret,st,cc,compCon);
        }
        while(! st.empty())
        {
            compCon[cc].push_back(st.top().second);
            chestie = st.top().first;
            st . pop();
        }
    }
    compCon[cc].push_back(chestie);

    out<<cc<<"\n";
    for(int i = 1;i<= cc; ++i)
    {
        for(auto j : compCon[i])
        {
            out<<j<<" ";
        }
        out<<"\n";
    }
}

void p3()
{
    int n,m,x,y;
    in>>n>>m;
    vector <vector <int>> adiacenta (n+1);

    for(int i =0; i<m; ++i)
    {
        in>>x>>y;
        adiacenta[x].push_back(y);
        adiacenta[y].push_back(x);
    }

    Graf g1 = Graf(n,m,adiacenta);

    g1.PA();
}



void Graf :: dfsNorm(int & nod, vector <bool>& viz, stack <int>& st)
{
    viz[nod] = true;
    //cout<<nod<<" ";
    for(int i : adiacenta[nod])
    {
        if( viz[i] == false)
        {
            //viz[nod] = true;
            dfsNorm(i,viz,st);
            //st.push(i);

        }
    }
    st.push(nod);

}

void Graf :: dfsNorm2(int & nod, vector <bool>& viz, int & nrC ,vector <vector<int>>& solutii)
{
    viz[nod] = true;
    solutii[nrC] . push_back(nod);
    //out<<nod<<" ";
    for(int i : rev[nod])
    {
        if( viz[i] == false)
        {
            viz[nod] = true;
            dfsNorm2(i,viz/*,rev*/,nrC,solutii);
            //st.push(i);

        }
    }

}



void Graf :: tareConexC()
{
    stack <int> st;
    vector <bool> viz (n+1, false);
    vector <vector<int>>  solutii (n+1);
    int nrC = 0;

    for(int i = 1;i <= n; ++i)
    {
        if(viz[i] == false)
        {
            dfsNorm(i,viz,st);
            //st.push(i);
        }
    }


    for(int i =1; i<= n; ++i)
        viz[i] = false;

    while(! st.empty())
    {
        if(viz[st.top()] == false)
        {
            nrC ++;
            dfsNorm2(st.top(),viz,nrC,solutii);
        }
        st.pop();
    }
    out<<nrC<<"\n";
    for(int i = 1;i<= nrC; ++i)
    {
        for(auto j : solutii[i])
        {
            out<<j<<" ";
        }
        out<<"\n";
    }

}

void p4 ()
{
    int n,m,x,y;
    in>>n>>m;
    vector <vector<int>> adiacenta (n+1), rev(n+1);
    for(int i = 1; i<=m ; ++i)
    {
        in>>x>>y;
        adiacenta[x].push_back(y);
        rev[y].push_back(x);
    }
    Graf g(n,m,adiacenta,rev);
    g.tareConexC();
}


void Graf :: dfsMC(int nodp, vector<int>& prnt, vector<int>& desc, vector<int>& ret, stack <pair<int,int>>& st)
{
    static int timp = 1;
    desc[nodp] = timp;
    ret[nodp] = timp;
    timp ++;

    for(auto nodc: adiacenta[nodp])
    {
        if(desc[nodc] == -1)
        {
            prnt[nodc] = nodp;
            dfsMC(nodc,prnt,desc,ret,st);

            ret[nodp] = minim(ret[nodp],ret[nodc]);


            if( desc[nodp] < ret[nodc]) // verific muchie crit
            {
                st. push(make_pair(nodp,nodc));
            }
        }
        else if(nodc != prnt[nodp])
        {
            ret[nodp] = minim(ret[nodp], desc[nodc]);
        }
    }

}

void Graf :: muchieC()
{
    vector<int> prnt (n+1,-1), desc (n+1,-1), ret (n+1,-1);
    stack <pair<int,int>> st;

    for(int i = 1; i<= n; ++i)
    {
        if(desc[i] == -1)
        {
            dfsMC(i,prnt,desc,ret,st);
        }
    }
    out<<"[";
    while(! st.empty())
    {
        out<<" ["<<st.top().first<<","<<st.top().second<<"] ";
        st.pop();
        if(! st.empty())
            out<<" , ";
    }
    out<<"]";
}

void p5()
{
    int n,m,x,y;
    in>> n >> m;
    vector <vector<int>> adiacenta (n+1);
    for(int i = 1; i<= m ; ++i)
    {
        in>>x>>y;
        adiacenta[x].push_back(y);
        adiacenta[y].push_back(x);
    }
    Graf g(n,m,adiacenta);
    g.muchieC();
}


void Graf :: topologic ()
{
    vector <bool> viz (n+1, false);
    vector <vector<int>> adiacenta;
    stack <int> st;

    for(int i = 1;i <= n; ++i)
    {
        if(! viz[i])
            dfsNorm(i,viz,st);
    }
    while(! st.empty())
    {
        out<< st.top() <<" ";
        st.pop();
    }
}

void p6()
{
    int n,m,x,y;
    in>> n >> m;
    vector <vector<int>> adiacenta (n+1);
    for(int i = 1; i<= m ; ++i)
    {
        in>>x>>y;
        adiacenta[x].push_back(y);
    }
    Graf g(n,m,adiacenta);
    g.topologic();
}


void p7()
{
    int n, ok = -1,val,x;
    in >> n;
    vector <int> havelH (n);
    for(int i = 0; i< n; ++i )
    {
        in>> x;
        havelH.push_back(x);
    }
    while(ok == -1)
    {
        sort(havelH.begin(), havelH.end(), greater<>());

        if (havelH[0] == 0)
        {
            ok = 1;
            break;
        }
        val = havelH[0];
        havelH.erase(havelH.begin() + 0);

        if(val > havelH.size())
        {
            ok = 0;
            break;
        }

        for(int i = 0; i < val; ++i)
        {
            havelH[i] --;
            if(havelH[i] < 0)
                ok = 0;
        }
    }
    if(ok == 1)
        out<<"Da";
    else
        out<<"NU";
}


int main()
{
    //p1();
    p2();
    //p3();
    //p4();
    //p5();
    //p6();
    //p7();
    return 0;
}
