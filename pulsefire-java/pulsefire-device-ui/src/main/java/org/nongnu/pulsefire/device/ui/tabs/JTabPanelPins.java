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

package org.nongnu.pulsefire.device.ui.tabs;

import java.awt.FlowLayout;
import java.awt.GridLayout;

import javax.swing.BoxLayout;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SpringLayout;

import org.nongnu.pulsefire.device.ui.JComponentFactory;
import org.nongnu.pulsefire.device.ui.SpringLayoutGrid;
import org.nongnu.pulsefire.device.ui.components.JCommandCheckBox;
import org.nongnu.pulsefire.device.ui.components.JCommandComboBox;
import org.nongnu.pulsefire.device.ui.components.JCommandLabel;
import org.nongnu.pulsefire.device.ui.components.JFireQMapTable;
import org.nongnu.pulsefire.wire.CommandName;

/**
 * JTabPanelSystem
 * 
 * @author Willem Cazander
 */
public class JTabPanelPins extends AbstractFireTabPanel {

	private static final long serialVersionUID = -552322342345005654L;

	public JTabPanelPins() {
		//setLayout(new SpringLayout());
		setLayout(new FlowLayout(FlowLayout.LEFT));
		
		JPanel wrap = new JPanel();
		wrap.setLayout(new SpringLayout());
		
		JPanel wrapL = new JPanel();
		wrapL.setLayout(new SpringLayout());
		wrapL.add(createSpi());
		wrapL.add(createPinsAvrMega());
		SpringLayoutGrid.makeCompactGrid(wrapL,2,1,0,0,6,6);
		
		JPanel wrapT = new JPanel();
		wrapT.setLayout(new SpringLayout());
		wrapT.add(wrapL);
		wrapT.add(createPinsAvr());
		wrapT.add(createPinsArm());
		SpringLayoutGrid.makeCompactGrid(wrapT,1,3);
		
		wrap.add(wrapT);
		wrap.add(createInt());
		SpringLayoutGrid.makeCompactGrid(wrap,2,1,0,0,0,0);
		add(wrap);
		
	}
	
	private JPanel createSpi() {
		JPanel wrapPanel = JComponentFactory.createJFirePanel(this,"spi");
		JPanel ioPanel = new JPanel();
		ioPanel.setLayout(new SpringLayout());
		
		ioPanel.add(new JCommandLabel(CommandName.spi_clock));
		ioPanel.add(new JCommandComboBox(CommandName.spi_clock));
		
		ioPanel.add(new JCommandLabel(CommandName.spi_chips));
		JPanel chipsPanel = new JPanel();
		chipsPanel.setLayout(new GridLayout(0,2));
		ioPanel.add(chipsPanel);
		JCheckBox box = null;
		
		box = new JCommandCheckBox(CommandName.spi_chips,0);
		box.setText("OUT8");
		box.putClientProperty("JComponent.sizeVariant", "mini");
		chipsPanel.add(box);
		box = new JCommandCheckBox(CommandName.spi_chips,1);
		box.setText("OUT16");
		box.putClientProperty("JComponent.sizeVariant", "mini");
		chipsPanel.add(box);
		box = new JCommandCheckBox(CommandName.spi_chips,2);
		box.setText("LCD");
		box.putClientProperty("JComponent.sizeVariant", "mini");
		chipsPanel.add(box);
		box = new JCommandCheckBox(CommandName.spi_chips,3);
		box.setText("LCD_MUX");
		box.putClientProperty("JComponent.sizeVariant", "mini");
		chipsPanel.add(box);
		box = new JCommandCheckBox(CommandName.spi_chips,4);
		box.setText("DOC8");
		box.putClientProperty("JComponent.sizeVariant", "mini");
		chipsPanel.add(box);
		box = new JCommandCheckBox(CommandName.spi_chips,5);
		box.setText("DOC16");
		box.putClientProperty("JComponent.sizeVariant", "mini");
		chipsPanel.add(box);
		box = new JCommandCheckBox(CommandName.spi_chips,6);
		box.setText("FREE0");
		box.putClientProperty("JComponent.sizeVariant", "mini");
		chipsPanel.add(box);
		box = new JCommandCheckBox(CommandName.spi_chips,7);
		box.setText("FREE1");
		box.putClientProperty("JComponent.sizeVariant", "mini");
		chipsPanel.add(box);
		
		SpringLayoutGrid.makeCompactGrid(ioPanel,2,2);
		wrapPanel.add(ioPanel);
		return wrapPanel;
	}
	
	private JPanel createPinsAvr() {
		JPanel wrapPanel = JComponentFactory.createJFirePanel(this,"avr");
		JPanel ioPanel = new JPanel();
		ioPanel.setLayout(new SpringLayout());
		
		ioPanel.add(new JCommandLabel	(CommandName.avr_pin2_map));
		ioPanel.add(new JCommandComboBox(CommandName.avr_pin2_map));
		
		ioPanel.add(new JCommandLabel	(CommandName.avr_pin3_map));
		ioPanel.add(new JCommandComboBox(CommandName.avr_pin3_map));
		
		ioPanel.add(new JCommandLabel	(CommandName.avr_pin4_map));
		ioPanel.add(new JCommandComboBox(CommandName.avr_pin4_map));
		
		ioPanel.add(new JCommandLabel	(CommandName.avr_pin5_map));
		ioPanel.add(new JCommandComboBox(CommandName.avr_pin5_map));
		
		ioPanel.add(JComponentFactory.createJLabel(this,"noteA"));
		ioPanel.add(JComponentFactory.createJLabel(this,"noteB"));
		
		SpringLayoutGrid.makeCompactGrid(ioPanel,5,2);
		wrapPanel.add(ioPanel);
		return wrapPanel;
	}

	private JPanel createPinsAvrMega() {
		JPanel wrapPanel = JComponentFactory.createJFirePanel(this,"mega");
		JPanel ioPanel = new JPanel();
		ioPanel.setLayout(new SpringLayout());
		
		ioPanel.add(new JCommandLabel	(CommandName.mega_port_a));
		ioPanel.add(new JCommandComboBox(CommandName.mega_port_a));
		
		ioPanel.add(new JCommandLabel	(CommandName.mega_port_c));
		ioPanel.add(new JCommandComboBox(CommandName.mega_port_c));
		
		SpringLayoutGrid.makeCompactGrid(ioPanel,2,2);
		wrapPanel.add(ioPanel);
		return wrapPanel;
	}
	
	private JPanel createPinsArm() {
		JPanel wrapPanel = JComponentFactory.createJFirePanel(this,"arm");
		wrapPanel.setLayout(new BoxLayout(wrapPanel,BoxLayout.PAGE_AXIS));
		wrapPanel.add(JComponentFactory.createJPanelJWrap(new JLabel("  __  todo  __  ")));
		return wrapPanel;
	}

	private JPanel createInt() {
		JPanel wrapPanel = JComponentFactory.createJFirePanel(this,"int");
		wrapPanel.setLayout(new BoxLayout(wrapPanel,BoxLayout.PAGE_AXIS));
		
		JPanel ioPanel = new JPanel();
		ioPanel.setLayout(new SpringLayout());
		
		ioPanel.add(new JCommandLabel	(CommandName.int_0mode));
		ioPanel.add(new JCommandComboBox(CommandName.int_0mode));
		ioPanel.add(new JCommandLabel	(CommandName.int_0trig));
		ioPanel.add(new JCommandComboBox(CommandName.int_0trig));
		ioPanel.add(new JCommandLabel	(CommandName.int_0freq_mul));
		ioPanel.add(new JCommandComboBox(CommandName.int_0freq_mul));
		
		ioPanel.add(new JCommandLabel	(CommandName.int_1mode));
		ioPanel.add(new JCommandComboBox(CommandName.int_1mode));
		ioPanel.add(new JCommandLabel	(CommandName.int_1trig));
		ioPanel.add(new JCommandComboBox(CommandName.int_1trig));
		ioPanel.add(new JCommandLabel	(CommandName.int_1freq_mul));
		ioPanel.add(new JCommandComboBox(CommandName.int_1freq_mul));
		
		SpringLayoutGrid.makeCompactGrid(ioPanel,2,6);
		
		wrapPanel.add(ioPanel);
		wrapPanel.add(new JFireQMapTable(CommandName.int_map,"int0","int1"));
		
		return wrapPanel;
	}
	
	@Override
	public Class<?> getTabClassName() {
		return this.getClass();
	}
}
