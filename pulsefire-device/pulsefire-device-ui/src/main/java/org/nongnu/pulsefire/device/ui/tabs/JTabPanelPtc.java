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

import org.nongnu.pulsefire.device.io.protocol.CommandName;

/**
 * JTabPanelPtc
 * 
 * @author Willem Cazander
 */
public class JTabPanelPtc extends AbstractFireTabPanel {
	
	public JTabPanelPtc() {
		build(
			createCompactGrid(2, 2, 
				createFlowLeftFirePanel("config0",
					createCommandDial(CommandName.ptc_0run),
					createCommandDial(CommandName.ptc_0mul),
					createCommandStatusBox(CommandName.ptc_0step,CommandName.ptc_0map_idx,CommandName.ptc_0cnt)
				),
				createFlowLeftFirePanel("config1",
					createCommandDial(CommandName.ptc_1run),
					createCommandDial(CommandName.ptc_1mul),
					createCommandStatusBox(CommandName.ptc_1step,CommandName.ptc_1map_idx,CommandName.ptc_1cnt)
				),
				createCommandQMapTable(CommandName.ptc_0map),
				createCommandQMapTable(CommandName.ptc_1map)
			)
		);
	}
}
