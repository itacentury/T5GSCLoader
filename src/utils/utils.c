#include "utils.h"
#include "globals.h"

#include <string.h>

#include <sys/tty.h>
#include <sys/process.h>
#include <cell/fs/cell_fs_file_api.h>
#include <sysutil/sysutil_sysparam.h>

int sys_dbg_process_write(uint64_t address, const void *data, size_t size) {
    system_call_4(905, sys_process_getpid(), address, size, (uintptr_t)data);
    return_to_user_prog(int);
}

size_t get_file_size(char *filePath) {
    int size = 0;
    CellFsStat fstat;
    CellFsErrno err = cellFsStat(filePath, &fstat);
    if (err != CELL_FS_SUCCEEDED) {
        return err;
    }
    return fstat.st_size;
}

void set_empty_deflated_data(char *buffer) {
    int op[7];
    op[0] = 0x789C05B0;
    op[1] = 0xB1110000;
    op[2] = 0x04C48CC3;
    op[3] = 0x6869543A;
    op[4] = 0xF7BBE758;
    op[5] = 0xAEE75302;
    op[6] = 0x109802FE;
    sys_dbg_process_write((uintptr_t)buffer, &op, 7 * 4);
}

void hex_str_to_padded_hex_str(char *out, char *hexStr) {
    char *outPtr;
    char *tmp = hexStr;

    if (!tmp || !out)
        return;

    if (tmp[1] == 'x')
        tmp += 2;

    int hexLen = strlen(tmp);

    if ((hexLen % 2) != 0) { // must be even
        hexLen++;
        out[0] = '0';
        outPtr = &out[1];
    }
    else
        outPtr = out;

    strcpy(outPtr, tmp);
}

void hex_str_to_buffer(char *out, char *hexStr, size_t hexLen) {
    if (!out || !hexStr || hexLen < 1)
        return;

    size_t index = 0;
    while (index < (hexLen * 2)) {
        char c = hexStr[index];
        int value = 0;
        if (c >= '0' && c <= '9')
            value = (c - '0');
        else if (c >= 'A' && c <= 'F')
            value = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')
            value = (10 + (c - 'a'));
        else
            return;

        out[(index / 2)] += (value << (((index + 1) % 2) * 4));
        index++;
    }
}

int hex_str_to_int32(char *hexStr, size_t hexLen) {
    char out[4];
    memset(out, 0, 4);
    int start = (4 - hexLen);
    if (start < 0)
        start = 0;

    hex_str_to_buffer((char*)(out + start), hexStr, hexLen);

    return *(int*)(&out);
}

// allows iPrintln_GameMessage and iPrintlnBold_GameMessage by noping Com_GetParseThreadInfo in 0x001AF228 CL_ConsolePrint
void RemoveThreadIDCheckOnCL_ConsolePrint() {
    uint32_t PPC[] = { 0x60000000 };
	for (int i = 0; i < 0x4; i++)
        sys_dbg_process_write(0x1AF264 + (i * 4), &PPC[0], 4);
}

void RSATest() {
    uint32_t PPC[] = {0x60000000};
	sys_dbg_process_write(0x230258, &PPC[0], 4);
    sys_dbg_process_write(0x23025C, &PPC[0], 4);
    sys_dbg_process_write(0x230260, &PPC[0], 4);
	sys_dbg_process_write(0x230264, &PPC[0], 4);
	sys_dbg_process_write(0x230268, &PPC[0], 4);
}

// can use some cheat protected dvars
void RemoveCheatProtection() {
	uint32_t PPC[] = { 0x60000000, 0x3B200000 };
	sys_dbg_process_write(0x4C8EC4, &PPC[0], 4);
	sys_dbg_process_write(0x4C8ED0, &PPC[0], 4);
	sys_dbg_process_write(0x4C8ED4, &PPC[1], 4);
	
	//TEST
	sys_dbg_process_write(0x3E013C, &PPC[0], 4);
}

void checkScreenResolution() {
    float width = 1280.0f, height = 720.0f;

    CellVideoOutConfiguration config;
    cellVideoOutGetConfiguration(CELL_VIDEO_OUT_PRIMARY, &config, NULL);

    CellVideoOutResolution resInfo = {0};
    int res = cellVideoOutGetResolution(config.resolutionId, &resInfo);
    if (res == CELL_OK) {
        width  = (float)resInfo.width;
        height = (float)resInfo.height;
        printf(T5INFO "Detected screen resolution '%i×%i'", resInfo.width, resInfo.height);
    } else {
        width  = 1280.0f;
        height = 720.0f;
        printf(T5WARNING "Unknown screen resolution (%d), using 1280×720", config.resolutionId);
    }

    // hudScale = height >= 1080 ? 1.2f : 1.0f;
    hudScale = height >= 1080 ? 1.0f : 1.0f;

    screenResolution.width  = width;
    screenResolution.height = height;
    screenCenter.width      = width  * 0.5f;
    screenCenter.height     = height * 0.5f;
}

char byteArray[100];
char returnRead[100];

char* ReadBytes(int address, int length) {
	for (int i = 0; i < length; i++) {
		byteArray[i] = *(char*)(address + (i));
	}

	return byteArray;
}

void WriteBytes(int address, char* input, int length) {
	for (int i = 0; i < length; i++) {
		*(char*)(address + (i)) = input[i];
	}
}

char* ReadString(int address) {
	int strlength = 100;
	char* StrBytes = ReadBytes(address, strlength);
	for (int i = 0; i < strlength; i++) {
		if (StrBytes[i] != 0x00)
			returnRead[i] = StrBytes[i];
		else
			break;
	}

	return returnRead;
}

void WriteFloat(int Address, float Input) {
    *(float*)Address = Input;
}

int simpleAtoi(const char *str) {
    int result = 0;
    while (*str != '\0') {
        result = result * 10 + (*str - '0');
        str++;
    }
    
    return result;
}
