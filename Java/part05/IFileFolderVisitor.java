import java.lang.*;
import java.util.*;
////////////////////////////////
// Directory Visitor interface
//
//
interface IFileFolderVisitor
{
    void Visit(FileNode fn );
    void Visit(DirectoryNode dn );

}