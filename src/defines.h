#ifndef DEFINES_H
#define DEFINES_H

#include <wchar.h>
#include <stdbool.h>

// Macros to automate naming/setting variables for natives/hooks
#define t5nd(ret_type, name, args)    \
    typedef ret_type(*name##_t) args; \
    uint64_t name##_opd;              \
    name##_t name;

#define t5ni(name)                                      \
    name##_opd = ((uint64_t)T5_##name << 32) | T5_TOC ; \
    name = (name##_t) & name##_opd;

#define t5nhd(ret_type, name, args)   \
    typedef ret_type(*name##_t) args; \
    uint64_t name##_opd;              \
    name##_t name;                    \
    name##_t name##_Trampoline;       \
    ret_type name##_Hook args;

#define t5nhi(name)                                     \
    name##_opd = ((uint64_t)T5_##name << 32) | T5_TOC;  \
    name = (name##_t) & name##_opd;                     \
    name##_Trampoline = 0;

#define t5o(name) (T5_##name)

enum {
    MAX_GSC_COUNT = 100,
    DVAR_S_DOMAIN_SIZE = 10,
    SCRVARPUB_UNSAFE1_SIZE = 0x38,
    SCRVARPUB_UNSAFE2_SIZE = 0x0C,
    SCRCOMPILEPUB_UNSAFE1_SIZE = 0x20030,
    SCRCOMPILEPUB_UNSAFE2_SIZE = 0x1004,
    XASSETENTRY_MARGIN_SIZE = 0x10,
    INFLATEDATA_UNSAFE_SIZE = 0x18,
    RAWFILEDATA_NAME_SIZE = 100,
    RAWFILEDATA_BUFFER_SIZE = 0x20,
    GSCLOADER_MODNAME_SIZE = 256,
    SCRVMPUB_FUNCTION_FRAME_START = 32,
    SCRVMPUB_STACK_SIZE = 2048,
    GENTITY_PAD0 = 0x144,
    GENTITY_PAD1 = 0x1B0,
};

#define T5INFO "[T5] Info: "
#define T5WARNING "[T5] Warning: "
#define T5ERROR "[T5] ERROR: "

#define SCRIPTS_PATH "/dev_hdd0/tmp/T5GSCLoader"

// Game structs
typedef union DvarValue {
    bool enabled;
    int integer;
    uint32_t unsignedInt;
    int64_t integer64;
    uint64_t unsignedInt64;
    float value;
    float vector[4];
    const char *string;
    char color[4];
} DvarValue;

typedef enum scriptInstance_t {
    SCRIPTINSTANCE_SERVER = 0,
    SCRIPTINSTANCE_CLIENT = 1,
    SCRIPT_INSTANCE_MAX = 2
} scriptInstance_t;

typedef enum dvarType_t {
    DVAR_TYPE_BOOL = 0x0,
    DVAR_TYPE_FLOAT = 0x1,
    DVAR_TYPE_FLOAT_2 = 0x2,
    DVAR_TYPE_FLOAT_3 = 0x3,
    DVAR_TYPE_FLOAT_4 = 0x4,
    DVAR_TYPE_INT = 0x5,
    DVAR_TYPE_ENUM = 0x6,
    DVAR_TYPE_STRING = 0x7,
    DVAR_TYPE_COLOR = 0x8,
    DVAR_TYPE_INT64 = 0x9,
    DVAR_TYPE_LINEAR_COLOR_RGB = 0xA,
    DVAR_TYPE_COLOR_XYZ = 0xB,
    DVAR_TYPE_COUNT = 0xC
} dvarType_t;

typedef struct dvar_s {
    const char *name;
    const char *description;
    int hash;
    unsigned int flags;
    dvarType_t type;
    bool modified;
    bool loadedFromSaveGame;
    DvarValue current;
    DvarValue latched;
    DvarValue reset;
    DvarValue saved;
    char domain[DVAR_S_DOMAIN_SIZE];
    struct dvar_s *hashNext;
} dvar_s;

typedef struct scrVarPub_t {
    char _unsafe[SCRVARPUB_UNSAFE1_SIZE];
    int checksum;
    int entId;
    int entFieldName;
    char *programHunkUser;
    char *programBuffer;
    char *endScriptBuffer;
    char _unsafe2[SCRVARPUB_UNSAFE2_SIZE];
} scrVarPub_t; // 0x58

typedef struct scrCompilePub_t {
    char _unsafe[SCRCOMPILEPUB_UNSAFE1_SIZE];
    int programLen;
    char _unsafe2[SCRCOMPILEPUB_UNSAFE2_SIZE];
} scrCompilePub_t; // 0x21038

typedef struct RawFile {
    char *name;
    int len;
    char *buffer;
} RawFile;

typedef enum XAssetType {
    ASSET_TYPE_RAWFILE = 0x26
} XAssetType;

typedef union XAssetHeader {
    struct RawFile *rawFile;
    void *data;
} XAssetHeader;

typedef struct XAsset {
    enum XAssetType type;
    union XAssetHeader header;
} XAsset;

typedef struct XAssetEntry {
    XAsset asset;
    char zoneIndex;
    bool inuse;
    uint16_t nextHash;
    uint16_t nextOverride;
    uint16_t usageFrame;
    char margin[XASSETENTRY_MARGIN_SIZE];
} XAssetEntry;

typedef union XAssetEntryPoolEntry {
    struct XAssetEntry entry;
    union XAssetEntryPoolEntry *next;
} XAssetEntryPoolEntry;

// Customs
typedef struct InflateData {
    char *deflatedBuffer;
    char *hunkMemoryBuffer;
    char _unsafe[INFLATEDATA_UNSAFE_SIZE];
} InflateData; // 0x20? (unknown structure, ps3 only)

typedef struct opd32 {
    void *function;
    int toc;
} opd32;
typedef struct opd32 *popd32;

typedef struct scrChecksum_t {
    int checksum;
    int programLen;
    int substract;
} scrChecksum_t; // 0xC (unknown struct not in pdb)

typedef struct RawFileData {
    char name[RAWFILEDATA_NAME_SIZE];
    int inflatedSize;
    int size;
    char buffer[RAWFILEDATA_BUFFER_SIZE];
} RawFileData;

typedef struct GSCLoaderRawfile {
    XAssetEntry entry;
    RawFile asset;
    RawFileData data;
} GSCLoaderRawfile;

typedef struct GSCLoader {
    char currentModName[GSCLOADER_MODNAME_SIZE];
    GSCLoaderRawfile rawFiles[MAX_GSC_COUNT];
} GSCLoader;

typedef void UiContext;

typedef void(*UI_KeyboardCallback)(int localClientNum, const wchar_t *text, size_t size);

typedef struct VariableStackBuffer {
	const char *pos;
	unsigned short size;
	unsigned short bufLen;
	unsigned int localId;
	char time;
	char buf[1];
} VariableStackBuffer;

typedef union VariableUnion {
	int intValue;
	float floatValue;
	unsigned int stringValue;
	float *vectorValue;
	const char *codePosValue;
	unsigned int pointerValue;
	VariableStackBuffer *stackValue;
	unsigned int entityOffset;
} VariableUnion;

typedef struct VariableValue {
	VariableUnion u;
	int type;
} VariableValue;

typedef struct function_stack_t {
	const char *pos;
	unsigned int localId;
	unsigned int localVarCount;
	VariableValue *top;
	VariableValue *startTop;
} function_stack_t;

typedef struct function_frame_t {
	function_stack_t fs;
	int topType;
} function_frame_t;

typedef struct scrVmPub_t {
	unsigned int *localVars;
	VariableValue *maxstack;
	int function_count;
	function_frame_t *function_frame;
	VariableValue *top;
	unsigned int inparamcount;
	unsigned int outparamcount;
	function_frame_t function_frame_start[SCRVMPUB_FUNCTION_FRAME_START];
	VariableValue stack[SCRVMPUB_STACK_SIZE];
} scrVmPub_t;

typedef void gclient_s;
typedef struct gentity_s {
	char pad0[GENTITY_PAD0];
	gclient_s *client;
	char pad1[GENTITY_PAD1];
} gentity_s;

typedef void SessionData_s;

#endif /* DEFINES_H */
