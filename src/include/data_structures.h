#include <cstdint>
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

/* Adapted from 
 * Carrier - File System Forensic Analysis Chapter 13: NTFS Data Structures
 * and The Sleuth Kit source code */ 

#define FS_MAGIC 0x202020205346544E // "NTFS"

/* Boot Sector: First 16 clusters reserved */

#pragma pack(push, 1)
typedef struct {
	uint8_t jump_instruction[3];
    uint64_t oem_name;
    uint16_t bytes_x_sector;
    uint8_t sectors_x_cluster;
    uint16_t reserved_sectors;
    uint8_t unused1[5];
    uint8_t media_descriptor;
    uint8_t unused2[18];
    uint64_t sectors_x_volume;
    uint64_t cluster_MFT_start;
    uint64_t cluster_MFTMirr_start;
    uint8_t entry_size;
    uint8_t unused3[3];
    uint8_t index_size;
    uint8_t unused4[3];
    uint64_t serial_number;
    uint32_t unused5;
    uint8_t boot_code[426];
    uint16_t signature;
} bootSector;
#pragma pack(pop)

#define BS_MAGIC 0xAA55 // Signature

/* Basic MFT entry */

#pragma pack(push, 1)
typedef struct {
	uint32_t signature;
    uint16_t fixup_off;
    uint16_t fixup_entries;
    uint64_t LSN;             // $LogFile Sequence Number
    uint16_t sequence_value;
    uint16_t link_count;
    uint16_t first_attr_off;
    uint16_t flags;
    uint32_t used_size;
    uint32_t alloc_size;
	uint8_t base_reference[6];
	uint16_t next_attr_id;
} mftEntryHeader;
#pragma pack(pop)

#define MFT_MAGIC 0x454c4946 		// "FILE"
#define MFT_MAGIC_BAD 0x44414142	// "BAAD"
#define MFT_INUSE 0x0001		
#define MFT_DIR 0x0002

#define MDF_MFT 0x0
#define MDF_MFTMIRR 0x1
#define MDF_LOG 0x2
#define MDF_VOL 0x3
#define MDF_ATTRDEF 0x4
#define MDF_ROOT 0x5
#define MDF_BITMAP 0x6
#define MDF_BOOT 0x7
#define MDF_BADCL 0x8
#define MDF_SECURE 0x9
#define MDF_UPCASE 0xA
#define MDF_QUOTA 0x18
#define MDF_OBJID 0x19

/* Attribute data structure */

// Resident attribute

typedef struct {
	uint32_t size;
    uint16_t offset;
} residentData;

//Non-resident attribute

typedef struct {
	uint64_t VCN_start;    // Virtual Cluster Number
    uint64_t VCN_end;
    uint16_t offset;
    uint16_t compr_unit_size;
    uint32_t unused;
    uint64_t alloc_size;
    uint64_t actual_size;
    uint64_t init_size;
} non_residentData;

#pragma pack(push, 1)
typedef struct {
	uint32_t type;
    uint32_t length;
    uint8_t resident_flag;
    uint8_t name_length;
    uint16_t name_off;
    uint16_t flags;
    uint16_t attr_id;
} mftAttrHeader;
#pragma pack(pop)

// Flags
#define ATTR_RESIDENT 0x00
#define ATTR_NONRESIDENT 0x01

#define ATTR_COMPRESSED 0x0001
#define ATTR_ENCRYPTED 0x4000
#define ATTR_SPARSE 0x8000

#define END_OF_ENTRY 0xFFFFFFFF 

// Basic MFT Attributes

#define ATTR_STDINF               0x10  
#define ATTR_FILENAME             0x30  
#define ATTR_OBJECTID             0x40  
#define ATTR_SECURITY_DESC        0x50 
#define ATTR_VOLUME_NAME          0x60 
#define ATTR_VOLUME_INFO          0x70 
#define ATTR_DATA                 0x80 
#define ATTR_INDEX_ROOT           0x90 
#define ATTR_INDEX_ALLOCATION     0xA0 
#define ATTR_BITMAP               0xB0 
#define ATTR_REPARSE_POINT        0xC0 
#define ATTR_EA_INFORMATION       0xD0  // Extended attributes information
#define ATTR_EA                   0xE0  // Extended attributes
#define ATTR_PROPERTY_SET         0xF0  
#define ATTR_LOGGED_UTILITY_STREAM 0x100 

/* Index data structures */

// Node header

typedef struct {
    uint32_t start_off;
    uint32_t end_used_off;
    uint32_t end_alloc_off;
    uint32_t flags;
} idxNode_head;

// Flags
#define IDX_NODE_CHILD 0x00000001

// $INDEX_ROOT attribute header

#pragma pack(push, 1)
typedef struct {
    uint32_t type;
    uint32_t collation_rule;
    uint32_t size_bytes;
    uint8_t size_clusters;
    uint8_t unused[3];
    idxNode_head head;
} idxRoot_head;
#pragma pack(pop)

// Index Record header
typedef struct {
    uint32_t signature;
    uint16_t offset;
    uint16_t num_entries;
    uint64_t LSN;
    uint64_t VCN;
    idxNode_head head;
} idxRecord_head;

#define NTFS_IDXREC_MAGIC	0x58444e49 // "INDX"

// Generic index entry (also for directory)

#pragma pack(push, 1)
typedef struct {
	uint8_t file_ref[6]; 
	uint16_t seq_number;
    uint16_t length_entry;	
    uint16_t length_stream;		// For directory entry (length of $FILE_NAME attribute)
    uint8_t flags;
    uint8_t useless[3];
    uint8_t stream;
} idxEntry;
#pragma pack(pop)

#define IDX_SUBNODE 0x00000001
#define IDX_LAST 0x00000002

// macro for getting the VCN of a child node (only if flag is set)
#define GET_VCN_CHILD(e) (*(uint64_t*)((uint8_t*)(e) + (e)->length_entry - 8))

/* Attributes */

// $FILE_NAME

#pragma pack(push, 1)
typedef struct {
	uint8_t parent_reference[6];
	uint16_t seq_number;
	uint64_t creation_time;
	uint64_t modified_time;
	uint64_t mft_modified_time;
	uint64_t accessed_time;
	uint64_t alloc_size;
	uint64_t real_size;
	uint64_t flags;
	uint8_t name_length;
	uint8_t name_space;
} fileName;
#pragma pack(pop)

#define NAMEPSACE_POSIX 0x00
#define NAMEPSACE_WIN32 0x01
#define NAMEPSACE_DOS 0x02
#define NAMEPSACE_WIN_DOS 0x03

// $STANDARD_INFORMATION

#pragma pack(push, 1)
typedef struct {
	uint64_t creation_time;
	uint64_t modified_time;
	uint64_t mft_modified_time;
	uint64_t accessed_time;
	uint32_t dos_perms;
	uint32_t max_version;
	uint32_t version;
	uint32_t class_id;
	//uint32_t owner_id;
	//uint32_t security_id;
	//uint64_t quota_charged;
	//uint64_t usn;
} standardInfo;
#pragma pack(pop)

// Flags

#define STDINF_READONLY 0x00000001
#define STDINF_HIDDEN 0x00000002
#define STDINF_SYSTEM 0x00000004
#define STDINF_ARCHIVE 0x00000020
#define STDINF_DEVICE 0x00000040
#define STDINF_NORMAL 0x00000080
#define STDINF_TEMPORARY 0x00000100
#define STDINF_SPARSE_FILE 0x00000200
#define STDINF_REPARSE_POINT 0x00000400
#define STDINF_COMPRESSED 0x00000800
#define STDINF_OFFLINE 0x00001000
#define STDINF_NOT_CONTENT_INDEXED 0x00002000
#define STDINF_ENCRYPTED 0x00004000

// $VOLUME_NAME just has name (UFT-16 Unicode)

// $VOLUME_INFORMATION

#pragma pack(push, 1)
typedef struct {
	uint64_t unused;
	uint8_t max_version;
	uint8_t min_version;
	uint16_t flags;
} volumeInfo;
#pragma pack(pop)

// Flags
#define VOL_DIRTY 0x0001
#define VOL_RESIZE_LOG 0x0002
#define VOL_UPGRADE_MOUNT 0x0004
#define VOL_MOUNT_NT 0x0008
#define VOL_DELETE_USN 0x0010
#define VOL_REPAIR 0x0020
#define VOL_MOD_CHKDSK 0x8000

// $ATTRIBUTE_LIST

#pragma pack(push, 1)
typedef struct {
	uint32_t type;
	uint16_t length;
	uint8_t name_length;
	uint8_t name_off;
	uint64_t VCN;
	uint8_t file_reference[6];
	uint16_t seq_number;
	uint16_t id;
} attrList;
#pragma pack(pop)

// $OBJECT_ID

#pragma pack(push, 1)
typedef struct {
	uint64_t object_id[2];
	uint64_t birth_volume_id[2];
	uint64_t birth_object_id[2];
	uint64_t birth_domain_id[2];
} objectId_attr;
#pragma pack(pop)

// $REPARSE_POINT

typedef struct {            // Offset from byte 16 (end of struct)
	uint32_t flags;
	uint16_t size;
	uint16_t unused;
	uint16_t target_name_offset;
	uint16_t target_name_length;
	uint16_t print_target_name_offset;
	uint16_t print_target_name_length;
} reparsePoint;

// $BITMAP is just         a bitmap

/* Important files */

// $AttrDef

#pragma pack(push, 1)
typedef struct {
	uint64_t name[16];
	uint32_t type;
	uint32_t display_rule;
	uint32_t collation_rule;
	uint32_t flags;
	uint64_t min_size;
	uint64_t max_size;
} attrDef;
#pragma pack(pop)

// Flags
#define ATTRDEF_INDEX 0x00000002
#define ATTRDEF_RESIDENT 0x00000004
#define ATTRDEF_NONRESIDENT 0x00000008

// $ObjId

#pragma pack(push, 1)
typedef struct {
	uint16_t offset;
	uint16_t size;
	uint32_t unused;
	uint16_t idx_entry_size;
	uint16_t objid_size;
	uint32_t flags;
	uint64_t object_id[2];
	uint64_t file_reference;
	uint64_t birth_volume_id[2];
	uint64_t birth_object_id[2];
	uint64_t birth_domain_id[2];
} objId_file;
#pragma pack(pop)

// Flags are IDX_SUBNODE and IDX_LAST  

// $Quota

// $O index

typedef struct {
	uint16_t owner_id_offset;
	uint16_t owner_id_length;
	uint32_t unused;
	uint16_t idx_entry_size;
	uint16_t SID_size;
	uint32_t flags;
} o_quota;

// Flags are IDX_SUBNODE and IDX_LAST

// $Q index

#pragma pack(push, 1)
typedef struct {
	uint16_t info_offset;
	uint16_t info_length;
	uint32_t unused;
	uint16_t idx_entry_size;
	uint16_t owner_id_size;
	uint32_t flags;
	uint32_t owner_id;
	uint32_t version;
	uint32_t quota_flags;
	uint64_t bytes_charged;
	uint64_t time_of_last_charge;
	uint64_t threshold_value;
	uint64_t hard_limit_value;
	uint64_t exceeded_time;
	uint32_t SID[3];
} q_quota;
#pragma pack(pop)

#define DEFAULT_LIMITS 0x00000001
#define LIMIT_REACHED 0x00000002
#define ID_DELETED 0x00000004
#define TRACKING_DATA_USAGE 0x00000010
#define ENFORCING_DATA_USAGE 0x00000020
#define USAGE_TRACKING_REQUESTED 0x00000040
#define CREATE_LOG_WHEN_THRESHOLD_IS_MET 0x00000080
#define CREATE_LOG_WHEN_LIMIT_IS_MET 0x00000100
#define OUT_OF_DATE 0x00000200
#define CORRUPT 0x00000400
#define PENDING_DELETES 0x00000800

#endif