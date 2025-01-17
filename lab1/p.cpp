#include<bits/stdc++.h> 
using namespace std; 
  
// An interval has start time and end time 
struct Interval 
{ 
    int start, end; 
}; 
  
// Compares two intervals according to staring times. 
bool compareInterval(Interval i1, Interval i2) 
{ 
    return (i1.start > i2.start); 
} 
  
int main() 
{ 
    Interval arr[] =  { {6,8}, {1,9}, {2,4}, {4,7},{4,3}}; 
    int n = sizeof(arr)/sizeof(arr[0]); 
  
    // sort the intervals in increasing order of 
    // start time 
    sort(arr, arr+n, compareInterval); 
  
    cout << "Intervals sorted by start time : \n"; 
    for (int i=0; i<n; i++) 
       cout << "[" << arr[i].start<< "," << arr[i].end<< "] "; 
}
  