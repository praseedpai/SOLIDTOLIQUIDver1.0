import java.io.File;
import java.util.*;
public class DirectoryUtils {

  public static List<FILE_INFO> GatherAllFileInfo( String dir_so_far )
  {
	 File[] files = new File(dir_so_far).listFiles();
                   List<FILE_INFO> mst = new ArrayList<FILE_INFO>();
	GatherAllFiles2(files, dir_so_far,mst);
                   return mst;
   }
   
   public static List<String> GatherAllFiles(String dir_so_far){
	 File[] files = new File(dir_so_far).listFiles();
                   List<String> mst = new ArrayList<String>();
	 GatherAllFiles(files, dir_so_far,mst);
                   return mst;
   }
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

   public static void  GatherAllFiles2(File[] files, String dir_so_far, List<FILE_INFO> mst ) {
        for (File filename : files){
            if (filename.isDirectory()){  
                GatherAllFiles2(filename.listFiles(), dir_so_far +"\\" + filename.getName(), mst);
            }
            else{
	mst.add(new FILE_INFO(dir_so_far + "\\" + filename.getName(),filename.length()));
            }
          }
   }
   
  }  
 
    
