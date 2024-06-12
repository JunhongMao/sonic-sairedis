#pragma once

extern "C" {
#include <sai.h>
}
#include <inttypes.h>
#include <set>
#include <regex>
#include <climits>
#include <getopt.h>
#include "swss/table.h"
#include "meta/sai_serialize.h"
#include "sairediscommon.h"
#include <nlohmann/json.hpp>

// Default value: 100MB
static constexpr int64_t RDB_JSON_MAX_SIZE = 1024 * 1024 * 100;

namespace syncd
{
    class SaiDump
    {
        public:
            SaiDump() = default;
            ~SaiDump() = default;
            sai_status_t handleCmdLine(int argc, char **argv);
            void dumpFromRedisDb();
            void printUsage();
            sai_status_t dumpFromRedisRdbJson();
            sai_status_t preProcessFile(const std::string file_name);
            void traverseJson(const nlohmann::json & jsn);

        public:
            std::string rdbJsonFile;
            uint64_t rdbJSonSizeLimit;
            bool dumpTempView;
            bool dumpGraph;

        private:
            std::map<sai_object_id_t, const swss::TableMap*> g_oid_map;

        private:
            size_t get_max_attr_len(const swss::TableMap& map);
            std::string pad_string(std::string s, size_t pad);
            const swss::TableMap* get_table_map(sai_object_id_t object_id);
            void dumpGraphFun(const swss::TableDump& td);
            void print_attributes(size_t indent, const swss::TableMap& map);
    };
}
