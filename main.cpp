#include <windows.h>
#include "Phonebook.h"
#include "callHistory.h"
#include <string>
#include <sstream>

Phonebook phonebook;
CallHistory callHistory;

// Constants for Control IDs
#define ID_BUTTON_ADD 101
#define ID_BUTTON_DELETE 102
#define ID_BUTTON_SEARCH 103
#define ID_BUTTON_PREFIX_SEARCH 104
#define ID_BUTTON_DISPLAY 105
#define ID_BUTTON_MOST_CALLS 106
#define ID_BUTTON_RECENT_CALLS 107
#define ID_BUTTON_LONGEST_CALLS 108
#define ID_EDIT_NAME 109
#define ID_EDIT_NUMBER 110
#define ID_EDIT_EMAIL 111
#define ID_EDIT_PREFIX 112

// Function declarations
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void AddContact(HWND hwnd);
void DeleteContact(HWND hwnd);
void DisplayContacts(HWND hwnd);
void SearchContact(HWND hwnd);
void PrefixSearch(HWND hwnd);
void DisplayMostCalls(HWND hwnd);
void DisplayRecentCalls(HWND hwnd);
void DisplayLongestCalls(HWND hwnd);

// Entry point of application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Load saved data
    phonebook.loadContacts();
    callHistory.loadCallHistory();

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"PhoneBookApp";
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassW(&wc))
    {
        MessageBoxW(NULL, L"Failed to register window class.", L"Error", MB_ICONERROR);
        return 1;
    }

    HWND hwnd = CreateWindowExW(
        0,
        L"PhoneBookApp",
        L"Phonebook Application",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL);

    if (!hwnd)
    {
        MessageBoxW(NULL, L"Failed to create the main window.", L"Error", MB_ICONERROR);
        return 1;
    }

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hNameEdit, hNumberEdit, hEmailEdit, hPrefixEdit;
    static HWND hAddButton, hDeleteButton, hSearchButton, hPrefixSearchButton;
    static HWND hDisplayButton, hMostCallsButton, hRecentCallsButton, hLongestCallsButton;
    static HWND hResultBox;

    switch (msg)
    {
    case WM_CREATE:
        // Create static labels
        CreateWindowW(L"STATIC", L"Name:", WS_VISIBLE | WS_CHILD, 20, 20, 80, 20, hwnd, NULL, NULL, NULL);
        CreateWindowW(L"STATIC", L"Number:", WS_VISIBLE | WS_CHILD, 20, 50, 80, 20, hwnd, NULL, NULL, NULL);
        CreateWindowW(L"STATIC", L"Email:", WS_VISIBLE | WS_CHILD, 20, 80, 80, 20, hwnd, NULL, NULL, NULL);
        CreateWindowW(L"STATIC", L"Prefix:", WS_VISIBLE | WS_CHILD, 20, 110, 80, 20, hwnd, NULL, NULL, NULL);

        // Create edit controls for input
        hNameEdit = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 20, 150, 20, hwnd, (HMENU)ID_EDIT_NAME, NULL, NULL);
        hNumberEdit = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 50, 150, 20, hwnd, (HMENU)ID_EDIT_NUMBER, NULL, NULL);
        hEmailEdit = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 80, 150, 20, hwnd, (HMENU)ID_EDIT_EMAIL, NULL, NULL);
        hPrefixEdit = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 110, 150, 20, hwnd, (HMENU)ID_EDIT_PREFIX, NULL, NULL);

        // Create buttons for contacts
        hAddButton = CreateWindowW(L"BUTTON", L"Add Contact", WS_VISIBLE | WS_CHILD, 270, 20, 120, 30, hwnd, (HMENU)ID_BUTTON_ADD, NULL, NULL);
        hDeleteButton = CreateWindowW(L"BUTTON", L"Delete Contact", WS_VISIBLE | WS_CHILD, 270, 60, 120, 30, hwnd, (HMENU)ID_BUTTON_DELETE, NULL, NULL);
        hSearchButton = CreateWindowW(L"BUTTON", L"Search Contact", WS_VISIBLE | WS_CHILD, 270, 100, 120, 30, hwnd, (HMENU)ID_BUTTON_SEARCH, NULL, NULL);
        hPrefixSearchButton = CreateWindowW(L"BUTTON", L"Prefix Search", WS_VISIBLE | WS_CHILD, 270, 140, 120, 30, hwnd, (HMENU)ID_BUTTON_PREFIX_SEARCH, NULL, NULL);
        hDisplayButton = CreateWindowW(L"BUTTON", L"Display All", WS_VISIBLE | WS_CHILD, 400, 20, 120, 30, hwnd, (HMENU)ID_BUTTON_DISPLAY, NULL, NULL);

        // Create buttons for call history
        hMostCallsButton = CreateWindowW(L"BUTTON", L"Most Calls", WS_VISIBLE | WS_CHILD, 400, 60, 120, 30, hwnd, (HMENU)ID_BUTTON_MOST_CALLS, NULL, NULL);
        hRecentCallsButton = CreateWindowW(L"BUTTON", L"Recent Calls", WS_VISIBLE | WS_CHILD, 400, 100, 120, 30, hwnd, (HMENU)ID_BUTTON_RECENT_CALLS, NULL, NULL);
        hLongestCallsButton = CreateWindowW(L"BUTTON", L"Longest Calls", WS_VISIBLE | WS_CHILD, 400, 140, 120, 30, hwnd, (HMENU)ID_BUTTON_LONGEST_CALLS, NULL, NULL);

        // Create a list box for displaying results
        hResultBox = CreateWindowW(L"LISTBOX", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY, 20, 200, 740, 330, hwnd, NULL, NULL, NULL);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_BUTTON_ADD:
            AddContact(hwnd);
            break;
        case ID_BUTTON_DELETE:
            DeleteContact(hwnd);
            break;
        case ID_BUTTON_SEARCH:
            SearchContact(hwnd);
            break;
        case ID_BUTTON_PREFIX_SEARCH:
            PrefixSearch(hwnd);
            break;
        case ID_BUTTON_DISPLAY:
            DisplayContacts(hwnd);
            break;
        case ID_BUTTON_MOST_CALLS:
            DisplayMostCalls(hwnd);
            break;
        case ID_BUTTON_RECENT_CALLS:
            DisplayRecentCalls(hwnd);
            break;
        case ID_BUTTON_LONGEST_CALLS:
            DisplayLongestCalls(hwnd);
            break;
        }
        break;

    case WM_DESTROY:
        // Save data before closing
        phonebook.saveContacts();
        callHistory.saveCallHistory();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

    return 0;
}

// Add Contact
void AddContact(HWND hwnd)
{
    wchar_t name[100], number[100], email[100];
    GetWindowTextW(GetDlgItem(hwnd, ID_EDIT_NAME), name, 100);
    GetWindowTextW(GetDlgItem(hwnd, ID_EDIT_NUMBER), number, 100);
    GetWindowTextW(GetDlgItem(hwnd, ID_EDIT_EMAIL), email, 100);

    if (wcslen(name) == 0 || wcslen(number) == 0)
    {
        MessageBoxW(hwnd, L"Name and Number are required fields.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Convert wide strings to narrow strings
    char nameStr[100], numberStr[100], emailStr[100];
    wcstombs(nameStr, name, 100);
    wcstombs(numberStr, number, 100);
    wcstombs(emailStr, email, 100);

    if (phonebook.isDuplicate(nameStr, numberStr))
    {
        MessageBoxW(hwnd, L"Contact already exists!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    phonebook.addContact(nameStr, numberStr, emailStr);
    MessageBoxW(hwnd, L"Contact added successfully!", L"Success", MB_OK | MB_ICONINFORMATION);
    DisplayContacts(hwnd); // Refresh the display
}

// Delete Contact
void DeleteContact(HWND hwnd)
{
    wchar_t name[100];
    GetWindowTextW(GetDlgItem(hwnd, ID_EDIT_NAME), name, 100);

    if (wcslen(name) == 0)
    {
        MessageBoxW(hwnd, L"Name is required to delete a contact.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    char nameStr[100];
    wcstombs(nameStr, name, 100);

    phonebook.deleteContact(nameStr);
    MessageBoxW(hwnd, L"Contact deleted successfully!", L"Success", MB_OK | MB_ICONINFORMATION);
    DisplayContacts(hwnd); // Refresh the display
}

// Display All Contacts
void DisplayContacts(HWND hwnd)
{
    HWND hResultBox = FindWindowExW(hwnd, NULL, L"LISTBOX", NULL);
    SendMessageW(hResultBox, LB_RESETCONTENT, 0, 0);

    // Add header
    wchar_t header[256];
    _snwprintf(header, 256, L"%-20s %-15s %-30s", L"Name", L"Phone Number", L"Email");
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)header);
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)L"----------------------------------------");

    std::vector<AVLNode*> nodes;
    phonebook.inorderTraversal(phonebook.root, nodes);
    
    for (auto node : nodes)
    {
        wchar_t buffer[256];
        wchar_t nameW[100], numberW[100], emailW[100];
        mbstowcs(nameW, node->contact.name.c_str(), 100);
        mbstowcs(numberW, node->contact.phoneNumber.c_str(), 100);
        mbstowcs(emailW, node->contact.email.c_str(), 100);
        
        _snwprintf(buffer, 256, L"%-20s %-15s %-30s", nameW, numberW, emailW);
        SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)buffer);
    }
}

// Search Contact
void SearchContact(HWND hwnd)
{
    wchar_t name[100];
    GetWindowTextW(GetDlgItem(hwnd, ID_EDIT_NAME), name, 100);

    if (wcslen(name) == 0)
    {
        MessageBoxW(hwnd, L"Enter the name to search.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    char nameStr[100];
    wcstombs(nameStr, name, 100);

    if (phonebook.searchContactbyName(nameStr))
    {
        std::string number = phonebook.getPhoneNumberByName(nameStr);
        wchar_t numberW[100];
        mbstowcs(numberW, number.c_str(), 100);
        
        wchar_t message[256];
        _snwprintf(message, 256, L"Contact found!\nName: %ls\nNumber: %ls", name, numberW);
        MessageBoxW(hwnd, message, L"Success", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxW(hwnd, L"Contact not found!", L"Error", MB_OK | MB_ICONERROR);
    }
}

// Prefix Search
void PrefixSearch(HWND hwnd)
{
    wchar_t prefix[100];
    GetWindowTextW(GetDlgItem(hwnd, ID_EDIT_PREFIX), prefix, 100);

    if (wcslen(prefix) == 0)
    {
        MessageBoxW(hwnd, L"Enter a prefix to search.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    char prefixStr[100];
    wcstombs(prefixStr, prefix, 100);

    HWND hResultBox = FindWindowExW(hwnd, NULL, L"LISTBOX", NULL);
    SendMessageW(hResultBox, LB_RESETCONTENT, 0, 0);

    // Add header
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)L"Prefix Search Results:");
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)L"----------------------------------------");

    std::vector<AVLNode*> nodes;
    phonebook.inorderTraversal(phonebook.root, nodes);
    
    for (auto node : nodes)
    {
        if (node->contact.name.find(prefixStr) == 0)
        {
            wchar_t buffer[256];
            wchar_t nameW[100], numberW[100], emailW[100];
            mbstowcs(nameW, node->contact.name.c_str(), 100);
            mbstowcs(numberW, node->contact.phoneNumber.c_str(), 100);
            mbstowcs(emailW, node->contact.email.c_str(), 100);
            
            _snwprintf(buffer, 256, L"%-20s %-15s %-30s", nameW, numberW, emailW);
            SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)buffer);
        }
    }
}

// Display Most Calls
void DisplayMostCalls(HWND hwnd)
{
    HWND hResultBox = FindWindowExW(hwnd, NULL, L"LISTBOX", NULL);
    SendMessageW(hResultBox, LB_RESETCONTENT, 0, 0);

    // Add header
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)L"Most Calls:");
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)L"Name                Phone Number      Call Time            Duration");
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)L"----------------------------------------------------------------");

    std::priority_queue<CallRecord> maxHeap;
    for (const auto &record : callHistory.callRecords)
    {
        maxHeap.push(record);
    }

    while (!maxHeap.empty())
    {
        auto record = maxHeap.top();
        maxHeap.pop();

        wchar_t buffer[256];
        wchar_t nameW[100], numberW[100], timeW[100];
        mbstowcs(nameW, record.contactName.c_str(), 100);
        mbstowcs(numberW, record.phoneNumber.c_str(), 100);
        mbstowcs(timeW, record.callTime.c_str(), 100);

        _snwprintf(buffer, 256, L"%-20s %-15s %-20s %d mins", 
                nameW, numberW, timeW, record.callDuration);
        SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)buffer);
    }
}

// Display Recent Calls
void DisplayRecentCalls(HWND hwnd)
{
    HWND hResultBox = FindWindowExW(hwnd, NULL, L"LISTBOX", NULL);
    SendMessageW(hResultBox, LB_RESETCONTENT, 0, 0);

    // Add header
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)L"Recent Calls:");
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)L"Name                Phone Number      Call Time            Duration");
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)L"----------------------------------------------------------------");

    std::priority_queue<CallRecord, std::vector<CallRecord>, std::greater<CallRecord>> minHeap;
    for (const auto &record : callHistory.callRecords)
    {
        minHeap.push(record);
    }

    while (!minHeap.empty())
    {
        auto record = minHeap.top();
        minHeap.pop();

        wchar_t buffer[256];
        wchar_t nameW[100], numberW[100], timeW[100];
        mbstowcs(nameW, record.contactName.c_str(), 100);
        mbstowcs(numberW, record.phoneNumber.c_str(), 100);
        mbstowcs(timeW, record.callTime.c_str(), 100);

        _snwprintf(buffer, 256, L"%-20s %-15s %-20s %d mins", 
                nameW, numberW, timeW, record.callDuration);
        SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)buffer);
    }
}

// Display Longest Calls
void DisplayLongestCalls(HWND hwnd)
{
    HWND hResultBox = FindWindowExW(hwnd, NULL, L"LISTBOX", NULL);
    SendMessageW(hResultBox, LB_RESETCONTENT, 0, 0);

    // Add header
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)L"Longest Calls:");
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)L"Name                Phone Number      Call Time            Duration");
    SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)L"----------------------------------------------------------------");

    std::priority_queue<CallRecord> maxHeap;
    for (const auto &record : callHistory.callRecords)
    {
        maxHeap.push(record);
    }

    while (!maxHeap.empty())
    {
        auto record = maxHeap.top();
        maxHeap.pop();

        wchar_t buffer[256];
        wchar_t nameW[100], numberW[100], timeW[100];
        mbstowcs(nameW, record.contactName.c_str(), 100);
        mbstowcs(numberW, record.phoneNumber.c_str(), 100);
        mbstowcs(timeW, record.callTime.c_str(), 100);

        _snwprintf(buffer, 256, L"%-20s %-15s %-20s %d mins", 
                nameW, numberW, timeW, record.callDuration);
        SendMessageW(hResultBox, LB_ADDSTRING, 0, (LPARAM)buffer);
    }
}

// g++ main.cpp -o phonebook_gui.exe -lgdi32