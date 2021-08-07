
 
import io.reactivex.Observable;
import io.reactivex.functions.Consumer;
import io.reactivex.*;
import io.reactivex.functions.Action;
import io.reactivex.Observable;
import io.reactivex.Observer;
import io.reactivex.observers.*;
import io.reactivex.disposables.*;
import io.reactivex.schedulers.Schedulers;
import java.util.*;

import java.util.concurrent.CompletableFuture;

import java.util.concurrent.CountDownLatch;

import io.reactivex.BackpressureStrategy;
import io.reactivex.Flowable;
import io.reactivex.Observable;
import io.reactivex.Single;
import io.reactivex.functions.Action;
import io.reactivex.functions.Consumer;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;



interface BaseEventHandler
{
	default <T> Consumer<? super T> logNext() {
        return (Consumer<T>) val -> System.out.println("Subscriber received: {}" + val);
    }

    default <T> Consumer<? super T> logNextAndSlowByMillis(int millis) {
        return (Consumer<T>) val -> {
           // log.info("Subscriber received: {}", val);
            Helpers.sleepMillis(millis);
        };
    }

    default Consumer<? super Throwable> logError() {
        return err -> System.out.println("Subscriber received error '{}'"+ err.getMessage());
    }

    default Consumer<? super Throwable> logError(CountDownLatch latch) {
        return err -> {
            System.out.println("Subscriber received error '{}'"+ err.getMessage());
            latch.countDown();
        };
    }

    default Action logComplete() {
        return () -> System.out.println("Subscriber got Completed event");
    }

    default Action logComplete(CountDownLatch latch) {
        return () -> {
           // log.info("Subscriber got Completed event");
            latch.countDown();
        };
    }


    default <T> void subscribeWithLog(Flowable<T> flowable) {
        flowable.subscribe(
                logNext(),
                logError(),
                logComplete()
        );
    }

    default <T> void subscribeWithLog(Observable<T> observable) {
        observable.subscribe(
                logNext(),
                logError(),
                logComplete()
        );
    }

    default <T> void subscribeWithLogOutputWaitingForComplete(Observable<T> observable) {
        CountDownLatch latch = new CountDownLatch(1);

        observable.subscribe(
                logNext(),
                logError(latch),
                logComplete(latch)
        );

        Helpers.Wait(latch);
    }

    default <T> void subscribeWithLog(Single<T> single) {
        single.subscribe(
                val -> System.out.println("Subscriber received: {}" + val),
                logError()
        );
    }

    default <T> void subscribeWithLogOutputWaitingForComplete(Flowable<T> flowable) {
        CountDownLatch latch = new CountDownLatch(1);

        flowable.subscribe(
                logNext(),
                logError(latch),
                logComplete(latch)
        );

        Helpers.Wait(latch);
    }

    default <T> void subscribeWithLogOutputWaitingForComplete(Single<T> single) {
        CountDownLatch latch = new CountDownLatch(1);

        single.subscribe(
                val -> {
                    System.out.println("Subscriber received: {} and completed" + val);
                    latch.countDown();
                },
                logError(latch)
        );

        Helpers.Wait(latch);
    }

      default  <T> Flowable<T> periodicEmitter(T t1, T t2, T t3, int interval, TimeUnit unit) {
        return periodicEmitter(t1, t2, t3, interval, unit, interval);
    }

    default  <T> Flowable<T> periodicEmitter(T t1, T t2, T t3, int interval,
                                               TimeUnit unit, int initialDelay) {
        Flowable<T> itemsStream = Flowable.just(t1, t2, t3);
        Flowable<Long> timer = Flowable.interval(initialDelay, interval, unit);

        return Flowable.zip(itemsStream, timer, (key, val) -> key);
    }

    default  <T> Observable<T> periodicEmitter(T[] items, int interval,
                                               TimeUnit unit, int initialDelay) {
        Observable<T> itemsStream = Observable.fromArray(items);
        Observable<Long> timer = Observable.interval(initialDelay, interval, unit);

        return Observable.zip(itemsStream, timer, (key, val) -> key);
    }

    default  <T> Observable<T> periodicEmitter(T[] items, int interval,
                                               TimeUnit unit) {
        return periodicEmitter(items, interval, unit);
    }

    default  Flowable<String> delayedByLengthEmitter(TimeUnit unit, String...items) {
        Flowable<String> itemsStream = Flowable.fromArray(items);

        return itemsStream.concatMap(item -> Flowable.just(item)
                        .doOnNext(val -> System.out.println("Received {} delaying for {} " + val + " " + val.length()))
                        .delay(item.length(), unit)
                );
    }


}

class Helpers {

    
    public static void sleepMillis(int millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            System.out.println("Interrupted Thread");
            throw new RuntimeException("Interrupted thread");
        }
    }

    public static void Wait(CountDownLatch waitOn) {
        try {
            waitOn.await();
        } catch (InterruptedException e) {
            System.out.println("Interrupted waiting on CountDownLatch");
            throw new RuntimeException("Interrupted thread");
        }
    }

}

public class Test5
{
    public static void main(String[] args)
    {  
        Flowable<Integer> flowable = Flowable.just(1, 5, 10);

        flowable.subscribe(
                val -> System.out.println(val*val));

        CompletableFuture<String> completableFuture = CompletableFuture.
                supplyAsync(() -> { //starts a background thread the ForkJoin common pool
                      Helpers.sleepMillis(100);
                      return "red";
                });

        Single<String> single = Single.fromFuture(completableFuture);
        single.subscribe(val -> System.out.println("Stream completed successfully : {}"+ val));
        
    }
}