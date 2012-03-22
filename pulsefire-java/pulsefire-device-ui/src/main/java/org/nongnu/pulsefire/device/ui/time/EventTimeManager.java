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

package org.nongnu.pulsefire.device.ui.time;

import java.util.ArrayList;
import java.util.List;

/**
 * EventTimeManager manages all time triggers
 * 
 * @author Willem Cazander
 */
public class EventTimeManager {

	private EventTimeThread eventTimeThread = null;
	private List<EventTimeTrigger> eventTimeTriggers = null;
	
	public EventTimeManager() {
		eventTimeTriggers = new ArrayList<EventTimeTrigger>(100);
	}
	
	public boolean isRunning() {
		return eventTimeThread!=null;
	}
	
	public void start() {
		eventTimeThread = new EventTimeThread(this);
		eventTimeThread.setName("PulseFire-Timer");
		eventTimeThread.start();
	}
	
	public void shutdown() {
		eventTimeThread.shutdown();
		eventTimeThread = null;
	}
	
	public void addRunOnce(Runnable run) {
		EventTimeTrigger t = new EventTimeTrigger("runOnce",run,0);
		t.setTimeRuns(1); // run only once
		addEventTimeTrigger(t);
	}
	
	public void addEventTimeTrigger(EventTimeTrigger eventTimeTrigger) {
		eventTimeTriggers.add(eventTimeTrigger);
	}
	
	public void removeEventTimeTrigger(EventTimeTrigger eventTimeTrigger) {
		eventTimeTriggers.remove(eventTimeTrigger);
	}
	
	public EventTimeTrigger getEventTimeTriggerByName(String name) {
		for (EventTimeTrigger e:eventTimeTriggers) {
			if (e.getTriggerName().equals(name)) {
				return e;
			}
		}
		return null;
	}
	
	public List<EventTimeTrigger> getEventExecuteSteps() {
		List<EventTimeTrigger> result = new ArrayList<EventTimeTrigger>(20);
		long currentTime = System.currentTimeMillis();
		for (int i=0;i<eventTimeTriggers.size();i++) {
			EventTimeTrigger t = eventTimeTriggers.get(i);
			if (t.getTimeNextRun()<currentTime) {
				result.add(t);
			}
		}
		return result;
	}
}
