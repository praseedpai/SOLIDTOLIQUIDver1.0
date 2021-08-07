import java.io.File;
import java.util.*;




//////////////////
// Base interface for the File/Folder composite
//
//

public abstract class EntryNode {
    protected String name;
    protected int isdir;
    protected long fsize;
    public abstract long getSize();
    public abstract boolean isdir();
    //----------------- added in the Step 3
    public abstract void print();
    //---------------- added in the Step 4
    public abstract void accept( IFileFolderVisitor ivis );
    
}