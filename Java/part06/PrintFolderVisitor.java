import java.util.*;
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