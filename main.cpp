#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;


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

bool ProgPrefersCurrentDep(int** programmers, int progID, int dep, int currentDep, int n){
    for(int i = 0; i<n; i++){
        if(programmers[i][progID-1] == currentDep+1){
            cout << "Progr Prefers Current: TRUE\n";
            return true;
        }
        if(programmers[i][progID-1] == dep+1){
            cout << "Progr Prefers Current: FALSE\n";
            return false;
        }
    }
}

void MatchFunc(int** departments, int** programmers, int n){
    int progDep[n]; // stores the department of the programmer
    bool depFilled[n]; // if false, than that department is free
    for(int i=0; i<n; i++){
        progDep[i] = -1;
        depFilled[i] = false;
    }
    int emptyDepCount = n;
   while(emptyDepCount > 0){ // while there are free departments
        // cout<<"EmptyDepCount: "<<emptyDepCount<<endl;
        int dep; // Picking a free department NOTE: this is an index not actual num
        for( dep = 0; dep<n; dep++){
            if(depFilled[dep] == false){
                cout<<"Department Num: "<<dep+1<<endl;
                break; // continue
            }
        }
        // cout<<"Out of loop"<<endl;

        for(int i=0; i<n && depFilled[dep] == false; i++){
            cout<<"I value: "<< i<<endl;
            int prog = departments[i][dep]; // prog is actual ID number
            cout<<"ProgrammerID: "<<prog<<endl;
            if(progDep[prog-1] == -1){
                progDep[prog-1] = dep; //progDep is fully based on index
                depFilled[dep] = true;
                cout<<"Set dep Num: "<<dep+1<<" to prog: "<<prog<<endl;
                emptyDepCount--;
            }
            else{ // if prog isn't free, find current assignment
                int currentDep = progDep[prog-1];
                if(ProgPrefersCurrentDep(programmers, prog, dep, currentDep, n) == false){
                    cout<<"Set dep Num: "<<dep+1<<" to prog: "<<prog<<endl;
                    cout<<"Dep: "<<currentDep<< "no longer has a programmer."<<endl;
                    progDep[prog-1] = dep;
                    depFilled[dep] = true;
                    depFilled[currentDep] = false;
                }
            }
        }
        cout<<endl;
   }//end of while loop

    // Print the pairs
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
    
    GetPrefrences(departments, programmers, n, "matching-data.txt");
    for(int i = 0; i<n; i++){
        for(int j=0; j<n; j++){
            cout<<departments[i][j];
        }
        cout<<endl;
    }
    MatchFunc(departments, programmers, n);

}
