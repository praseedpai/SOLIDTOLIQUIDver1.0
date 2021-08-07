 import java.io.File;
import java.util.*;
import java.util.stream.*;
import java.util.stream.Collectors.*;

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
import io.reactivex.flowables.GroupedFlowable;
import io.reactivex.Observable;
import io.reactivex.Single;
import io.reactivex.functions.Action;
import io.reactivex.functions.Consumer;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;


class FileInformation
{
   public String name;
   public long size;
   public FileInformation( String pname,long psize )
   {
         name = pname;
         size = psize;

   }

   public String toString() {
        return name + " : " + size;            
   }

}

//////////////////
// Base interface for the File/Folder composite
//
//

abstract class EntryNode {
    protected String name;
    protected int isdir;
    protected long fsize;
    public abstract long getSize();
    public abstract boolean isdir();
    public abstract void accept( IFileFolderVisitor ivis );
    
}
////////////////////////////////
// Directory Visitor interface
//
//
interface IFileFolderVisitor
{
    void Visit(FileNode fn );
    void Visit(DirectoryNode dn );

}

//////////////////////////////////////
//
// The Node which represents Files
//
//
class FileNode extends EntryNode {
    
   public  FileNode(String pname, long size) { 
	isdir = 0; name = pname; fsize = size;
   }
   public boolean  isdir() { return isdir == 1; }
   public String getname() { return name; }
   
   public long getSize() {return fsize; }

   public void accept( IFileFolderVisitor ivis ){
       ivis.Visit(this);
    }

}

///////////////////////////////////////
// Node which represents Directory
//
//

class DirectoryNode extends EntryNode {
  ArrayList<EntryNode> files = new ArrayList<EntryNode>();
  public  DirectoryNode(String pname) { 
	isdir = 1; name = pname;
        
  }
  public ArrayList<EntryNode> GetAllFiles() {
       return files;
  }
  public boolean AddFile( String pname , long size) {
       files.add(new FileNode(pname,size));
       return true;
  }

  public boolean AddDirectory( DirectoryNode dn ) {
	files.add(dn);
        return true;
  }
  public boolean  isdir() { return isdir == 1; }
  public String getname() { return name; }
  public void   setname(String pname) { name = pname; }
  public long getSize() {return 0; }
  public void accept( IFileFolderVisitor ivis ){
       ivis.Visit(this);
    }

}


////////////////////////////////////////////
// Visitor interface 
//
//
//

class PrintFolderVisitor implements IFileFolderVisitor
{

    public void Visit(FileNode fn ) {
       System.out.println("  File -> " + fn.getname() +
            "Size -> " + fn.getSize());
    }
    public void Visit(DirectoryNode dn ) {
	System.out.println("Directory -> " + dn.getname() );

        //Traversing list through Iterator 
        ArrayList<EntryNode> list = dn.GetAllFiles(); 
  	Iterator itr=list.iterator();  
 	 while(itr.hasNext()){ 
              EntryNode nt = (EntryNode)itr.next();
              if ( nt.isdir() )
                  Visit((DirectoryNode)nt);
              else
   		( (FileNode) nt).accept(this);  
  	}  
    }
 
}

//////////////////////////////////////
//
// Flatten the File/Folders into a linear list
//
//
class FlattenVisitor implements IFileFolderVisitor
{
    ArrayList<FileInformation> files = new ArrayList<FileInformation>();
    String CurrDir = "";
    public FlattenVisitor() {

 	files = new ArrayList<FileInformation>();
    }

    ArrayList<FileInformation> GetAllFiles() {
         return files;
    }

    public void Visit(FileNode fn ) {
       files.add( 
        new FileInformation(
          CurrDir +"\\" + fn.getname(),fn.getSize()));
       
    }
    public void Visit(DirectoryNode dn ) {
	// System.out.println("Directory -> " + dn.getname() );
        CurrDir = dn.getname();
        //Traversing list through Iterator 
        ArrayList<EntryNode> list = dn.GetAllFiles(); 
  	Iterator itr=list.iterator();  
 	 while(itr.hasNext()){ 
              EntryNode nt = (EntryNode)itr.next();
              if ( nt.isdir() ) {
                  CurrDir = ((DirectoryNode)nt).getname();
                  ((DirectoryNode)nt).accept(this);
              }
              else
   		( (FileNode) nt).accept(this);  
  	}  
    }
 
}

//////////////////////////////////////////
//
//
//

class DirectoryInfo {
   DirectoryNode root = null;
   DirectoryNode addptr = null;
   
   public DirectoryInfo( String rootdir ) {
    
      root =  new DirectoryNode(rootdir);
      
   }
   
   public EntryNode Traverse () {

       EntryNode ns = null;

       File a = new File(root.getname());

       if ( a.isDirectory() )
         ns = visitAllDirsAndFiles(a);

       return ns;

   }

   public static EntryNode visitAllDirsAndFiles(File dir) {

      
      
      if (dir.isDirectory()) {
         
         DirectoryNode ns = new DirectoryNode(dir.getPath());
        // System.out.println(dir.getPath());
         String[] children = dir.list();
         
         for (int i = 0; i < children.length; i++) {
           File temp = new File(dir,children[i]);
           if ( temp.isDirectory() ) {
               EntryNode nst =  visitAllDirsAndFiles(temp);
               ns.AddDirectory((DirectoryNode)nst);
           }
           else {
               ns.AddFile( temp.getName(),temp.length());

           }
         }
         return ns;
      } 
      else {
         System.out.println(" Error    ");
         return null;
      }
   }

}

public class TraverseDirectory02 {

   public static ArrayList<FileInformation> GetAllFiles( String directory)
   {

	DirectoryInfo temp = new DirectoryInfo(directory);
     	EntryNode ns = temp.Traverse();
        FlattenVisitor fs = new FlattenVisitor();
        ((DirectoryNode)ns).accept(fs);
        return fs.GetAllFiles();
   }

   public static ArrayList<FileInformation> GetAllFiles(String dir,String ext)
   {

        return   GetAllFiles(dir).stream().filter(
                  s -> s.name.endsWith(ext)).collect(
Collectors.toCollection(ArrayList<FileInformation>::new));

   }

   public static ArrayList<FileInformation> GetAllFilesLessThan(String dir,long sizemax)
   {

        return   GetAllFiles(dir).stream().filter(
                  s -> s.size < sizemax).collect(
Collectors.toCollection(ArrayList<FileInformation>::new));

   }

   public static void DumpDirectory( DirectoryNode ds )
   {
          System.out.println("Directory ->" + ds.getname());
           ArrayList<EntryNode> list = ds.GetAllFiles(); 
  	  Iterator itr=list.iterator();  
 	 while(itr.hasNext()){ 
              EntryNode nt = (EntryNode)itr.next();
              if ( nt.isdir() )
                  DumpDirectory((DirectoryNode)nt);
              else
   		System.out.println("    File -> "+ ( (FileNode)nt).getname()); 
  	}  


          

   }

   public static void PrintAllContents ( ArrayList<FileInformation> list ) {
         Iterator itr=list.iterator();  
 	 while(itr.hasNext()){ 
              FileInformation nt = (FileInformation)itr.next();
              System.out.println(nt.name + " : " + nt.size);
  	}  

   }


   public static void PrintAllContents2 ( ArrayList<FileInformation> list ) {
         
          list.stream().forEach(System.out::println);
   }

   public static void main(String[] argv) throws Exception { 
     DirectoryInfo temp = new DirectoryInfo("E:\\CPPvsRUST");
     EntryNode ns = temp.Traverse();
    


    // ArrayList<FileInformation> s =
     //      GetAllFiles("E:\\CPPvsRUST","cpp");
    
    // PrintAllContents(s);

     ArrayList<FileInformation> str =
           GetAllFilesLessThan("E:\\CPPvsRUST",1000);
  
     
     PrintAllContents2(str);
    System.out.println("---------------------------------Rx Output ------------------------------");
    Observable<FileInformation> observable = Observable.fromIterable(str);
    observable.subscribe(s -> System.out.println(s));
      
   } 
   
}       