using System;

using System.Windows;
using System.Reactive;
using System.Reactive.Linq;
using System.Reactive.Disposables;
using System.Reactive.Subjects;

namespace FRP_Solutions
{
    class Program
    {

        
        static void Main(string[] args)
        {
              //---------------------- This will print Even numbers to the Console
              var evenNumbers = Observable.Range(0, 10).Where(i => i % 2 == 0).Subscribe(Console.WriteLine);

            var result =
	from i in Observable.Range(1, 100)
	from j in Observable.Range(1, 100)
	from k in Observable.Range(1, 100)
		where k * k == i * i + j * j
		select new { a = i, b = j, c = k };
            // A Subscriber with
            // A callback (Lambda) which prints value,
            // A callback for Exception
            // A callback for Completion

            IDisposable subscription = result.Subscribe(
         		   x => Console.WriteLine("OnNext: {0}", x),
        	                    ex => Console.WriteLine("OnError: {0}", ex.Message),
         		   () => Console.WriteLine("OnCompleted"));
        }
    }
}