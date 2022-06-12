
import javax.swing.*;
import java.util.logging.*;

class main{
    public static void main()
    {
	  Thread thread = new Thread()
	  {
	    public void run()
	    {
	    			       ConsoleHandler console = new ConsoleHandler();
		       Logger java_awt = Logger.getLogger("java.awt");
		       Logger java_awt_focus = Logger.getLogger("java.awt.focus");
		       Logger java_awt_event = Logger.getLogger("java.awt.event");
		       Logger java_awt_mixing = Logger.getLogger("java.awt.mixing");
		       Logger sun_awt = Logger.getLogger("sun.awt");
		       Logger sun_windows = Logger.getLogger("sun.awt.windows");
		       Logger sun_x11 = Logger.getLogger("sun.awt.X11");

		      
	    	try
	    	{

		       Handler fh = new FileHandler("wombat.log");		       

		       java_awt.setLevel(Level.WARNING);
	   	       java_awt_focus.setLevel(Level.WARNING);
		       java_awt_event.setLevel(Level.WARNING);
		       java_awt_mixing.setLevel(Level.WARNING);
		       sun_awt.setLevel(Level.WARNING);
		       sun_windows.setLevel(Level.WARNING);
		       sun_x11.setLevel(Level.WARNING);
		       sun_x11.addHandler(fh);
		       java_awt.addHandler(fh);
	   	       java_awt_focus.addHandler(fh);
		       java_awt_event.addHandler(fh);
		       java_awt_mixing.addHandler(fh);
		       sun_awt.addHandler(fh);
		       sun_windows.addHandler(fh);


	    	}
	    	catch(java.io.IOException ex)
	    	{
	    		//System.out.println(ex.toString());
	    	}

    java.awt.EventQueue.invokeLater(new Runnable() {
        public void run() {
        
	       JFrame frame = new JFrame("My First GUI");
	       frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	       frame.setSize(300,300);
	       JButton button = new JButton("Press");
	       frame.getContentPane().add(button); // Adds Button to content pane of frame
	       frame.setVisible(true);	    	
           
        }
    });	    	

	   
	    }
	  };

      thread.setUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
         public void uncaughtException(Thread t, Throwable e) {
            System.out.println(t + " throws exception: " + e);
         }
      });

	  thread.start();
    }

    public static void main(String[] args)
    {
    	main();
    }
}