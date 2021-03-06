/*
 * Copyright (c) 2011, Willem Cazander
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided
 * that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this list of conditions and the
 *   following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 *   the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package org.nongnu.pulsefire.device.ui;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GraphicsDevice;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.Date;
import java.util.EventObject;
import java.util.List;
import java.util.Properties;
import java.util.logging.Handler;
import java.util.logging.LogManager;
import java.util.logging.Logger;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JComponent;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;
import javax.swing.UIManager;

import org.jdesktop.application.Application;
import org.jdesktop.application.FrameView;
import org.jdesktop.application.SingleFrameApplication;
import org.nongnu.pulsefire.device.io.transport.DeviceData;
import org.nongnu.pulsefire.device.io.transport.DeviceWireManager;
import org.nongnu.pulsefire.device.io.transport.DeviceWireManagerController;
import org.nongnu.pulsefire.device.io.transport.serial.SerialDeviceWireManager;
import org.nongnu.pulsefire.device.ui.pull.PulseFireDataPuller;
import org.nongnu.pulsefire.device.ui.pull.PulseFireTimeData;
import org.nongnu.pulsefire.device.ui.pull.UpdatePwmData;
import org.nongnu.pulsefire.device.ui.time.EventTimeManager;
import org.nongnu.pulsefire.device.ui.time.EventTimeTrigger;
import org.nongnu.pulsefire.lib.rxtx.RXTXNative;
import org.nongnu.pulsefire.lib.rxtx.RXTXNative.DefaultPhasedBootIntegration;


/**
 * PulseFire application
 * 
 * @author Willem Cazander
 */
public class PulseFireUI extends SingleFrameApplication {

	private DeviceWireManagerController deviceManagerController = null;
	private PulseFireTimeData timeData = null;
	private EventTimeManager eventTimeManager = null;
	private PulseFireDataLogManager dataLogManager = null;
	private PulseFireUIBuildInfo buildInfo = null;
	private PulseFireUISettingManager settingsManager = null;
	private boolean fullScreen = false;
	private long startTimeTotal = System.currentTimeMillis();
	private Logger logger = null;
	
	static public void main(String[] args) {
		Application.launch(PulseFireUI.class, args);
	}
	
	private void setupBuildInfo() {
		if (buildInfo!=null) {
			return;
		}
		try {
			Class<?> infoClass = Class.forName(PulseFireUIBuildInfo.class.getPackage().getName()+"."+PulseFireUIBuildInfo.class.getSimpleName()+"Impl");
			buildInfo = (PulseFireUIBuildInfo)infoClass.newInstance();
			return;
		} catch (Exception e) {
			logger.warning("Could not load build info impl fallback to local one.");
		}
		buildInfo = new PulseFireUIBuildInfo() {
			@Override
			public String getVersion() {
				return "0.0.0-Development";
			}
			@Override
			public String getBuildDate() {
				return new Date().toString();
			}
		};
	}
	
	/**
	 * Config logging and setup logger object.
	 */
	private void setupLogging() {
		File logConfig = new File("logfile.properties");
		if (logConfig.exists()) {
			InputStream in = null;
			try {
				in = new FileInputStream(logConfig);
				LogManager.getLogManager().readConfiguration(in);
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (in!=null) {
					try {
						in.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		} else {
			Logger rootLogger = Logger.getAnonymousLogger();
			while (rootLogger.getParent()!=null) {
				rootLogger = rootLogger.getParent();
			}
			for (Handler h:rootLogger.getHandlers()) {
				h.setFormatter(new PatternLogFormatter());
			}
		}
		logger = Logger.getLogger(PulseFireUI.class.getName());
	}
	
	protected void initialize(String[] args) {
		super.initialize(args);
		try {
			long startTime = System.currentTimeMillis();
			setupLogging();   // init logging with config
			setupBuildInfo(); // Get build version info
			logger.info("Starting PulseFire-UI version: "+buildInfo.getVersion()+" build: "+buildInfo.getBuildDate());
			
			for (String argu:args) {
				if ("-fs".equals(argu)) {
					fullScreen = true;
				}
			}
			
			RXTXNative.defaultPhasedBoot(new File("."),new DefaultPhasedBootIntegration() {
				@Override
				public void showAndExit(String message) {
					JOptionPane.showMessageDialog(null, message, "RXTXNative Error", JOptionPane.ERROR_MESSAGE);
					System.exit(1);
				}
				@Override
				public void log(String message) {
					logger.info(message);
				}
			});
			
			settingsManager = new PulseFireUISettingManager(getContext());
			settingsManager.loadSettings();
			deviceManagerController = new DeviceWireManagerController();
			deviceManagerController.addDeviceManager(new SerialDeviceWireManager());
			eventTimeManager = new EventTimeManager();
			eventTimeManager.start();
			timeData = new PulseFireTimeData();
			dataLogManager = new PulseFireDataLogManager();
			dataLogManager.start();
			String colorName = installColorsLaF();
			logger.info("Color schema selected: "+colorName);
			long stopTime = System.currentTimeMillis();
			logger.info("PulseFireUI initialized in "+(stopTime-startTime)+" ms.");
		} catch (Throwable e) {
			StringWriter sw = new StringWriter();
			e.printStackTrace(new PrintWriter(sw));
			JOptionPane.showMessageDialog(null, "Fatal Initialize Error:\n"+sw.getBuffer().toString(), "PulseFire Initialize Error", JOptionPane.ERROR_MESSAGE);
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	protected void startup() {
		try {
			long startTime = System.currentTimeMillis();
			addExitListener(new ShutdownManager());
			
			FrameView mainView = getMainView();
			mainView.getFrame().setMinimumSize(new Dimension(1024-64,768-128));
			mainView.setComponent(new JMainPanel());
			mainView.getFrame().setTitle(mainView.getFrame().getTitle()+" "+buildInfo.getVersion());
			// //new JFireGlassPane(mainView.getFrame());
			
			mainView.getComponent().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("F11"),"toggleFullScreen");
			mainView.getComponent().getActionMap().put("toggleFullScreen",toggleFullScreenAction);
			
			show(mainView); // init bsaf
			if (fullScreen) {
				fullScreen = !fullScreen;
				toggleFullScreen();
			}
			
			eventTimeManager.addEventTimeTrigger(new EventTimeTrigger("refreshData",new PulseFireDataPuller(),PulseFireDataPuller.INIT_SPEED));
			eventTimeManager.addEventTimeTriggerConnected(new EventTimeTrigger("updatePwmData",new UpdatePwmData(),UpdatePwmData.INIT_SPEED));
			
			//new org.nongnu.pulsefire.device.ui.JNimbusColorFrame(getMainFrame()).setVisible(true);
			//org.nongnu.pulsefire.device.ui.debug.JDebugPanel debugPanel = org.nongnu.pulsefire.device.ui.debug.JDebugPanel.openDebugFrame("PulseFire Debug");
			//debugPanel.setDebugComponent(mainView.getComponent());
			
			long stopTime = System.currentTimeMillis();
			logger.info("PulseFireUI startup in "+(stopTime-startTime)+" ms total startup in "+(stopTime-startTimeTotal)+" ms.");
		} catch (Exception e) {
			dataLogManager.stop();
			eventTimeManager.shutdown();
			StringWriter sw = new StringWriter();
			e.printStackTrace(new PrintWriter(sw));
			JOptionPane.showMessageDialog(null, "Fatal Startup Error:\n"+sw.getBuffer().toString(), "PulseFire Startup Error", JOptionPane.ERROR_MESSAGE);
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	private Action toggleFullScreenAction = new AbstractAction("toggleFullScreen") {
		private static final long serialVersionUID = -857683386678673114L;
		@Override
		public void actionPerformed(ActionEvent e) {
			toggleFullScreen();
		}
	};
	
	private void toggleFullScreen() {
		if (getDeviceManager().isConnected()) {
			// Only allow switch when disconnected else we get low level errors;
			// NPE: at java.awt.Component$BltBufferStrategy.contentsLost(Component.java:4455)
			// OOM: Java heap space at java.awt.image.DataBufferInt.<init>
			//note: eventTimeManager.shutdown(); does not help
			return;
		}
		FrameView mainView = getMainView();
		GraphicsDevice gd = mainView.getFrame().getGraphicsConfiguration().getDevice();
		if (!gd.isFullScreenSupported()) {
			return;
		}

		mainView.getFrame().dispose();
		if (!fullScreen) {
			mainView.getFrame().setUndecorated(true);
			gd.setFullScreenWindow(mainView.getFrame());
			
		} else {
			mainView.getFrame().setUndecorated(false);
			gd.setFullScreenWindow(null);
		}
		mainView.getFrame().pack();
		mainView.getFrame().setVisible(true);
		fullScreen = !fullScreen;
	}
	
	private String installColorsLaF() {
		UIManager.put("TabbedPane.font",		Font.decode("SansSerif-BOLD-12"));
		UIManager.put("TitledBorder.font",		Font.decode("SansSerif-BOLD-16"));
		UIManager.put("FireDial.font",			Font.decode("SansSerif-9"));
		
		String colorName = getSettingsManager().getSettingString(PulseFireUISettingKeys.LAF_COLORS);
		ClassLoader cl = Thread.currentThread().getContextClassLoader();
		if (cl==null) {
			cl = this.getClass().getClassLoader();
		}
		InputStream in = cl.getResourceAsStream("org/nongnu/pulsefire/device/ui/resources/colors/"+colorName+".properties");
		if (in==null) {
			logger.warning("Color schema not found: "+colorName);
			return "unknown";
		}
		try {
			Properties p = new Properties();
			p.load(in);
			for (Object key:p.keySet()) {
				String value = p.getProperty(key.toString());
				Color colorValue = Color.decode(value);
				UIManager.put(key,colorValue);
			}
			
			if (UIManager.get("gridColor")==null) {
				UIManager.put("gridColor",UIManager.getColor("nimbusGreen").darker());
			}
			
			// Invert focus painters
			List<Object> keys = new ArrayList<Object>(UIManager.getLookAndFeelDefaults().keySet());
			for (Object keyObj:keys) {
				if (!(keyObj instanceof String)) {
					continue;
				}
				String key = (String)keyObj;
				
				if (key.endsWith("[Focused].backgroundPainter")==false & key.endsWith("[Focused].iconPainter")==false) {
					continue;
				}
				String preKey = key.substring(0,key.indexOf("["));
				String postKey = "backgroundPainter";
				if (key.contains("iconPainter")) {
					postKey = "iconPainter";
				}
				
				logger.finer("Flipping painters of key: "+preKey);
				
				Object focusPainter = UIManager.getLookAndFeelDefaults().get(preKey+"[Focused]."+postKey);
				Object mouseOverPainter = UIManager.getLookAndFeelDefaults().get(preKey+"[MouseOver]."+postKey);
				UIManager.getLookAndFeelDefaults().put(preKey+"[Focused]."+postKey,mouseOverPainter);
				UIManager.getLookAndFeelDefaults().put(preKey+"[MouseOver]."+postKey,focusPainter);
				
				if (key.contains("iconPainter")) {
					Object focusPainterSelected = UIManager.getLookAndFeelDefaults().get(preKey+"[Focused+Selected]."+postKey);
					Object mouseOverPainterSelected = UIManager.getLookAndFeelDefaults().get(preKey+"[MouseOver+Selected]."+postKey);
					UIManager.getLookAndFeelDefaults().put(preKey+"[Focused+Selected]."+postKey,mouseOverPainterSelected);
					UIManager.getLookAndFeelDefaults().put(preKey+"[MouseOver+Selected]."+postKey,focusPainterSelected);
				}
			}
			/*
			Object tabFocusPainter = UIManager.getLookAndFeelDefaults().get("TabbedPane:TabbedPaneTab[Focused+MouseOver+Selected].backgroundPainter");
			Object tabMouseOverPainter = UIManager.getLookAndFeelDefaults().get("TabbedPane:TabbedPaneTab[Enabled+MouseOver].backgroundPainter");
			UIManager.getLookAndFeelDefaults().put("TabbedPane:TabbedPaneTab[Focused+MouseOver+Selected].backgroundPainter",tabMouseOverPainter);
			UIManager.getLookAndFeelDefaults().put("TabbedPane:TabbedPaneTab[Enabled+MouseOver].backgroundPainter",tabFocusPainter);
			
			Object tabPressedFocusPainter = UIManager.getLookAndFeelDefaults().get("TabbedPane:TabbedPaneTab[Focused+Pressed+Selected].backgroundPainter");
			Object tabPressedMouseOverPainter = UIManager.getLookAndFeelDefaults().get("TabbedPane:TabbedPaneTab[Pressed+Selected].backgroundPainter");
			UIManager.getLookAndFeelDefaults().put("TabbedPane:TabbedPaneTab[Focused+Pressed+Selected].backgroundPainter",tabPressedMouseOverPainter);
			UIManager.getLookAndFeelDefaults().put("TabbedPane:TabbedPaneTab[Pressed+Selected].backgroundPainter",tabPressedFocusPainter);
			*/
		} catch (IOException e) {
			logger.warning("Could not load color schema: "+colorName+" error: "+e.getMessage());
		} finally {
			try {
				in.close();
			} catch (IOException e) {
				logger.warning(e.getMessage());
			}
		}
		return colorName;
	}
	
	/**
	 * Small hack, is called after main frame is visable so that dialog is centered on application.
	 */
	@Override
	protected void ready() {
		JMainPanel mainPanel = (JMainPanel)getMainView().getComponent();
		mainPanel.topPanelSerial.autoConnect();
	}

	static public PulseFireUI getInstance() {
		return getInstance(PulseFireUI.class);
	}
	
	public DeviceWireManagerController getDeviceManagerController() {
		return deviceManagerController;
	}

	public EventTimeManager getEventTimeManager() {
		return eventTimeManager;
	}
	
	public PulseFireTimeData getTimeData() {
		return timeData;
	}
	
	public DeviceWireManager getDeviceManager() {
		return getDeviceManagerController().getDefaultDeviceManager();
	}
	
	public DeviceData getDeviceData() {
		return getDeviceManager().getDeviceData();
	}
	
	public PulseFireUISettingManager getSettingsManager() {
		return settingsManager;
	}
	
	class ShutdownManager implements ExitListener {
		@Override
		public boolean canExit(EventObject e) {
			return true;
		}
		@Override
		public void willExit(EventObject event) {
			logger.info("Shutdown requested.");
			long startTime = System.currentTimeMillis();
			
			getSettingsManager().setSettingInteger(PulseFireUISettingKeys.UI_SPLIT_CONTENT,((JMainPanel)getMainView().getComponent()).contentSplitPane.getDividerLocation());
			getSettingsManager().setSettingInteger(PulseFireUISettingKeys.UI_SPLIT_BOTTOM,((JMainPanel)getMainView().getComponent()).bottomSplitPane.getDividerLocation());
			getSettingsManager().setSettingInteger(PulseFireUISettingKeys.UI_SPLIT_BOTTOM_LOG,((JMainPanel)getMainView().getComponent()).bottomLogSplitPane.getDividerLocation());
			getSettingsManager().saveSettings();
			
			dataLogManager.stop();
			eventTimeManager.shutdown();
			PulseFireUI.getInstance().getDeviceManager().disconnect(false);
			for (int i=0;i<20;i++) {
				try { Thread.sleep(100); } catch (InterruptedException e) {}
				if (PulseFireUI.getInstance().getDeviceManager().isConnected()==false) {
					break;
				}
			}
			long stopTime = System.currentTimeMillis();
			logger.info("PulseFireUI stopped in "+(stopTime-startTime)+" ms.");
		}
	}
}
