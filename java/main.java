
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
	       Logger sun_awt = Logger.getLogger("sun.awt");
	       Logger sun_x11 = Logger.getLogger("sun.awt.X11");
	       java_awt.addHandler(console);
	       sun_awt.addHandler(console);
	       sun_x11.addHandler(console);


	       JFrame frame = new JFrame("My First GUI");
	       frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	       frame.setSize(300,300);
	       JButton button = new JButton("Press");
	       frame.getContentPane().add(button); // Adds Button to content pane of frame
	       frame.setVisible(true);
	    }
	  };

	  thread.start();
    }

    public static void main(String[] args)
    {
    	main();
    }
}