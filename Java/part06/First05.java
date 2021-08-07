// version 6.0 of the Directory Traversal
// 
//
//
import java.io.File;
import java.util.*;
import java.util.stream.*;
import java.util.stream.Collectors.*;

class First05{
  
   
    
 
    // Main Method
    public static void main(String[] args)
    {

        if ( args.length != 1 ) { System.out.println("Give one Single Command line argument"); return ; }
        // array to store the name of files and directories
        File[] files = new File(args[0]).listFiles();
       if ( files == null ) { System.out.println("Could not retrieve file"); return; }
        // call displayFiles function to display files
     /*   List<String> lst  = DirectoryUtils.GatherAllFiles(args[0]);
         for (String filename : lst){
                  System.out.println(filename);
        }

        List<FILE_INFO> lst2 = DirectoryUtils.GatherAllFileInfo(args[0]);
         for (FILE_INFO file : lst2){
                  System.out.println(file.name + " = " + file.size);
        }

        EntryNode ns = DirectoryUtils.GetAllHives(args[0]);
       // DirectoryUtils.DumpDirectory( (DirectoryNode)ns);

         ((DirectoryNode)ns).accept( new PrintFolderVisitor ());

        ArrayList<FILE_INFO>  ficol = DirectoryUtils.GetAllFilesasCollection( args[0]);
        for (FILE_INFO file : ficol){
                  System.out.println(file.name + " = " + file.size);
        }
*/
         ArrayList<FILE_INFO> fiext = DirectoryUtils.GetAllFilesWithExt(args[0],"cpp");
         fiext.stream().forEach(System.out::println);
         fiext = DirectoryUtils.GetAllFilesLessThan(args[0],1000);
         fiext.stream().forEach(System.out::println);

    }
}