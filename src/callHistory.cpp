#include "callHistory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

CallRecord::CallRecord(const string &contactName, const string &phoneNumber, const string &callTime, int callDuration)
    : contactName(contactName), phoneNumber(phoneNumber), callTime(callTime), callDuration(callDuration) {}

void CallHistory::addCallRecord(const string &contactName, const string &phoneNumber, const string &callTime, int callDuration)
{
    string truncatedName = contactName.substr(0, 20); // Truncate name to 20 characters
    string truncatedPhoneNumber = phoneNumber.substr(0, 15); // Truncate phone number to 15 characters

    callRecords.push_back(CallRecord(truncatedName, truncatedPhoneNumber, callTime, callDuration));
}

void CallHistory::displayMostCalls()
{
    cout << "Most Calls:\n";
    cout << "Name\t\tPhone Number\t\tCall Time\t\tCall Duration\n";
    cout << "-------------------------------------------------------------\n";

    // Use a max heap to get the most calls
    priority_queue<CallRecord> maxHeap;
    for (const auto &record : callRecords)
    {
        maxHeap.push(record);
    }

    while (!maxHeap.empty())
    {
        auto record = maxHeap.top();
        maxHeap.pop();
        cout << record.contactName << "\t\t" << record.phoneNumber << "\t\t" << record.callTime << "\t\t" << record.callDuration << " mins\n";
    }
}

void CallHistory::displayRecentCalls()
{
    cout << "Recent Calls:\n";
    cout << "Name\t\tPhone Number\t\tCall Time\t\tCall Duration\n";
    cout << "-------------------------------------------------------------\n";

    // Use a min heap to get the most recent calls
    priority_queue<CallRecord, vector<CallRecord>, greater<CallRecord>> minHeap;
    for (const auto &record : callRecords)
    {
        minHeap.push(record);
    }

    while (!minHeap.empty())
    {
        auto record = minHeap.top();
        minHeap.pop();
        cout << record.contactName << "\t\t" << record.phoneNumber << "\t\t" << record.callTime << "\t\t" << record.callDuration << " mins\n";
    }
}

void CallHistory::displayLongestCalls()
{
    cout << "Longest Calls:\n";
    cout << "Name\t\tPhone Number\t\tCall Time\t\tCall Duration\n";
    cout << "-------------------------------------------------------------\n";

    // Use a max heap to get the longest calls
    priority_queue<CallRecord> maxHeap;
    for (const auto &record : callRecords)
    {
        maxHeap.push(record);
    }

    while (!maxHeap.empty())
    {
        auto record = maxHeap.top();
        maxHeap.pop();
        cout << record.contactName << "\t\t" << record.phoneNumber << "\t\t" << record.callTime << "\t\t" << record.callDuration << " mins\n";
    }
}

void CallHistory::loadCallHistory()
{
    ifstream file("callHistory.csv");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string contactName, phoneNumber, callTime;
            int callDuration;

            getline(ss, contactName, ',');
            getline(ss, phoneNumber, ',');
            getline(ss, callTime, ',');
            ss >> callDuration;

            addCallRecord(contactName, phoneNumber, callTime, callDuration);
        }
        file.close();
    }
}

void CallHistory::saveCallHistory()
{
    ofstream file("callHistory.csv");
    if (file.is_open())
    {
        for (const auto &record : callRecords)
        {
            file << record.contactName << "," << record.phoneNumber << "," << record.callTime << "," << record.callDuration << "\n";
        }
        file.close();
    }
}