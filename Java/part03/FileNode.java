//////////////////////////////////////
//
// The Node which represents Files
//
//
public class FileNode extends EntryNode {
    
   public  FileNode(String pname, long size) { 
	isdir = 0; name = pname; fsize = size;
   }
   public boolean  isdir() { return isdir == 1; }
   public String getname() { return name; }
   
   public long getSize() {return fsize; }
   public void print() { System.out.println(name + " " +fsize); }

}
