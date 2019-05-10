#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<assert.h>
#include<limits.h>
#include"sds.h"
#include"sdsalloc.h"

const char *SDS_NOINIT = "SDS_NOINIT";

static inline int sdsHdrSize(char type) {
	switch(type & SDS_TYPE_MASK) {
		case SDS_TYPE_5:
			return sizeof(struct sdshdr5);
		case SDS_TYPE_8:
			return sizeof(struct sdshdr8);
		case SDS_TYPE_16:
			return sizeof(struct sdshdr16);
		case SDS_TYPE_32:
			return sizeof(struct sdshdr32);
		case SDS_TYPE_64:
			return sizeof(struct sdshdr64);
	}
	return 0;
}

static inline char sdsReqType(size_t string_size) {
	if(string_size < 1 << 5)
		return SDS_TYPE_5;
	if(string_size < 1 << 8)
		return SDS_TYPE_8;
	if(string_size < 1 << 16)
		return SDS_TYPE_16;
	#if (LONG_MAX == LLONG_MAX)
		if(string_size < 1ll<<32)
			return  SDS_TYPE_32;
		return SDS_TYPE_64;
	#else
		return SDS_TYPE_32;
	#endif
}

sds sdsnewlen(const void *init,size_t initlen) {
	void *sh;
	sds s;
	char type = sdsReqType(initlen);
	
	if(type == SDS_TYPE_5 && initlen == 0) type = SDS_TYPE_8;
	int hdrlen = sdsHdrSize(type);
	unsigned char *fp;
	
	sh = s_malloc(hdrlen+initlen+1);
	if(init==SDS_NOINIT) init = NULL;
	else if(!init)
  		memset(sh,0,hdrlen+initlen+1);
	if(sh == NULL) return NULL;
	s = (char*)sh+hdrlen;
	fp = ((unsigned char *)s)-1;
	switch(type){
		case SDS_TYPE_5:{
			*fp = type | (initlen << SDS_TYPE_BITs);
			break;
		}
		case SDS_TYPE_8:{
			SDS_HDR_VAR(8,s);
			sh->len = initlen;
			sh->alloc = initlen;
			*fp = type;
			break;
		}
		case SDS_TYPE_16:{
			SDS_HDR_VAR(16,s);
			sh->len = initlen;
			sh->alloc = initlen;
			*fp = type;
			break;
		}
		case SDS_TYPE_32:{
			SDS_HDR_VAR(32,s);
			sh->len = initlen;
			sh->alloc = initlen;
			*fp = type;
			break;
		}
		case SDS_TYPE_64:{
			SDS_HDR_VAR(64,s);
			sh->len = initlen;
			sh->alloc = initlen;
			*fp = type;
			break;
		}	
	}
	if (initlen && init)
		memcpy(s,init,initlen);
	s[initlen] = '\0';
	return s;
}

sds sdsempty(void){
	return sdsnewlen("",0);
}

sds sdsnew(const char *init) {
	size_t initlen = (init == NULL) ? 0 : strlen(init);
	return sdsnewlen(init,initlen);
}

sds sdsdup(const sds s) {
	return sdsnewlen(s,sdslen(s));
}

void sdsfree(sds s){
	if(s == NULL) return;
	s_free((char*)s-sdsHdrSize(s[-1]));
}

void sdsupdatelen(sds s){
	size_t reallen = strlen(s);
	sdssetlen(s,reallen);
}

void sdsclear(sds s){
	sdssetlen(s,0);
	s[0] = '\0';
}

sds sdsMakeRoomFor(sds s,size_t addlen) {
	void *sh,*newsh;
	size_t avail = sdsavail(s);
	size_t len,newlen;
	char type,oldtype = s[-1] & SDS_TYPE_MASK;
	int hdrlen;

	if(avail >= addlen) return s;
	
	len = sdslen(s);
	sh = (char*)s-sdsHdrSize(oldtype);
	newlen = (len+addlen);
	if(newlen < SDS_MAX_PREALLOC)
		newlen *= 2;
	else
		newlen += SDS_MAX_PREALLOC;

	type = sdsReqType(newlen);
	
	if(type == SDS_TYPE_5) type = SDS_TYPE_8;

	hdrlen = sdsHdrSize(type);
	if(oldtype == type) {
		newsh = s_realloc(sh,hdrlen+newlen+1);
		if(newsh == NULL) return NULL;
		s = (char*)newsh + hdrlen;
	} else {
		newsh = s_malloc(hdrlen+newlen+1);
		if(newsh == NULL) return NULL;
		memcpy((char*)newsh+hdrlen,s,len+1);
		s_free(sh);
		s = (char*)newsh+hdrlen;
		s[-1] = type;
		sdssetlen(s,len);
	}
	sdssetalloc(s,newlen);
	return s;
}

sds sdsRemoveFreeSpace(sds s){
	void *sh,*newsh;
	char type,oldtype = s[-1] & SDS_TYPE_MASK;
	int hdrlen,oldhdrlen = sdsHdrSize(oldtype);
	size_t len = sdslen(s);
	sh = (char*)s-oldhdrlen;

	type = sdsReqType(len);
	hdrlen = sdsHdrSize(type);

	if(oldtype == type || type > SDS_TYPE_8) {
		newsh = s_realloc(sh,oldhdrlen+len+1);
		if(newsh == NULL) return NULL;
		s = (char*)newsh+oldhdrlen;
	} else {
		newsh = s_malloc(hdrlen+len+1);
		if(newsh == NULL) return NULL;
		memcpy((char*)newsh+hdrlen,s,len+1);
		s_free(sh);
		s = (char*)newsh+hdrlen;
		s[-1] = type;
		sdssetlen(s,len);
	}
	sdssetalloc(s,len);
	return s;
}

size_t sdsAllocSize(sds s) {
	size_t alloc = sdsalloc(s);
	return sdsHdrSize(s[-1])+alloc+1;
}

void *sdsAllocPtr(sds s){
	return (void*)(s-sdsHdrSize(s[-1]));
}

void sdsIncrLen(sds s,ssize_t incr){
	unsigned char flags = s[-1];
	size_t len;
	switch(flags & SDS_TYPE_MASK) {
		case SDS_TYPE_5:{
			unsigned char *fp = ((unsigned char*)s)-1;
			unsigned char oldlen = SDS_TYPE_5_LEN(flags);
			assert((incr > 0 && oldlen+incr < 32)||(incr < 0 && oldlen >= (unsigned int)(-incr)));
			*fp = SDS_TYPE_5 | ((oldlen+incr) << SDS_TPE_BITS);
			len = oldlen+incr;
			break;
		}
		case SDS_TYPE_8:{
			SDS_HDR_VAR(8,s);
			assert((incr >= 0 && sh->alloc-sh->len >= incr) || (incr < 0 && sh->len >= (unsigned int)(-incr)));
			len = (sh->len += incr);
			break;
		}
		case SDS_TYPE_16:{
			SDS_HDR_VAR(16,s);
			assert((incr >= 0 && sh->alloc-sh->len >= incr) || (incr < 0 && sh->len >= (unsigned int)(-incr)));
			len = (sh->len += incr);
			break;
		}
		case SDS_TYPE_32:{
			SDS_HDR_VAR(32,s);
			assert((incr >= 0 && sh->alloc - sh->len >= incr) || (incr < 0 && sh->len >= (unsigned int)(-incr)));
			len = (sh->len += incr);
			break;
		}
		case SDS_TYPE_64:{
			SDS_HDR_VAR(64,s);
			assert((incr >= 0 && sh->alloc-sh->len >= (uint64_t)incr) || (incr < 0 && sh->len >= (uint64_t)(-incr)));
			len = (sh->len += incr);
			break;
		}
		default:len = 0;
	}
	s[len] = '\0';
}

sds sdsgrowzero(sds s,size_t len) {
	size_t curlen = sdslen(s);
	
	if(len <= curlen) return s;
	s = sdsMakeRoomFor(s,len-curlen);
	if(s == NULL) return NULL;
	
	memset(s+curlen,0,(len-curlen+1));
	sdssetlen(s,len);
	return s;
}

sds sdscatlen(sds s,const void *t,size_t len){
	size_t curlen = sdslen(s);
	
	s = sdsMakeRoomFor(s,len);
	if(s == NULL) return NULL;
	memcpy(s+curlen,t,len);
	sdssetlen(s,curlen+len);
	s[curlen+len] = '\0';
	return s;
}

sds sdscat(sds s,const char *t){
	return sdscatlen(s,t,strlen(t));
}

sds sdscatsds(sds s,const sds t){
	return sdscatlen(s,t,sdslen(t));
}

sds sdscpylen(sds s,const char *t,size_t len){
	if(sdsalloc(s) < len) {
		s = sdsMakeRoomFor(s,len-sdslen(s));
		if(s == NULL) return NULL;
	}
	memcpy(s,t,len);
	s[len] = '\0';
	sdssetlen(s,len);
	return s;
}

sds sdscpy(sds s,const char *t){
	returnsdscpylen(s,t,strlen(t));
}

#define SDS_LLSTR_SIZE 21
int sdsll2str(char *s,long long value){
	char *p,aux;
	unsigned long long v;
	size_t l;

	v = (value < 0) ? -value : value;
	p = s;
	do {
		*p++ = '0'+(v%10);
		v /= 10;
	}while(v);
	if(value < 0) *p++ = '-';
	
	l = p-s;
	*p = '\0';

	/* Reverse the string. */
	p--;
	while(s < p){
		aux = *s;
		*s = *p;
		*p = aux;
		s++;
		p--;
	}
	return l;
		
}


