#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
using namespace std;

struct Call {
    string customerID;
    int callTime;
    time_t arrivalTime;
    Call(string id, int t) : customerID(id), callTime(t) {
        arrivalTime = time(0);
    }
};

class Operator {
public:
    string name;
    queue<Call> q;
    vector<Call> callLog;

    Operator(string n) : name(n) {}

    void addCall(const Call& c) {
        q.push(c);
    }

    Call answerCall() {
        if (q.empty()) return Call("", 0);
        Call c = q.front();
        q.pop();
        callLog.push_back(c);
        return c;
    }

    vector<Call> viewQueue() {
        vector<Call> v;
        queue<Call> temp = q;
        while (!temp.empty()) {
            v.push_back(temp.front());
            temp.pop();
        }
        return v;
    }

    bool isQueueEmpty() {
        return q.empty();
    }

    int totalQueuedTime() {
        int sum = 0;
        queue<Call> temp = q;
        while (!temp.empty()) {
            sum += temp.front().callTime;
            temp.pop();
        }
        return sum;
    }

    double averageWaitingTime() {
        if (q.empty()) return 0;
        time_t now = time(0);
        double sum = 0;
        queue<Call> temp = q;
        while (!temp.empty()) {
            sum += difftime(now, temp.front().arrivalTime);
            temp.pop();
        }
        return sum / q.size();
    }

    Call longestWaitingCall() {
        if (q.empty()) return Call("", 0);
        queue<Call> temp = q;
        Call longest = temp.front();
        double maxWait = difftime(time(0), longest.arrivalTime);
        while (!temp.empty()) {
            double wait = difftime(time(0), temp.front().arrivalTime);
            if (wait > maxWait) {
                longest = temp.front();
                maxWait = wait;
            }
            temp.pop();
        }
        return longest;
    }
};

class CallCenter {
public:
    vector<Operator> operators;
    CallCenter(vector<string> names) {
        for (auto &n : names) operators.push_back(Operator(n));
    }

    void addCall(string customerID, int callTime) {
        int idx = 0;
        int minTime = operators[0].totalQueuedTime();
        for (int i = 1; i < operators.size(); i++) {
            if (operators[i].totalQueuedTime() < minTime) {
                minTime = operators[i].totalQueuedTime();
                idx = i;
            }
        }
        operators[idx].addCall(Call(customerID, callTime));
    }

    Call answerCall(string operatorName) {
        for (auto &op : operators) {
            if (op.name == operatorName) return op.answerCall();
        }
        return Call("",0);
    }

    void viewQueue(string operatorName) {
        for (auto &op : operators) {
            if (op.name == operatorName) {
                vector<Call> v = op.viewQueue();
                cout << "Queue for " << operatorName << ":\n";
                for (auto &c : v)
                    cout << "CustomerID: " << c.customerID << ", CallTime: " << c.callTime << " mins\n";
            }
        }
    }

    void operatorCallLogs() {
        for (auto &op : operators) {
            cout << "Call Log for " << op.name << ":\n";
            for (auto &c : op.callLog)
                cout << "CustomerID: " << c.customerID << ", CallTime: " << c.callTime << " mins\n";
        }
    }

    void operatorAverageWaitingTime() {
        cout << fixed << setprecision(2);
        for (auto &op : operators)
            cout << "Average Waiting Time for " << op.name << ": " << op.averageWaitingTime() << " seconds\n";
    }

    void operatorNextAvailableTime() {
        for (auto &op : operators)
            cout << "Next Available Time for " << op.name << ": " << op.totalQueuedTime() << " mins\n";
    }

    void viewLongestWaitingCallPerOperator() {
        for (auto &op : operators) {
            Call c = op.longestWaitingCall();
            if (c.customerID != "")
                cout << "Longest Waiting Call for " << op.name << ": "
                     << "CustomerID: " << c.customerID
                     << ", CallTime: " << c.callTime
                     << " mins, Waiting: " << difftime(time(0), c.arrivalTime) << " seconds\n";
        }
    }
};

int main() {
    CallCenter cc({"Arpita", "Rohit", "Neha"});
    cc.addCall("C001", 5);
    cc.addCall("C002", 3);
    cc.addCall("C003", 7);
    cc.addCall("C004", 4);

    cc.viewQueue("Arpita");
    cc.viewQueue("Rohit");
    cc.viewQueue("Neha");

    cc.answerCall("Arpita");
    cc.operatorCallLogs();
    cc.operatorAverageWaitingTime();
    cc.operatorNextAvailableTime();
    cc.viewLongestWaitingCallPerOperator();

    return 0;
}
