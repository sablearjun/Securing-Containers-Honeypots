#ifndef FLEET_REMEDIATIONSERVICE_H
#define FLEET_REMEDIATIONSERVICE_H

#include <string>
#include <tuple>
#include <vector>
#include "../3rdparty/SQLiteCpp/include/SQLiteCpp/SQLiteCpp.h"
#include "../3rdparty/SQLiteCpp/include/SQLiteCpp/Database.h"
#include "../common/EdrDataTypes.h"

struct IndicatorHash
{
    std::string md5, sha1, sha256;
};

IndicatorHash GetIndicatorFromFd(int fd);

enum IOCType
{
    Hash,
    Filename,
    Filepath,
    FilenamePattern,
    FilepathPattern
};

struct IOCSource
{
    std::vector<std::string> indicators;
    IOCType type;
    RemediationPolicy policy;
};

class ThreatSignatureService
{
private:
    std::string local_path;
    std::string URL;
    SQLite::Database *db;
    std::vector<IOCSource> ioc_sources;
public:
    ThreatSignatureService(std::string update_url, std::string db_path);
    bool reload();
    std::vector<IOCSource> GetIOCSources();
};

class RemediationService
{
private:
    std::vector<IOCSource> threat_indicators;
    RemediationPolicy default_policy;
    std::vector<std::tuple<std::string,std::string,std::string,std::string, RemediationPolicy, std::function<bool(const BlockingEvent)>>> blocking_rules;
public:
    RemediationService(std::vector<IOCSource> indicators);
    void loadBlockingRules(const std::vector<std::tuple<std::string,std::string,std::string,std::string, RemediationPolicy, std::function<bool(const BlockingEvent)>>>& rules);
    RemediationPolicy decision(std::string hash);
    RemediationPolicy decision(BlockingEvent event);
};

#endif //FLEET_REMEDIATIONSERVICE_H
