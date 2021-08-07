using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

interface IComparitorStrategy<T> {  int Execute(T a, T b);}
class IntComparitor : IComparitorStrategy<int> {
        public int Execute(int a, int b) {  return a > b ? 1 : (b > a ) ? -1 : 0;  }
}
class DoubleComparitor : IComparitorStrategy<double>{
        public int Execute(double a, double b){    return a > b ? 1 : (b > a) ? -1 : 0;}
}  
 static class Program{
        private static void BSort<T>(this T[] arr, IComparitorStrategy<T> test) where T : struct{
            int n = arr.Length;
              for(int i = 0; i<n; ++i)
                for (int j = 0; j < n-i-1; j++)
                   if (test.Execute(arr[j],arr[j + 1]) > 0)   {     T temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp;}
        }
       static void Main(string[] args) {
            if ( args.Length == 0 )    {  Console.WriteLine("No Command Line ARguments");  return; }
            int [] arr = new int[args.Length];
            for( int i=0; i< arr.Length ; ++i )
                 arr[i] = Convert.ToInt32(args[i]);
            arr.BSort(new IntComparitor ());
            foreach( var n2 in arr )  { Console.WriteLine(n2); }
        }
    }
