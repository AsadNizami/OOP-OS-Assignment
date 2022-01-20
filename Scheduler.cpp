#include <bits/stdc++.h>
#include <fstream>
using namespace std;
#define forn(x, n) for(int x=0; x<n; x++)
#define pb push_back
#include "header/Process.h"

struct compare_at{
    bool operator()(Process &p1, Process &p2){
        if (p1.arr_time != p2.arr_time)
            return p1.arr_time > p2.arr_time;
        return p1.burst_time > p2.burst_time;
    }
};

struct compare_sht{
    bool operator()(Process &p1, Process &p2){
        return p1.burst_time > p2.burst_time;
    }
};

struct record{
    double curr_time;
    int pid;
    string status;
};

bool comp_at(Process &p1, Process &p2){
        if (p1.arr_time != p2.arr_time)
            return p1.arr_time < p2.arr_time;
        return p1.burst_time < p2.burst_time;
    }

bool time_order(Process &p1, Process &p2){
    if (p1.comp_time == -1 and p2.comp_time == -1){
        return p1.arr_time < p2.arr_time;
    }
    else if (p1.comp_time != -1 and p2.comp_time == -1){
        return p1.comp_time < p2.arr_time;
    }
    else if (p1.comp_time == -1 and p2.comp_time != -1){
        return p1.comp_time < p2.arr_time;
    }
    return p1.arr_time < p2.arr_time;
}

bool rec_order(struct record &r1, struct record &r2){
    if (r1.curr_time != r2.curr_time)
        return r1.curr_time < r2.curr_time;
    else
        return (r1.status == "Exit" ? true:false);
}

void insert(vector<record> &rec, Process &ele, string status, double curr_time){
    struct record r;
    r.curr_time = curr_time;
    r.pid = ele.pid;
    r.status = status;
    rec.pb(r);
}

class Scheduler{
public:
    Process calc(Process &ele){
        ele.turn_time = ele.comp_time - ele.arr_time;
        ele.wait_time = ele.turn_time - ele.burst_time;
        ele.res_time = ele.wait_time;
        return ele;
    }
    void display_process(vector<Process> &order){
        ofstream outfile;
        outfile.open("process.txt", ios_base::out);
        
        sort(order.begin(), order.end(), time_order);

        outfile << "PID\t" << "Arrival time\t" << "Burst time\t" 
        << "Completion time\t" << "Turn around time\t" << "Wait time\t" << "Response time\n";
        for(auto ele: order){
            outfile << fixed << setprecision(3) << ele.pid << "\t" << ele.arr_time << "\t\t" <<
            ele.burst_time << "\t\t" << ele.comp_time << "\t\t" <<
            ele.turn_time << "\t\t\t" << abs(ele.wait_time) << "\t\t" << abs(ele.res_time) << "\n";
        }
        outfile.close();
    }

    void status_helper(vector<Process> &order){
        vector<record> rec;
        double curr_time = order[0].arr_time;
        struct record ele;

        for(auto proc: order){
            ele.pid = proc.pid;
            ele.curr_time = proc.arr_time;
            ele.status = "Arrived";
            rec.pb(ele); // Arrived

            ele.curr_time = max(curr_time, proc.arr_time);
            ele.status = "Running";
            curr_time = proc.comp_time;
            rec.pb(ele); // Running

            ele.curr_time = proc.comp_time;
            ele.status = "Exit";
            rec.pb(ele); // Exit
        }

        sort(rec.begin(), rec.end(), rec_order);
        display_status(rec);
    }

    void display_status(vector<record> &rec){
        ofstream outfile;
        outfile.open("status.txt", ios_base::out);
        
        outfile << "Time \t" << "Process ID \t" << "Status \n";
        for(auto r: rec){
            outfile << r.curr_time << "\t\t" << r.pid << "\t\t" << r.status << "\n";
        }
        outfile.close();
    }

    void FCFS(vector<Process> &rq){
        priority_queue<Process, vector<Process>, compare_at> ready_q;
        forn(i, rq.size())  ready_q.push(rq[i]);
        vector<Process> order;

        double curr_time = 0;

        while (ready_q.size() != 0){
            Process ele = ready_q.top();
            ready_q.pop();
            curr_time = max(0.0, curr_time-ele.arr_time);
            ele.comp_time = ele.arr_time + ele.burst_time + curr_time;
            curr_time = ele.comp_time;
            ele = calc(ele);

            order.pb(ele);
        }
        display_process(order);
        status_helper(order);
    }

    void helper_process(vector<Process> &proc, vector<double> first_t){
        // STRF
        forn(i, proc.size()){
            calc(proc[i]);
            proc[i].res_time = first_t[i+1] - proc[i].arr_time;
        }
    }

    void SRTF(vector<Process> &proc){
        vector<Process> pr = proc;
        sort(pr.begin(), pr.end(), comp_at);
        reverse(pr.begin(), pr.end());

        priority_queue<Process, vector<Process>, compare_sht> ready_q;
        vector<double> first_t(pr.size()+1);
        vector<record> rec;

        forn(i, pr.size())  first_t[i+1] = -1;

        int processed = 0, size = pr.size();
        double curr_time = 0;
        Process ele(0, 0);
        struct record r;
        bool flag = true;
        ready_q.push(pr.back());
        pr.pop_back();

        while (processed != size){
            if (ready_q.size() == 0 and pr.size() != 0 and flag){
                ready_q.push(pr.back());
                pr.pop_back();
                continue;
            }
            else if (flag){
                ele = ready_q.top();
                ready_q.pop();
                if (curr_time < ele.arr_time){
                    curr_time = ele.arr_time;
                    insert(rec, ele, "Arrived", curr_time);
                }                
                insert(rec, ele, "Running", curr_time);
                if(first_t[ele.pid] == -1)  first_t[ele.pid] = curr_time;
                flag = false;
            }

            else if (pr.size() != 0 and pr.back().arr_time <= ele.burst_time + curr_time){
                ele.burst_time -= pr.back().arr_time - curr_time;
                curr_time = pr.back().arr_time;
                insert(rec, pr.back(), "Arrived", curr_time);

                ready_q.push(pr.back());
                pr.pop_back();
                if (ready_q.top().burst_time < ele.burst_time){
                    insert(rec, ele, "Preempted", curr_time);

                    ready_q.push(ele);
                    ele = ready_q.top();
                    cout << ele.pid << "\n";
                    insert(rec, ele, "Running", curr_time);
                    if(first_t[ele.pid] == -1)  first_t[ele.pid] = curr_time;
                    ready_q.pop();
                }
            }

            else{
                curr_time += ele.burst_time;
                processed++;
                proc[ele.pid-1].comp_time = curr_time; 
                insert(rec, ele, "Exit", curr_time);
                ele.comp_time = curr_time;

                if (ready_q.size() != 0){
                    ele = ready_q.top();
                    ready_q.pop();
                    insert(rec, ele, "Running", curr_time);
                    if(first_t[ele.pid] == -1)  first_t[ele.pid] = curr_time;
                }
                else{
                    flag = true;
                }
            }
            
        }
        display_status(rec);
        helper_process(proc, first_t);
        display_process(proc);
    }

    void RR(Process &pr, double tq){}
};


int main(){
    Scheduler sch;
    Process_Creator prc(5);

    sch.SRTF(prc.pr);
    forn(i, 5)  prc.pr[i].print_info();
}