package pl.m4tx.DuplicateLinesFinder;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ResourceBundle;

import javax.swing.Box;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

import pl.m4tx.swing.HyperlinkLabel;
import pl.m4tx.utils.ImgUtils;
import pl.m4tx.utils.SystemUtils;

public class AboutBox extends JDialog {
    public AboutBox(JFrame parent, ResourceBundle localeRb) {
	super(parent, localeRb.getString("about"), true);

	Box b = Box.createVerticalBox();
	b.add(Box.createGlue());
	ImageIcon icon = ImgUtils.createImageIcon(this, "images/icon_32.png");
	b.add(new JLabel("<html>Duplicate Lines Finder 2.0<br>"
		+ localeRb.getString("byM4tx"), icon, SwingConstants.CENTER));
	b.add(Box.createVerticalStrut(15));
	b.add(new JLabel("<html>" + localeRb.getString("specialThanks")
		+ "<br>rhino - " + localeRb.getString("englishTranslation"),
		SwingConstants.CENTER));
	b.add(Box.createVerticalStrut(15));
	HyperlinkLabel homepageButton = new HyperlinkLabel(
		localeRb.getString("homepage"), "http://m4tx.pl/",
		SwingConstants.CENTER);

	b.add(homepageButton);
	b.add(Box.createGlue());
	getContentPane().add(b, "Center");

	JPanel p2 = new JPanel();
	JButton ok = new JButton(localeRb.getString("ok"));
	ok.addActionListener(new ActionListener() {

	    @Override
	    public void actionPerformed(ActionEvent arg0) {
		setVisible(false);
	    }
	});
	p2.add(ok);
	getContentPane().add(p2, "South");

	setLocationRelativeTo(null);
	setResizable(false);
	setSize(new Dimension(300, 200));
    }
}
