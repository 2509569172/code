#ifndef __SDS_H
#define __SDS_H

#define SDS_MAX_PREALLOC (1024*1024)
const char *SDS_NOINIT;

#include <sys/types.h>
#include <stdarg.h>
#include <stdint.h>

typedef char *sds;

struct __attribute__ ((__packed__)) sdshdr5 {
	unsigned char flags;
	char buf[];
};

struct __attribute__ ((__packed__)) sdshdr8 {
	uint8_t len;
	uint8_t alloc;
	unsigned char flags;
	char buf[];
};

struct __attribute__ ((__packed__)) sdshdr16 {
	uint16_t len;
	uint16_t alloc;
	unsigned char flags;
	char buf[];
};

struct __attribute__ ((__packed__)) sdshdr32 {
	uint32_t len;
	uint32_t alloc;
	unsigned char flags;
	char buf[];
};

struct __attribute__ ((__packed__)) sdshdr64 {
	uint64_t len;
	uint64_t alloc;
	unsigned char flags;
	char buf[];
};

#define SDS_TYPE_5 0
#define SDS_TYPE_8 1
#define SDS_TYPE_16 2
#define SDS_TYPE_32 3
#define SDS_TYPE_64 4
#define SDS_TYPE_MASK 7
#define SDS_TYPE_BITS 3
#define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
#define SDS_HDR(T,s) ((struct sdshdr##T *)((s)-(sizeof(struct sdshdr##T))))
#define SDS_TYPE_5_LEN(f) ((f)>>SDS_TYPE_BITS)

static inline size_t sdslen(const sds s) {
	unsigned char flags = s[-1];
	switch(flags & SDS_TYPE_MASK) {
		case SDS_TYPE_5:
			return SDS_TYPE_5_LEN(flags);
		case SDS_TYPE_8:
			return SDS_HDR(8,s)->len;
		case SDS_TYPE_16:
			return SDS_HDR(16,s)->len;
		case SDS_TYPE_32:
			return SDS_HDR(32,s)->len;
		case SDS_TYPE_64:
			return SDS_HDR(64,s)->len;
	}
	return 0;
}

static inline size_t sdsavail(const sds s){
	unsigned char flags = s[-1];
	switch(flags & SDS_TYPE_MASK) {
		case SDS_TYPE_5:{
			return 0;
		}
		case SDS_TYPE_8:{
			SDS_HDR_VAR(8,s);
			return sh->alloc - sh->len;
		}
		case SDS_TYPE_16:{
			SDS_HDR_VAR(16,s);
			return sh->alloc - sh->len;
		}
		case SDS_TYPE_32:{
			SDS_HDR_VAR(32,s);
			return sh->alloc - sh->len;
		}
		case SDS_TYPE_64:{
			SDS_HDR_VAR(64,s);
			return sh->alloc - sh->len;
		}
	}
	return 0;
}

static inline void sdssetlen(sds s,size_t newlen) {
	unsigned char flags = s[-1];
	switch(flags & SDS_TYPE_MASK) {
		case SDS_TYPE_5:
			{
				unsigned char *fp = ((unsigned char*)s)-1;
				*fp = SDS_TYPE_5 | (newlen << SDS_TYPE_BITS);
			}
			break;
		case SDS_TYPE_8:
			SDS_HDR(8,s)->len = newlen;
			break;
		case SDS_TYPE_16:
			SDS_HDR(16,s)->len = newlen;
			break;
		case SDS_TYPE_32:
			SDS_HDR(32,s)->len = newlen;
			break;
		case SDS_TYPE_64:
			SDS_HDR(64,s)->len = newlen;
			break;
	}
}

static inline void sdsinclen(sds s,size_t inc){
	unsigned char flags = s[-1];
	switch(flags & SDS_TYPE_MASK) {
		case SDS_TYPE_5:
			{
				unsigned char *fp = ((unsigned char*)s)-1;
				unsigned char newlen = SDS_TYPE_5_LEN(flags)+inc;
				*fp = SDS_TYPE_5 | (newlen << SDS_TYPE_BITS);
			}
			break;
		case SDS_TYPE_8:
			SDS_HDR(8,s)->len += inc;
			break;
		case SDS_TYPE_16:
			SDS_HDR(16,s)->len += inc;
			break;
		case SDS_TYPE_32:
			SDS_HDR(32,s)->len += inc;
			break;
		case SDS_TYPE_64:
			SDS_HDR(64,s)->len += inc;
			break;
	}
}

static inline size_t sdsalloc(const sds s) {
	unsigned char flags = s[-1];
	switch(flags & SDS_TYPE_MASK) {
		case SDS_TYPE_5:
			return SDS_TYPE_5_LEN(flags);
		case SDS_TYPE_8:
			return SDS_HDR(8,s)->alloc;
		case SDS_TYPE_16:
			return SDS_HDR(16,s)->alloc;
		case SDS_TYPE_32:
			return SDS_HDR(32,s)->alloc;
		case SDS_TYPE_64:
			return SDS_HDR(64,s)->alloc;
	}
	return 0;	
}

static inline size_t sdssetalloc(sds s,size_t newlen){
	unsigned char flags = s[-1];
	switch(flags & SDS_TYPE_MASK) {
		case SDS_TYPE_5:
			/* nothing to do,this type has no total allocation info. */
			break;
		case SDS_TYPE_8:
			SDS_HDR(8,s)->alloc = newlen;
			break;
		case SDS_TYPE_16:
			SDS_HDR(16,s)->alloc = newlen;
			break;
		case SDS_TYPE_32:
			SDS_HDR(32,s)->alloc = newlen;
			break;
		case SDS_TYPE_64:
			SDS_HDR(64,s)->alloc = newlen;
			break;
	}
}

sds sdsnewlen(const void *init,size_t initlen);
sds sdsnew(const char *init);
sds sdsempty(void);
sds sdsup(const sds s);
void sdsfree(sds s);
sds sdsgrowzero(sds s,size_t len);
sds sdscat(sds s,const char *t);
sds sdscatsds(sds s,const sds t);
sds sdscpylen(sds s,const char *t,size_t len);
sds sdscpy(sds s,const char *t);

sds sdscatvprintf(sds s,const char *fmt,va_list ap);
#ifdef __GNUC__
sds sdscatprintf(sds s,const char *fmt,...);
sds sdstrim(sds s,const char *cset);
void sdsrange(sds s,ssize_t start,ssize_t end);
void sdsupdatelen(sds s);
void sdsclear(sds s);
int sdscmp(const sds s1,const sds s2);
sds *sdssplitlen(const char *s,ssize_t len,const char *sep,int seplen,int *count);
void sdsfreesplitres(sds *tokens,int count);
void sdstolower(sds s);
void sdstoupper(sds s);
sds sdsfromlonglong(long long value);
sds sdscatrepr(sds s,const char *p,size_t len);
sds *sdssplitargs(const char *line,int *argc);
sds sdsmapchars(sds s,const char *from,const char *to,size_t setlen);
sds sdsjoin(char **argv,int argc,char *sep);
sds sdsjoinsds(sds *argv,int argc,const char *sep,size_t seplen);

sds sdsMakeRoomFor(sds s,size_t addlen);
void sdsIncrLen(sds s,ssize_t incr);
sds sdsRemoveFreeSpace(sds s);
size_t sdsAllocSiZe(sds s);
void *sdsAllocPtr(sds s);

void *sds_malloc(size_t size);
void *sds_realloc(void *ptr,size_t size);
void sds_free(void *ptr);

#ifdef REDIS_TEST
int sdsTest(int argc,char *argv[]);
#endif

#endif
