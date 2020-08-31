package edu.handong.service;

public class FuzzingInfo {
	
	public String ip = "";
	public String last_update = "";
	public int runtime = -1;
	public int cycles_done = -1;
	public int total_paths = -1;
	public int unique_crashes = -1;
	public int unique_hangs = -1;

	public FuzzingInfo(String ip, String last_update, int runtime, int cycles_done, int total_paths, int unique_crashes, int unique_hangs) {
	    // ...
		this.ip = ip;
		this.last_update = last_update;
		this.runtime = runtime;
		this.cycles_done = cycles_done;
		this.total_paths = total_paths;
		this.unique_crashes = unique_crashes;
		this.unique_hangs = unique_hangs;
	 }
}
