package com.rjay.wsip.worker.proxy;

import com.rjay.wsip.protocol.header.HeaderKeys;
import com.rjay.wsip.protocol.msg.SipStatusMsg;
import com.rjay.wsip.worker.AbstractWorker;
import com.rjay.wsip.protocol.msg.SipRequestMsg;
import com.rjay.wsip.worker.registrar.contact.ContactContext;
import com.rjay.wsip.worker.registrar.contact.ContactHolder;
import io.netty.channel.ChannelHandlerContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ProxyWorker extends AbstractWorker {
	
	private Logger logger = LoggerFactory.getLogger(ProxyWorker.class);

	@Override
	public void innerWork(ChannelHandlerContext ctx,SipRequest msg) {
		String contact = msg.getContact();
		ContactContext contactContext = ContactHolder.getOkContactContext(contact);
		if(null == contactContext) {
			logger.error("contact:{} not found",contact);
			return;
		}
		contactContext.getChannel().writeAndFlush(msg);
	}

	public void innertWork(ChannelHandlerContext ctx, SipStatusMsg msg) {
		String contact = msg.getContact();
		ContactContext contactContext = ContactHolder.getOkContactContext(contact);
		if(null == contactContext) {
			logger.error("contact:{} not found",contact);
			return;
		}
		contactContext.getChannel().writeAndFlush(msg);
	}
}
