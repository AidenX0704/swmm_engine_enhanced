#include <string.h>
#include <stdio.h>
#include <time.h>
#include "swmm5.h"

int main(int argc, char *argv[]) {
    char *inputFile;
    char *reportFile;
    char *binaryFile;
    char *arg1;
    char blank[] = "";
    int version, vMajor, vMinor, vRelease;
    char errMsg[128];
    int msgLen = 127;
    time_t start;
    double runTime;

    version = swmm_getVersion();
    vMajor = version / 10000;
    vMinor = (version - 10000 * vMajor) / 1000;
    vRelease = (version - 10000 * vMajor - 1000 * vMinor);
    start = time(0);

    // --- 检查命令行参数数量
    if (argc == 1) {
        printf("\nNot Enough Arguments (See Help --help)\n\n");
    } else if (argc == 2) {
        // --- 提取第一个参数
        arg1 = argv[1];

        if (strcmp(arg1, "--help") == 0 || strcmp(arg1, "-h") == 0) {
            // Help
            printf("\n\nSTORMWATER MANAGEMENT MODEL (SWMM) HELP\n\n");
            printf("COMMANDS:\n");
            printf("\t--help (-h)       SWMM Help\n");
            printf("\t--version (-v)    Build Version\n");
            printf("\t--run             Run SWMM simulation\n");
            printf("\t--nodeseries      Process node series from .inp and .csv\n");
            printf("\nRUNNING A SIMULATION:\n");
            printf("\t runswmm <input file> <report file> <optional output file>\n\n");
        } else if (strcmp(arg1, "--version") == 0 || strcmp(arg1, "-v") == 0) {
            // Output version number
            printf("\n%d.%d.%0d\n\n", vMajor, vMinor, vRelease);
        } else if (strcmp(arg1, "--run") == 0) {
            // Placeholder for run command (if any specific logic is needed)
            printf("\nPlease provide the input, report, and binary file names.\n");
        } else if (strcmp(arg1, "--nodeseries") == 0) {
            printf("\nPlease provide the input file and csv file names.\n");
        } else {
            printf("\nUnknown Argument (See Help --help)\n\n");
        }
    } else {
        // --- 提取文件名从命令行参数
        if (strcmp(argv[1], "--run") == 0) {
            if (argc < 4) {
                printf("\nNot Enough Arguments for --run (input, report, and optional binary file required)\n\n");
                return 1;
            }

            inputFile = argv[2];
            reportFile = argv[3];
            if (argc > 4) binaryFile = argv[4];
            else binaryFile = blank;
            printf("\n... EPA SWMM %d.%d (Build %d.%d.%0d)\n", vMajor, vMinor,
                vMajor, vMinor, vRelease);

            // --- 执行 SWMM
            swmm_run(inputFile, reportFile, binaryFile);

            // 显示运行状态
            runTime = difftime(time(0), start);
            printf("\n\n... EPA SWMM completed in %.2f seconds.", runTime);
            if (swmm_getError(errMsg, msgLen) > 0) printf(" There are errors.\n");
            else if (swmm_getWarnings() > 0) printf(" There are warnings.\n");
            else printf("\n");
        } 
        else if (strcmp(argv[1], "--nodeseries") == 0) {
            if (argc < 4) {
                printf("\nNot Enough Arguments for --nodeseries (input file and csv file required)\n\n");
                return 1;
            }

            inputFile = argv[2];
            char *csvFile = argv[3];

            // 调用处理节点时间序列的函数
            get_coordinates_from_inp_and_csv(inputFile, csvFile);

            printf("\nNode series data processed successfully.\n");
        } 
        else {
            printf("\nUnknown Argument (See Help --help)\n\n");
        }
    }

    return 0;
}
