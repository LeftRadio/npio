/**
  *****************************************************************************
  * @file      boardtype.c
  * @author    leftradio
  * @version   v1.0.0
  * @date      2019
  * @brief     sourse for boardtype
  *****************************************************************************
**/

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "boardtype.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
const char* s5p_board_cputempfile = "/sys/class/hwmon/hwmon0/device/temp_label";
const char* s5p_board_max_cputempfile = "/sys/class/hwmon/hwmon0/device/temp_max";
const char* allwinner_tempfile = "/sys/class/thermal/thermal_zone0/temp";

/* Extern variables ---------------------------------------------------------*/
const BoardHardwareInfo gAllBoardHardwareInfo[] = {
    /* */
    {"MINI6410", -1, S3C6410_COMMON, "S3C6410_Board", ""},
    {"MINI210",  -1, S5PV210_COMMON, "S5PV210_Board", ""},
    {"TINY4412", -1, S5P4412_COMMON, "S5P4412_Board", ""},
    {"mini2451",  0, S3C2451_COMMON, "S3C2451_Board", ""},

    /* s5p4418 */
    {"nanopi2",     0, NanoPi2,      "NanoPi2",      ""},
    {"nanopi2",     1, NanoPC_T2,    "NanoPC-T2",    ""},
    {"nanopi2",     2, NanoPi_S2,    "NanoPi-S2",    ""},
    {"nanopi2",     3, Smart4418,    "Smart4418",    ""},
    {"nanopi2",     4, NanoPi2_Fire, "NanoPi2-Fire", ""},
    {"nanopi2",     5, NanoPi_M2,    "NanoPi-M2",    ""},
    {"nanopi2",     7, NanoPi_M2A,   "NanoPi-M2A",   ""},
    {"nanopi2", 0x103, Smart4418SDK, "Smart4418SDK", ""},

    /* s5p6818 */
    {"nanopi3", 1, NanoPC_T3,    "NanoPC-T3",    ""},
    {"nanopi3", 2, NanoPi_M3B,   "NanoPi-M3B",   ""},
    {"nanopi3", 3, Smart6818,    "Smart6818",    ""},
    {"nanopi3", 4, NanoPC_T3T,   "NanoPC-T3T",   ""},
    {"nanopi3", 5, NanoPi_Fire3, "NanoPi-Fire3", ""},
    {"nanopi3", 7, NanoPi_M3,    "NanoPi-M3",    ""},

    /* allwinner H3 */
    // kernel 3.x
    {"sun8i", 0, NanoPi_M1,       "NanoPi-M1",       "0(0)"},
    {"sun8i", 0, NanoPi_NEO,      "NanoPi-NEO",      "1(0)"},
    {"sun8i", 0, NanoPi_NEO_Air,  "NanoPi-NEO-Air",  "2(0)"},
    {"sun8i", 0, NanoPi_M1_Plus,  "NanoPi-M1-Plus",  "3(0)"},
    {"sun8i", 0, NanoPi_Duo,      "NanoPi-Duo",      "4(0)"},
    {"sun8i", 0, NanoPi_NEO_Core, "NanoPi-NEO-Core", "5(0)"},
    {"sun8i", 0, NanoPi_K1,       "NanoPi-K1",       "6(0)"},
    {"sun8i", 0, NanoPi_Hero,     "NanoPi-Hero",     "7(0)"},
    {"sun8i", 0, NanoPi_Duo2,     "NanoPi-Duo2",     "8(0)"},
    {"sun8i", 0, NanoPi_R1,       "NanoPi-R1",       "9(0)"},
    // kernel 4.x
    {"Allwinnersun8iFamily", 0, NanoPi_M1,       "NanoPi-M1",       "0(0)"},
    {"Allwinnersun8iFamily", 0, NanoPi_NEO,      "NanoPi-NEO",      "1(0)"},
    {"Allwinnersun8iFamily", 0, NanoPi_NEO_Air,  "NanoPi-NEO-Air",  "2(0)"},
    {"Allwinnersun8iFamily", 0, NanoPi_M1_Plus,  "NanoPi-M1-Plus",  "3(0)"},
    {"Allwinnersun8iFamily", 0, NanoPi_Duo,      "NanoPi-Duo",      "4(0)"},
    {"Allwinnersun8iFamily", 0, NanoPi_NEO_Core, "NanoPi-NEO-Core", "5(0)"},
    {"Allwinnersun8iFamily", 0, NanoPi_K1,       "NanoPi-K1",       "6(0)"},
    {"Allwinnersun8iFamily", 0, NanoPi_Hero,     "NanoPi-Hero",     "7(0)"},
    {"Allwinnersun8iFamily", 0, NanoPi_Duo2,     "NanoPi-Duo2",     "8(0)"},
    {"Allwinnersun8iFamily", 0, NanoPi_R1,       "NanoPi-R1",       "9(0)"},

    /* armbian+Neo2 */
    {"sun50iw1p1", 4, NanoPi_NEO2, "NanoPi-NEO2", "1(0)"},

    /* allwinner H5 */
    // kernel 3.x
    {"sun50iw2", 4, NanoPi_NEO2,      "NanoPi-NEO2",      "1(0)"},
    {"sun50iw2", 4, NanoPi_M1_Plus2,  "NanoPi-M1-Plus2",  "3(0)"},
    {"sun50iw2", 4, NanoPi_NEO_Plus2, "NanoPi-NEO-Plus2", "2(0)"},
    {"sun50iw2", 4, NanoPi_NEO_Core2, "NanoPi-NEO-Core2", "0(0)"},
    {"sun50iw2", 4, NanoPi_K1_Plus,   "NanoPi-K1-Plus",   "4(0)"},
    // kernel 4.x
    {"Allwinnersun50iw2Family", 4, NanoPi_NEO2,      "NanoPi-NEO2",      "1(0)"},
    {"Allwinnersun50iw2Family", 4, NanoPi_M1_Plus2,  "NanoPi-M1-Plus2",  "3(0)"},
    {"Allwinnersun50iw2Family", 4, NanoPi_NEO_Plus2, "NanoPi-NEO-Plus2", "2(0)"},
    {"Allwinnersun50iw2Family", 4, NanoPi_NEO_Core2, "NanoPi-NEO-Core2", "0(0)"},
    {"Allwinnersun50iw2Family", 4, NanoPi_K1_Plus,   "NanoPi-K1-Plus",   "4(0)"},

    /* k2 */
    {"Amlogic", 0, NanoPi_K2, "NanoPi-K2", ""},

    /* t4 */
    {"nanopi4", 0, NanoPC_T4,   "NanoPC-T4",   ""},
    {"nanopi4", 1, NanoPi_M4,   "NanoPi-M4",   ""},
    {"nanopi4", 2, NanoPC_T4,   "NanoPC-T4",   ""},
    {"nanopi4", 4, NanoPi_NEO4, "NanoPi-NEO4", ""},
};

/* Extern function ----------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

/**
  * @brief  getFieldValueInCpuInfo
  * @param  char* hardware, int hardwareMaxLen, char* revision, int revisionMaxLen
  * @retval int value
  */
static int getFieldValueInCpuInfo(char* hardware, int hardwareMaxLen, char* revision, int revisionMaxLen) {
    /* */
    int j, i, n;
    char lineUntrim[1024], line[1024], *p, *p2;
    FILE *f;
    int isGotHardware = 0;
    int isGotRevision = 0;

    /* verify cpuinfo for platform */
    if (!(f = fopen("/proc/cpuinfo", "r"))) {
        printf("open /proc/cpuinfo failed.");
        return -1;
    }

    while (!feof(f)) {
        if (!fgets(lineUntrim, sizeof(lineUntrim), f)) {
            break;
        }
        else {
            j = 0;
            for (i = 0; i < strlen(lineUntrim); i++) {
                if (lineUntrim[i] == ' ' || lineUntrim[i] == '\t' || lineUntrim[i] == '\r' || lineUntrim[i] == '\n') {
                }
                else {
                    line[j++] = lineUntrim[i];
                }
            }
            line[j] = 0x00;
            n = strlen(line);
            if (n > 0) {
                //printf("LINE: %s\n", line);

                if (isGotHardware == 0) {
                    if (p = strtok(line, ":")) {
                        if (strncasecmp(p, "Hardware", strlen("Hardware")) == 0) {
                            if (p = strtok(0, ":")) {
                                memset(hardware, 0, hardwareMaxLen);
                                strncpy(hardware, p, hardwareMaxLen - 1);
                                isGotHardware = 1;
                            }
                            continue;
                        }
                    }
                }

                if (isGotRevision == 0) {
                    if (p2 = strtok(line, ":")) {
                        if (strncasecmp(p2, "Revision", strlen("Revision")) == 0) {
                            if (p2 = strtok(0, ":")) {
                                memset(revision, 0, revisionMaxLen);
                                strncpy(revision, p2, revisionMaxLen - 1);
                                isGotRevision = 1;
                            }
                            continue;
                        }
                    }
                }

                if (isGotHardware == 1 && isGotRevision == 1) {
                    break;
                }
            }
        }
    }
    /* */
    fclose(f);
    return (isGotHardware + isGotRevision);
}

/**
  * @brief  getAllwinnerBoardID
  * @param  char* boardId, int boardIdMaxLen
  * @retval int value
  */
static int getAllwinnerBoardID(char* boardId, int boardIdMaxLen) {
    /* */
    int j, i, n;
    char lineUntrim[1024], line[1024], *p, *p2;
    const char* sunxi_board_id_fieldname = "sunxi_board_id";
    FILE *f;
    int ret = -1;

    if (!(f = fopen("/etc/sys_info", "r"))) {
        return -1;
    }

    while (!feof(f)) {
        if (!fgets(lineUntrim, sizeof(lineUntrim), f)) {
            break;
        }
        else {
            j = 0;
            for (i = 0; i < strlen(lineUntrim); i++) {
                if (lineUntrim[i] == ' ' || lineUntrim[i] == '\t' || lineUntrim[i] == '\r' || lineUntrim[i] == '\n') {
                }
                else {
                    line[j++] = lineUntrim[i];
                }
            }
            line[j] = 0x00;
            n = strlen(line);
            if (n > 0) {
                // printf("LINE: %s\n", line);
                if (p = strtok(line, ":")) {
                    if (strncasecmp(p, sunxi_board_id_fieldname, strlen(sunxi_board_id_fieldname)) == 0) {
                        // printf("\t\tkey=\"%s\"\n", p);
                        if (p = strtok(0, ":")) {
                            // printf("\t\tv=\"%s\"\n", p);
                            memset(boardId, 0, boardIdMaxLen);
                            strncpy(boardId, p, boardIdMaxLen - 1);
                            ret = 0;
                            break;
                        }
                    }
                }
            }
        }
    }

    fclose(f);
    return ret;
}

/**
  * @brief  trimwhitespaces
  *         Note: This function returns a pointer to a substring of the original string.
  *         If the given string was allocated dynamically, the caller must not overwrite
  *         that pointer with the returned value, since the original pointer must be
  *         deallocated using the same allocator with which it was allocated.  The return
  *         value must NOT be deallocated using free() etc.
  * @param  char *str
  * @retval char pointer
  */
static char* trimwhitespaces(char *str) {
    char *end;
    /* trim leading space */
    while ( isspace((unsigned char)*str) ) {
        str++;
    }
    /* veryfy, if all */
    if (*str == 0) {
        return str;
    }
    /* trim trailing space */
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    /* write new null terminator character */
    end[1] = '\0';
    /* */
    return str;
}

/**
  * @brief  getBoardDisplayName
  * @param  char* boardName, int boardNameMaxLen
  * @retval int value
  */
static int getBoardDisplayName(char* boardName, int boardNameMaxLen) {
    /* */
    int j, i, n;
    char lineUntrim[1024], line[1024], *lineNoSpaces, *p, *p2;
    const char* release_board_name_fieldname = "BOARD_NAME";
    FILE *f;
    int ret = -1;

    if (!(f = fopen("/etc/friendlyelec-release", "r"))) {
        if (!(f = fopen("/etc/armbian-release", "r"))) {
            return -1;
        }
    }

    while (!feof(f)) {
        if (!fgets(lineUntrim, sizeof(lineUntrim), f)) {
            break;
        }
        else {
            j = 0;

            // Trim leading and traling spaces
            lineNoSpaces = trimwhitespaces(lineUntrim);

            // Substitute inner spaces with dashes "-" and remove not alphanumeric chars
            for (i = 0; i < strlen(lineNoSpaces); i++) {
                if (lineNoSpaces[i] == '"' || lineNoSpaces[i] == '\t' || lineNoSpaces[i] == '\r' || lineNoSpaces[i] == '\n') {
                }
                else {
                    line[j++] = (lineNoSpaces[i] == ' ') ? '-' : lineNoSpaces[i];
                }
            }
            line[j] = 0x00;
            n = strlen(line);
            if (n > 0) {
                //printf("LINE: %s\n", line);
                if (p = strtok(line, "=")) {
                    if (strncasecmp(p, release_board_name_fieldname, strlen(release_board_name_fieldname)) == 0) {
                        //printf("\t\tkey=\"%s\"\n", p);
                        if (p = strtok(0, "=")) {
                            //printf("\t\tv=\"%s\"\n", p);
                            memset(boardName, 0, boardNameMaxLen);
                            strncpy(boardName, p, boardNameMaxLen - 1);
                            ret = 0;
                            break;
                        }
                    }
                }
            }
        }
    }
    /* */
    fclose(f);
    return ret;
}


/* Exported functions -------------------------------------------------------*/

/**
  * @brief  getBoardDisplayName
  * @param  char* boardName, int boardNameMaxLen
  * @retval int value
  */
int getBoardType(BoardHardwareInfo** retBoardInfo) {
    /* */
    char hardware[255];
    char revision[255];
    char boardDisplayName[255];
    char allwinnerBoardID[255];
    int i;
    memset(hardware, 0, sizeof(hardware));
    memset(revision, 0, sizeof(revision));
    int ret = getFieldValueInCpuInfo(hardware, sizeof(hardware), revision, sizeof(revision));
    if (!(ret > 0)) {
        #if NP_DEBUG
            printf("%s, ret:%d\n", "getFieldValueInCpuInfo failed", ret);
        #endif
        return -1;
    }

    const char* a64 = "sun50iw1p1";
    const char* amlogic = "Amlogic";
    const char* h3 = "sun8i";
    const char* h5 = "sun50iw2";
    const char* h3_kernel4 = "Allwinnersun8iFamily";
    const char* h5_kernel4 = "Allwinnersun50iw2Family";

    #if NP_DEBUG
        printf("hardware:%s,revision:%s\n", hardware, revision);
    #endif

    //a64 and amlogic, only check hardware
    if (strncasecmp(hardware, a64, strlen(a64)) == 0 || strncasecmp(hardware, amlogic, strlen(amlogic)) == 0) {
        for (i = 0; i < (sizeof(gAllBoardHardwareInfo) / sizeof(BoardHardwareInfo)); i++) {
            if (strncasecmp(gAllBoardHardwareInfo[i].kernelHardware, hardware, strlen(gAllBoardHardwareInfo[i].kernelHardware)) == 0) {
                if (retBoardInfo != 0) {
                    *retBoardInfo = (BoardHardwareInfo*)&gAllBoardHardwareInfo[i];
                }
                return gAllBoardHardwareInfo[i].boardTypeId;
            }
        }
        return -1;
    }

    // h3 and h5, check hardware and boardid
    if (strncasecmp(hardware, h3, strlen(h3)) == 0 || strncasecmp(hardware, h5, strlen(h5)) == 0
        || strncasecmp(hardware, h3_kernel4, strlen(h3_kernel4)) == 0 || strncasecmp(hardware, h5_kernel4, strlen(h5_kernel4)) == 0) {
        int ret = getAllwinnerBoardID(allwinnerBoardID, sizeof(allwinnerBoardID));
        if (ret != 0) {
            ret = getBoardDisplayName(boardDisplayName, sizeof(boardDisplayName));
            if (ret != 0) {
                printf("failed opening one of the following files /sys/class/sunxi_info/sys_info /etc/friendlyelec-release /etc/armbian-release\n");
            }
            else{
                //printf("got boardDisplayName: %s\n", boardDisplayName);
                for (i = 0; i < (sizeof(gAllBoardHardwareInfo) / sizeof(BoardHardwareInfo)); i++) {
                    //printf("\t\t enum, start compare[%d]: %s <--> %s\n", i, gAllBoardHardwareInfo[i].kernelHardware, hardware);
                    if (strncasecmp(gAllBoardHardwareInfo[i].kernelHardware, hardware, strlen(gAllBoardHardwareInfo[i].kernelHardware)) == 0) {
                        if (strncasecmp(gAllBoardHardwareInfo[i].boardDisplayName, boardDisplayName, strlen(gAllBoardHardwareInfo[i].boardDisplayName)) == 0) {
                            if (retBoardInfo != 0) {
                                *retBoardInfo = (BoardHardwareInfo*)&gAllBoardHardwareInfo[i];
                            }
                            return gAllBoardHardwareInfo[i].boardTypeId;
                        }
                    }
                    //printf("\t\t enum, end compare[%d]\n", i);
                }
            }
        }
        else {
            //printf("got boardid: %s\n", allwinnerBoardID);
            for (i = 0; i < (sizeof(gAllBoardHardwareInfo) / sizeof(BoardHardwareInfo)); i++) {
                //printf("\t\t enum, start compare[%d]: %s <--> %s\n", i, gAllBoardHardwareInfo[i].kernelHardware, hardware);
                if (strncasecmp(gAllBoardHardwareInfo[i].kernelHardware, hardware, strlen(gAllBoardHardwareInfo[i].kernelHardware)) == 0) {
                    if (strncasecmp(gAllBoardHardwareInfo[i].allwinnerBoardID, allwinnerBoardID, strlen(gAllBoardHardwareInfo[i].allwinnerBoardID)) == 0) {
                        if (retBoardInfo != 0) {
                            *retBoardInfo = (BoardHardwareInfo*)&gAllBoardHardwareInfo[i];
                        }
                        return gAllBoardHardwareInfo[i].boardTypeId;
                    }
                }
                //printf("\t\t enum, end compare[%d]\n", i);
            }
        }
        return -1;
    }

    if (strlen(revision) == 0) {
        //printf("failed, revision is empty.");
        return -1;
    }

    char revision2[257];
    // sprintf(revision2, "0x%s", revision);
    int iRev;
    iRev = strtol(revision2, NULL, 16);

    // other, check hardware and revision
    for (i = 0; i < (sizeof(gAllBoardHardwareInfo) / sizeof(BoardHardwareInfo)); i++) {
        if (strncasecmp(gAllBoardHardwareInfo[i].kernelHardware, hardware, strlen(gAllBoardHardwareInfo[i].kernelHardware)) == 0) {
            if (gAllBoardHardwareInfo[i].kernelRevision == -1 || gAllBoardHardwareInfo[i].kernelRevision == iRev) {
                if (retBoardInfo != 0) {
                    *retBoardInfo = (BoardHardwareInfo*)&gAllBoardHardwareInfo[i];
                }
                return gAllBoardHardwareInfo[i].boardTypeId;
            }
        }
    }
    return -1;
}


/*******************************************************************************
      END FILE
*******************************************************************************/