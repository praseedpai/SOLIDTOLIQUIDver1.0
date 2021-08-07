import java.lang.*;
import java.util.*;
//////////////////////////////////////
//
// Flatten the File/Folders into a linear list
//
//
class FlattenVisitor implements IFileFolderVisitor
{
    ArrayList<FILE_INFO> files = null;
    String CurrDir = "";
    public FlattenVisitor() {

 	files = new ArrayList<FILE_INFO>();
    }

    ArrayList<FILE_INFO> GetAllFiles() {
         return files;
    }

    public void Visit(FileNode fn ) {
       files.add( 
        new FILE_INFO(
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