using System;
using System.Collections.Generic;

interface IComparitorStrategy {  int Execute(Object a, Object b); }
class IntComparitor : IComparitorStrategy {
        public int Execute(Object a, Object b) { int  pa  = (int)a ;   int pb  =(int) b ; return pa > pb ? 1 : (pb > pa ) ? -1 : 0; }
}
static class Program{
         private static void BSort(this int[] arr, IComparitorStrategy test) {
            int n = arr.Length;
              for(int i = 0; i<n; ++i)
                for (int j = 0; j < n-i-1; j++)
                   if (test.Execute(arr[j],arr[j + 1]) > 0) {  int  temp = arr[j]; arr[j] = arr[j + 1];  arr[j + 1] = temp;  }
        }

      static void Main(string[] args){
         //---- Check whether there is any Command Line argument
        if ( args.Length == 0 ) {  Console.WriteLine("No Command Line ARguments");    return; }
       //------ Allocate Array
        int [] arr = new int[args.Length];
        for( int i=0; i< arr.Length ; ++i  ) {  arr[i] = Convert.ToInt32(args[i]); }
         //---------- A Bubble Sort Routine
         arr.BSort(new IntComparitor ());
         //-------------------- Spit the Output
          foreach( var n2 in arr )  { Console.WriteLine(n2); }
    } // Main
} // PRogram