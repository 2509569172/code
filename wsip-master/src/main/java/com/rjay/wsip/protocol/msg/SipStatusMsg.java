package com.rjay.wsip.protocol.msg;

import com.rjay.wsip.protocol.header.HeaderKeys;
import org.apache.commons.lang.StringUtils;

import java.util.Map;

public class SipStatusMsg extends SipMsg {
	
	private String version;
	
	private String statusCode;
	
	private String statusName;

	public SipStatusMsg(String version,String statusCode,String statusName) {
		this.version = version;
		this.statusCode = statusCode;
		this.statusName = statusName;
	}
	
	public String getVersion() {
		return version;
	}

	public void setVersion(String version) {
		this.version = version;
	}
	
	public String getStatusCode() {
		return statusCode;
	}
	
	public void setStatusCode(String statusCode) {
		this.statusCode = statusCode;
	}
	
	public String getStatusName() {
		return statusName;
	}
	
	public void setStatusName(String statusName) {
		this.statusName = statusName;
	}

	public String getDirective() {
		String cseq = this.getHeaders().get(HeaderKeys.Key_CSEQ);
		return cseq.split(" ")[1];
	}

	@Override
	public String toString() {
		StringBuffer sb = new StringBuffer(version + " " + statusCode + " " + statusName + "\r\n");
		for(Map.Entry<String,String> entry : headers.entrySet()) {
			sb.append(entry.getKey() +": "+ entry.getValue() + "\r\n");
		}
		sb.append("\r\n");
		if(StringUtils.isNotEmpty(body)) {
			sb.append(body);
		}
		return sb.toString();
	}
}
