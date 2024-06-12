#include <gtest/gtest.h>
#include "meta/sai_serialize.h"
#include "saidump.h"

#define ARRAYLEN(arr) (int)(sizeof(arr) / sizeof((arr)[0]))

TEST(SaiDump, printUsage)
{
    SWSS_LOG_ENTER();
    testing::internal::CaptureStdout();
    syncd::SaiDump m_saiDump;
    m_saiDump.printUsage();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(true, output.find("Usage: saidump [-t] [-g] [-r] [-m] [-h]") != std::string::npos);
}

TEST(SaiDump, handleCmdLine)
{
    SWSS_LOG_ENTER();
    syncd::SaiDump m_saiDump;
    const char *cmd1[] = {"saidump", "-r", "./dump.json", "-m", "100"};
    m_saiDump.handleCmdLine(ARRAYLEN(cmd1), const_cast<char **>(cmd1));
    EXPECT_EQ(m_saiDump.rdbJsonFile, "./dump.json");
    EXPECT_EQ(m_saiDump.rdbJSonSizeLimit, RDB_JSON_MAX_SIZE);

    //reset, otherwise getopt_long could not be called correctly again
    optind = 0;
    const char *cmd2[] = {"saidump", "-g"};
    m_saiDump.handleCmdLine(ARRAYLEN(cmd2), const_cast<char **>(cmd2));
    EXPECT_EQ(m_saiDump.dumpGraph , true);

    optind = 0;
    const char *cmd3[] = {"saidump", "-t"};
    m_saiDump.handleCmdLine(ARRAYLEN(cmd3), const_cast<char **>(cmd3));
    EXPECT_EQ(m_saiDump.dumpTempView , true);
}

TEST(SaiDump, dumpFromRedisRdbJson)
{
    SWSS_LOG_ENTER();
    syncd::SaiDump m_saiDump;
    m_saiDump.rdbJsonFile = "";
    EXPECT_EQ(SAI_STATUS_FAILURE, m_saiDump.dumpFromRedisRdbJson());
    m_saiDump.rdbJsonFile = "./dump.json";
    EXPECT_EQ(SAI_STATUS_SUCCESS, m_saiDump.dumpFromRedisRdbJson());
}

TEST(SaiDump, preProcessFile)
{
    SWSS_LOG_ENTER();
    syncd::SaiDump m_saiDump;
    EXPECT_EQ(SAI_STATUS_FAILURE, m_saiDump.preProcessFile(""));
    m_saiDump.rdbJSonSizeLimit = 0;
    EXPECT_EQ(SAI_STATUS_FAILURE, m_saiDump.preProcessFile("./dump.json"));
    m_saiDump.rdbJSonSizeLimit = RDB_JSON_MAX_SIZE;
    EXPECT_EQ(SAI_STATUS_SUCCESS, m_saiDump.preProcessFile("./dump.json"));
    m_saiDump.rdbJsonFile = "./dump.json";
    EXPECT_EQ(SAI_STATUS_SUCCESS, m_saiDump.dumpFromRedisRdbJson());
}

TEST(SaiDump, dumpFromRedisDb)
{
    SWSS_LOG_ENTER();
    syncd::SaiDump m_saiDump;
    m_saiDump.dumpFromRedisDb();
    std::string output = testing::internal::GetCapturedStdout();
    std::cout << output << "\n";
}
