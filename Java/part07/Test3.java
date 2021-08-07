
 
import io.reactivex.Observable;
import io.reactivex.functions.Consumer;
 
public class Test3
{
    public static void main(String[] args)
    {  
        //producer
        Observable<String> observable = Observable.just("how", "to", "do", "in", "java");
 
        //consumer
        Consumer<? super String> consumer = System.out::println;
 
        //Transformation using map() method
	//observable.map(w -> w.toUpperCase()).subscribe(consumer);
        observable.map(w -> w.toUpperCase().hashCode()).subscribe(consumer);
        
    }
}