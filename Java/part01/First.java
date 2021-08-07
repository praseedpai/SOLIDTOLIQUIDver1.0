// Preliminary version of the Directory Traversal
// 
//
//
import java.io.File;
class First {
   
    // function to display fies
    public static void displayFiles(File[] files,String dir_so_far )
    {
        // traversing through the files array
        for (File filename : files)
        {
           
            // if a sub directory is found,
            // print the name of the sub
            // directory
           
            if (filename.isDirectory())
            {
                System.out.println("Directory: "
                                   + filename.getName());
               
                // and call the displayFiles function
                // recursively to list files present
                // in sub directory
                displayFiles(filename.listFiles(), dir_so_far +"\\" + filename.getName());
             }
           
            // print the file name present in given path
            else
            {
                System.out.println("File: "+
                                 dir_so_far +"\\"  + filename.getName());
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
        displayFiles(files,args[0]);
    }
}