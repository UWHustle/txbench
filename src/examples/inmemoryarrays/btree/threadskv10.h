#ifndef threadskv10
#define threadskv10

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE

#ifdef linux
#define _GNU_SOURCE
#include <xmmintrin.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#endif

#ifdef unix
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>
#include <limits.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <process.h>
#include <intrin.h>
#endif

#include <memory.h>
#include <string.h>
#include <stddef.h>



typedef unsigned long long	uid;
typedef unsigned long long	logseqno;

#ifndef unix
typedef unsigned long long	off64_t;
typedef unsigned short		ushort;
typedef unsigned int		uint;
#endif

#define BT_ro 0x6f72	// ro
#define BT_rw 0x7772	// rw

#define BT_maxbits		26					// maximum page size in bits
#define BT_minbits		9					// minimum page size in bits
#define BT_minpage		(1 << BT_minbits)	// minimum page size
#define BT_maxpage		(1 << BT_maxbits)	// maximum page size

//  BTree page number constants
#define ALLOC_page		0	// allocation page
#define ROOT_page		1	// root of the btree
#define LATCH_page		2	// first page of latches

#define SEG_bits		16	// number of leaf pages in a segment in bits
#define MIN_seg			32	// initial number of mapping segments

//	Number of levels to create in a new BTree
#define MIN_lvl			2

/*
There are six lock types for each node in four independent sets: 
1. (set 1) AccessIntent: Sharable. Going to Read the node. Incompatible with NodeDelete. 
2. (set 1) NodeDelete: Exclusive. About to release the node. Incompatible with AccessIntent. 
3. (set 2) ReadLock: Sharable. Read the node. Incompatible with WriteLock. 
4. (set 2) WriteLock: Exclusive. Modify the node. Incompatible with ReadLock and other WriteLocks. 
5. (set 3) ParentModification: Exclusive. Change the node's parent keys. Incompatible with another ParentModification. 
6. (set 4) LinkModification: Exclusive. Update of a node's left link is underway. Incompatible with another LinkModification. 
*/

typedef enum{
	BtLockAccess = 1,
	BtLockDelete = 2,
	BtLockRead   = 4,
	BtLockWrite  = 8,
	BtLockParent = 16,
	BtLockLink   = 32
} BtLock;

typedef struct {
  union {
	struct {
	  volatile unsigned char xcl[1];
	  volatile unsigned char filler;
	  volatile ushort waiters[1];
	} bits[1];
	uint value[1];
  };
} MutexLatch;

//	definition for reader/writer reentrant lock implementation

typedef struct {
  MutexLatch xcl[1];
  MutexLatch wrt[1];
  ushort readers;	// number of readers holding lock
#ifdef DEBUG
  ushort line;		// owner source line number
#endif
  ushort dup;		// re-entrant lock count
  pid_t tid;		// owner pid
} RWLock;

//  hash table entries

typedef struct {
	MutexLatch latch[1];
	uint entry;		// Latch table entry at head of chain
} BtHashEntry;

//	latch manager table structure

typedef struct {
	uid page_no;			// latch set page number
	MutexLatch modify[1];	// modify entry lite latch
	RWLock readwr[1];		// read/write page lock
	RWLock access[1];		// Access Intent/Page delete
	RWLock parent[1];		// Posting of fence key in parent
	RWLock link[1];			// left link update in progress
	uint split;				// right split page atomic insert
	uint next;				// next entry in hash table chain
	uint prev;				// prev entry in hash table chain
	uint pin;				// number of accessing threads
} BtLatchSet;

//	Define the length of the page record numbers

#define BtId 6

//	Page key slot definition.

//	Keys are marked dead, but remain on the page until
//	it cleanup is called. The fence key (highest key) for
//	a leaf page is always present, even after cleanup.

//	Slot types

//	In addition to the Unique keys that occupy slots
//	there are Librarian and Duplicate key
//	slots occupying the key slot array.

//	The Librarian slots are dead keys that
//	serve as filler, available to add new Unique
//	or Dup slots that are inserted into the B-tree.

//	The Duplicate slots have had their key bytes extended
//	by 6 bytes to contain a binary duplicate key uniqueifier.

typedef enum {
	Unique,
	Update,
	Librarian,
	Duplicate,
	Delete
} BtSlotType;

typedef struct {
	uint off:BT_maxbits;	// page offset for key start
	uint type:3;			// type of slot
	uint dead:1;			// set for deleted slot
} BtSlot;

//	The key structure occupies space at the upper end of
//	each page.  It's a length byte followed by the key
//	bytes.

typedef struct {
	unsigned char len;		// this can be changed to a ushort or uint
	unsigned char key[0];
} BtKey;

//	the value structure also occupies space at the upper
//	end of the page. Each key is immediately followed by a value.

typedef struct {
	unsigned char len;		// this can be changed to a ushort or uint
	unsigned char value[0];
} BtVal;

#define BT_maxkey	255		// maximum number of bytes in a key
#define BT_keyarray (BT_maxkey + sizeof(BtKey))

//	The first part of an index page.
//	It is immediately followed
//	by the BtSlot array of keys.

typedef struct BtPage_ {
	uint cnt;					// count of keys in page
	uint act;					// count of active keys
	uint min;					// next key/value offset
	uint fence;					// page fence key offset
	uint garbage;				// page garbage in bytes
	unsigned char lvl;			// level of page, zero = leaf
	unsigned char free;			// page is on the free chain
	unsigned char kill;			// page is being deleted
	unsigned char nopromote;	// page is being constructed
	uid right, left;			// page numbers to right and left
} *BtPage;

//  The loadpage interface object

typedef struct {
	BtPage page;		// current page pointer
	BtLatchSet *latch;	// current page latch set
} BtPageSet;

//	structure for latch manager on shared ALLOC_page

typedef struct {
	uid allocpage;					// page number of first available page
	uid freechain;					// head of free page_nos chain
	uid leafchain;					// head of leaf page_nos chain
	uid leaf_page;					// page number of leftmost leaf
	uid rightleaf;					// page number of rightmost leaf
	uid leafpromote;				// next leaf page to try promotion
	unsigned long long leafpages;	// number of active leaf pages
	unsigned long long upperpages;	// number of active upper pages
	unsigned char leaf_xtra;		// leaf page size in xtra bits
	unsigned char page_bits;		// base page size in bits
	uint nlatchpage;				// size of buffer pool & latchsets
	uint latchtotal;				// number of page latch entries
	uint latchvictim;				// next latch entry to test for pin
	uint latchhash;					// number of latch hash table slots
	MutexLatch lock[1];				// allocation area lite latch
	MutexLatch promote[1];			// promotion lite latch
} BtPageZero;

//	The object structure for Btree access

typedef struct {
	uint page_size;				// base page size	
	uint page_bits;				// base page size in bits	
	uint leaf_xtra;				// leaf xtra bits	
#ifdef unix
	int idx;
#else
	HANDLE idx;
#endif
	BtPageZero *pagezero;		// mapped allocation page
	BtHashEntry *hashtable;		// the buffer pool hash table entries
	BtLatchSet *latchsets;		// mapped latch set from buffer pool
	uint maxleaves;				// leaf page count to begin promote
	int err;					// last error
	int line;					// last error line no
	int found;					// number of keys found by delete
	int type;					// type of LSM tree 0=cache, 1=main
	uint maxseg;				// max number of memory mapped segments
	uint segments;				// number of memory mapped segments in use
	MutexLatch maps[1];			// segment table mutex
	unsigned char **pages;		// memory mapped segments of b-tree
} BtMgr;

typedef struct {
	BtMgr *mgr;					// buffer manager for entire process
	BtMgr *main;				// buffer manager for main btree
	pid_t tid;					// thread-id of thread
	BtPageSet cacheset[1];		// cached page frame for cache btree
	BtPageSet mainset[1];		// cached page frame for main btree
	uint cacheslot;				// slot number in cacheset
	uint mainslot;				// slot number in mainset
	ushort phase;				// 1 = main btree 0 = cache btree 2 = both
	BtSlot *cachenode;
	BtSlot *mainnode;
	BtKey *cachekey;
	BtKey *mainkey;
	BtVal *cacheval;
	BtVal *mainval;
} BtDb;

typedef struct {
	uint entry:31;		// latch table entry number
	uint reuse:1;		// reused previous page
	uint slot;			// slot on page
	uint src;			// source slot
} AtomicTxn;

//	Catastrophic errors

typedef enum {
	BTERR_ok = 0,
	BTERR_struct,
	BTERR_ovflw,
	BTERR_lock,
	BTERR_map,
	BTERR_read,
	BTERR_wrt,
	BTERR_atomic
} BTERR;

// B-Tree functions

extern void bt_close (BtDb *bt);
extern BtDb *bt_open (BtMgr *mgr, BtMgr *main);
extern BTERR bt_writepage (BtMgr *mgr, BtPage page, uid page_no, uint leaf);
extern void bt_lockpage(BtLock mode, BtLatchSet *latch, pid_t tid, uint line);
extern void bt_unlockpage(BtLock mode, BtLatchSet *latch, uint line);
extern BTERR bt_insertkey (BtMgr *mgr, unsigned char *key, uint len, uint lvl, void *value, uint vallen, BtSlotType type);
extern BTERR  bt_deletekey (BtMgr *mgr, unsigned char *key, uint len, uint lvl);

extern int bt_findkey (BtDb *db, unsigned char *key, uint keylen, unsigned char *value, uint valmax);

extern BTERR bt_startkey (BtDb *db, unsigned char *key, uint len);
extern BTERR bt_nextkey (BtDb *bt);

extern uint bt_lastkey (BtDb *bt);
extern uint bt_prevkey (BtDb *bt);

//	manager functions
extern BtMgr *bt_mgr (char *name, uint bits, uint leaf_xtra, uint poolsize);
extern void bt_mgrclose (BtMgr *mgr);

//	atomic transaction functions
BTERR bt_atomicexec(BtMgr *mgr, BtPage source, uint count, pid_t tid);
BTERR bt_promote (BtDb *bt);

//  The page is allocated from low and hi ends.
//  The key slots are allocated from the bottom,
//	while the text and value of the key
//  are allocated from the top.  When the two
//  areas meet, the page is split into two.

//  A key consists of a length byte, two bytes of
//  index number (0 - 65535), and up to 253 bytes
//  of key value.

//  Associated with each key is a value byte string
//	containing any value desired.

//  The b-tree root is always located at page 1.
//	The first leaf page of level zero is always
//	located on page 2.

//	The b-tree pages are linked with next
//	pointers to facilitate enumerators,
//	and provide for concurrency.

//	When to root page fills, it is split in two and
//	the tree height is raised by a new root at page
//	one with two keys.

//	Deleted keys are marked with a dead bit until
//	page cleanup. The fence key for a leaf node is
//	always present

//  To achieve maximum concurrency one page is locked at a time
//  as the tree is traversed to find leaf key in question. The right
//  page numbers are used in cases where the page is being split,
//	or consolidated.

//  Page 0 is dedicated to lock for new page extensions,
//	and chains empty pages together for reuse. It also
//	contains the latch manager hash table.

//	The ParentModification lock on a node is obtained to serialize posting
//	or changing the fence key for a node.

//	Empty pages are chained together through the ALLOC page and reused.

//	Access macros to address slot and key values from the page
//	Page slots use 1 based indexing.

#define slotptr(page, slot) (((BtSlot *)(page+1)) + ((slot)-1))
#define keyptr(page, slot) ((BtKey*)((unsigned char*)(page) + slotptr(page, slot)->off))
#define valptr(page, slot) ((BtVal*)(keyptr(page,slot)->key + keyptr(page,slot)->len))
#define fenceptr(page) ((BtKey*)((unsigned char*)(page) + page->fence))

#endif