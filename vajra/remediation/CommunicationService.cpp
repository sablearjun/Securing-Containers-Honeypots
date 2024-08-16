#include <thread>

#include "FilesystemMonitor.h"
#include "../3rdparty/json.hpp"

nlohmann::json SerializeMessage(const BlockingResponse& message)
{
    nlohmann::json payload;

    payload["was_blocked"] = message.wasBlocked;
    payload["metadata"] = message.metadata;
    payload["reason"] = message.reason;
    payload["event"]["file_path"] = message.event.file_path;
    payload["event"]["permission"] = message.event.permission_requested;
    payload["event"]["pid"] = message.event.pid;
    payload["event"]["fd"] = message.event.fd;
    payload["event"]["process_entry"]["pid"] = message.event.entry.pid;
    payload["event"]["process_entry"]["path"] = message.event.entry.path;
    payload["event"]["process_entry"]["cmdline"] = message.event.entry.cmdline;
    payload["event"]["process_entry"]["cwd"] = message.event.entry.cwd;
    payload["event"]["process_entry"]["ppid"] = message.event.entry.ppid;
    payload["event"]["process_entry"]["syscall"] = message.event.entry.syscall;
    payload["event"]["process_entry"]["event_time"] = message.event.entry.event_time;
    payload["event"]["process_entry"]["user_id"] = message.event.entry.user_id;
    payload["event"]["process_entry"]["group_id"] = message.event.entry.group_id;
    payload["event"]["process_entry"]["effective_gid"] = message.event.entry.effective_group_id;
    payload["event"]["process_entry"]["effective_uid"] = message.event.entry.effective_user_id;
    payload["event"]["process_entry"]["owner_uid"] = message.event.entry.owner_user_id;
    payload["event"]["process_entry"]["owner_gid"] = message.event.entry.owner_group_id;
    payload["event"]["process_entry"]["parents"] = ProcessEntryListToJson(message.event.parents);

    return payload;
}

std::string CreatePayloadFromMessage(const std::vector<BlockingResponse>& message_batch)
{
    nlohmann::json payload;

    for (const auto& message : message_batch)
    {
        payload.push_back(SerializeMessage(message));
    }

    return payload.dump();
}

CommunicationService::CommunicationService(const std::string& host)
{
    this->remote_host = host;
    batch_size = 20;
}

CommunicationService::~CommunicationService()
{
    //
}

void CommunicationService::SendResponseAsync(BlockingResponse response)
{
    std::lock_guard<std::mutex> lock(message_mutex);
    message_queue.push_back(response);
}

void CommunicationService::run()
{
    //
}

[[noreturn]] void CommunicationService::SendResponse()
{
    std::vector<BlockingResponse> batch;

    /*
     * By default, we try to batch the responses in group of batch_size. However, if queue happens to have less messages
     * than batch_size, we send all of them in one shot.
     */

    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(message_mutex);
            if (message_queue.size() < batch_size)
            {
                for (auto & message : message_queue)
                {
                    batch.push_back(message);
                }
            }
            else
            {
                for (unsigned int index = 0; index < batch_size; ++index)
                {
                    batch.push_back(message_queue[index]);
                }
            }
        }

        std::string payload = CreatePayloadFromMessage(batch);
        if (client.SendRequest(this->remote_host, RequestType::POST, payload) && client.GetHTTPResponseCode() == 200)
        {
            // data has been sent successfully. Time to remove the data from message queue.
            {
                std::lock_guard<std::mutex> lock(message_mutex);
                for (const auto& item : batch)
                {
                    message_queue.pop_front();
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}