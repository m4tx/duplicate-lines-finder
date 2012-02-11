package pl.m4tx.DuplicateLinesFinder;

import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Locale;
import java.util.ResourceBundle;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import javax.swing.SwingWorker;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.border.EmptyBorder;

import pl.m4tx.swing.ButtonProgressbar;
import pl.m4tx.utils.ImgUtils;
import pl.m4tx.utils.UnitUtils;

public class MainWindow extends JFrame implements ActionListener,
	PropertyChangeListener {
    protected JButton chooseFileToProcess;
    protected JButton chooseDestFile;
    protected ButtonProgressbar startCancel;
    protected JLabel statsLabel;
    protected JTextField separatorTextField;

    protected JMenuItem exitMenuItem;
    protected JMenuItem aboutMenuItem;

    protected boolean srcFileChoosen = false;
    protected boolean dstFileChoosen = false;
    protected FileProcessThread fileProcessThread;

    protected final JFileChooser fc;

    // i18n
    ResourceBundle rb;

    public class FileProcessThread extends SwingWorker<Void, Void> {
	// Stats
	protected long timeBefore;
	protected long timeAfter;
	protected int sourceSize;
	protected int destSize = 0;
	protected int sourceLines;
	protected int destLines = 0;

	@Override
	protected Void doInBackground() throws Exception {
	    startCancel.progressbar.setIndeterminate(true);
	    timeBefore = System.currentTimeMillis();

	    setProgress(0);

	    FileReader inputStream = null;
	    FileWriter outputStream = null;

	    String srcFilename = chooseFileToProcess.getText();
	    String dstFilename = chooseDestFile.getText();
	    String separator = separatorTextField.getText();

	    if (separator.equals("")) {
		separator = "\n";
	    }

	    try {
		File srcFile = new File(srcFilename);

		if (!srcFile.exists() || !srcFile.isFile()) {
		    System.out.println("File doesn\'t exist");
		    return null;
		}

		// Here we get the actual size
		int srcFilesize = (int) srcFile.length();
		sourceSize = srcFilesize;

		inputStream = new FileReader(srcFile);

		char[] cbuf = new char[srcFilesize];
		inputStream.read(cbuf);

		if (inputStream != null) {
		    inputStream.close();
		}

		ArrayList<String> savedStrings = new ArrayList<String>();
		String[] strings = (new String(cbuf)).split(separator);

		sourceLines = strings.length;

		outputStream = new FileWriter(dstFilename);

		startCancel.progressbar.setIndeterminate(false);

		boolean saveString = true;
		for (int i = 0; i < strings.length; i++) {
		    for (int j = 0; j < savedStrings.size(); j++) {
			if (strings[i].equals(savedStrings.get(j))) {
			    saveString = false;
			    break;
			}
		    }
		    if (saveString) {
			savedStrings.add(strings[i]);
			outputStream.write(strings[i] + separator);
			destSize += strings[i].length() + separator.length();
			destLines++;
		    } else {
			saveString = true;
		    }
		    setProgress((int) ((float) i / (float) strings.length * 100));
		}
		if (outputStream != null) {
		    outputStream.close();
		}
	    } catch (FileNotFoundException e) {
		e.printStackTrace();
	    } catch (IOException e) {
		e.printStackTrace();
	    }

	    timeAfter = System.currentTimeMillis();
	    
	    return null;
	}

	@Override
	protected void done() {
	    startCancel.setState((byte) 0);
	    if (isCancelled()) {
		updateStatsLabel();
	    } else {
		updateStatsLabel(sourceSize, destSize, sourceLines, destLines,
			(int) (timeAfter - timeBefore));
	    }
	}
    }

    MainWindow() {
	super("Duplicate Lines Finder 2.0");

	// i18n
	rb = ResourceBundle.getBundle("pl.m4tx.DuplicateLinesFinder.Locale",
		Locale.getDefault());

	try {
	    UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
	} catch (ClassNotFoundException e) {
	    e.printStackTrace();
	} catch (InstantiationException e) {
	    e.printStackTrace();
	} catch (IllegalAccessException e) {
	    e.printStackTrace();
	} catch (UnsupportedLookAndFeelException e) {
	    e.printStackTrace();
	}

	setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	setSize(600, 400);
	setVisible(true);
	setLocationRelativeTo(null);

	setIconImage(ImgUtils.createImageIcon(this, "images/icon_256.png").getImage());

	Container pane = getContentPane();

	JPanel mainPanel = new JPanel();
	GridBagLayout mainPanelLayout = new GridBagLayout();
	mainPanelLayout.rowWeights = new double[] { 0.0, 0.0, Double.MIN_VALUE };
	mainPanel.setLayout(mainPanelLayout);
	mainPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
	pane.add(mainPanel);

	GridBagConstraints c = new GridBagConstraints();
	c.fill = GridBagConstraints.HORIZONTAL;
	c.weightx = 0.5;

	c.gridx = 0;
	c.insets = new Insets(0, 0, 5, 0);
	c.anchor = GridBagConstraints.FIRST_LINE_START;
	c.gridwidth = 2;

	c.gridy = 0;
	JMenuBar menuBar = new JMenuBar();
	JMenu programMenu = new JMenu(rb.getString("program"));
	menuBar.add(programMenu);
	exitMenuItem = new JMenuItem(rb.getString("exit"));
	exitMenuItem.addActionListener(this);
	programMenu.add(exitMenuItem);
	JMenu helpMenu = new JMenu(rb.getString("help"));
	menuBar.add(helpMenu);
	aboutMenuItem = new JMenuItem(rb.getString("about"));
	aboutMenuItem.addActionListener(this);
	helpMenu.add(aboutMenuItem);
	setJMenuBar(menuBar);
	c.gridy = 1;
	chooseFileToProcess = new JButton(rb.getString("chooseFileToProcess"));
	chooseFileToProcess.setHorizontalAlignment(SwingConstants.LEFT);
	chooseFileToProcess.addActionListener(this);
	mainPanel.add(chooseFileToProcess, c);
	c.gridy = 2;
	chooseDestFile = new JButton(rb.getString("chooseDestFile"));
	chooseDestFile.setHorizontalAlignment(SwingConstants.LEFT);
	chooseDestFile.addActionListener(this);
	mainPanel.add(chooseDestFile, c);
	c.gridy = 3;
	startCancel = new ButtonProgressbar(rb.getString("startCancelStart"),
		rb.getString("startCancelCancel"));
	startCancel.button1.setEnabled(false);
	startCancel.button1.addActionListener(this);
	startCancel.button2.addActionListener(this);
	mainPanel.add(startCancel, c);
	c.gridy = 4;
	c.gridwidth = 1;
	c.gridx = 0;
	JPanel separatorPanel = new JPanel();
	separatorPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
	JLabel separatorLabel = new JLabel(rb.getString("separator"));
	separatorLabel.setToolTipText(rb.getString("separatorTooltip"));
	separatorTextField = new JTextField();
	separatorTextField.setPreferredSize(new Dimension(40, 23));
	separatorTextField.setToolTipText(rb.getString("separatorTooltip"));
	separatorPanel.add(separatorLabel);
	separatorPanel.add(separatorTextField);
	mainPanel.add(separatorPanel, c);
	c.gridx = 1;
	statsLabel = new JLabel();
	statsLabel.setHorizontalAlignment(SwingConstants.RIGHT);
	updateStatsLabel();
	mainPanel.add(statsLabel, c);
	c.gridy = 5;
	c.gridwidth = 2;
	c.gridx = 0;
	c.weighty = 1.0;
	mainPanel.add(new JPanel(), c);

	fc = new JFileChooser();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
	if (e.getSource() == chooseFileToProcess) {
	    String returnVal = openFileDialog(false);

	    if (returnVal != "") {
		srcFileChoosen = true;
		chooseFileToProcess.setText(returnVal);
		chooseFileToProcess.setToolTipText(returnVal);
		activateStartCancelIfPossible();
	    }
	} else if (e.getSource() == chooseDestFile) {
	    String returnVal = openFileDialog(true);

	    if (returnVal != "") {
		dstFileChoosen = true;
		chooseDestFile.setText(returnVal);
		chooseDestFile.setToolTipText(returnVal);
		activateStartCancelIfPossible();
	    }
	} else if (e.getSource() == startCancel.button1) {
	    fileProcessThread = new FileProcessThread();
	    fileProcessThread.addPropertyChangeListener(this);
	    fileProcessThread.execute();
	} else if (e.getSource() == startCancel.button2) {
	    fileProcessThread.cancel(true);
	} else if (e.getSource() == exitMenuItem) {
	    System.exit(0);
	} else if (e.getSource() == aboutMenuItem) {
	    AboutBox aboutBox = new AboutBox(this, rb);
	    aboutBox.setVisible(true);
	}
    }

    protected void activateStartCancelIfPossible() {
	if (srcFileChoosen && dstFileChoosen)
	    startCancel.button1.setEnabled(true);
    }

    protected void updateStatsLabel() {
	statsLabel.setText("<html><div style='text-align:right'><b>"
		+ rb.getString("sourceFileSize") + "</b>: "
		+ rb.getString("notApplicable") + "<br><b>"
		+ rb.getString("destFileSize") + "</b>: "
		+ rb.getString("notApplicable") + "<br><b>"
		+ rb.getString("saved") + "</b>: "
		+ rb.getString("notApplicable") + "<br><b>"
		+ rb.getString("linesInSourceFile") + "</b>: "
		+ rb.getString("notApplicable") + "<br><b>"
		+ rb.getString("linesInDestinationFile") + "</b>: "
		+ rb.getString("notApplicable") + "<br><b>"
		+ rb.getString("difference") + "</b>: "
		+ rb.getString("notApplicable") + "<br><b>"
		+ rb.getString("processingTime") + "</b>: "
		+ rb.getString("notApplicable"));
    }

    protected void updateStatsLabel(int sourceFileSize, int destFileSize,
	    int sourceFileLines, int destFileLines, int time) {
	DecimalFormat decimalFormat = new DecimalFormat("#,##0.0");

	statsLabel.setText("<html><div style='text-align:right'><b>"
		+ rb.getString("sourceFileSize")
		+ "</b>: "
		+ UnitUtils.fileSize(sourceFileSize)
		+ "<br><b>"
		+ rb.getString("destFileSize")
		+ "</b>: "
		+ UnitUtils.fileSize(destFileSize)
		+ "<br><b>"
		+ rb.getString("saved")
		+ "</b>: "
		+ UnitUtils.fileSize(sourceFileSize - destFileSize)
		+ " ("
		+ decimalFormat.format((1 - (double) destFileSize
			/ (double) sourceFileSize) * 100) + "%)<br><b>"
		+ rb.getString("linesInSourceFile") + "</b>: "
		+ Integer.toString(sourceFileLines) + "<br><b>"
		+ rb.getString("linesInDestinationFile") + "</b>: "
		+ Integer.toString(destFileLines) + "<br><b>"
		+ rb.getString("difference") + "</b>: "
		+ Integer.toString(sourceFileLines - destFileLines) + "<br><b>"
		+ rb.getString("processingTime") + "</b>: "
		+ UnitUtils.time(time, rb));
    }

    /**
     * Shows open or save file dialog.
     * 
     * @param saveDialog
     *            Set it true if you want to show save file dialog, set false if
     *            you want to show open file dialog.
     * @return Absolute path of the choosen file, empty String if user didn't
     *         choose any file.
     */
    protected String openFileDialog(boolean saveDialog) {
	int returnVal;

	if (saveDialog)
	    returnVal = fc.showSaveDialog(this);
	else
	    returnVal = fc.showOpenDialog(this);

	if (returnVal == JFileChooser.APPROVE_OPTION) {
	    File file = fc.getSelectedFile();
	    return file.getAbsolutePath();
	} else {
	    return "";
	}
    }

    @Override
    public void propertyChange(PropertyChangeEvent e) {
	if ("progress" == e.getPropertyName()) {
	    startCancel.progressbar.setValue((Integer) e.getNewValue());
	}
    }
}
