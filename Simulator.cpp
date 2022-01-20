#include <bits/stdc++.h>
#include <direct.h>

using namespace std;
#define forn(x, n) for(int x=0; x<n; x++)
#define pb push_back
#define name "Output"
#include "header/Process.h"
#include "header/Scheduler.h"

int main(){
    double sim_time, tq;
    int option;
    _mkdir(name);
    
    cout << "Enter Simulation Time\n";
    cin >> sim_time;
    cout << "Option: \n1: First Come First Serve\n" <<
            "2: Shortest Remaining Time First\n" <<
            "3: Round Robin\n";
    cin >> option;

    Process_Creator prc(2*sim_time);
    Scheduler sch(sim_time);
    if (option == 1)    sch.FCFS(prc.pr);
    else if(option == 2)    sch.SRTF(prc.pr);
    else if (option == 3){
        cout << "Enter Time Quantum\n";
        cin >> tq;
        sch.RR(prc.pr, tq);
    }
    // forn(i, sim_time)   prc.pr[i].print_info();
}
