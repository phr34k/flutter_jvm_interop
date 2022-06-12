
import javax.swing.*;

class main{
    public static void main()
    {
	  Thread thread = new Thread()
	  {
	    public void run()
	    {
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
}