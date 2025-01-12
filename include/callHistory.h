#ifndef CALLHISTORY_H
#define CALLHISTORY_H

#include <string>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

struct CallRecord
{
    string contactName;
    string phoneNumber;
    string callTime;
    int callDuration;

    CallRecord(const string &contactName, const string &phoneNumber, const string &callTime, int callDuration);

    // Overload the < and > operators for CallRecord
    bool operator<(const CallRecord &other) const {
        return callDuration < other.callDuration;
    }

    bool operator>(const CallRecord &other) const {
        return callDuration > other.callDuration;
    }
};

class CallHistory
{
public:
    vector<CallRecord> callRecords;

public:
    void addCallRecord(const string &contactName, const string &phoneNumber, const string &callTime, int callDuration);
    void displayMostCalls();
    void displayRecentCalls();
    void displayLongestCalls();
    void loadCallHistory();
    void saveCallHistory();
};

#endif // CALLHISTORY_H