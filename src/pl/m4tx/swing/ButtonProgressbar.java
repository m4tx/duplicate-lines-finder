package pl.m4tx.swing;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JProgressBar;

public class ButtonProgressbar extends JPanel implements ActionListener {
    public JButton button1;
    public JProgressBar progressbar;
    public JButton button2;

    /**
     * state: 0 - button1 displayed; 1 - progressbar displayed; 2 - button2
     * displayed;
     * */
    protected byte state;

    public ButtonProgressbar(String button1Label, String button2Label) {
	setLayout(new GridLayout(1, 0, 0, 0));
	
	button1 = new JButton(button1Label);
	progressbar = new JProgressBar();
	progressbar.setValue(0);
	progressbar.setStringPainted(true);
	//progressbar.setIndeterminate(true);
	button2 = new JButton(button2Label);

	button1.setSize(getSize());
	progressbar.setSize(getSize());
	button2.setSize(getSize());
	
	button1.addActionListener(this);
	button2.addActionListener(this);

	progressbar.addMouseListener(new java.awt.event.MouseAdapter() {

	    public void mouseEntered(java.awt.event.MouseEvent evt) {
		state = 2;
		updateWidgets();
	    }

	    public void mouseExited(java.awt.event.MouseEvent evt) {
	    }
	});

	button2.addMouseListener(new java.awt.event.MouseAdapter() {

	    public void mouseEntered(java.awt.event.MouseEvent evt) {
	    }

	    public void mouseExited(java.awt.event.MouseEvent evt) {
		if (state == 2) {
		    state = 1;
		    updateWidgets();
		}
	    }
	});

	state = 0;
	updateWidgets();
    }
    
    @Override
    public Dimension getPreferredSize() {
	return button1.getPreferredSize();
    }
    
    /**
     * Updates widgets according to state.
     */
    protected void updateWidgets() {
	remove(button1);
	remove(progressbar);
	remove(button2);

	if (state == 0)
	    add(button1);
	else if (state == 1)
	    add(progressbar);
	else if (state == 2)
	    add(button2);
	
	updateUI();
    }

    /**
     * @return the state
     */
    public byte getState() {
        return state;
    }

    /**
     * @param state the state to set
     */
    public void setState(byte state) {
        this.state = state;
        updateWidgets();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
	if (e.getSource() == button1) {
	    state = 1;
	    updateWidgets();
	} else if (e.getSource() == button2) {
	    state = 0;
	    updateWidgets();
	}
    }
}
