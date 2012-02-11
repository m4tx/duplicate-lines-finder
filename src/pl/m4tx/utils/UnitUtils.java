package pl.m4tx.utils;

import java.text.DecimalFormat;
import java.util.ResourceBundle;

public class UnitUtils {
    public static String fileSize(double size) {
	DecimalFormat decimalFormat = new DecimalFormat("#,##0.0"); // Format to
								    // 1 decimal
								    // place

	if (size < 1024) {
	    return decimalFormat.format(size) + "b";
	} else if (size < Math.pow(1024, 2)) {
	    return decimalFormat.format(size / 1024) + "KiB";
	} else if (size < Math.pow(1024, 3)) {
	    return decimalFormat.format(size / Math.pow(1024, 2)) + "MiB";
	} else if (size < Math.pow(1024, 4)) {
	    return decimalFormat.format(size / Math.pow(1024, 3)) + "GiB";
	} else if (size < Math.pow(1024, 5)) {
	    return decimalFormat.format(size / Math.pow(1024, 4)) + "TiB";
	} else if (size < Math.pow(1024, 6)) {
	    return decimalFormat.format(size / Math.pow(1024, 5)) + "PiB";
	} else if (size < Math.pow(1024, 7)) {
	    return decimalFormat.format(size / Math.pow(1024, 6)) + "EiB";
	} else if (size < Math.pow(1024, 8)) {
	    return decimalFormat.format(size / Math.pow(1024, 7)) + "ZiB";
	} else {
	    return decimalFormat.format(size / Math.pow(1024, 8)) + "YiB";
	}
    }

    public static String time(double time, ResourceBundle localeRb) {
	DecimalFormat decimalFormat = new DecimalFormat("#,##0.0");

	if (time < 1000) {
	    return decimalFormat.format(time / 1000) + localeRb.getString("sec");
	} else if (time < 1000*60) {
	    return decimalFormat.format(time / (1000*60)) + localeRb.getString("min");
	} else if (time < 1000*60*60) {
	    return decimalFormat.format(time / (1000*60*60)) + localeRb.getString("h");
	}

	return Double.toString(time);
    }
}
