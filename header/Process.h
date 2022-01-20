#include <time.h>
class Process{
public:
    static int pi;
    int pid;
    double arr_time;
    double burst_time;
    double comp_time;
    double turn_time;
    double wait_time;
    double res_time;

    Process(double at, double bt){
        pid = pi++;
        arr_time = at;
        burst_time = bt;
        comp_time = -1;
        turn_time = -1;
        wait_time = -1;
        res_time = -1;
    }

    void print_info(){
        // ofstream outfile;
        // outfile.open("process.txt", ios_base::app);

        cout
        << "Process id: \t" << pid << '\n'
        << "Arrival time: \t" << arr_time << '\n'
        << "Burst time: \t" << burst_time << '\n'
        << "Completion time: \t" << comp_time << '\n'
        << "Turn around time: \t" << turn_time << '\n'
        << "Wait time: \t" << wait_time << '\n'
        << "Responce time: \t" << res_time << "\n\n";
    }
};


class Process_Creator{
public:
    const int big = 123456;
    int total_process;
    vector<Process> pr;

    Process_Creator(int tp){
        total_process = tp;
        process_gen();
    }

    void process_gen(){
        srand(time(NULL));
        forn(i, total_process){
            double at = (double)(rand())/big*10;
            double bt = (double)(rand())/big*10;
            at = round(at*1000)/1000;
            bt = round(bt*1000)/1000;
            Process temp(at, bt);
            pr.pb(temp);
        }
    }
};
int Process::pi = 1;
