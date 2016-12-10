/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab2;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.*;
import java.awt.geom.*;
import java.util.Hashtable;
import javax.swing.*;
import javax.swing.border.TitledBorder;
import java.io.*;
import java.util.*;



/**
 *
 * @author mesay
 */
public class GM2 extends JFrame implements ItemListener, ActionListener, MouseListener, MouseMotionListener {
    
        JButton editflag, deleteflag, duplicateflag, rationalBezier, resetflag, redraw, wireinput, drawCrust, drawNNCrust;
        JPanel loc, loc2, loc3, loc4, container;
        JSlider uval;
        CheckboxGroup grp1 = new CheckboxGroup();
        TextField tf;
        String text="No point selected";
        String msg= " "; 
        boolean isButtonPressed = false;
        boolean edit_flag = false;
        boolean drag_flag = false;
        boolean delete_flag = false;
        boolean duplicate_flag = false;
        boolean reset_flag = false;
        boolean redraw_flag = false;
        boolean wireinput_flag = false;
        boolean crust_flag = false;
        boolean drawCrust_flag = false;
        boolean nncrust_flag = false;
        boolean isclosed = false;
        float[][] coord = new float[100][2];   // the mouse coordinates
        int index = -1;
        float p_x[] = new float[100000];
        float p_y[] = new float[100000];
        float p_z[] = new float[100000];
        float wire_x[] = new float[100000];
        float wire_y[] = new float[100000];
        int wirelength = 0;
        float p[][] = new float[100000][2];
        boolean drawn = false;
        int discrete_count = 0;
        int drag_index;
        int delete_index;
        int degree_bspline;
        boolean c1contflag = false;
        boolean bezier_active = false;
        boolean quad_active = false;
        boolean drawNN = false;
        Checkbox bez_sample, bspline_sample , bez_decastel, bspline_sub, c1cont, crust, nncrust; 
    GM2()
    {
        setVisible(true);
        setPreferredSize(new Dimension (1000,700));
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        getContentPane().setBackground(Color.YELLOW);
        this.setLayout(null);
        pack();
        setLocationRelativeTo(null);
        container = new JPanel(new GridLayout(4,4));
        
        
        container.setLocation(600,5);
        container.setSize(320,500);
        loc = new JPanel();
                
        loc2 = new JPanel();
        loc3 = new JPanel();
        loc4 = new JPanel();
        uval = new JSlider(JSlider.HORIZONTAL, 0, 300, 150);
        uval.setValue(50);
       
        uval.setPaintTicks(true);
        uval.setMajorTickSpacing(20);
        uval.setMinorTickSpacing(5);
        uval.setBorder(new TitledBorder("Value of u for discretization"));
  
        Hashtable labelTable = new Hashtable();
        labelTable.put( 0, new JLabel("0.0") );
        labelTable.put( 50, new JLabel("0.05") );
        labelTable.put( 100, new JLabel("0.1") );
        labelTable.put( 150, new JLabel("0.15") );
        labelTable.put( 200, new JLabel("0.2") );
        labelTable.put( 250, new JLabel("0.25") );
        labelTable.put( 300, new JLabel("0.3") );
        labelTable.put( 350, new JLabel("0.35") );
        uval.setLabelTable( labelTable );
        uval.setPaintLabels(true);
        
        resetflag = new JButton("Reset");
        editflag = new JButton(" Edit");
        deleteflag = new JButton("Delete");
        duplicateflag = new JButton("Duplicate");
        redraw = new JButton("Redraw");
        rationalBezier = new JButton("Rational");
        wireinput = new JButton("Wire Input");
        drawCrust = new JButton("Draw Crust");
        drawNNCrust = new JButton("Draw NNCrust");
        
        loc.add(resetflag);
        loc.add(wireinput);
        loc.add(editflag);
        loc.add(deleteflag);
        loc.add(duplicateflag);
        loc.add(redraw);
        loc.add(rationalBezier);
        loc.add(drawCrust);
        loc.add(drawNNCrust);
        
        bez_sample = new Checkbox("Bezier curve",grp1,true);
        bspline_sample = new Checkbox("Cubic B-Spline",grp1,true); 
        bez_decastel = new Checkbox("Bezier DeCastel",grp1,true);
        bspline_sub = new Checkbox("Quad B-Spline subdivision",grp1,true);
        c1cont = new Checkbox("Bezier C1 continuous",grp1,true);
        crust =  new Checkbox("Crust Algorithm",grp1,true);
        nncrust = new Checkbox("NNCrust Algorithm",grp1,true);
        
        bez_sample.addItemListener(this);
        bspline_sample.addItemListener(this);
        bez_decastel.addItemListener(this);
        bspline_sub.addItemListener(this);
        c1cont.addItemListener(this);
        crust.addItemListener(this);
        nncrust.addItemListener(this);
        rationalBezier.addActionListener(this);
        editflag.addActionListener(this);
        deleteflag.addActionListener(this);
        duplicateflag.addActionListener(this);
        resetflag.addActionListener(this);
        redraw.addActionListener(this);
        wireinput.addActionListener(this);
        drawCrust.addActionListener(this);
        drawNNCrust.addActionListener(this);
        tf = new TextField(2);
        
        loc3.add(tf);        
        
        loc2.add(bspline_sample);
        loc2.add(bez_decastel);
        loc2.add(bspline_sub);
        loc2.add(bez_sample);
        loc2.add(crust);
        loc2.add(nncrust);
        loc4.add(uval);
        
        container.add(loc);
        container.add(loc2);
        container.add(loc3);
        container.add(loc4);
        add(container);
        
        addMouseListener( this );
        addMouseMotionListener( this );
    }
    @Override
    public void itemStateChanged(ItemEvent ex) {
        crust_flag = false;
        drawCrust_flag = false;
        nncrust_flag = false;
         if (ex.getItemSelectable() == bez_sample)
         {
             text = "Don't forget to put value of discretization 'u' !";
             repaint();
             
         }
         else if(ex.getItemSelectable() == nncrust)
         {
             nncrust_flag = true;
             repaint();
         }
         else if(ex.getItemSelectable() == crust)
         {
             crust_flag = true;
             repaint();
         }
         else if(ex.getItemSelectable() == bspline_sample)
         {
             text = "Don't forget to put value of discretization 'u' !";
             repaint();
         }
         else if(ex.getItemSelectable() == bez_decastel)
         {
             text = "Don't forget to put number of iterations 'm' ! Value current:" + tf.getText();
             repaint();
         }
         else if(ex.getItemSelectable() == bspline_sub)
         {
             text = "Don't forget to put number of iterations 'm' ! Value current:" + tf.getText();
             repaint();
             
         }
         else if(ex.getItemSelectable() == c1cont)
         {
             text = "Don't forget to change value of u in slider !";
            repaint();
         
         }
         else
         {
            text = "Wrong value";
            repaint();
         }
    }
    
    void makeg1cont() {
    if(index <2)
        return;
    if(coord[0][0]!=p_x[discrete_count-1] || coord[0][1]!=p_y[discrete_count-1])
{
    System.out.println("Not closed"); 
    System.out.println("("+p_x[0]+","+p_y[0]+") ("+p_x[discrete_count-1]+","+p_y[discrete_count-1]+")");
    return;
}
    float tempx = p_x[0], tempy = p_y[0];
    System.out.println(" Closed curve");
    p_x[0] = (p_x[1]+p_x[discrete_count-2])/2;
    p_y[0] = (p_y[1]+p_y[discrete_count-2])/2;
    p_x[discrete_count-1] = (p_x[1]+p_x[discrete_count-2])/2;
    p_y[discrete_count-1] = (p_y[1]+p_y[discrete_count-2])/2;
    JOptionPane.showMessageDialog(null, "It is closed curve. Move first point from ("+tempx+","+tempy+") to ("+p_x[0]+","+p_y[0]+") to make g1 cont");
}
    @Override
    public void paint(Graphics g){  

        g.setFont(new Font("TimesRoman", Font.PLAIN, 20)); 
        g.setColor(Color.GRAY);
        g.fillRect(608,150,330,250);
        Color c= new Color(9,53,13);
       //add(reset);
       
    g.setColor(c);
    g.setFont(new Font("TimesRoman", Font.PLAIN, 15)); 
    g.drawString("Select curve:",100,15);
    // This will draw a rectangle (xco,yco,xwidth,height);
     g.drawRect(100,100,500,500);

    // This will fill a rectangle
      g.fillRect(102,102,496,496); 
      //add(uval);
      g.setColor(Color.red);
      Graphics2D g2 = (Graphics2D) g;
      if(index!=-1)
      {
          for(int i=0;i<=index;i++)
          {
              g2.draw(new Line2D.Double(coord[i][0]-1,coord[i][1]-1,coord[i][0]+1,coord[i][1]+1));
              g2.draw(new Line2D.Double(coord[i][0]+1,coord[i][1]-1,coord[i][0]-1,coord[i][1]+1));
          }
          
      }
      if(crust_flag == true && drawCrust_flag == false)
      {
          System.out.println("Entered crust");
          for(int i=0;i<=index;i++)
          {
              g2.draw(new Line2D.Double(coord[i][0]-1,coord[i][1]-1,coord[i][0]+1,coord[i][1]+1));
              g2.draw(new Line2D.Double(coord[i][0]+1,coord[i][1]-1,coord[i][0]-1,coord[i][1]+1));
          }
          g.setColor(Color.gray);
          for(int i=1;i<=index;i++)
              g2.draw(new Line2D.Double(coord[i-1][0],coord[i-1][1],coord[i][0],coord[i][1]));
          writecoord();
      }
      else if(drawCrust_flag == true)
      {
          drawCrust_flag = false;
          g.setColor(Color.blue);
          paintcoord(true);
          for(int i=0;i<=index;i++)
          {
              g2.draw(new Line2D.Double(coord[i][0]-1,coord[i][1]-1,coord[i][0]+1,coord[i][1]+1));
              g2.draw(new Line2D.Double(coord[i][0]+1,coord[i][1]-1,coord[i][0]-1,coord[i][1]+1));
          }
          g.setColor(Color.black);
         // System.out.println("val:index: "+index);
         // writecoord();
          for(int i=1;i<=index;i++)
          {
              g2.draw(new Line2D.Double(coord[i-1][0],coord[i-1][1],coord[i][0],coord[i][1]));
          }
              
          g.setColor(Color.red);
          paintcoord(false);
          for(int i=0;i<=index;i++)
          {
              g2.draw(new Line2D.Double(coord[i][0]-1,coord[i][1]-1,coord[i][0]+1,coord[i][1]+1));
              g2.draw(new Line2D.Double(coord[i][0]+1,coord[i][1]-1,coord[i][0]-1,coord[i][1]+1));
          }
          g.setColor(Color.white);
          System.out.println("val:index: "+index);
          
          for(int i=1;i<=index;i++)
          {
             //g2.draw(new Line2D.Double(coord[i-1][0],coord[i-1][1],coord[i][0],coord[i][1]));
            //  System.out.println(coord[i-1][0]+","+coord[i-1][1]+"\n");
          }
          //g2.draw(new Line2D.Double(coord[0][0],coord[0][1],coord[index][0],coord[index][1]));
          float coord1[][] = new float[100][2];
          float coord2[][] = new float[100][2];
          float coordres[][] = new float[300][2];
          float coordres2[][] = new float[300][2];
          int coord_index = edgecoord(coord1,coord2);
          int countres = join( coord, index, coord1, coord2, coord_index, coordres);
//mergecoord(coord, index,  coord1, coord2, coord_index, coordres, true);
          for(int i=1;i<=countres;i++)
          {
            g2.draw(new Line2D.Double(coordres[i][0],coordres[i][1],coordres[i-1][0],coordres[i-1][1]));
          }
          g2.draw(new Line2D.Double(coordres[countres][0],coordres[countres][1],coordres[0][0],coordres[0][1]));
          discrete_count = countres+2;
          for(int i=0;i<=countres;i++)
          {
              p_x[i] = coordres[i][0];
              p_y[i] = coordres[i][1];
          }
          p_x[countres+1] = coordres[0][0];     p_y[countres+1] = coordres[0][1];
          
                 int interval;
                        if((tf.getText()).equals("") == true)
                            interval = 10;
                        else interval = Integer.parseInt(tf.getText());
                        if(isclosed == true) makeg1cont();  
                        extrusion(p_x, p_y,p_z, interval);
                        revolution(p_x, p_y,p_z, interval);
                        
           //countres = mergecoord(coord, index,  coord1, coord2, coord_index, coordres2, false);
          //for(int i=1;i<=countres;i++)
         // {
           //   g2.draw(new Line2D.Double(coordres2[i][0],coordres2[i][1],coordres2[i-1][0],coordres2[i-1][1]));
         // }
          //g2.draw(new Line2D.Double(220,383,151,365));
          //for(int i=0;i<=coord_index;i++)
          //{
              //System.out.print(coord1[i][0]+" "+coord1[i][1]+" ");
             //   System.out.println(coord2[i][0]+" "+coord2[i][1]+" ");
                //g2.draw(new Line2D.Double(coord1[i][0],coord1[i][1],coord2[i][0],coord2[i][1]));
          //}
                        
          //System.out.println(coord[index][0]+","+coord[index][1]+"\n");
      }
      else if(drawn == true)
      {
             g.setColor(Color.white);
             
             if(nncrust_flag == true)
             {
                 System.out.print("Entered NN-Crust");
                 g.setColor(Color.cyan);
                 int nearest_coord[] = new int[index+1];
                 int second_ncoord[] = new int[index+1];
                 if(index>=1)
                 
                 {  boolean check = NNCrust(nearest_coord,second_ncoord);
                 float coordres[][] = new float[index+1][2];
                 if(drawNN == true)
                 {
                     System.out.println("Entered join");
                     joinNN2(coord, nearest_coord, second_ncoord, index, coordres);
                     for(int i=0;i<=index;i++)
                     {
                         coord[i][0] = coordres[i][0];
                         coord[i][1] = coordres[i][1];
                     }
                     //index++;
                     //coord[index] = coord[0];
                               discrete_count = index+2;
                    for(int i=0;i<=index;i++)
                    {
                        p_x[i] = coord[i][0];
                        p_y[i] = coord[i][1];
                    }
                    p_x[index+1] = coord[0][0];     p_y[index+1] = coord[0][1];

                     int interval;
                        if((tf.getText()).equals("") == true)
                            interval = 10;
                        else interval = Integer.parseInt(tf.getText());
                        //if(isclosed == true) makeg1cont();  
                        extrusion(p_x, p_y,p_z, interval);
                        revolution(p_x, p_y,p_z, interval);
                     drawNN= false;
                 }
                    for(int i=1;i<=index;i++)
                    {
                            g2.draw(new Line2D.Float(coord[i][0],coord[i][1],coord[nearest_coord[i]][0],coord[nearest_coord[i]][1]));
                        if(second_ncoord[i] !=-1) 
                            g2.draw(new Line2D.Float(coord[i][0],coord[i][1],coord[second_ncoord[i]][0],coord[second_ncoord[i]][1]));
                        //g2.draw(new Line2D.Double(coordres[i][0],coordres[i][1],coordres[i-1][0],coordres[i-1][1]));
                    }
                 
                 g.setColor(Color.white);
                 //discrete_count = index+1;
                /*for(int i=0;i<=index;i++)
                {
                    p_x[i] = coordres[i][0];
                    p_y[i] = coordres[i][1];
                }
                p_x[index+1] = coord[0][0];     p_y[index+1] = coord[0][1];
                 int interval;
                        if((tf.getText()).equals("") == true)
                            interval = 10;
                        else interval = Integer.parseInt(tf.getText());
                        if(isclosed == true) makeg1cont();  
                        extrusion(p_x, p_y,p_z, interval);
                        revolution(p_x, p_y,p_z, interval);
                 */
              }
              }
                else if(c1contflag == true){
                    for(int i=1;i<discrete_count-2;i++)
                    {
                        System.out.print("inside c1cont");
                        if(p_x[i-1]>102 && p_y[i-1]>102 && p_x[i]>102 && p_y[i]>102 && p_x[i-1]<596 && p_y[i-1]<596 && p_x[i]<596 && p_y[i]<596)
                                g2.draw(new Line2D.Double(p_x[i-1],p_y[i-1],p_x[i],p_y[i]));
                        c1contflag = false;
            
                    }   
                }
                else{
                        for(int i=1;i<discrete_count;i++)
                        {
                            if(p_x[i-1]>102 && p_y[i-1]>102 && p_x[i]>102 && p_y[i]>102 && p_x[i-1]<596 && p_y[i-1]<596 && p_x[i]<596 && p_y[i]<596)
                                    g2.draw(new Line2D.Double(p_x[i-1],p_y[i-1],p_x[i],p_y[i]));
       
                        }
                        int interval;
                        if((tf.getText()).equals("") == true)
                            interval = 10;
                        else interval = Integer.parseInt(tf.getText());
                        if(isclosed == true) makeg1cont();  
                        extrusion(p_x, p_y,p_z, interval);
                        revolution(p_x, p_y,p_z, interval);
             
                        boolean bool1 = false;
                        if( bezier_active == false)
                        {
                            boolean contour = (revolution(p_x, p_y, p_z, interval));
                            System.out.println("Contour: "+contour);
                            if(contour == true && quad_active == false)
                                JOptionPane.showMessageDialog(null, "There is an intersection.");
                        }
                        else
                        {
                            revolution(p_x, p_y, p_z, interval);
                            for(int i=0;i<=index;i++){
                            bool1 = intersectionPairChec(coord, i);
                            if(bool1==true){
                                System.out.println("There is an intersection.");
                                if(quad_active == false)
                                    JOptionPane.showMessageDialog(null, "There is an intersection.");
                                }
                        }
                if(bool1 == false)
                      System.out.println("There is no intersection present.");
                  
                    }
              
                if(wireinput_flag==true)
                {
                    //wireinput_flag = false;
                    sweep(p_x, p_y, p_z, wirelength);
                    lofting(interval);
                }

            }
            drawn = false;
      }
      
      
}
    float[][] subDivide(float coordd[][],int coord_n, int m, float u)    {
        float[][] poly1 = new float[0][2];
        float[][] poly2 = new float[0][2];
        
        if(m==1)
        {
            float[][] intterm;
            intterm = oneSubDivide(coordd, coord_n, poly1, -1, poly2, 0.5f, coord_n);
            System.out.println("\ncame back sub divide m =1");
            for(int i=0;i<intterm.length;i++)
            {
                System.out.print("("+intterm[i][0]+","+intterm[i][1]+")");
            }
            return intterm;
            
        }
        ///////////////////////////
        ///////////////end of m
        //////////////////////////
        //coord_n = 2*(coord_n+1);
        float[][] new_coordd;// = new int[2*coord_n][2];
        new_coordd = oneSubDivide(coordd, coord_n, poly1, -1, poly2, 0.5f, coord_n);
        float[][] coordd1 = new float[coord_n+1][2];
        float[][] coordd2 = new float[coord_n+1][2];
        float[][] coordd3;// = new int[100][2];
        float[][] coordd4;// = new int[100][2];

        int count=-1;
        for(int i=0;i<=coord_n;i++)
        {   
            count++;
            coordd1[i][0] = new_coordd[count][0];
            coordd1[i][1] = new_coordd[count][1];
        }
        count--;
        for(int i=0;i<=coord_n;i++)
        {
            count++;
            coordd2[i][0] = new_coordd[count][0];
            coordd2[i][1] = new_coordd[count][1];
        }
        System.out.println("I am here 2nd functn, count = "+count+", "+2*(coord_n)+1);
        coordd3 = subDivide(coordd1, coord_n, m-1, u);
        coordd4 = subDivide(coordd2, coord_n, m-1, u);
        float[][] coordd5 = new float[coordd3.length+coordd4.length-1][2];
        System.out.println("\ncoordinates3:");
        for(int i=0;i<coordd3.length;i++)
        {
            coordd5[i][0] = coordd3[i][0];
            coordd5[i][1] = coordd3[i][1];
            System.out.print("("+coordd5[i][0]+","+coordd5[i][1]+")");
        }
            

        //System.out.println("I am here also");

        
        System.out.println("\ncoordinates4:");
        for(int i=0;i<coordd4.length;i++)
            System.out.print("("+coordd4[i][0]+","+coordd4[i][1]+")");
        System.out.println("\n coord5: "+coordd5.length);
        for(int i=0;i<coordd4.length;i++)
        {
            coordd5[coordd3.length + i -1][0] = coordd4[i][0];
            coordd5[coordd3.length + i -1][1] = coordd4[i][1];
            
        }
        for(int i=0;i<coordd5.length;i++)
        {
            System.out.print("("+coordd5[i][0]+","+coordd5[i][1]+")");
        }
        count = -1;
        //for(int i=0;i<=2*(2*(coord_n-1))+1;i++)
        //{
         //   count++;
         //   if(i<2*coord)
         // coordd5[i]=
         //   System.out.print("("+coordd3[i][0]+","+coordd3[i][1]+")");
       // }
            discrete_count = coordd5.length;//2*(2*(coord_n-1))+1;
            
        return coordd5;
        //coordd1 = subDivide(coordd)
    }
    
    float[][] oneSubDivide(float coordd[][], int coordd_n, float poly1[][], int poly_n, float poly2[][], float u, int n)    {
        float p[][];
        if(poly_n == -1) p = new float[1][2];
        else p = new float[2*poly_n+3][2];   //extra +1 for p0
        if(n==0)
        {
            int count = -1;
//            discrete_count = 2*(poly_n)+1;
            //System.out.println("one subdivide n=0, poly1");
            for(int i=0;i<=poly_n;i++)
            {
                count++;
               // System.out.println("Inside poly1");
              //  System.out.print("("+poly1[i][0]+","+poly1[i][1]+")"+" ");
                p[count][0] = poly1[i][0];
                p[count][1] = poly1[i][1];
            }
            System.out.println("("+coordd[0][0]+","+coordd[0][1]+")"+" ");
            count++;
            p[count][0] = coordd[0][0];
            p[count][1] = coordd[0][1];
            System.out.println("\n one subdivide n=0, poly");
             for(int i=0;i<=poly_n;i++)
             {
                count++;
                //System.out.println("Inside poly2");
                //System.out.print("("+poly2[i][0]+","+poly2[i][1]+")"+" ");
                p[count][0] = poly2[i][0];
                p[count][1] = poly2[i][1];
             }
             System.out.println("Poly p1.p0.p2");
             for(int i=0;i<p.length;i++)
             {
                 System.out.print("("+p[i][0]+","+p[i][1]+")"+" ");
             }
         //    discrete_count = count;
            System.out.println(" ");
            System.out.println("count= "+count+", index = "+discrete_count);
            return p;
        }
        ////////////////////////////
        //if  n  not equal to zero
        ////////////////////////////
        int i;
        System.out.println("\n n not equal to zero: value of poly1:");
        
        for(i=0;i<=poly_n;i++)
        {
            System.out.print("("+poly1[i][0]+","+poly1[i][1]+")"+" ");
        }
        System.out.println("\n n not equal to zero: value of poly2:");
        
        for(i=0;i<=poly_n;i++)
        {
            System.out.print("("+poly2[i][0]+","+poly2[i][1]+")"+" ");
        }
        poly_n++;
        float [][] ptemp = new float[poly_n+1][2];
        for(i=0;i<=(poly_n-1);i++)
        {
            ptemp[i][1] = poly1[i][1];
            ptemp[i][0] = poly1[i][0];
        }
        ptemp[poly_n][0] = coordd[0][0]; 
        ptemp[poly_n][1] = coordd[0][1]; 
        
        poly1 = ptemp;
        
        System.out.println("\n n not equal to zero: value of poly1 post append:");
        
        for(i=0;i<=poly_n;i++)
        {
            System.out.print("("+poly1[i][0]+","+poly1[i][1]+")"+" ");
        }
        
        float [][] ptemp1 = new float[poly_n+1][2];
        for(i=0;i<=poly_n-1;i++)
        {
            ptemp1[i][1] = poly2[i][1];
            ptemp1[i][0] = poly2[i][0];
        }
        poly2 = ptemp1;
        
        
        //poly2[poly_n][0] = coord[coord_n][0];
        
        for(i = poly_n;i>=1;i--)
        {
            poly2[i][0] = poly2[i-1][0]; 
            poly2[i][1] = poly2[i-1][1]; 
        }
        
        poly2[0][0] = coordd[coordd_n][0];
        poly2[0][1] = coordd[coordd_n][1];
        
        System.out.println("\n n not equal to zero: value of poly2.post append:");
        
        for(i=0;i<=poly_n;i++)
        {
            System.out.print("("+poly2[i][0]+","+poly2[i][1]+")"+" ");
        }
        
        float [][] new_coordd1 = new float[coordd_n][2];
        System.out.println("\ncoord_n = "+ coordd_n +"Poly1:");
        
        //for(i=0;i<poly1.length;i++)
        //{
         //   System.out.print("("+poly1[i][0]+","+poly1[i][1]+")");
       //}
        //System.out.println("This guy: ("+coordd[coordd_n][0]+","+coordd[coordd_n][1]+")"+" coord_n:"+coordd_n);
        System.out.println("\nnew coord1:");
        for(i = 0; i<new_coordd1.length;i++)
        {
            new_coordd1[i][0] = (coordd[i][0]+ (u*(coordd[i+1][0]-coordd[i][0])));
            new_coordd1[i][1] = (coordd[i][1]+ (u*(coordd[i+1][1]-coordd[i][1])));
            System.out.print("("+new_coordd1[i][0]+","+new_coordd1[i][1]+")");
        }
        System.out.println("\nPoly2:");
        
        //for(int i=0;i<poly2.length;i++)
        //{
        //    System.out.print("("+poly2[i][0]+","+poly2[i][1]+")");
        //}
        
        float[][] interm;// = new int[100][2];
        interm = oneSubDivide(new_coordd1, coordd_n-1, poly1, poly_n, poly2, u,n-1);
        System.out.println("Interm one subdivide");
        for(i=0;i<interm.length;i++)
            System.out.print("("+interm[i][0]+","+interm[i][1]+")");
        return interm;
    }

    float generatorN(float [][][]N, int d, int []t) {
    
    //for(i=0;i<=index;i++)
    //    N[i][1] = 1;
    
    for(int i=0;i<=index;i++)
    {
    //    us = t[i];
    //    ue = t[i+1];
        for(float u = 0;u<index+d;u+=0.02)
        {
            int v = (int)Math.floor(u);
           // System.out.print(N[i][d-1][v]);
            //System.out.print(N[i+1][d-1][v]);
            //System.out.print(t[i+d-1]);
           // System.out.print(t[i+d]);
            N[i][d][v] = ((u-t[i])*N[i][d-1][v]/(t[i+d-1]-t[i])) + ((t[i+d]-u)*N[i+1][d-1][v]/(t[i+d] - t[i+1]));
        }
        
    }
    return 0.0f;
}
    
    float[][][] arbitraryBSpline(float [][][]N, int d, int []t){

    for(int i = 0;i<=index+1;i++)
    {
        for(int u = 0;u<index+d+1;u++)
        {
            //N[i][1][u] =1;
            if(t[u] == u) N[i][1][u] =1;
            else N[i][1][u] =0;
            }
    }
    for(int k=1;k<=d;k++)
        generatorN(N,k,t);
    return N;
}

    int fact(int n)    {
        int result;
       if(n==0 || n==1)
         return 1;

       result = fact(n-1) * n;
       return result;
    }
    
    int BezierPoints(float inc)    {
        float u1=0.000f;
        int i;
        boolean flag = false;
        for(i=0;i<=discrete_count;i++)
        {
        p_x[i] = 0;    
        p_y[i] = 0;
        }
        System.out.println("Value of discrete:"+discrete_count);

        discrete_count = 0;
        do{
            
            for(i=0;i<=index;i++)
            {
            p_x[discrete_count] += coord[i][0]* Math.pow(u1,i) * Math.pow(1-u1,index-i) *(fact(index)/(fact(i)*fact(index-i))) ;
            p_y[discrete_count] += coord[i][1]* Math.pow(u1,i) * Math.pow(1-u1,index-i) *(fact(index)/(fact(i)*fact(index-i))) ;
            }
            u1+=inc;
            
            discrete_count++;
            if(flag == true)
                break;
            if(u1>1 && u1-inc< 1)
                {u1 = 1; flag = true;}
        }while(u1<=1);
        return 0;
    }
    float[][] oneCornerLoop(float coordd[][], int index)    {
        float aug_mat[][] = new float[100000][2];
        int i;
        discrete_count = 0;
        for(i=1;i<index;i++)
        {
            aug_mat[discrete_count][0] = 3*coordd[i-1][0]/4 + coordd[i][0]/4; 
            aug_mat[discrete_count][1] = 3*coordd[i-1][1]/4 + coordd[i][1]/4; 
            aug_mat[++discrete_count][0] = coordd[i-1][0]/4 + 3*coordd[i][0]/4; 
            aug_mat[discrete_count][1] = coordd[i-1][1]/4 + 3*coordd[i][1]/4; 
            aug_mat[++discrete_count][0] = 3*coordd[i][0]/4 + coordd[i+1][0]/4; 
            aug_mat[discrete_count][1] = 3*coordd[i][1]/4 + coordd[i+1][1]/4; 
            aug_mat[++discrete_count][0] = coordd[i][0]/4 + 3*coordd[i+1][0]/4; 
            aug_mat[discrete_count][1] = coordd[i][1]/4 + 3*coordd[i+1][1]/4; 
        }
        return aug_mat;
    }
    int BSpline(float inc)    {
        float u = 0, u3, u2;
        float p0_x,p1_x,p2_x,p3_x;
        float p0_y,p1_y,p2_y,p3_y;
        int i,count=0;
        for(i=0;i<=discrete_count;i++)
        {
        p_x[discrete_count] = 0;    
        p_y[discrete_count] = 0;
        }
        discrete_count = 0;
        do{
            p0_x = coord[count][0];
            p1_x = coord[count+1][0];
            p2_x = coord[count+2][0];
            p3_x = coord[count+3][0];
            
            p0_y = coord[count][1];
            p1_y = coord[count+1][1];
            p2_y = coord[count+2][1];
            p3_y = coord[count+3][1];
            
            for(u=0;u<=1;u+=inc)
            {
              
                u3 = u*u*u;
                u2 = u*u;
                p_x[discrete_count] = (float)(Math.pow(1-u,3)*p0_x/6 + (3*u3-6*u2+4)*p1_x/6 + (-3*u3+3*u2+3*u+1)*p2_x/6 + (u3)*p3_x/6);
                p_y[discrete_count] = (float)(Math.pow(1-u,3)*p0_y/6 + (3*u3-6*u2+4)*p1_y/6 + (-3*u3+3*u2+3*u+1)*p2_y/6 + (u3)*p3_y/6);
                discrete_count++;  
            }
            if(u-inc != 1 || u!= 1)
             {
                 u = 1;
                u3 = u*u*u;
                u2 = u*u;
                p_x[discrete_count] = (float)(Math.pow(1-u,3)*p0_x/6 + (3*u3-6*u2+4)*p1_x/6 + (-3*u3+3*u2+3*u+1)*p2_x/6 + (u3)*p3_x/6);
                p_y[discrete_count] = (float)(Math.pow(1-u,3)*p0_y/6 + (3*u3-6*u2+4)*p1_y/6 + (-3*u3+3*u2+3*u+1)*p2_y/6 + (u3)*p3_y/6);
                discrete_count++;  
            }   
            count++;
        }while(count<=index-3);
        return 0;
    }
      
    int rationalBezier(float w[])    {
        float u1=0.000f;
        int i;
        float wt[] = new float[10000];
        for(i=0;i<=discrete_count;i++)
        {
        p_x[i] = 0;    
        p_y[i] = 0;
        }
        System.out.println("Value of discrete:"+discrete_count);

        discrete_count = 0;
        do{
            
            for(i=0;i<=index;i++)
            {
            
            p_x[discrete_count] += w[i] * coord[i][0]* Math.pow(u1,i) * Math.pow(1-u1,index-i) *(fact(index)*1.0/(1.0*fact(i)*fact(index-i))) ;
            p_y[discrete_count] += w[i] * coord[i][1]* Math.pow(u1,i) * Math.pow(1-u1,index-i) *(fact(index)*1.0/(1.0*fact(i)*fact(index-i))) ;
            wt[discrete_count]  += (w[i] * Math.pow(u1,i) * Math.pow(1-u1,index-i) *1.0 * (fact(index)*1.0/(fact(i)*fact(index-i)))) ;
           // System.out.print("("+p_x[discrete_count]+","+p_y[discrete_count]+")" );
            //System.out.print("wt:"+wt[discrete_count]+"w:"+ w[i]+ " "+Math.pow(u1,i)+" "+Math.pow(1-u1,index-i)+" "+(fact(index)*1.0/(fact(i)*fact(index-i)))  +"  || ");
            }
            u1+=0.002;
            System.out.println("("+p_x[discrete_count]+","+p_y[discrete_count]+") "+(int)(wt[discrete_count]));
             p_x[discrete_count] = (float)((float)p_x[discrete_count]/(wt[discrete_count]));
            p_y[discrete_count] = (float)((float)p_y[discrete_count]/(wt[discrete_count]));
            discrete_count++;
        }while(u1<1);
        return 0;
    }
    boolean isCollinear(float x1,float y1,float x2,float y2,float x3,float y3,int j)    {
        float a = ((float)(x1+x3)/2);
        float b = ((float)(y1+y3)/2);
        System.out.println("\n Collinearity:"+(a-(float)x2)+" "+(b-(float)y2));
        if(Math.abs(a-x2) <= 5 || Math.abs(b-y2) <= 5)
        {
            coord[j][0] = (x1+x3)/2.0f;
            coord[j][1] = (y1+y3)/2;
            return true;
        }
        return false;
        
    }
    void c1cont(float inc) {
        if((index)%3!=0)
        {
            text ="Need more points";
            System.out.println("Need more points");
            repaint();
            return;
        }
        int i,j, val;
        float u1 = 0.0f;
        for(i=0;i<=discrete_count;i++)
        {
        p_x[i] = 0;    
        p_y[i] = 0;
        }
        System.out.println("Value of discrete:"+discrete_count);
        val = 3;
        discrete_count = -1;
        for(j=0;j<=(index);j+=3)
        {
        do{
             discrete_count++;
            //index =3;
            for(i=0;i<=3;i++)
            {
            p_x[discrete_count] += coord[i+j][0]* Math.pow(u1,i) * Math.pow(1-u1,val-i) *(fact(val)/(fact(i)*fact(val-i))) ;
            p_y[discrete_count] += coord[i+j][1]* Math.pow(u1,i) * Math.pow(1-u1,val-i) *(fact(val)/(fact(i)*fact(val-i))) ;
            }
            u1+=inc;
           
        }while(u1<1);
        //discrete_count++;
       
        //System.out.println("Discrete Before:"+discrete_count);
        if(j!=0 && isCollinear(coord[j-1][0],coord[j-1][1],coord[j][0],coord[j][1],coord[j+1][0],coord[j+1][1],j))
            text = " curve "+((j)/3)+ " and curve "+((j)/3+1)+" are collinear";
        else if(j!=0) text = " curve "+((j)/3)+ " and curve not "+((j)/3+1)+" are collinear";
        else text = "only one j";
        u1 = 0; 
        }
      discrete_count++;
      u1 = 1;
      j+=3;
      for(i=0;i<=3;i++)
            {
            p_x[discrete_count] += coord[i+j][0]* Math.pow(u1,i) * Math.pow(1-u1,val-i) *(fact(val)/(fact(i)*fact(val-i))) ;
            p_y[discrete_count] += coord[i+j][1]* Math.pow(u1,i) * Math.pow(1-u1,val-i) *(fact(val)/(fact(i)*fact(val-i))) ;
            }
      //discrete_count++;
            //u1+=inc;
    }

        @Override
    public void actionPerformed(ActionEvent e) {
	
               
               
    if(e.getSource() == resetflag)
      {
          index = -1;
        text="Clean Slate!";
          System.out.println("Inside reset");
        //String msg= " ";
        drawNN = false;
        drawCrust_flag = false;
        crust_flag = false;
        nncrust_flag = false;
        isclosed  = false;
        wireinput_flag = false;
        isButtonPressed = false;
        edit_flag = false;
        delete_flag = false;
        duplicate_flag = false;
        reset_flag = false;
        coord = new float[100][2];   // the mouse coordinates
        p_x = new float[100000];
        p_y = new float[100000];
        p = new float[100000][2];
        drawn = false;
        discrete_count = 0;
        drag_index =-1;
        delete_index =-1;
        degree_bspline =-1;
        repaint();
      }
    else if(e.getSource() == wireinput)
    {
        wire_x = Arrays.copyOf(p_x, p_x.length);
        wire_y = Arrays.copyOf(p_y, p_y.length);
        wireinput_flag = true;
        index = -1;
        wirelength = discrete_count;
        isButtonPressed = false;
        edit_flag = false;
        delete_flag = false;
        duplicate_flag = false;
        reset_flag = false;
        coord = new float[100][2];   // the mouse coordinates
        p_x = new float[100000];
        p_y = new float[100000];
        p = new float[100000][2];
        drawn = false;
        discrete_count = 0;
        drag_index =-1;
        delete_index =-1;
        degree_bspline =-1;
        repaint();
        
    }
    else if(e.getSource() == redraw)
    {
        redraw_flag = true;
        MouseEvent e1 = null;
        mouseReleased(e1);
    }
           else if(e.getSource() == editflag)
                {
                    edit_flag = true;
                }
            else if(e.getSource() == deleteflag)
            {
                text = "Please delete Point";
                delete_flag = true;
            }
            else if(e.getSource() == duplicateflag)
            {
                text = "Please select point to duplicate";
                duplicate_flag = true;
            }
            else if(e.getSource() == drawCrust)
             {
             drawCrust_flag = true;
             crust_flag = false;
             repaint();
                     
             }
            else if(e.getSource() == drawNNCrust)
            {
                System.out.println("CAME to NNCURST BUTTON");
                drawNN = true;
                repaint();
            }
            else if(e.getSource() == rationalBezier)
            {
                    text = "Please enter weights for rational Bezier: comma separated. For random, keep it blank";
                    int local = 0;
                    bezier_active = true;
                            float inc;
                float w[] = new float[index+1];
                
                if((tf.getText()).equals("") == true)
                {
                        do
                    {
                        w[local] = (float)(Math.random()*50.0f+1);
                        local++;
                        
                    }while(local<=index);
                    rationalBezier(w);
                    drawn = true;
                    repaint();
                }
                else 
                {
                    String wt[] = (tf.getText()).split(",");
                    if(wt.length != (index+1))
                    {
                        System.out.println("Not enough weights");
                        text = "Not enough weights. Either keep text empty or enter proper weight.";
                        repaint();
                        return;
                    }
                    System.out.println("Weight values are:");
                    for(int i =0;i<=index;i++)
                    {
                        w[i] = Float.parseFloat(wt[i]);
                        System.out.print(w[i]+" ");
                    }
                    rationalBezier(w);
                    drawn = true;
                    repaint();
                    
                    //for(int i=0;i)
                    
                }
  
                
                
                    
                }
	}
    
        @Override
     public void mouseEntered( MouseEvent e ) {
      // called when the pointer enters the applet's rectangular area
   }
        @Override
   public void mouseExited( MouseEvent e ) {
      // called when the pointer leaves the applet's rectangular area
   }
        @Override
   public void mouseClicked( MouseEvent e ) {
      // called after a press and release of a mouse button
      // with no motion in between
      // (If the user presses, drags, and then releases, there will be
      // no click event generated.)
   }
        @Override
   public void mousePressed( MouseEvent e ) {  // called after a button is pressed down
      isButtonPressed = true;
      if(edit_flag == true)
      { float min = 100000000.0f, dist;
          for(int i =0;i<=index;i++)
          {
              dist= (float)Math.sqrt((Math.pow((e.getX()-coord[i][0]),2) + Math.pow((e.getY()-coord[i][1]),2) ));
              if(min > dist)
              {
                 min = dist;
                 drag_index = i;
                  
              }
          }
      }
      //e.consume();
      else if(delete_flag == true)
      {
          System.out.println("\nInside delete flag");
          float min = 100000000.0f, dist;
          for(int i =0;i<=index;i++)
          {
              dist= (float)Math.sqrt((Math.pow((e.getX()-coord[i][0]),2) + Math.pow((e.getY()-coord[i][1]),2) ));
              if(min > dist)
              {
                 min = dist;
                 delete_index = i;
                  
              }
          }
          //int[][] coor_new = new int[index][2];
          for(int i=delete_index;i<index;i++)
          {
              coord[i][0] = coord[i+1][0];
              coord[i][1] = coord[i+1][1];
          }
          //coord = coor_new;
          index--;
          drawn = false;
          repaint();
          //delete_flag = false;
      }
      else if(duplicate_flag ==true)
      {
          int duplicate_index = 0;
          System.out.println("\nInside duplicate flag");
          float min = 100000000.0f, dist;
          for(int i =0;i<=index;i++)
          {
              dist= (float)Math.sqrt((Math.pow((e.getX()-coord[i][0]),2) + Math.pow((e.getY()-coord[i][1]),2) ));
              if(min > dist)
              {
                 min = dist;
                 duplicate_index = i;
                  
              }
          }
          index ++;
          for(int i=index; i>duplicate_index; i--)
          {
              coord[i][0] = coord[i-1][0];
              coord[i][1] = coord[i-1][1];
          }
          drawn = false;
          repaint();
          //coord[duplicate_index][0] = coord[duplicate_index][0]
      }
      
     if(e!=null) e.consume();
   }
   void check_closed(float coord[][],int index)
   {
       float min = 100000;
       int pos =-1;
       for(int i=0;i<index;i++)
       {
           float x1 = coord[i][0], y1 = coord[i][1];
           float x2 = coord[index][0], y2 = coord[index][1];
           float dist = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) ;
           if(min>dist) {min = dist;    pos = i;}
           
       }
       
       //System.out.println("\n\n\nDist: "+min+"\n\n\n");
       if(min<=5)
       {    System.out.println("Curve is closed");
           coord[index][0] = coord[pos][0];
           coord[index][1] = coord[pos][1];
            isclosed = true;
       }
       else {isclosed = false;      System.out.println("Curve is not closed: "+min);}
       
   }

   @Override
   public void mouseReleased( MouseEvent e ) {  // called after a button is released
      boolean change_flag = false;
       if(edit_flag == true && drag_flag == false)
      {
          edit_flag = false;
          coord[drag_index][0] = e.getX();
          coord[drag_index][1] = e.getY();
        if(e!=null)  e.consume();
          change_flag = true;
          //repaint();
          //return;
      }
      if(delete_flag == true && drag_flag == false)
      {
          delete_flag = false;
         if(e!=null) e.consume();
          change_flag = true;
          //return;
      }
      if(duplicate_flag == true)
      {
          duplicate_flag = false;
         if(e!=null) e.consume();
          change_flag = true;
          //return;
      }
       isButtonPressed = false;
      if(redraw_flag ==true ||drag_flag ==true ||( e.getX()<=596  && e.getX()>=102 && e.getY()<=596  && e.getY()>=102))
      {
          quad_active = false;
          if(redraw_flag == false && drag_flag == false)
          {
              if(change_flag == false)
          {
            index++;
            coord[index][0] = e.getX();
            coord[index][1] = e.getY();
            text = "Mouse at (" + coord[index][0] + "," + coord[index][1] + ")";
            check_closed(coord,index);
            //setBackground( Color.black );
            repaint();  
          }
              
          }
          else if(drag_flag == true) drag_flag = false;
          else redraw_flag =false;
          
      
      Checkbox chk = grp1.getSelectedCheckbox();
      if((chk.getLabel()).equals("Quad B-Spline subdivision")==true  && index >=1)
      {
          bezier_active = false;
          quad_active = true;
          text = "Don't forget to enter value of 'm' !";
          repaint();
          float[][] coordd = new float[index+1][2];
          int m = 0,i;
          if((tf.getText()).equals("") == true || Integer.parseInt(tf.getText())>10)
            m = 3;
          else m = Integer.parseInt(tf.getText());
                        for( i = 0; i <= index; i++)
                        {
                            coordd[i][0] = coord[i][0];
                            coordd[i][1] = coord[i][1];                        
                        }
          float interm[][];
          int local_count =-1;
          interm = oneCornerLoop(coordd,index);
          for(i=1;i<=m;i++)
          {
                interm = oneCornerLoop(interm,discrete_count);
          }
          while(local_count<=discrete_count)
                        {
                            local_count ++;
                            p_x[local_count] = interm[local_count][0];
                            p_y[local_count] = interm[local_count][1];
                            
                        }
          drawn = true;
          repaint();
          
      }
      else if((chk.getLabel()).equals("NNCrust Algorithm")==true && index > 1 )
      {
          drawn = true;
      }
      else if((chk.getLabel()).equals("Arbitrary B-Spline")==true && index == 5 )
      {
          quad_active = false;
          int t[] = {0,0,0,1,2,3,4,4,4};
          //System.out.println("Got inside");
          int d =3;
          if((tf.getText()).equals(""))
              d=3;
          else
              Integer.parseInt(tf.getText());
              float [][][] N = new float[index+2][d+1][index+d+1];    //i,D,u
          arbitraryBSpline(N, d, t);
          for(int i=0;i<index+2;i++)
              for(int j =0;j<d;j++)
                  for(int k=0;k<index+d+1;k++)
                      if(N[i][j][k]==1)System.out.print(N[i][j][k]);
          
      }
      
      else if((chk.getLabel()).equals("Bezier C1 continuous")==true && index >=1 )
      {
          quad_active = false;
          bezier_active = true;
          text = "Don't forget to change value of u in slider !";
          repaint();
          int value = uval.getValue();
          //int value = (Integer.parseInt(tf.getText()));
          float inc = ((float)value)/1000.0f;
          System.out.println("Value of inc = "+inc);
          /*if((tf.getText()).equals("") == true)
                inc = 0.002f;
          else inc = Float.parseFloat(tf.getText());*/
          c1cont(inc);
          c1cont(inc);
          c1contflag = true;
         // discrete_count--;
          //discrete_count--;
          drawn = true;
          repaint();
      }
      else if((chk.getLabel()).equals("Bezier DeCastel")==true  && index >=1)
                {
                    quad_active = false;
                        bezier_active = true;
                        String str = tf.getText();
                    	System.out.println(str);
                        text = "Don't forget to put number of iterations 'm' ! Value current:" + tf.getText();
                        float[][] coordd = new float[index+1][2];
                        for(int i = 0; i <= index; i++)
                        {
                            coordd[i][0] = coord[i][0];
                            coordd[i][1] = coord[i][1];                        
                        }
                         
                        float interm[][];// =  new int[1000][2];
                        //int local_count= 0;
                        if((tf.getText()).equals("") == true)
                            interm = subDivide(coordd, index, 2, 0.5f);
                        else
                            interm = subDivide(coordd, index, Integer.parseInt(tf.getText()),0.5f);
                       // discrete_count = -1;
                        drawn = true;
                        for(int i=0;i<interm.length;i++)
                        {
                            //discrete_count ++;
                            p_x[i] = interm[i][0];
                            p_y[i] = interm[i][1];
                            //local_count ++;
                        }
                        discrete_count = interm.length;
                       // index = 13;
                       //System.out.println("calling re-paint "+discrete_count+" "+drawn);
                        repaint();  
                }
      else if ((chk.getLabel()).equals("Cubic B-Spline")==true)
      {
          quad_active = false;
          bezier_active = false;
        System.out.println("B-Spline was pressed");
        text = "Don't forget to put value of discretization 'u' !";
        drawn = true;
        int value = uval.getValue();
        //int value = (Integer.parseInt(tf.getText()));
          float inc = ((float)value)/1000.0f;
          System.out.println("Value of inc = "+inc);
        /*if((tf.getText()).equals("") == true)
                inc = 0.002f;
          else inc = Float.parseFloat(tf.getText());
        */
        BSpline(inc);
        discrete_count--;
        repaint();
      }
      else if((chk.getLabel()).equals("Bezier curve")==true)
      {
          /*float inc = 0.02f;          
          if((tf.getText()).equals("") == true)
                inc = 0.002f;
          else inc = Float.parseFloat(tf.getText());
          if(inc>0.9f)
          {
              text="Too large value of inc";
              inc = 0.002f;
          }*/
          quad_active = false;
          bezier_active = true;
          int value = uval.getValue();
        //  int value = (Integer.parseInt(tf.getText()));
          float inc = ((float)value)/1000.0f;
          System.out.println("Value of inc = "+inc);
          
                    System.out.println("Bezier Sample active");
                    text = "Bezier Sample active. Don't forget value of discretization u !";
                    drawn = true;
                    BezierPoints(inc);
                    repaint();          
      }
    if(e!=null)  e.consume();
      }
   }
        @Override
   public void mouseMoved( MouseEvent e ) {  // called during motion when no buttons are down
      int mx = e.getX();
      int my = e.getY();
      if( e.getX()<=596  && e.getX()>=102 && e.getY()<=596  && e.getY()>=102)
       setCursor(Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR));
      else
          setCursor(Cursor.getDefaultCursor());
    //  showStatus( "Mouse at (" + mx + "," + my + ")" );
     // repaint();
     
      //e.consume();
   }
        @Override
   public void mouseDragged( MouseEvent e ) {  // called during motion with buttons down
      int mx = e.getX();
      int my = e.getY();
      //showStatus( "Mouse at (" + mx + "," + my + ")" );
      text = "Mouse at (" + mx + "," + my + ")";
      if(edit_flag == true)
      {
        coord[drag_index][0] = e.getX();
        coord[drag_index][1] = e.getY();
        //mouseReleased()
        drag_flag = true;
        MouseEvent e1 = null;
        mouseReleased(e1);
        repaint();
      }
      
   if(e!=null)   e.consume();
   }
 
   int find(float x, float y, float coord[][], int index)
   {
       for(int i=0;i<=index;i++)
           if(coord[i][0]==x && coord[i][1]==y)
               return i;
       return -1;
   }
   boolean find2(float x,float y, float coordres[][], int index)
   {
       for(int i=0;i<=index;i++)
           if(coordres[i][0] == x && coordres[i][1] == y)
               return true;
       return false;
   }
   public void joinNN2(float coord[][],int nearest[], int second_nearest[], int index, float coordres[][])
   {
       int i=0;
       int ni,sni;
       coordres[0] = coord[0];
       System.out.println("NN2("+coordres[0][0]+","+coordres[0][1]+")");
       do{
           int ind = find(coordres[i][0], coordres[i][1],coord, index);
           ni = nearest[ind];
           sni = second_nearest[ind];
           boolean found = find2(coord[ni][0],coord[ni][1],coordres,i);
           if(found ==false)    //push the second one
           {
               i++;
               coordres[i][0] = coord[ni][0];
               coordres[i][1] = coord[ni][1];
           }
           else //push the first one
           {
               i++;
               coordres[i][0] = coord[sni][0];
               coordres[i][1] = coord[sni][1];
           }
           System.out.println("("+coordres[i][0]+","+coordres[i][1]+")");
       }while(i!=index);
       
   }
public int joinNN(float coord[][],int nearest[], int second_nearest[], int index, float coordres[][])
{
    int cindex = 0,i;
    
    
    coordres[cindex][0] = coord[0][0];   coordres[cindex][1] = coord[0][1];
    
    float nx = coord[nearest[0]][0];
    float ny = coord[nearest[0]][1];
    int ni = nearest[0];
    float snx  = -1;//coord[second_nearest[0]][0];
    float sny  = -1;//coord[second_nearest[0]][1];
    int sni = second_nearest[0];
     System.out.println("sni: "+sni+" ni:"+ni+" nx: "+nx+" ny:"+ny+" snx:"+snx+" sny:"+sny+" coord: "+coord[cindex][0]+" "+coord[cindex][1]);
    // for first vertex
    ++cindex;
    coordres[cindex][0] = nx; coordres[cindex][1] = ny;
    //if((coordres[0][0] == snx && coordres[0][1] == sny)|| sni ==-1)
    for( i=0;i<=index;i++)
    {
        if(coord[i][0] == nx && coord[i][1] == ny)
        {
            ni = nearest[i];
            sni = second_nearest[i];
            nx = coord[ni][0];
            ny = coord[ni][1];
            if(sni !=-1)
            {
                snx = coord[second_nearest[i]][0];
                sny = coord[second_nearest[i]][1];
            }
            else {
                snx =-1;    sny =-1;
            }
        }
     }
    //ni = nearest[ni];
    //sni = second_nearest[ni];
    //        nx = coord[nearest[ni]][0];
    //        ny = coord[nearest[ni]][1];
     //       snx = coord[second_nearest[ni]][0];
     //       sny = coord[second_nearest[ni]][1];
    //if(coordres[0][0] == snx && coordres[0][1] == sny)
    //nx = coord[nearest[ni]][0];
    //ny = coord[nearest[ni]][1];
  //  if(sni!=-1)
   // {
   //     snx = coord[second_nearest[ni]][0];
    //    sny = coord[second_nearest[ni]][1];
  //  }
    System.out.println("sni: "+sni+" ni:"+ni+" nx: "+nx+" ny:"+ny+" snx:"+snx+" sny:"+sny+" coord: "+coord[cindex][0]+" "+coord[cindex][1]);
    
    for(i=1;i<index;i++)
    {
        System.out.println("sni: "+sni+" ni:"+ni+" nx: "+nx+" ny:"+ny+" snx:"+snx+" sny:"+sny+" coord: "+coord[cindex][0]+" "+coord[cindex][1]);
        if((coordres[i-1][0] == snx && coordres[i-1][1] == sny)|| sni ==-1  )
        {
            coordres[i+1][0] = nx;
            coordres[i+1][1] = ny;
         /*   ni = nearest[ni];
            sni = second_nearest[ni];
            nx = coord[nearest[ni]][0];
            ny = coord[nearest[ni]][1];
            snx = coord[second_nearest[ni]][0];
            sny = coord[second_nearest[ni]][1];
*/      System.out.print("First near going.");
            for(int j=0;j<=index;j++)
            {
            if(coord[j][0] == nx && coord[j][1] == ny)
            {
            ni = nearest[j];
            sni = second_nearest[j];
            nx = coord[ni][0];
            ny = coord[ni][1];
            if(sni !=-1)
            {
                snx = coord[sni][0];
                sny = coord[sni][1];
            }
            else {
                snx =-1;    sny =-1;
            }
            }
            }
        }
        else if((coordres[i-1][0] == nx && coordres[i-1][1] == ny) || ni ==-1)
        {
            System.out.print("Second near going.");
            coordres[i+1][0] = snx;
            coordres[i+1][1] = sny;
            /*ni = nearest[sni];
            sni = second_nearest[sni];
            nx = coord[nearest[sni]][0];
            ny = coord[nearest[sni]][1];
            //if(second_nearest[sni]!=-1)
            {snx = coord[second_nearest[sni]][0];
            sny = coord[second_nearest[sni]][1];
            }*/
            for(int j=0;j<=index;j++)
            {
            if(coord[j][0] == snx && coord[j][1] == sny)
            {
            ni = nearest[j];
            sni = second_nearest[j];
            nx = coord[ni][0];
            ny = coord[ni][1];
            if(sni !=-1)
            {
                snx = coord[sni][0];
                sny = coord[sni][1];
            }
            else {
                snx =-1;    sny =-1;
            }
            }
            }
        }
        else {
            System.out.println("ERROR!!!!"+sni);
        }
        ++cindex;
        //coordres[++cindex][0] = nx; coordres[++cindex][1] = ny;
    }
    return cindex;
}
public int join(float coord[][], int index, float coord1[][], float coord2[][], int coord_index, float coordreturn[][])
{
    float coordres[][] = new float[300][2];
    int countres = mergecoord(coord, index, coord1, coord2, coord_index, coordres, true);
    int j=0,i;
    float temp[][] = new float[300][2];
    //REVERSAL
    for(i=countres;i>=0;i--)
    {
        temp[j][0] = coordres[i][0];
        temp[j][1] = coordres[i][1];
        j++;
    }
    for(i=0;i<=countres;i++)
    {
        coordres[i][0] = temp[i][0];
        coordres[i][1] = temp[i][1];
        System.out.println("("+coordres[i][0]+" , "+coordres[i][1]+")");
    }
    System.out.println("Part 2");
    int k=0;
    //APPEND COORD
    for(i=countres+1;i<=countres+index+1;i++)
    {
        
        coordres[i][0] = coord[k][0];
        coordres[i][1] = coord[k][1];
        k++;
        System.out.println("("+coordres[i][0]+" , "+coordres[i][1]+")");
    }
    countres = i-1;
    int countres2 = mergecoord(coordres, countres, coord1, coord2, coord_index, coordreturn, false);

    for(i=0;i<=countres2+countres;i++)
        coordreturn[i+countres2] = coordres[i];
    
    return countres2+countres;
}
public int mergecoord(float coord[][], int index, float coord1[][], float coord2[][], int coord_index, float coordres[][], boolean start)
{
    System.out.println("Entered mergecoord");
    float wfx,wfy;
    int countres = -1;
    if(start == true)
    {countres++; wfx= coord[0][0]; wfy = coord[0][1];}
    else 
        {countres++; wfx= coord[index][0]; wfy = coord[index][1];}
    float prevx =-1, prevy =-1;
    coordres[0][0] = wfx;  
    coordres[0][1] = wfy;
     System.out.println("("+coordres[0][0]+" , "+coordres[0][1]+")");
    boolean notf = false, danger = false;
    
    while(notf == false)
    {
        
    for(int i=0;i<=coord_index;i++)
    {
        if(coord1[i][0] == wfx && coord1[i][1] == wfy && coord2[i][0] != prevx && coord2[i][1] != prevy )
        {
            countres++;
            wfx = coord2[i][0];
            wfy = coord2[i][1];
            prevx = coord1[i][0];
            prevy = coord1[i][1];
            coordres[countres][0] = wfx;  
            coordres[countres][1] = wfy;
            System.out.println("coord1("+coordres[countres][0]+" , "+coordres[countres][1]+")");
            
             
            break;
        }
        else if(coord2[i][0] == wfx && coord2[i][1] == wfy && coord1[i][0] != prevx && coord1[i][1] != prevy)
        {
            countres++;
            wfx = coord1[i][0];
            wfy = coord1[i][1];
            prevx = coord2[i][0];
            prevy = coord2[i][1];
            coordres[countres][0] = wfx;  
            coordres[countres][1] = wfy;
            System.out.println("coord2("+coordres[countres][0]+" , "+coordres[countres][1]+")");
            
            break;
        }
        else if(i==coord_index)
        {
            notf = true;
            danger = true;
            break;      
        }

    }
            if(countres >= coord_index)
            break;
    }
    if(danger == true)
        System.out.println("All value not found");
    if(countres > coord_index + 1)
        System.out.println("Value greater");
    System.out.println("Outside");
    int i,j;
    for( i=0, j=countres;i<index;i++,j++)
    {
        
        //coordres[j][0] = coord[i][0];
        //coordres[j][1] = coord[i][1];
        //countres++;
       // System.out.println("("+coordres[j][0]+" , "+coordres[j][1]+")");
    }
    
    System.out.println("finally");
    for( i=0;i<=countres;i++)
        System.out.println("("+coordres[i][0]+" , "+coordres[i][1]+")");
    return countres;
}
public int edgecoord(float coord1[][], float coord2[][])
{
    FileInputStream fstream;
    int coord_index =-1;
    try
    {
        fstream = new FileInputStream("C:\\Users\\mesay\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\Meshing\\Meshing\\output\\crust_inter2.txt");
        DataInputStream in = new DataInputStream(fstream);  
        BufferedReader br = new BufferedReader(new InputStreamReader(in));
        String strLine = " ";
        //System.   out.printf(strLine +  "viva ");
          while ((strLine = br.readLine()) != null)   {
            if(strLine.length()>0 ){
                String[] tokens = strLine.split(" ");
                coord_index++;
                coord1[coord_index][0] = Float.parseFloat(tokens[0]);
                coord1[coord_index][1] = Float.parseFloat(tokens[1]);
                coord2[coord_index][0] = Float.parseFloat(tokens[2]);
                coord2[coord_index][1] = Float.parseFloat(tokens[3]);
                
            }
              
            
            }
   in.close();
    }
    catch(Exception e)
    {
        System.out.println("Error:"+e);
    }
    return coord_index;
}
public void paintcoord(boolean flag)
{
    index=-1;
    
    FileInputStream fstream;
    try{
        if(flag == true)
            fstream = new FileInputStream("C:\\Users\\mesay\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\Meshing\\Meshing\\input\\coord.txt");
        else
            fstream = new FileInputStream("C:\\Users\\mesay\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\Meshing\\Meshing\\output\\crust_inter.txt");
    
        
        DataInputStream in = new DataInputStream(fstream);  
        BufferedReader br = new BufferedReader(new InputStreamReader(in));
        String strLine = " ";
        if(flag == true)    strLine = br.readLine();
        //System.out.printf(strLine +  "viva ");
          while ((strLine = br.readLine()) != null)   {
            if(strLine.length()>0 ){
                String[] tokens = strLine.split(" ");
                index++;
                coord[index][0] = Float.parseFloat(tokens[0]);
                coord[index][1] = Float.parseFloat(tokens[1]);
            }
              
            
            }
   in.close();
   }
    catch (Exception e){
     System.err.println("Error: " + e.getMessage());
   }
         
}

public void writecoord()
{
    try
    {
        PrintWriter writer = new PrintWriter("C:\\Users\\mesay\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\Meshing\\Meshing\\input\\coord.txt", "UTF-8");
        writer.println(index+"\n");
        for(int i=0;i<=index;i++)
        {
            writer.println(coord[i][0]+" "+coord[i][1]+"\n");
        }
        writer.close();
    }
        catch(Exception e)
        {
            System.err.println(e);
        }
    
}
public void sweep(float p_x[],float p_y[], float p_z[], int slice)
{
           float px1,py1,pz1;
       
///////////////////////////////////////////           
////////wirelength: 21 items from 0 to 20
///////////////////////////////////////////
       
           
    try{
        PrintWriter writer = new PrintWriter("\"C:\\\\Users\\\\mesay\\\\OneDrive\\\\Documents\\\\Visual Studio 2015\\\\Projects\\\\Meshing\\\\Meshing\\input\\sweep.off", "UTF-8");
        writer.println("OFF");
        writer.println((wirelength*(discrete_count))+" "+((wirelength-1)*(discrete_count-1))+" 0");
            
        //System.out.println("discrete_count: "+discrete_count+" wire_length:"+wirelength);
        
        for(int z = 0; z<wirelength; z++)
        {
            float ychange,xchange;
            float normalwire1[] = new float[2];
            float normalwire2[] = new float[2];;
            if(z!=0)
            {
                ychange = wire_y[z] - wire_y[z-1];
                xchange = wire_x[z] - wire_x[z-1];
                normalwire1[0] = -ychange;
                normalwire1[1] = xchange;
                normalwire2[0] = ychange;
                normalwire2[1] = -xchange;
            }
            else
            {
                ychange = 0;
                xchange = 0;
                normalwire1[0] = 0;
                normalwire1[1] = 0;
                normalwire2[0] = 0;
                normalwire2[1] = 0;
            }
            
        for(int i=0;i<discrete_count;i++)
        {
            //float xchange = wire_x[z-1] - wire_x[z];
            //px1 = p_x[i]+xchange;
            float norm1[] = new float[2];
            float norm2[] = new float[2];
            if(i!=0)
            {
                norm1[0] = -(p_y[i]-p_y[i-1]);
                norm1[1] = p_x[i]-p_y[i-1];
                norm2[0] = p_y[i]-p_y[i-1];
                norm2[1] = -(p_x[i]-p_y[i-1]);
                px1 = p_x[i]-p_x[i-1];
            }
            else px1 = 0;
            py1 = p_y[i]+ychange;
            //writer.println(p_x[i]+" "+py1+" "+wire_x[z]);
            writer.println((wire_x[z]+px1)+" "+wire_y[z]+" "+p_y[i]);
        } 
        }
        
       int count = 0, j, i;
       float z = -50.0f;
       for(count= 0;count<slice;count++)
       {
           p_z[count] = z;
           z+= 10.0f;       
       }
       
       for(j=0;j<wirelength-1;j++)
       {for(i = 0;i<discrete_count-1;i++)
           {
               writer.print("4 "+(i+j*(discrete_count))+" "+(i+1+j*(discrete_count)));
               writer.println(" "+(discrete_count+i+1+j*(discrete_count))+" "+(discrete_count+i+j*(discrete_count)));
           }
       
       }
       writer.close();
            
    }       
        catch(Exception e)
        {
            System.err.println(e);
        }

    
    
}

public boolean intersection(float x1,float y1,float x2, float y2, float a1,float b1,float a2, float b2)
{
    Line2D line1 = new Line2D.Float(x1, y1, x2, y2);
    Line2D line2 = new Line2D.Float(a1, b1, a2, b2);
    boolean result = line2.intersectsLine(line1);
    return result;
}
    public double determinant(double A[][],int N)

    {
        double det=0;
        if(N == 1)
            det = A[0][0];
        else if (N == 2)
            det = A[0][0]*A[1][1] - A[1][0]*A[0][1];
        else
        {
            det=0;
            for(int j1=0;j1<N;j1++)
            {
                double[][] m = new double[N-1][];
                for(int k=0;k<(N-1);k++)
                    m[k] = new double[N-1];
                for(int i=1;i<N;i++)
               {
                    int j2=0;
                    for(int j=0;j<N;j++)
                    {
                        if(j == j1)
                            continue;
                        m[i-1][j2] = A[i][j];
                        j2++;
                    }
                }
                det += Math.pow(-1.0,1.0+j1+1.0)* A[0][j1] * determinant(m,N-1);
            }

        }
        //System.out.println("Value of det:"+det);
        return det;
    }
    
public boolean intersection2(float x1,float y1,float x2, float y2, float a1,float b1,float a2, float b2)
{
    double A1[][]={{x1,y1,1},{x2,y2,1},{a1,b1,1}};
    double A2[][]={{x1,y1,1},{x2,y2,1},{a2,b2,1}};
    double val1 = determinant(A1, 3) ;
    double val2 = determinant(A2, 3) ;
    //System.out.println("val1: "+val1+" val2: "+val2);
    System.out.println("val1-val2: "+(val1-val2));
    if((val1<0.0 && val2>0)||(val1>0 && val2<0))
        if(val1-val2>0.0005) return true;
    double A3[][]={{a1,b1,1},{a2,b2,1},{x1,y1,1}};
    double A4[][]={{a1,b1,1},{a2,b2,1},{x2,y2,1}};
    val1 = determinant(A3, 3) ;
    val2 = determinant(A4, 3) ;
    //System.out.println("val3:"+val1+"val4:"+val2);
    System.out.println("val3-val4: "+(val1-val2));
    if((val1<0 && val2>0)||(val1>0 && val2<0))
        if(val1-val2>0.0005) return true;
    return false;
}
public boolean intersectionPairChec(float [][]pointset, int index)
{
   // System.out.print("index= "+index);
    for(int i=1;i<index;i++)
    {
        boolean res = intersection(pointset[i-1][0],pointset[i-1][1],pointset[i][0],pointset[i][1],pointset[index-1][0],pointset[index-1][1],pointset[index][0],pointset[index][1]);
        //System.out.print(" ("+pointset[i-1][0]+","+pointset[i-1][1]+")");
        //System.out.print("("+pointset[i][0]+","+pointset[i][1]+")intersect with");
        //System.out.print("("+pointset[index-1][0]+","+pointset[index-1][1]+")");
        //System.out.print("("+pointset[index][0]+","+pointset[index][1]+")\n");
        if(res == true)
        {
            if(i!=index-1)
            {
                System.out.print(" ("+pointset[i-1][0]+","+pointset[i-1][1]+")");
                System.out.print("("+pointset[i][0]+","+pointset[i][1]+")intersect with");
                System.out.print("("+pointset[index-1][0]+","+pointset[index-1][1]+")");
                System.out.print("("+pointset[index][0]+","+pointset[index][1]+")\n");
                System.out.println("returns true");
                return true;                
            }
            
        }
        
        
    }
    return false;
}
public  float[] removeDuplicates(float[] A,int discrete_count) {
	if (discrete_count < 2)
		return A;
 
	int j = 0;
	int i = 1;
 
	while (i < discrete_count) {
		if (A[i] == A[j]) {
			i++;
		} else {
			j++;
			A[j] = A[i];
			i++;
		}
	}
 
	float[] B = Arrays.copyOf(A, j + 1);
 
	return B;
}
    // Variables declaration - do not modify                     
   public boolean revolution(float p_x[],float p_y[], float p_z[], int slice)
   {
       
       float px1,py1,pz1,theta,inc = (2*3.141f)/(slice);
       //removeDuplicates(p_x, discrete_count);
       //removeDuplicates(p_y, discrete_count);
       boolean finalres = false;
       int countl = -1;
       float[][] pointset = new float[discrete_count][2];
        try{
        PrintWriter writer = new PrintWriter("C:\\Users\\mesay\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\Meshing\\Meshing\\input\\revolution.off", "UTF-8");
        PrintWriter writer2 = new PrintWriter("C:\\Users\\mesay\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\Meshing\\Meshing\\input\\revolutionpoints.off", "UTF-8");
        writer.println("OFF");
        writer2.println(slice+" "+discrete_count+"\n");
        writer.println((slice*(discrete_count))+" "+((slice)*(discrete_count-1))+" 0");
            //System.out.println("discrete_count "+discrete_count);
        for(float z = 1;z<=slice;z++)
        for(int i=0;i<discrete_count;i++)
        {
            theta = (float)(inc*z);
            px1 = (float)(Math.cos(theta)*p_x[i] - Math.sin(theta)*z);
            pz1 = (float)(Math.sin(theta)*p_x[i] + Math.cos(theta)*z);
            writer.println(px1+" "+p_y[i]+" "+pz1);
            writer2.println(px1+" "+p_y[i]+" "+pz1);
            if(z==1 && i%5 == 0)
            {
                countl ++;
                //if(countl!=0 && p_x[i] != pointset[countl-1][0] && p_y[i] != pointset[countl-1][1])
                       pointset[countl][0] = p_x[i];
                        pointset[countl][1] = p_y[i];
                
                if(finalres!=true)finalres = intersectionPairChec(pointset,countl);
                
            }
        }
        writer2.close();
      
       for(int i=0;i<discrete_count;i++)
            System.out.print("("+p_x[i]+","+p_y[i]+")\t");
        
       System.out.println("\nFrom revoltion. Final: "+finalres); 
       int count = 0, j, i;
       
       float z = -50.0f;
       for(count= 0;count<slice;count++)
       {
           p_z[count] = z;
           z+= 10.0f;       
       }
       for(j=0;j<slice;j++)
       {for(i = 0;i<discrete_count-1;i++)
           {
               if(j==slice-1)
               {
                   int expr = (i+j*discrete_count)%discrete_count;
                   writer.print("4 "+(i+j*discrete_count)+" "+(i+1+j*(discrete_count)));
                   writer.println(" "+(expr+1)+" "+(expr));
                 
                   
               }
               else
               {
               writer.print("4 "+(i+j*(discrete_count))+" "+(i+1+j*(discrete_count)));
               writer.println(" "+(discrete_count+i+1+j*(discrete_count))+" "+(discrete_count+i+j*(discrete_count)));
                       
               }
               
           }
       
       }
       writer.close();
            
        }       
        catch(Exception e)
        {
            System.err.println(e);
        }
       return finalres;
   }
   
   public void extrusion(float p_x[],float p_y[], float p_z[], int slice)
   {
       
        try{
            System.out.println("disc: "+discrete_count);
        PrintWriter writer = new PrintWriter("C:\\Users\\mesay\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\Meshing\\Meshing\\input\\extrusion.off", "UTF-8");
        PrintWriter writer2 = new PrintWriter("C:\\Users\\mesay\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\Meshing\\Meshing\\input\\extrusionpoints.off", "UTF-8");
        writer.println("OFF");
        float [][] pxc = new float[slice][discrete_count];
        float [][] pyc = new float[slice][discrete_count];
        float [][] pzc = new float[slice][discrete_count];
        writer.println((slice*(discrete_count))+" "+((slice-1)*(discrete_count-1))+" 0");
            //System.out.println("discrete_count "+discrete_count);
            writer2.println(slice+" "+discrete_count+"\n");
        for(int z = 1;z<=slice;z++)
        {
        for(int i=0;i<discrete_count;i++)
        {
            pxc[z-1][i] = p_x[i];
            pyc[z-1][i] = p_y[i];
            pzc[z-1][i] = p_z[i];
            writer.println(p_x[i]+" "+p_y[i]+" "+(float)z*10);
            writer2.println(p_x[i]+" "+p_y[i]+" "+(float)z*10);
        } 
        }
        writer2.close();
       int count = 0, j, i;
       float z = -50.0f;
       for(count= 0;count<slice;count++)
       {
           p_z[count] = z;
           z+= 10.0f;       
       }
       for(j=0;j<slice-1;j++)
       {for(i = 0;i<discrete_count-1;i++)
           {
               writer.print("4 "+(i+j*(discrete_count))+" "+(i+1+j*(discrete_count)));
               writer.println(" "+(discrete_count+i+1+j*(discrete_count))+" "+(discrete_count+i+j*(discrete_count)));
           }
       
       }
       writer.close();
            
        }       
        catch(Exception e)
        {
            System.err.println(e);
        }
       
   }
   
   public void lofting(int slice)
   {
       int i,j;
       try{
        PrintWriter writer = new PrintWriter("C:\\Users\\mesay\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\Meshing\\Meshing\\lofting.off", "UTF-8");
        writer.println("OFF");
        writer.println((slice*(discrete_count))+" "+((slice-1)*(discrete_count-1))+" 0");
            //System.out.println("discrete_count "+discrete_count);
            System.out.print("ORIGINAL");
            for(i=0;i<discrete_count;i++)
          {
              
            System.out.print("("+p_x[i]+","+p_y[i]+")");
            
          }
            System.out.println("WIRE");
             for(i=0;i<wirelength;i++)
          {
              
            System.out.print("("+wire_x[i]+","+wire_y[i]+")");
            
          }
       for(i=0;i<discrete_count;i++)
       {
           float x1 = p_x[i], y1 = p_y[i]; 
           float x2 = wire_x[i], y2 = wire_y[i]; 
           float dist = (float)Math.sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
           for(j=1;j<=slice;j++)
           {
               float z = 5*dist/j;//j
               float x = x1 + 5/j;//(x2-x1)/j;
               float y = y1+ (x-x1)*((y2-y1)/(x2-x1));
               writer.println(x+" "+(y)+" "+(z)); 
               
           }
           
       }
       for(j=0;j<slice-1;j++)
       {for(i = 0;i<discrete_count-1;i++)
           {
               writer.print("4 "+(i+j*(discrete_count))+" "+(i+1+j*(discrete_count)));
               writer.println(" "+(discrete_count+i+1+j*(discrete_count))+" "+(discrete_count+i+j*(discrete_count)));
           }
       
       }
       writer.close();
            
        }       
        catch(Exception e)
        {
            System.err.println(e);
        }
       
       
   }
    public void writetoFile(float p_x[],float p_y[])
    {
        try{
        PrintWriter writer = new PrintWriter("C:\\Users\\mesay\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\Meshing\\Meshing\\newFile.off", "UTF-8");
        writer.println("OFF");
        writer.println(discrete_count+" "+(discrete_count)+" 0");
        for(float z = -1;z<=10;z++)
        for(int i=0;i<discrete_count;i++)
            writer.println(p_x[i]+" "+p_y[i]+" "+z);
        //for(int i=0;i<discrete_count;i++)
        //    writer.println("2 "+i+" "+(i+1));
        
        writer.close();
        }
        catch(Exception e)
        {
            System.err.println(e);
        }
    
    }
public float distance(float ax, float ay, float bx, float by)
{   return (((ax-bx)*(ax-bx)) + ((ay-by)*(ay-by)));       }

public boolean angle(float p1[],float p2[], float p3[])//p1 is vertex
{
    double p12 = distance(p1[0],p1[1],p2[0],p2[1]);
    double p13 = distance(p1[0],p1[1],p3[0],p3[1]);
    double p23 = distance(p2[0],p2[1],p3[0],p3[1]);
    double angle = Math.acos((p12+p13-p23)/(2*Math.sqrt(p12)*Math.sqrt(p13)));
    System.out.println("angle"+Math.toDegrees(angle) );
    //sleep(100);
    if(Math.toDegrees(angle)>90)
         return true;
    else return false;
    
}

private boolean angleBetween(float center[], float current[], float previous[]) {

  double angle =  Math.toDegrees(Math.atan2(current[0] - center[0],current[1] - center[1])-
                        Math.atan2(previous[0]- center[0],previous[1]- center[1]));
  if(angle>88)
      return true;
    return false;
}
public boolean NNCrust(int nearest_coord[],int second_ncoord[])
{
    //coord[][],index
    float min, min2, distt;
    int pos, pos2;
    boolean setflag = false;
    for(int p=0;p<=index;p++)
    {   
        min = Float.MAX_VALUE;
        min2 = Float.MAX_VALUE;
        pos =-1;
        pos2 =-1;
        for(int q=0;q<=index;q++)
        {
            if(p != q)
            {
                distt = distance(coord[p][0], coord[p][1], coord[q][0], coord[q][1]);
                if(min> distt)
                {
                    min = distt;
                    pos = q;
                }
            }
        }//q loop
        if(pos == -1)
        { System.out.println("Som Ting Wong");  }
        nearest_coord[p] = pos;
        //pos =-1;
        for(int r=0;r<=index;r++)
        {
            if(r!=p && r!=pos && angle(coord[p], coord[pos], coord[r]))
            {
                System.out.println("Entered returns sum ting");
                distt = distance(coord[p][0], coord[p][1], coord[r][0], coord[r][1]);
                if(min2> distt)
                {
                    min2 = distt;
                    pos2 = r;
                }
            }
        }//r loop
        if(pos2 == -1)
        { System.out.println("Som Ting Wong returns");  }
        else { System.out.println("NoTing Wong");  }
        second_ncoord[p] = pos2;
    }//p loop
    return setflag;
    //return nearest_coord;
}
public static void main(String args[]){
        
        Mesh tr = new Mesh();
        GM2 myFrame = new GM2();
        //tr.DooSabin("C:\\Users\\mesay\\Desktop\\extrusion.off",myFrame.p_x,myFrame.p_y,myFrame.p_z);
	
       // myFrame.writetoFile();
        //myFrame.getContentPane().setBackground(Color.GRAY);
}

}