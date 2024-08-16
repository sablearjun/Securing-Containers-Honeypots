#include "../../common/plugin_base.h"
#include "blocking_rules.h"

class ProcessBlockingProcessor : public AbstractBlockingPlugin
{
private:
    std::vector<std::tuple<std::string,std::string,std::string,std::string, RemediationPolicy, std::function<bool(const BlockingEvent)>>> blocking_rules;
public:
    ProcessBlockingProcessor()
    {
        blocking_rules.emplace_back("REDIS_WRITING_FILE","","","", RemediationPolicy::Deny, RedisModifyingFiles);
    }

    ~ProcessBlockingProcessor() override {}

    int load() override
    {
        return 0;
    }

    int unload() override
    {
        return 0;
    }

    PluginInfo version() override
    {
        PluginInfo info;

        info.name = "Process Blocking Ruleset";
        info.version_revision = 0;
        info.version_minor = 1;
        info.version_major = 0;
        info.publisher = "self";
        info.version_string = "-dev";

        return info;
    }

    std::vector<std::tuple<std::string,std::string,std::string,std::string, RemediationPolicy, std::function<bool(const BlockingEvent)>>> process_blocking_rules() override
    {
        return this->blocking_rules;
    }
};

extern "C"
{
void* module_init()
{
    ProcessBlockingProcessor *obj = new ProcessBlockingProcessor();
    return reinterpret_cast<void*>(obj);
}


void module_exit(void* ptr)
{
    if (ptr)
        delete reinterpret_cast<ProcessBlockingProcessor*>(ptr);
}
}