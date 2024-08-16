#include <memory>
#include <tuple>
#include <utility>
#include <openssl/sha.h>
#include <unistd.h>
#include "RemediationService.h"

const char kHexNum[] = "0123456789abcdef";
#define SHA_SIZE 64

IndicatorHash GetIndicatorFromFd(int fd)
{
    IndicatorHash indicator;

    char buffer[4096];
    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    do {
        auto len = read(fd, buffer, 4096);
        if (len > 0) {
            SHA256_Update(&ctx, buffer, len);
        } else if(len == 0) {
            break;
        } else if (len < 0 && errno == EINTR) {
            continue;
        } else {
            return indicator;
        }
    } while(true);

    std::unique_ptr<unsigned char[]> hash{new unsigned char[SHA256_DIGEST_LENGTH]()};
    SHA256_Final(hash.get(), &ctx);

    std::string hex_hash;
    hex_hash.reserve(SHA_SIZE);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hex_hash += kHexNum[hash[i] >> 4];
        hex_hash += kHexNum[hash[i] & 0xf];
    }

    indicator.sha256 = hex_hash;

    return indicator;
}

RemediationService::RemediationService(std::vector<IOCSource> indicators)
{
    this->threat_indicators = std::move(indicators);
}

RemediationPolicy RemediationService::decision(std::string hash)
{
    RemediationPolicy response = this->default_policy;
    for (const auto& source : this->threat_indicators)
    {
        if (source.type != Hash)
            continue;

        for (const auto& ioc : source.indicators)
        {
            if (hash == ioc)
            {
                response = source.policy;
            }
        }
    }
    return response;
}

RemediationPolicy RemediationService::decision(BlockingEvent event)
{
    RemediationPolicy response = this->default_policy;

    for (auto rule : this->blocking_rules)
    {
        auto detection_function = std::get<5>(rule);
        if (detection_function(event))
        {
            response = std::get<4>(rule);

            /*
             * If we get a block, we ignore everything else. If we get an allow/log, we continue with other rules,
             * in case some other rule wants to block.
             */

            if (response == RemediationPolicy::Deny)
                break;
        }
    }

    return response;
}

void RemediationService::loadBlockingRules(const std::vector<std::tuple<std::string,std::string,std::string,std::string,
                                           RemediationPolicy, std::function<bool(const BlockingEvent)>>>& rules)
{
    this->blocking_rules = rules;
}