/*Drzewa BST - szablon map 2017 Iwona Cieslik*/

#include <iostream>
#include <string>
using namespace std;

#include "solution.h"

///////////////////////////////////////////////////////////////////////////

int main() 
{
   ios_base::sync_with_stdio(false);      
   int z, n;
   string coupon, coupon2, name;
   map<string, string> T;

   cin >> z;
   while(z--) 
   {
      cin >> n;
      for (int i=0; i<n; ++i) 
	  {
	    string op;
  	    cin >> op;
  	     
        if (op == "INSERT") 
		{
           cin >> coupon >> name;	           
		   T[coupon] = name;      	           
        }

        if (op == "FIND") 
		{
	        cin >> coupon;
	        iteratorM<string, string> it = T.find(coupon);
	        cout << "FIND ";	           
	        if (it) cout << it->GetKey() << " " << it->GetName() << endl; else cout << "ERROR" << endl;	           
        }

        if (op == "DELETE") 
		{
            cin >> coupon;
            if (T.erase(coupon)) cout << "OK" << endl; else cout << "ERROR" << endl;
	    }
       
        if (op == "PRINT") T.print();     
	    if (op == "SIZE") cout << T.size() << endl;
        if (op == "FIRST") 
		{
	   	    iteratorM<string, string> it = T.begin();
	   	    if (it) cout << *it; else cout << "EMPTY" << endl;
	    }
        if (op == "LAST") {
	   	    iteratorM<string, string> it = T.end();
	   	    if (it) cout << *it; else cout << "EMPTY" << endl;
	    }       
     }    //for
       
     T.clean();
     }  
   return 0;     
   }



