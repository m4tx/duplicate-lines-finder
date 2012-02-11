package pl.m4tx.swing;

import java.awt.Cursor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.net.URI;
import java.net.URISyntaxException;

import javax.swing.JLabel;
import javax.swing.SwingConstants;

import pl.m4tx.utils.SystemUtils;

public class HyperlinkLabel extends JLabel {
    /**
     * @param text
     *            The text which is shown as a text.
     * @param link
     *            The URL where the link goes.
     * @param horizontalAlignment
     *            Horizontal alignment of the text on the label. One of the
     *            following constants defined in SwingConstants: LEFT, CENTER,
     *            RIGHT, LEADING or TRAILING.
     */
    public HyperlinkLabel(String text, final String link,
	    int horizontalAlignment) {
	super("<html><a href=\"" + link + "\">" + text + "</a></html>",
		horizontalAlignment);

	setCursor(new Cursor(Cursor.HAND_CURSOR));

	addMouseListener(new MouseListener() {

	    @Override
	    public void mouseReleased(MouseEvent e) {
	    }

	    @Override
	    public void mousePressed(MouseEvent e) {
		try {
		    SystemUtils.openUrl(new URI(link));
		} catch (URISyntaxException e1) {
		    e1.printStackTrace();
		}
	    }

	    @Override
	    public void mouseExited(MouseEvent e) {
	    }

	    @Override
	    public void mouseEntered(MouseEvent e) {
	    }

	    @Override
	    public void mouseClicked(MouseEvent arg0) {
	    }
	});
    }
}
