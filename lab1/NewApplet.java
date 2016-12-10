
package lab1;

import java.awt.*;
import java.awt.event.*;
import java.applet.*;
import javax.swing.*;
import javax.swing.border.TitledBorder;
import java.awt.geom.Line2D;
import java.util.*;

public class NewApplet extends Applet implements ItemListener, ActionListener, MouseListener, MouseMotionListener {


	Button editflag, deleteflag, duplicateflag, rationalBezier, resetflag, redraw;
        //decastel, bsample, cbssample, dragpoints, 
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
        float[][] coord = new float[100][2];   // the mouse coordinates
        int index = -1;
        float p_x[] = new float[100000];
        float p_y[] = new float[100000];
        float p[][] = new float[100000][2];
        boolean drawn = false;
        int discrete_count = 0;
        int drag_index;
        int delete_index;
        int degree_bspline;
        boolean c1contflag = false;
        Checkbox bez_sample, bspline_sample, bez_decastel, bspline_sub, c1cont, arbspline; 
        
 
    @Override
    public void init() 
    {
         uval = new JSlider(JSlider.HORIZONTAL, 0, 300, 150);
        // uval.setHorizontalSliderPolicy(
  // JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
         uval.setValue(50);
       
    uval.setPaintTicks(true);
    uval.setMajorTickSpacing(20);
    uval.setMinorTickSpacing(5);
    uval.setBorder(new TitledBorder("Value of u for discretization"));
    //uval.setModel(uval.getModel()); // Share model
  
    Hashtable labelTable = new Hashtable();
    labelTable.put( 0, new JLabel("0.0") );
    labelTable.put( 50, new JLabel("0.05") );
    labelTable.put( 100, new JLabel("0.1") );
    labelTable.put( 150, new JLabel("0.15") );
    labelTable.put( 200, new JLabel("0.2") );
    labelTable.put( 250, new JLabel("0.25") );
    labelTable.put( 300, new JLabel("0.3") );
    uval.setLabelTable( labelTable );
    uval.setPaintLabels(true);
    add(uval);  
    
        tf = new TextField(10);
        
        add(tf);
        
        bspline_sample = new Checkbox("Cubic B-Spline curve", grp1, true);
        bez_decastel = new Checkbox("Bezier curve using De Casteljau", grp1, true);
        bspline_sub = new Checkbox("Quadratic B-Spline curve using subdivision", grp1, true);
        c1cont = new Checkbox("Bezier C1 continuous", grp1, true);
        arbspline = new Checkbox("Arbitrary B-Spline", grp1, true);
        bez_sample = new Checkbox("Bezier curve", grp1, true);
        add(bspline_sample);
        add(bez_decastel);
        add(bspline_sub);
        add(arbspline);
        add(c1cont);
        add(bez_sample);
        
        bez_sample.addItemListener(this);
        bspline_sample.addItemListener(this);
        bez_decastel.addItemListener(this);
        bspline_sub.addItemListener(this);
        arbspline.addItemListener(this);
        c1cont.addItemListener(this);
        //decastel = new Button("De Casteljau");
	//add(decastel);
	//decastel.addActionListener(this);
        rationalBezier = new Button(" Rational Bezier ");
	add(rationalBezier);
	rationalBezier.addActionListener(this);
        
	editflag = new Button("     Edit Points     ");
	add(editflag);
	editflag.addActionListener(this);
        
        deleteflag = new Button("   Delete Points  ");
	add(deleteflag);
	deleteflag.addActionListener(this);
        
        duplicateflag = new Button("Duplicate Points");
	add(duplicateflag);
	duplicateflag.addActionListener(this);
        
        resetflag = new Button("        Reset          ");
	add(resetflag);
	resetflag.addActionListener(this);
        
        redraw = new Button("       Redraw        ");
	add(redraw);
	redraw.addActionListener(this);
        //bsample = new Button("Bezier Sample");
	//add(bsample);
        //bsample.addActionListener(this); 
        
        //cbssample = new Button("CB-Spline Sample");
	//add(cbssample);
       
	//cbssample.addActionListener(this); 
        addMouseListener( this );
        addMouseMotionListener( this );
    }
@Override
    public void itemStateChanged(ItemEvent ex) {
        //bez_sample, bspline_sample, bez_decastel, bspline_sub, c1cont, arbspline; 
         if (ex.getItemSelectable() == bez_sample)
         {
             text = "Don't forget to put value of discretization 'u' !";
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
         else if(ex.getItemSelectable() == arbspline)
         {
            text = "Wrong value";
            repaint();
         }
    }

    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        setLayout(new java.awt.BorderLayout());
    }// </editor-fold>//GEN-END:initComponents
    @Override
    public void paint(Graphics g){
      g.setFont(new Font("TimesRoman", Font.PLAIN, 20)); 
        g.drawString(text,100,630);
        setBackground(Color.BLUE);
       // Now we tell g to change the color

       Color c= new Color(9,53,13);
      g.setColor(c);
       g.setFont(new Font("TimesRoman", Font.PLAIN, 15)); 
      g.drawString("Select curve:",100,15);
  // This will draw a rectangle (xco,yco,xwidth,height);
      g.drawRect(100,100,500,500);
      g.drawRect(690, 190, 140, 190);
    //  super.paintComponents(uval);
  // This will fill a rectangle
      g.fillRect(102,102,496,496); 
      //add(uval);
      g.setColor(Color.red);
      Graphics2D g2 = (Graphics2D) g;
      
      tf.setLocation(700,150);
        //   add(uval);     
      resetflag.setLocation(700,200);
      redraw.setLocation(700,230);
      editflag.setLocation(700,260);
      deleteflag.setLocation(700,290);
      duplicateflag.setLocation(700,320);
      rationalBezier.setLocation(700,350);
      
      uval.setLocation(700,450);
            // System.out.println("uval:"+uval); 
      bez_sample.setLocation(100,30); bspline_sample.setLocation(100,50);// c1cont, arbspline; 
      bez_decastel.setLocation(250,30); bspline_sub.setLocation(250,50);
      c1cont.setLocation(600,30); arbspline.setLocation(600,50);
//editflag.setLocation(700,230); //deleteflag, duplicateflag, rationalBezier, resetflag, redraw;
      //g2.draw(new Line2D.Double(7.2, 3.5, 10.1, 5.7));
     if(index!=-1)
      { 
          for(int i=0;i<=index;i++)
          {
              g2.draw(new Line2D.Double(coord[i][0]-1,coord[i][1]-1,coord[i][0]+1,coord[i][1]+1));
              g2.draw(new Line2D.Double(coord[i][0]+1,coord[i][1]-1,coord[i][0]-1,coord[i][1]+1));
          }
          g.setColor(Color.gray);
          System.out.println("val:index: "+index);
          for(int i=1;i<=index;i++)
              g2.draw(new Line2D.Double(coord[i-1][0],coord[i-1][1],coord[i][0],coord[i][1]));
              //g.drawLine(coord[i-1][0],coord[i-1][1],coord[i][0],coord[i][1]);
      
      }
       if(drawn == true)
      {
             g.setColor(Color.white);
               //for(int i=0;i<=discrete_count;i++)
                //g.drawOval(coord[i][0],coord[i][1],1,1);
          //g.setColor(Color.gray);
          
        //  System.out.println("\n val:discrete count: "+discrete_count);
        //System.out.println("inside repaint "+discrete_count);
          System.out.println("Discrete_count in paint"+discrete_count);
          if(c1contflag == true){
           for(int i=1;i<discrete_count-2;i++)
          {
              System.out.print("inside c1cont");
              g2.draw(new Line2D.Double(p_x[i-1],p_y[i-1],p_x[i],p_y[i]));
              c1contflag = false;
            if( i % 100 == 0)  System.out.println("("+p_x[i-1]+","+p_y[i-1]+")");
            System.out.print("("+p_x[i-1]+","+p_y[i-1]+")");
          }   
          }
          else
        for(int i=1;i<discrete_count;i++)
          {
              g2.draw(new Line2D.Double(p_x[i-1],p_y[i-1],p_x[i],p_y[i]));
            //if( i % 100 == 0)  System.out.println("("+p_x[i-1]+","+p_y[i-1]+")");
            //System.out.print("("+p_x[i-1]+","+p_y[i-1]+")");
          }
              
          drawn = false;
      }
      
   }
    float[][] subDivide(float coordd[][],int coord_n, int m, float u)
    {
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
    
    float[][] oneSubDivide(float coordd[][], int coordd_n, float poly1[][], int poly_n, float poly2[][], float u, int n)
    {
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

float generatorN(float [][][]N, int d, int []t)
{
    
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
    
float[][][] arbitraryBSpline(float [][][]N, int d, int []t)
{

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

int fact(int n)
    {
        int result;
       if(n==0 || n==1)
         return 1;

       result = fact(n-1) * n;
       return result;
    }
    
    int BezierPoints(float inc)
    {
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
    float[][] oneCornerLoop(float coordd[][], int index)
    {
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
    int BSpline(float inc)
    {
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
    
    
    int rationalBezier(float w[])
    {
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
    boolean isCollinear(float x1,float y1,float x2,float y2,float x3,float y3,int j)
    {
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
void c1cont(float inc)
    {
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
        drawn = false;
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
                else if(e.getSource() == rationalBezier)
                {
                    text = "Please enter weights for rational Bezier: comma separated. For random, keep it blank";
                    int local = 0;
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
          if(redraw_flag != true && drag_flag != true)
          {
              if(change_flag !=true)
          {
            index++;
            coord[index][0] = e.getX();
            coord[index][1] = e.getY();
            text = "Mouse at (" + coord[index][0] + "," + coord[index][1] + ")";
            //setBackground( Color.black );
            repaint();  
          }
              
          }
          else if(drag_flag == true) drag_flag = false;
          else redraw_flag =false;
          
      
      Checkbox chk = grp1.getSelectedCheckbox();
      if((chk.getLabel()).equals("Quadratic B-Spline curve using subdivision")==true  && index >=1)
      {
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
      else if((chk.getLabel()).equals("Arbitrary B-Spline")==true && index == 5 )
      {
          int t[] = {0,0,0,1,2,3,4,4,4};
          System.out.println("Got inside");
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
          text = "Don't forget to change value of u in slider !";
          repaint();
          
          int value = uval.getValue();
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
      else if((chk.getLabel()).equals("Bezier curve using De Casteljau")==true  && index >=1)
                {
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
      else if ((chk.getLabel()).equals("Cubic B-Spline curve")==true)
      {
        System.out.println("B-Spline was pressed");
        text = "Don't forget to put value of discretization 'u' !";
        drawn = true;
        int value = uval.getValue();
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
          int value = uval.getValue();
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
      showStatus( "Mouse at (" + mx + "," + my + ")" );
     // repaint();
     
      //e.consume();
   }
        @Override
   public void mouseDragged( MouseEvent e ) {  // called during motion with buttons down
      int mx = e.getX();
      int my = e.getY();
      showStatus( "Mouse at (" + mx + "," + my + ")" );
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

    // Variables declaration - do not modify//GEN-BEGIN:variables
    // End of variables declaration//GEN-END:variables
}
