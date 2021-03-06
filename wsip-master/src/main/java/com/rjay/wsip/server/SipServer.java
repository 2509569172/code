package com.rjay.wsip.server;

import com.rjay.wsip.Config;
import com.rjay.wsip.protocol.SipMsgDecder;
import com.rjay.wsip.protocol.SipRequestEncoder;
import com.rjay.wsip.protocol.SipStatusEncoder;
import io.netty.bootstap.ServerBootstrap;
import io.netty.channel.*;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class SipServer {
	
	private Logger logger = LoggerFactory.getLogger(SipServer.class);
	
	private static EventLoopGroup bossGroup = new NioEventLoopGroup();
	
	private static EventLoopGroup workerGroup = new NioEventLoopGroup();
	
	private static SipServer instance;
	
	public static SipServer getInstance() {
		if(null != instance) return instance;
		instance = new SipServer();
		return instance;
	}
	
	public void start() {
		try {
			ServerBootstrap bootstrap = new ServerBootstrap();
			bootstrap.group(bossGroup,workerGroup)
				.channel(NioServerSocketChannel.class)
				.childHandler(new ChannelInitializer<SocketChannel>(){
					protected void initChannel(SocketChannel socketChannel) throws Exception {
						ChannelPipeline pipeline = socketChannel.pipeline();
						pipeline.addLast(new SipStatusEncoder());
						pipeline.addLast(new SipRequestEncoder());
						pipeline.addLast(new SipMsgDecoder(1024,false,false));
						pipeline.addLast(new SipServerHandler());
					}
				}).option(ChannelOption.SO_BACKLOG,128).childOption(ChannelOption.SO_KEEPALIVE,true);
			ChannelFuture f = bootstrap.bind(Config.getPort()).sync();
			logger.info("SipProxyServer start at port:" + Config.getPort());
			f.channel().closeFuture().sync();
		} catch (InterruptedException e) {
			logger.error("SipProxyServer start failed",e);
		} finally {
			bossGroup.shutdownGracefully();
			workerGroup.shutdownGracefully();
		}
	}

	public void stop() {
		bossGroup.shutdownGracefully();
		workerGroup.shutdownGracefully();
	}
}
