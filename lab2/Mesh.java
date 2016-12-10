
package lab2;
import java.util.*;
import java.io.*;

public class Mesh {
    public int readCoordinate(String fileName, float p_x[], float p_y[], float p_z[])
    {
        Scanner sc2 = null, sc3 = null;
        int count = -1;
        
        try
        {
            sc2 = new Scanner(new File(fileName));
            sc3 = new Scanner(new File(fileName));
            Scanner s2 = new Scanner(sc2.nextLine());
            Scanner s3 = new Scanner(sc3.nextLine());//OFF containing line
            //s3 = new Scanner(sc3.nextLine());
            float ij = -1;
                if((s2.next()).equals("OFF")!=true)
                {
                    System.out.println("Unknown file format.");
                    System.exit(0);
                }
            while (sc2.hasNextLine() && (ij != 4.0)) 
            {
                s3 = new Scanner(sc3.nextLine());
                ij = Float.parseFloat(s3.next());                
                s2 = new Scanner(sc2.nextLine());
                
                while (s2.hasNext()) 
                {
                    String s = s2.next();
                    p_x[++count] = Float.parseFloat(s);
                    s = s2.next();
                    p_y[count] = Float.parseFloat(s);
                    s = s2.next();
                    p_z[count] = Float.parseFloat(s);
                   // System.out.println(s);
                }
            }
        
        }
        catch(Exception e)
                {
                System.out.println("Error"+e);    
                }
        count--;
      return count;  
    }
    public void genVertexFace(float p_x[], float p_y[], float p_z[],int count)
    {
        
    }
    public void DooSabin(String fileName, float p_x[], float p_y[], float p_z[])
    {
        System.out.println("Inside doo sabin");
        int count = readCoordinate(fileName,p_x,p_y,p_z);
        
    }
}
