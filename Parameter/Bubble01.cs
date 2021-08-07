using System;
using System.Collections.Generic;
 
static class Program{
        static void Main(string[] args){
         //---- Check whether there is any Command Line argument
        if ( args.Length == 0 ) {  Console.WriteLine("No Command Line ARguments");    return; }
       //------ Allocate Array
        int [] arr = new int[args.Length];
        for( int i=0; i< arr.Length ; ++i )
                 arr[i] = Convert.ToInt32(args[i]);
              //---------- A Bubble Sort Routine
              int n = arr.Length;
              for(int i = 0; i<n; ++i)
                for (int j = 0; j < n-i-1; j++)
                   if (arr[j]>arr[j + 1]){    int temp = arr[j];  arr[j] = arr[j + 1];  arr[j + 1] = temp;}
            //-------------------- Spit the Output
            foreach( var n2 in arr )  { Console.WriteLine(n2); }
    } // Main
} // PRogram