
import java.util.concurrent.CountDownLatch;

public class Helpers {

    default <T> Consumer<? super T> logNext() {
        return (Consumer<T>) val -> System.out.println("Subscriber received: {}", val);
    }

    default <T> Consumer<? super T> logNextAndSlowByMillis(int millis) {
        return (Consumer<T>) val -> {
           // log.info("Subscriber received: {}", val);
            Helpers.sleepMillis(millis);
        };
    }

    default Consumer<? super Throwable> logError() {
        return err -> log.error("Subscriber received error '{}'", err.getMessage());
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
    public static void sleepMillis(int millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            System.out.println("Interrupted Thread");
            throw new RuntimeException("Interrupted thread");
        }
    }

    public static void wait(CountDownLatch waitOn) {
        try {
            waitOn.await();
        } catch (InterruptedException e) {
            System.out.println("Interrupted waiting on CountDownLatch");
            throw new RuntimeException("Interrupted thread");
        }
    }

}