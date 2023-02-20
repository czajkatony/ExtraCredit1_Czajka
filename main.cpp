#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

// This function pulls data on preferences from a text file, name given by user
// Formatting should be the same as 'matching-data.txt' with departments first
// rows represent preference level and columns represent corresponding dep / prog
void GetPrefrences(int** departments, int** programmers, int n,string fileName){
    ifstream myFile;
    myFile.open(fileName);
    if(!myFile){
        cout<<"Error, file could not be opened. Could be naming issue.\n";
        return;
    }
    char numFromFile;
    int** selectedArr = nullptr;
    for(int i=0; i<2*n;i++){
        if(i<n){
            selectedArr = departments;
        }
        else{
            selectedArr = programmers;
        }
        for(int j=0; j<n; j++){
            myFile.get(numFromFile);
            int number = atoi(&numFromFile);
            if(number == 0){
                j--;
                continue;
            }
            else{
                if(selectedArr == programmers){
                    selectedArr[i-5][j] = number;
                }
                else{
                    selectedArr[i][j] = number;
                }
            }
        }
    }
    myFile.close();
}

// This function prints and takes values for the menu so user can enter the
// number of programmers and departments n, as well as the name of the txt file
void printMenu(int &n, string &fileName){
    cout<<"Please enter the number of programmers / departments (must be equal): ";
    cin >> n;
    cout<<"Please enter the filename containing the array of preferences";
    cout<<" (Departments should be first, then Programmers): ";
    cin.ignore(); 
    getline(cin, fileName);
    if(n<=1){
        cout<<"Please enter a new number, values < 2 are not accepted.\n";
        printMenu(n,fileName);
    }
}

// Function checks if programmer prefers its current assigned department or the
// potential new department, returns a bool to reflect that decision
bool ProgPrefersCurrentDep(int** programmers, int progID, int dep, int currentDep, int n){
    for(int i = 0; i<n; i++){
        // returns true if currentDep# is higher on priority list
        if(programmers[i][progID-1] == currentDep+1){
            return true;
        }
        // else false
        if(programmers[i][progID-1] == dep+1){
            return false;
        }
    }
}

// Main matching function, takes in arrays of preferences for both and n
void MatchFunc(int** departments, int** programmers, int n){
    int progDep[n]; // stores the currently assigned department of the programmer
    bool depFilled[n]; // if false, than that department has no programmer 
    for(int i=0; i<n; i++){ // filling the arrays with their default values
        progDep[i] = -1;
        depFilled[i] = false;
    }
    int emptyDepCount = n;
   while(emptyDepCount > 0){ // while there are empty departments
        int dep; // Picking a free department NOTE: this is an index not actual num
        for( dep = 0; dep<n; dep++){
            if(depFilled[dep] == false){
                break; // continue
            }
        }
        // loops through potential programmers if department has none
        for(int i=0; i<n && depFilled[dep] == false; i++){
            int prog = departments[i][dep]; // prog is actual ID number
            if(progDep[prog-1] == -1){ // if potential prog doesn't have a dep
                                       // assign it to dep
                progDep[prog-1] = dep; //progDep is fully based on index
                depFilled[dep] = true;
                emptyDepCount--;
            }
            else{ // if prog isn't free, find current assignment
                int currentDep = progDep[prog-1];
                // checks if prog likes current assignment better than this potential one
                // If it does, then nothing happens and next potential programmer is checked
                // If it likes this one better, than it gets assigned to it and old dep
                // is flagged as empty
                if(ProgPrefersCurrentDep(programmers, prog, dep, currentDep, n) == false){
                    progDep[prog-1] = dep;
                    depFilled[dep] = true;
                    depFilled[currentDep] = false;
                }
            }
        }
   }//end of while loop, only ends once all departments are full

    // Print the pairs for user to see
    for(int i = 0; i<n; i++){
        for(int j=0; j<n; j++){
            if(progDep[j] == i){
                cout<<"Department: "<<i+1<<" will get Programmer: "<<j+1<<endl;
            }
        }
    }
}

int main(){
    int n;
    string fileName;
    printMenu(n, fileName);

    int** programmers = new int*[n];
    int** departments = new int*[n];
    for(int i = 0; i<n; i++){
        programmers[i] = new int[n];
        departments[i] = new int[n];
    }
    
    GetPrefrences(departments, programmers, n, fileName);
    MatchFunc(departments, programmers, n);

    // memory management
    for(int i = 0; i<n; i++){
        delete[] programmers[i];
        delete[] departments[i];
    }
    delete[] programmers;
    delete[] departments;
}
