#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

class Process {
public:
    int pid;  // Process ID
    int bt;   // Burst Time
    int art;  // Arrival Time
    int start_time; // Start time when the process begins execution
    int completion_time; // Completion time when the process finishes execution
    int waiting_time; // Waiting time for the process
    int turnaround_time; // Turnaround time for the process

    Process(int pid, int bt, int art) {
        this->pid = pid;
        this->bt = bt;
        this->art = art;
    }
};

void findFCFS(std::vector<Process>& procs) {
    int time = 0;
    for (Process& p : procs) {
        if (time < p.art)
            time = p.art;
        p.start_time = time;
        p.completion_time = p.start_time + p.bt;
        p.waiting_time = p.start_time - p.art;
        p.turnaround_time = p.completion_time - p.art;
        time = p.completion_time;
    }
}

void findSRTF(std::vector<Process>& procs) {
    int n = procs.size();
    std::vector<int> rt(n);
    for (int i = 0; i < n; i++)
        rt[i] = procs[i].bt;

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    bool check = false;

    while (complete != n) {
        for (int j = 0; j < n; j++) {
            if ((procs[j].art <= t) && (rt[j] < minm) && rt[j] > 0) {
                minm = rt[j];
                shortest = j;
                check = true;
            }
        }

        if (!check) {
            t++;
            continue;
        }

        rt[shortest]--;
        minm = rt[shortest] > 0 ? rt[shortest] : INT_MAX;

        if (rt[shortest] == 0) {
            complete++;
            check = false;
            finish_time = t + 1;
            procs[shortest].completion_time = finish_time;
            procs[shortest].turnaround_time = finish_time - procs[shortest].art;
            procs[shortest].waiting_time = procs[shortest].turnaround_time - procs[shortest].bt;
        }
        t++;
    }
}

void findRR(std::vector<Process>& procs, int quantum) {
    int n = procs.size();
    std::queue<int> q;
    std::vector<int> rt(n);
    std::vector<bool> is_in_queue(n, false);

    for (int i = 0; i < n; i++)
        rt[i] = procs[i].bt;

    int t = 0, current;

    while (true) {
        bool done = true;
        for (int i = 0; i < n; i++) {
            if (rt[i] > 0) {
                done = false;
                if (procs[i].art <= t && !is_in_queue[i]) {
                    q.push(i);
                    is_in_queue[i] = true;
                }
            }
        }
        if (done) break;

        if (!q.empty()) {
            current = q.front();
            q.pop();
            is_in_queue[current] = false;
            int slice = std::min(rt[current], quantum);
            t += slice;
            rt[current] -= slice;
            if (rt[current] == 0) {
                procs[current].completion_time = t;
                procs[current].turnaround_time = t - procs[current].art;
                procs[current].waiting_time = procs[current].turnaround_time - procs[current].bt;
            }
        }
        else {
            t++;
        }
    }
}

void printResults(const std::vector<Process>& procs) {
    std::cout << "PID\tBT\tWT\tTAT\n";
    float total_wt = 0, total_tat = 0;
    for (const Process& p : procs) {
        total_wt += p.waiting_time;
        total_tat += p.turnaround_time;
        std::cout << p.pid << "\t" << p.bt << "\t" << p.waiting_time << "\t" << p.turnaround_time << "\n";
    }
    std::cout << "\nAverage waiting time = " << total_wt / procs.size() << "\n";
    std::cout << "Average turnaround time = " << total_tat / procs.size() << "\n";
}

int main() {
    std::vector<Process> procs = {
        Process(1, 10, 0),
        Process(2, 1, 2),
        Process(3, 2, 4),
        Process(4, 3, 1),
        Process(5, 4, 3)
    };
    int quantum = 4;

    std::cout << "FCFS Scheduling:\n";
    findFCFS(procs);
    printResults(procs);

    std::cout << "\nSRTF Scheduling:\n";
    findSRTF(procs);
    printResults(procs);

    std::cout << "\nRound Robin Scheduling:\n";
    findRR(procs, quantum);
    printResults(procs);

    return 0;
}
