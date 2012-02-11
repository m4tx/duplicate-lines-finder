package pl.m4tx.utils;

import java.awt.Desktop;
import java.io.IOException;
import java.net.URI;

public class SystemUtils {
    public static void openUrl(URI uri) {
	if (Desktop.isDesktopSupported()) {
	    try {
		Desktop.getDesktop().browse(uri);
	    } catch (IOException e) {
		e.printStackTrace();
	    }
	} else {
	}
    }

}
