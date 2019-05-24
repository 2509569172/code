package com.rjay.wsip.util;

import java.util.Arrays;
import java.util.Map;

public class MapUtils {
	
	public static void copy(Map<String,String> from,Map<String,String> to,ValueFilter valueFilter,String... keys) {
		Arrays.asList(keys).stream().
			forEach(key->to.put(key,valueFilter.newValue(key,from.get(key))));
	}
	
	public interface ValueFilter {
		String newValue(String key,String value);
	}
}
