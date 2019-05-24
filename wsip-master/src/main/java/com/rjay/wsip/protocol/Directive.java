package com.rjay.wsip.protocol;

public enum Directive {
	
	REGISTER,

	INVITE,

	REFER,
	
	NOTIFY,
	
	PUBLISH,
	
	SUBSCRIBE,
	
	UPDATE,
	
	INFO,
	
	MESSAGE,
	
	OPTIONS,
	
	CANCEL,
	
	ACK,
	
	BYE;

	public static boolean constains(String name) {
		for(Directive directive : values()) {
			if(directive.name().equals(name)) {
				return true;
			}
		}
		return false;
	}
         	
}
