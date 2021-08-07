import java.io.File;
import java.util.*;
public class DirectoryUtils {

 public static ArrayList<FILE_INFO> GetAllFilesasCollection( String directory)
   {

       DirectoryInfo temp = new DirectoryInfo(directory);
       EntryNode ns = temp.Traverse();
        FlattenVisitor fs = new FlattenVisitor();
        ((DirectoryNode)ns).accept(fs);
        return fs.GetAllFiles();
   }


  public static EntryNode GetAllHives(String dir_so_far ) {
	DirectoryInfo di = new DirectoryInfo(dir_so_far);
                   EntryNode ret_val = di.Traverse();
	return ret_val;
  }

   public static void DumpDirectory( DirectoryNode ds )
   {
          ds.print();
          ArrayList<EntryNode> list = ds.GetAllFiles(); 
          Iterator itr=list.iterator();  
          while(itr.hasNext()){ 
                 EntryNode nt = (EntryNode)itr.next();
                 if ( nt.isdir() )
                       DumpDirectory((DirectoryNode)nt);
                 else {  ( (FileNode)nt).print(); }
          }         

   }
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
 
//----- Private class for Directory Info 
class DirectoryInfo {
   DirectoryNode root = null;
   DirectoryNode addptr = null;
   public DirectoryInfo( String rootdir ) { root =  new DirectoryNode(rootdir); }
   
   public EntryNode Traverse () {
       EntryNode ns = null;
       File a = new File(root.getname());
       if ( a.isDirectory() )
         ns = visitAllDirsAndFiles(a);
       return ns;
   }

   private static EntryNode visitAllDirsAndFiles(File dir) {
      if (dir.isDirectory()) {
         DirectoryNode ns = new DirectoryNode(dir.getPath());
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
         System.out.println(" What you have given is not a directory...");
         return null;
      }
   }

}
    
