import java.lang.*;
import java.io.*;


public class test
{

public static void main(String [] args) throws Exception{

RandomAccessFile file = new RandomAccessFile("file.bin", "rw");

file.writeInt(0x0000FFFF);
file.close();

}

}
