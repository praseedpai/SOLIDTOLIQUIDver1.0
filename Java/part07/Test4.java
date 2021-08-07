
 
import io.reactivex.Observable;
import io.reactivex.functions.Consumer;
import io.reactivex.*;
import io.reactivex.functions.Action;
import io.reactivex.Observable;
import io.reactivex.Observer;
import io.reactivex.observers.*;
import io.reactivex.disposables.*;
import io.reactivex.schedulers.Schedulers;
 
public class Test4
{
    public static void main(String[] args)
    {  
        Disposable d =
     Observable.range(1, 5)
     .subscribeWith(new DisposableObserver<Integer>() {
         @Override public void onStart() {
             System.out.println("Start!");
         }
         @Override public void onNext(Integer t) {
             if (t == 3) {
                 dispose();
             }
             System.out.println(t);
         }
         @Override public void onError(Throwable t) {
             t.printStackTrace();
         }
         @Override public void onComplete() {
             System.out.println("Done!");
         }
     });
 // ...
 d.dispose();
 
        
    }
}