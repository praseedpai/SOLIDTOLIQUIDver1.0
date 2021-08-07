import java.util.*;
///////////////////////////////////////
// Node which represents Directory
//
//

public class DirectoryNode extends EntryNode {
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
  public void print() { System.out.println(name + " " +"size has to be computed"); }

}