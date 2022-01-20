#include <bits/stdc++.h>
using namespace std;
#define forn(x, n) for(int x=0; x<n; x++)
#define pb push_back
#include "header/Process.h"


int Process::pi = 1;
int main(){
    Process_Creator prc(5);
    forn(i, 5)  prc.pr[i].print_info();
}
