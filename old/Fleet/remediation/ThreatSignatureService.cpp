#include <boost/algorithm/string.hpp>
#include "RemediationService.h"

ThreatSignatureService::ThreatSignatureService(std::string remote_url, std::string db_path)
{
    this->URL = remote_url;
    this->local_path = db_path;
    db = nullptr;
    db = new SQLite::Database(this->local_path);
}

bool ThreatSignatureService::reload()
{
    bool result = false;
    return result;
}

std::vector<IOCSource> ThreatSignatureService::GetIOCSources()
{
    std::vector<IOCSource> sources;

    SQLite::Statement query(*db, "SELECT * FROM ioc_list;");

    while(query.executeStep())
    {
        IOCSource source;
        int ioc_type = query.getColumn(0).getInt();
        int remediation = query.getColumn(1).getInt();
        std::string indicator_entry = query.getColumn(2).getString();
        std::vector<std::string> indicators;
        boost::split(indicators, indicator_entry, boost::is_any_of("\n"));

        source.type = (IOCType)ioc_type;
        source.policy = (RemediationPolicy)remediation;
        source.indicators = indicators;

        sources.push_back(source);
    }

    return sources;
}