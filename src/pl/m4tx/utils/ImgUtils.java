package pl.m4tx.utils;

import java.net.URL;

import javax.swing.ImageIcon;

public class ImgUtils {
    /**
     * Creates an image icon from the specified the path.
     * @param path Path to the image.
     * @return Created ImageIcon or null if the path is invalid.
     */
    public static ImageIcon createImageIcon(Object parent, String path) {
        URL imgURL = parent.getClass().getResource(path);
        if (imgURL != null) {
            return new ImageIcon(imgURL);
        } else {
            return null;
        }
    }
}
