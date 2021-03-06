package com.rjay.wsip.worker.message;

import com.rjay.wsip.protocol.msg.SipRequestMsg;
import com.rjay.wsip.protocol.msg.SipStatusMsg;
import com.rjay.wsip.protocol.header.HeaderKeys;
import com.rjay.wsip.worker.AbstractWorker;
import com.rjay.wsip.worker.register.contact.ContactContext;
import com.rjay.wsip.worker.register.contact.ContactHolder;
import io.netty.channel.ChannelHandlerContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class MessageWorker extends AbstractWorker {
	
	private Logger logger = LoggerFactory.getLogger(MessageWorker.class);

	@Override
	public void innerWork(final ChannelHandlerContext ctx,final SipRequestMsg msg) {
		String body = msg.getBody();
		if(null == body || body.contains("<?xml")){
			return;
		}
		String contact = msg.getBody();
		ContactContext contactContext = ContactHolder.getOkContactContext(contact);
		if(null == contactContext) {
			logger.error("contact:{} not found",contact);
			return;
		}
		contactContext.getChannel().writeAndFlush(msg);
	}

	@Override
	public void innerWork(ChannelHandlerContext ctx,SipStatusMsg msg) {
		String contact = msg.getContact();
		ContactContext contactContext = ContactHolder.getOkContactContext(contact);
		if(null == contactContext) {
			logger.error("contact:{} not found ",contact);
			return;
		}
		contactContext.getChannel().writeAndFlush(msg);
	}
}
