#include "t5.h"
#include "utils.h"
#include "offsets.h"
#include "cshook.h"

#include <string.h>

#include <sys/timer.h>
#include <sys/memory.h>
#include <cell/error.h>
#include <cell/fs/cell_fs_file_api.h>

void init_offsets() {
    // Set native offsets
    t5ni(DB_FindXAssetHeader);
    t5ni(DB_LinkXAssetEntry);
    t5ni(Dvar_FindVar);
    t5ni(Scr_GetFunctionHandle);
    t5ni(Scr_ExecThread);
    t5ni(Scr_FreeThread);
    t5ni(Scr_GetNumParam);
    t5ni(Scr_GetString);

    // Set struct offsets
    scrVarPub = (scrVarPub_t*)(t5o(ScrVarPub));
    scrCompilePub = (scrCompilePub_t*)(t5o(ScrCompilePub));
}

int init_hooks() {
    // Set hooks offsets
    t5nhi(cellSpursLFQueuePushBody);
    t5nhi(Scr_GetChecksum);
    t5nhi(Scr_LoadGameType);
    t5nhi(Scr_LoadScript);
    t5nhi(Scr_GetFunction);
    t5nhi(Menu_PaintAll);
    t5nhi(ClientCommand);

    // Create and enable hooks
    int res;
    if ((res = cs_hook_install(cellSpursLFQueuePushBody, CS_HOOK_TYPE_IMPORT)) < 0)
        return res;
    if ((res = cs_hook_install(Scr_GetChecksum, CS_HOOK_TYPE_CTR)) < 0)
        return res;
    if ((res = cs_hook_install(Scr_LoadGameType, CS_HOOK_TYPE_CTR)) < 0)
        return res;
    if ((res = cs_hook_install(Scr_LoadScript, CS_HOOK_TYPE_CTR)) < 0)
        return res;
    if ((res = cs_hook_install(Scr_GetFunction, CS_HOOK_TYPE_CTR)) < 0)
        return res;
    if ((res = cs_hook_install(Menu_PaintAll, CS_HOOK_TYPE_CTR)) < 0)
        return res;
    if ((res = cs_hook_install(ClientCommand, CS_HOOK_TYPE_CTR)) < 0)
        return res;

    return 0;
}

GSCLoaderRawfile *get_loader_rawfile_from_deflated_buffer(char *deflatedBuffer) {
    for (int i = 0; i < MAX_GSC_COUNT; i++) {
        if ((uintptr_t)deflatedBuffer == (uintptr_t)(loader.rawFiles[i].data.buffer + 2))
            return &loader.rawFiles[i];
    }
    
    return NULL;
}

void get_or_create_mod_path(char *path) {
    int fd;
    CellFsErrno err;
    char pathMods[CELL_FS_MAX_FS_PATH_LENGTH];
    sprintf(pathMods, SCRIPTS_PATH "/%s", isMultiplayer ? "mp" : "zm");

    // Check scripts folder existing and create it if necessary
    err = cellFsMkdir(SCRIPTS_PATH, CELL_FS_DEFAULT_CREATE_MODE_1);
    if (err != CELL_FS_SUCCEEDED && err != CELL_FS_EEXIST) {
        printf(T5ERROR "Cannot create the path '%s' (0x%08X).", SCRIPTS_PATH, err);
        return;
    }

    err = cellFsOpendir(pathMods, &fd);
    if (err != CELL_FS_SUCCEEDED) {
        cellFsClosedir(fd);
        printf(T5ERROR "Cannot open 'scripts' directory (0x%08X).", err);
        return;
    }

    uint64_t read;
    CellFsDirent ent;
    read = sizeof(CellFsDirent);
    while (!cellFsReaddir(fd, &ent, &read)) {
        if (!read)
            break;

        if (strstr(ent.d_name, ".mod")) {
            strcpy(loader.currentModName, ent.d_name);
            sprintf(path, SCRIPTS_PATH "/%s/%s", isMultiplayer ? "mp" : "zm", ent.d_name);
            path[strlen(path) - 4] = 0;
        }
    }

    cellFsClosedir(fd);
}

static void create_assets_from_scripts_recursive(const char *path, const char *relative, int *assetIndex, bool *mainLinked) {
    int fd;
    CellFsErrno err = cellFsOpendir(path, &fd);
    if (err != CELL_FS_SUCCEEDED) {
        printf(T5ERROR "Cannot open '%s' directory (0x%08X).\n", path, err);
        return;
    }

    uint64_t read = sizeof(CellFsDirent);
    CellFsDirent ent;
    char newRelative[CELL_FS_MAX_FS_PATH_LENGTH];

    while (!cellFsReaddir(fd, &ent, &read) && *assetIndex < MAX_GSC_COUNT) {
        if (!read)
            break;

        char fullPath[CELL_FS_MAX_FS_PATH_LENGTH];
        // Create the full path for the file or subdirectory
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, ent.d_name);

        int testFd;
        // Check if fullPath is a directory
        if (cellFsOpendir(fullPath, &testFd) == CELL_FS_SUCCEEDED) {
            cellFsClosedir(testFd);
            // Skip special directories
            if (strcmp(ent.d_name, ".") != 0 && strcmp(ent.d_name, "..") != 0) {
                // If a relative path already exists, append the new subdirectory
                if (strlen(relative) > 0) {
                    snprintf(newRelative, sizeof(newRelative), "%s/%s", relative, ent.d_name);
                } else {
                    snprintf(newRelative, sizeof(newRelative), "%s", ent.d_name);
                }
                // Recursive call for the subdirectory
                create_assets_from_scripts_recursive(fullPath, newRelative, assetIndex, mainLinked);
            }
        } else if (strstr(ent.d_name, ".gsc") != NULL) {
            printf(T5INFO "Creating a new asset entry for '%s'.\n", fullPath);
            int idx = *assetIndex;
            // Set the pointers for name and buffer in the asset structure
            loader.rawFiles[idx].asset.name = (char*)&loader.rawFiles[idx].data.name;
            loader.rawFiles[idx].asset.buffer = (char*)&loader.rawFiles[idx].data.inflatedSize;
            loader.rawFiles[idx].asset.len = 0xDEAD;

            set_empty_deflated_data(loader.rawFiles[idx].data.buffer);

            // Directly adopt the relative path
            if (strlen(relative) > 0) {
                snprintf(loader.rawFiles[idx].data.name, sizeof(loader.rawFiles[idx].data.name),
                         "%s/%s", relative, ent.d_name);
            } else {
                // If no subdirectory exists: set a default path, e.g., "maps/mp/" or "maps/zm/" depending on the mode
                snprintf(loader.rawFiles[idx].data.name, sizeof(loader.rawFiles[idx].data.name),
                         "maps/%s/%s", isMultiplayer ? "mp" : "zm", ent.d_name);
            }

            int fileSize = get_file_size(fullPath);
            loader.rawFiles[idx].data.inflatedSize = fileSize + 1;
            loader.rawFiles[idx].data.size = 0x1B;
            loader.rawFiles[idx].entry.asset.type = ASSET_TYPE_RAWFILE;
            loader.rawFiles[idx].entry.asset.header.rawFile = &loader.rawFiles[idx].asset;

            XAssetEntryPoolEntry *entry = 0;
            XAssetHeader header;
            DB_FindXAssetHeader(&header, ASSET_TYPE_RAWFILE, loader.rawFiles[idx].asset.name, true, -1);
            if (header.rawFile != &loader.rawFiles[idx].asset) {
                entry = DB_LinkXAssetEntry(&loader.rawFiles[idx].entry, 0);
                if (!entry) {
                    printf(T5ERROR "Linking asset '%s' failed.\n", loader.rawFiles[idx].asset.name);
                    continue;
                }
                if (strcmp(ent.d_name, "main.gsc") == 0)
                    *mainLinked = true;

                (*assetIndex)++;
            }
        }
        // Reset read size for the next iteration of the loop
        read = sizeof(CellFsDirent);
    }
    cellFsClosedir(fd);
}

bool create_assets_from_scripts(char *path) {
    int assetIndex = 0;
    bool mainLinked = false;

    create_assets_from_scripts_recursive(path, "", &assetIndex, &mainLinked);

    return mainLinked && (assetIndex > 0);
}

int init_game() {
    // Current process is Black Ops MP 1.13 MP or ZM?
    if (*(int*)(0x1002C) == 0xB5A4A0)
        isMultiplayer = true;
    else if (*(int*)(0x1002C) == 0xA56728)
        isMultiplayer = false;
    else {
        printf(T5ERROR "The process is not Black Ops 1.13 MP/ZM.");
        return -1;
    }

    // Init offsets / hooks according MP/ZM
    init_offsets();
    int err;
    if ((err = init_hooks()) < 0) {
        printf(T5ERROR "Hooks install failed (0x%08X).", err);
        return -2;
    }

    // Show a warning on boot in case no mod is set
    char modPath[CELL_FS_MAX_FS_PATH_LENGTH];
    get_or_create_mod_path(modPath);
    if (!*modPath) {
        printf(T5WARNING "Mod file not found, create a .mod file in '%s/%s' with a name that is equal to a mod folder to load it (no game restart required using ftp).", SCRIPTS_PATH, isMultiplayer ? "mp" : "zm");
    }

    return 0;
}
