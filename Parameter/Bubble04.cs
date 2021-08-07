using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

static class Program{
       private static void BSort2<T>(this T[] arr, Func<T,T,int> test) where T : struct {
            int n = arr.Length;
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n - i - 1; j++)
                    if (test(arr[j], arr[j + 1]) > 0)  {T temp = arr[j]; arr[j] = arr[j + 1];  arr[j + 1] = temp;}
        }
        static void Main(string[] args){
            if ( args.Length == 0 ){Console.WriteLine("No Command Line ARguments");  return;  }
            int [] arr = new int[args.Length];
            for( int i=0; i< arr.Length ; ++i ) {arr[i] = Convert.ToInt32(args[i]); }
            Func<int ,int ,int> fn = (int a, int b ) => { return (a > b) ? 1 : -1; };
            arr.BSort2(fn);
            foreach( var n2 in arr ) {  Console.WriteLine(n2);}
        } //Main
 }//Program
