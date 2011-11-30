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

package org.nongnu.pulsefire.device.flash;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.nongnu.pulsefire.device.flash.avr.Stk500Controller;
import org.nongnu.pulsefire.device.flash.avr.Stk500v2Controller;

/**
 * FlashManager can load the hex file and flash devices based on settings.
 * 
 * @author Willem Cazander
 */
public class FlashManager {

	static public void main(String argu[]) throws Exception {
		if (argu.length<6) {
			System.out.println("\nUsage:\n"+
					"  -P <port>     Serial port.\n"+
					"  -c <proto>    Programmer protocol.\n"+
					"  -f <file>     Flash hex file.\n"+
					"Optional:\n"+
					"  -v            Verbose output.\n"+
					"  -V            Verify flash.\n"+
					"  -PP <param>   Port parameters.\n"+
					"  -s <sign>     Device signature in hex.\n"+
					"");
			return;
		}
		
		List<String> arguList = new ArrayList<String>(argu.length);
		for (String a:argu) { arguList.add(a); } 
		Iterator<String> arguIterator = arguList.iterator();
		FlashControllerConfig config = new FlashControllerConfig();
		
		while (arguIterator.hasNext()) {
			String arg = arguIterator.next();
			if ("-v".equals(arg)) {
				config.setLogDebug(true);
				continue;
			}
			if ("-V".equals(arg)) {
				config.setFlashVerify(true);
				continue;
			}
			if ("-P".equals(arg)) {
				if (arguIterator.hasNext()==false) {
					System.out.println("No argument for -P given");
					return;
				}
				config.setPort(arguIterator.next());
				continue;
			}
			if ("-PP".equals(arg)) {
				if (arguIterator.hasNext()==false) {
					System.out.println("No argument for -PP given");
					return;
				}
				config.setPortParameter(arguIterator.next());
				continue;
			}
			if ("-c".equals(arg)) {
				if (arguIterator.hasNext()==false) {
					System.out.println("No argument for -c given");
					return;
				}
				config.setPortProtocol(arguIterator.next());
				continue;
			}
			if ("-f".equals(arg)) {
				if (arguIterator.hasNext()==false) {
					System.out.println("No argument for -f given");
					return;
				}
				File hexFile = new File(arguIterator.next());
				try {
					config.setFlashData(new FlashHexReader().loadHex(hexFile));
				} catch (FileNotFoundException e) {
					System.out.println("Hex file not found: "+hexFile.getAbsolutePath());
					return;
				} catch (IOException e) {
					System.out.println("Error reading hex file: "+e.getMessage());
					return;
				}
				continue;
			}
			if ("-s".equals(arg)) {
				if (arguIterator.hasNext()==false) {
					System.out.println("No argument for -s given");
					return;
				}
				String devSignStr = arguIterator.next();
				if (devSignStr.startsWith("0x")) {
					devSignStr = devSignStr.substring(2);
				}
				int deviceSign = Integer.parseInt(devSignStr, 16);
				config.setDeviceSignature(deviceSign);
			}
		}
		FlashProgramController fm = createFlashController(config);
		fm.addFlashLogListener(new FlashLogListener() {
			@Override
			public void flashLogMessage(String message) {
				System.out.println(message);
			}
		});
		fm.flash(config);
	}

	static public FlashProgramController createFlashController(FlashControllerConfig flashControllerConfig) {
		if (flashControllerConfig==null) {
			throw new NullPointerException("Can't fill flash backend with null config.");
		}
		flashControllerConfig.verifyConfig(); // check the config
		FlashProgramController backendController = null;
		if ("stk500".equals(flashControllerConfig.getPortProtocol())) {
			backendController = new Stk500Controller();
		} else if ("arduino".equals(flashControllerConfig.getPortProtocol())) {
			backendController = new Stk500Controller();
		} else if ("stk500v2".equals(flashControllerConfig.getPortProtocol())) {
			backendController = new Stk500v2Controller();
		} else {
			throw new IllegalStateException("Unknow port protocol: "+flashControllerConfig.getPortProtocol());
		}
		return backendController;
	}
}
