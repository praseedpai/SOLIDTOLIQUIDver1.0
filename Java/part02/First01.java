// version 2.0 of the Directory Traversal
// 
//
//
import java.io.File;
import java.util.*;
class First01 {
   public static void  GatherAllFiles(File[] files, String dir_so_far, List<String> mst ) {
        for (File filename : files){
            if (filename.isDirectory()){  
                GatherAllFiles(filename.listFiles(), dir_so_far +"\\" + filename.getName(), mst);
            }
            else{
	mst.add(dir_so_far + "\\" + filename.getName());
            }
          }
   }
   
    
 
    // Main Method
    public static void main(String[] args)
    {

        if ( args.length != 1 ) { System.out.println("Give one Single Command line argument"); return ; }
        // array to store the name of files and directories
        File[] files = new File(args[0]).listFiles();
       if ( files == null ) { System.out.println("Could not retrieve file"); return; }
        // call displayFiles function to display files
        List<String> lst  = DirectoryUtils.GatherAllFiles(args[0]);
         for (String filename : lst){
                  System.out.println(filename);
        }

        List<FILE_INFO> lst2 = DirectoryUtils.GatherAllFileInfo(args[0]);
         for (FILE_INFO file : lst2){
                  System.out.println(file.name + " = " + file.size);
        }


    }
}