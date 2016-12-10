/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab1;

import java.awt.*;
import java.util.Hashtable;
import javax.swing.*;

/**
 *
 * @author mesay
 */
public class GM extends JFrame{// implements ItemListener, ActionListener, MouseListener, MouseMotionListener{

    JButton editflag, deleteflag, duplicateflag, rationalBezier, resetflag, redraw;
    JPanel pan;
    JFrame jf;
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

    
    public GM()
    {
      //  getContentPane().setLayout(new GridLayout(3, 3));
        jf = new JFrame();
        jf.setVisible(true);
        jf.setSize(800,800);
        jf.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    pan = new JPanel();
         uval = new JSlider(JSlider.HORIZONTAL, 0, 300, 150);
        // uval.setHorizontalSliderPolicy(
  // JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
         uval.setValue(50);
       
    uval.setPaintTicks(true);
    uval.setMajorTickSpacing(20);
    uval.setMinorTickSpacing(5);
   // uval.setBorder(new TitledBorder("Value of u for discretization"));
    //uval.setModel(uval.getModel()); // Share model
//    Surface.outputOff();
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
    //add(uval);  
    
        tf = new TextField(10);
        
      //  add(tf);
        
        bspline_sample = new Checkbox("Cubic B-Spline curve", grp1, true);
        bez_decastel = new Checkbox("Bezier curve using De Casteljau", grp1, true);
        bspline_sub = new Checkbox("Quadratic B-Spline curve using subdivision", grp1, true);
        c1cont = new Checkbox("Bezier C1 continuous", grp1, true);
        arbspline = new Checkbox("Arbitrary B-Spline", grp1, true);
        bez_sample = new Checkbox("Bezier curve", grp1, true);
        //add(bspline_sample);
        //add(bez_decastel);
        //add(bspline_sub);
        //add(arbspline);
        //add(c1cont);
        //add(bez_sample);
        
        //bez_sample.addItemListener(this);
        //bspline_sample.addItemListener(this);
        //bez_decastel.addItemListener(this);
        //bspline_sub.addItemListener(this);
        //arbspline.addItemListener(this);
        //c1cont.addItemListener(this);
        //decastel = new Button("De Casteljau");
	//add(decastel);
	//decastel.addActionListener(this);
        rationalBezier = new JButton(" Rational Bezier ");
        rationalBezier.setPreferredSize(new Dimension(40, 40));
	rationalBezier.setBounds(100 , 30, 10 , 5  );
        pan.add(rationalBezier);
        
	//rationalBezier.addActionListener(this);
        
	editflag = new JButton("     Edit Points     ");
        editflag.setPreferredSize(new Dimension(40, 40));
        editflag.setBounds(120 , 30, 10 , 5 );
	pan.add(editflag);
	//editflag.addActionListener(this);
        
        deleteflag = new JButton("   Delete Points  ");
        deleteflag.setPreferredSize(new Dimension(40, 40));
        deleteflag.setBounds( 140 , 30, 10 , 5 );
	pan.add(deleteflag);
	//deleteflag.addActionListener(this);
        
        duplicateflag = new JButton("Duplicate Points");
        duplicateflag.setPreferredSize(new Dimension(40, 40));
        duplicateflag.setBounds(160 , 30, 10 , 5  );
	pan.add(duplicateflag);
	//duplicateflag.addActionListener(this);
        
        resetflag = new JButton("        Reset          ");
        resetflag.setPreferredSize(new Dimension(40, 40));
        resetflag.setBounds(180 , 30 ,10 , 5  );
	pan.add(resetflag);
	//resetflag.addActionListener(this);
        
        redraw = new JButton("       Redraw        ");
        redraw.setPreferredSize(new Dimension(40, 40));
        redraw.setBounds(200 , 30,10 , 5  );
	pan.add(redraw);
        jf.add(pan);
}
    
        @Override
    public void paint(Graphics g){}
  /*      
      g.setFont(new Font("TimesRoman", Font.PLAIN, 20)); 
        g.drawString(text,100,630);
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
      //resetflag.setLocation(700,200);
      //redraw.setLocation(700,230);
      //editflag.setLocation(700,260);
      //deleteflag.setLocation(700,290);
      //duplicateflag.setLocation(700,320);
      //rationalBezier.setLocation(700,350);
      
      uval.setLocation(700,450);
    }*/
public static void main(String args[]){

	GM myFrame = new GM();
        //JPanel mainPanel = new JPanel();
        //mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
        
        //myFrame.start();
       
        //myFrame.start();
	}      
}
