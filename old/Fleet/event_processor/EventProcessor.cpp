#include "../3rdparty/json.hpp"
#include "EventProcessor.h"

#include <thread>
#include <utility>

std::vector<ProcessEntry> default_processor(const std::shared_ptr<pqxx::connection> &connection, unsigned long long id)
{
    std::vector<ProcessEntry> result;
    return result;
}

std::vector<BpfProcessEntry> bpf_default_processor(const std::shared_ptr<pqxx::connection> &connection, unsigned long long id)
{
    std::vector<BpfProcessEntry> result;
    return result;
}

EventProcessor::EventProcessor(std::shared_ptr<ConnectionPool> pool)
{
    this->pool = std::move(pool);
    this->process_event_processor = default_processor;
    this->socket_event_processor = default_processor;
    this->user_event_processor = default_processor;
    this->bpf_process_event_processor = bpf_default_processor;
    this->bpf_socket_event_processor = bpf_default_processor;
    this->bpf_file_event_processor = bpf_default_processor;
}

void EventProcessor::setProcessEventProcessor(
    std::function<std::vector<ProcessEntry>(const std::shared_ptr<pqxx::connection> &, unsigned long long)> processor)
{
    this->process_event_processor = std::move(processor);
}

void EventProcessor::setSocketEventProcessor(
    std::function<std::vector<ProcessEntry>(const std::shared_ptr<pqxx::connection> &, unsigned long long)> processor)
{
    this->socket_event_processor = std::move(processor);
}

void EventProcessor::setUserEventProcessor(
    std::function<std::vector<ProcessEntry>(const std::shared_ptr<pqxx::connection> &, unsigned long long)> processor)
{
    this->user_event_processor = std::move(processor);
}

void EventProcessor::setBPFProcessEventProcessor(
    std::function<std::vector<BpfProcessEntry>(const std::shared_ptr<pqxx::connection> &, unsigned long long)> processor)
{
    this->bpf_process_event_processor = std::move(processor);
}

void EventProcessor::setBPFSocketEventProcessor(
    std::function<std::vector<BpfProcessEntry>(const std::shared_ptr<pqxx::connection> &, unsigned long long)> processor)
{
    this->bpf_socket_event_processor = std::move(processor);
}

void EventProcessor::setBPFFileEventProcessor(
    std::function<std::vector<BpfProcessEntry>(const std::shared_ptr<pqxx::connection> &, unsigned long long)> processor)
{
    this->bpf_file_event_processor = std::move(processor);
}

[[noreturn]] void EventProcessor::ProcessEventProcessor()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_process_event_id", "select id from process_events where (process_tree is null or process_tree = '') limit 500");
        connection->prepare("update_process_tree", "update process_events set process_tree = $1 where id = $2");

        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("get_process_event_id");

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                auto conn = pool->GetConnection();
                nlohmann::json process_tree = ProcessEntryListToJson(this->process_event_processor(conn, record_id));
                pool->ReturnConnection(conn);

                transaction.exec_prepared("update_process_tree", process_tree.dump(), record_id);
                transaction.commit();
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        connection->unprepare("get_process_event_id");
        connection->unprepare("update_process_tree");
        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void EventProcessor::SocketEventProcessor()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_socket_event_id", "select id from socket_events where (process_tree is null or process_tree = '') limit 500");
        connection->prepare("update_process_tree", "update socket_events set process_tree = $1 where id = $2");

        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("get_socket_event_id");

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                auto conn = pool->GetConnection();
                nlohmann::json process_tree = ProcessEntryListToJson(this->socket_event_processor(conn, record_id));
                pool->ReturnConnection(conn);

                transaction.exec_prepared("update_process_tree", process_tree.dump(), record_id);
                transaction.commit();
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        connection->unprepare("get_socket_event_id");
        connection->unprepare("update_process_tree");
        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void EventProcessor::UserEventProcessor()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_user_event_id", "select id from user_events where (process_tree is null or process_tree = '') limit 500");
        connection->prepare("update_process_tree", "update user_events set process_tree = $1 where id = $2");

        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("get_user_event_id");

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                auto conn = pool->GetConnection();
                nlohmann::json process_tree = ProcessEntryListToJson(this->user_event_processor(conn, record_id));
                pool->ReturnConnection(conn);

                transaction.exec_prepared("update_process_tree", process_tree.dump(), record_id);
                transaction.commit();
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        connection->unprepare("get_user_event_id");
        connection->unprepare("update_process_tree");
        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void EventProcessor::BPFProcessEventProcessor()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_bpf_process_event_id", "select id from bpf_process_events where (process_tree is null or process_tree = '') limit 500");
        connection->prepare("update_bpf_process_tree", "update bpf_process_events set process_tree = $1 where id = $2");

        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("get_bpf_process_event_id");

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                auto conn = pool->GetConnection();
                nlohmann::json process_tree = BPFProcessEntryListToJson(this->bpf_process_event_processor(conn, record_id));
                pool->ReturnConnection(conn);

                transaction.exec_prepared("update_bpf_process_tree", process_tree.dump(), record_id);
                transaction.commit();
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        connection->unprepare("get_bpf_process_event_id");
        connection->unprepare("update_bpf_process_tree");
        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void EventProcessor::BPFSocketEventProcessor()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_bpf_socket_event_id", "select id from bpf_socket_events where (process_tree is null or process_tree = '') limit 500");
        connection->prepare("update_bpf_socket_tree", "update bpf_socket_events set process_tree = $1 where id = $2");

        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("get_bpf_socket_event_id");

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                auto conn = pool->GetConnection();
                nlohmann::json process_tree = BPFProcessEntryListToJson(this->bpf_socket_event_processor(conn, record_id));
                pool->ReturnConnection(conn);

                transaction.exec_prepared("update_bpf_socket_tree", process_tree.dump(), record_id);
                transaction.commit();
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        connection->unprepare("get_bpf_socket_event_id");
        connection->unprepare("update_bpf_socket_tree");
        pool->ReturnConnection(connection);
    }
}

[[noreturn]] void EventProcessor::BPFFileEventProcessor()
{
    while (true)
    {
        auto connection = pool->GetConnection();
        connection->prepare("get_bpf_file_event_id", "select id from bpf_file_events where (process_tree is null or process_tree = '') limit 500");
        connection->prepare("update_bpf_file_tree", "update bpf_file_events set process_tree = $1 where id = $2");

        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("get_bpf_file_event_id");

        if (!result.empty())
        {
            for (unsigned int i = 0; i < result.size(); ++i)
            {
                long record_id = result.at(i)["id"].as<long>();
                auto conn = pool->GetConnection();
                nlohmann::json process_tree = BPFProcessEntryListToJson(this->bpf_file_event_processor(conn, record_id));
                pool->ReturnConnection(conn);

                transaction.exec_prepared("update_bpf_file_tree", process_tree.dump(), record_id);
                transaction.commit();
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        connection->unprepare("get_bpf_file_event_id");
        connection->unprepare("update_bpf_file_tree");
        pool->ReturnConnection(connection);
    }
}

void EventProcessor::run()
{
    std::thread process_event_thread(&EventProcessor::ProcessEventProcessor, this);
    std::thread socket_event_thread(&EventProcessor::SocketEventProcessor, this);
    std::thread user_event_thread(&EventProcessor::UserEventProcessor, this);
    std::thread bpf_process_event_thread(&EventProcessor::BPFProcessEventProcessor, this);
    std::thread bpf_socket_event_thread(&EventProcessor::BPFSocketEventProcessor, this);
    std::thread bpf_file_event_thread(&EventProcessor::BPFFileEventProcessor, this);

    process_event_thread.join();
    socket_event_thread.join();
    user_event_thread.join();
    bpf_process_event_thread.join();
    bpf_socket_event_thread.join();
    bpf_file_event_thread.join();
}