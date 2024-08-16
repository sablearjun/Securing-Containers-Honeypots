#include "../../3rdparty/json.hpp"
#include "config.h"
#include "../../common/logger.h"

HTTPMessage config_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json config_list_json, config_list;
    std::string id;
    try
    {
        config_list = request.query;
        if(config_list.contains("id"))
        {
            id = config_list["id"];
        }
        if (id.empty())
        {
            connection->prepare("get_config_list", "select * from config_queries order by id");
            pqxx::work transaction{*connection};
            pqxx::result result;
            result = transaction.exec_prepared("get_config_list");
            connection->unprepare("get_config_list");
            for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
            {
                config_list_json["config_queries"] += { \
                {"id", c["id"].as<std::string>()},
                {"query", c["query"].as<std::string>()},
                {"query_name", c["query_name"].as<std::string>()}
                };
            }
            if(result.size()<=0)
            {
                response.status = boost::beast::http::status::ok;
                response.body = "{\n"
                                "  \"status\":\"Fail\",\n"
                                "  \"message\":\"No data available\"\n"
                                "}";
                response.header["Content-Type"] = "application/json";
                return response;
            }
            else
            {
            response.status = boost::beast::http::status::ok;
            response.body = config_list_json.dump();
            response.header["Content-Type"] = "application/json";
            transaction.commit();
            return response;
            }
        }
        else
        {
            connection->prepare("get_config_list", "select * from config_queries where id = $1 order by id");
            pqxx::work transaction{*connection};
            pqxx::result result;
            result = transaction.exec_prepared("get_config_list",id);
            connection->unprepare("get_config_list");

            for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
            {
                config_list_json["config_queries"] += { \
                {"id", c["id"].as<std::string>()},
                {"query", c["query"].as<std::string>()},
                {"query_name", c["query_name"].as<std::string>()}
                };
            }
            if(result.size()<=0)
            {
                response.status = boost::beast::http::status::ok;
                response.body = "{\n"
                                "  \"status\":\"Fail\",\n"
                                "  \"message\":\"No data available\"\n"
                                "}";
                response.header["Content-Type"] = "application/json";
                return response;
            }
            else
            {
            response.status = boost::beast::http::status::ok;
            response.body = config_list_json.dump();
            response.header["Content-Type"] = "application/json";
            transaction.commit();
            return response;
            }
        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to get list of configurations failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch(...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get config list";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to get config list\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    } 
}

HTTPMessage table_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json config_list_json, config_list;
    std::string id;
    try
    {
        config_list = request.query;
        {   
            connection->prepare("get_table_list", "select table_name from config_table_name");
            pqxx::work transaction{*connection};
            pqxx::result result;
            result = transaction.exec_prepared("get_table_list");
            connection->unprepare("get_table_list");
            for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
            {

                config_list_json["table_name"]+=c["table_name"].as<std::string>();
               
            }
            if(result.size()<=0)
            {
                response.status = boost::beast::http::status::ok;
                response.body = "{\n"
                                "  \"status\":\"Fail\",\n"
                                "  \"message\":\"No data available\"\n"
                                "}";
                response.header["Content-Type"] = "application/json";
                return response;
            }
            else
            {
            response.status = boost::beast::http::status::ok;
            response.body = config_list_json.dump();
            response.header["Content-Type"] = "application/json";
            transaction.commit();
            return response;
            }
        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to get list of table name failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch(...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get table name list";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to get table name list\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    } 
}

HTTPMessage config_delete(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json config_delete_json;
    int id;
    std::string body = request.body;
    try
    {
        config_delete_json = nlohmann::json::parse(body);
        id = config_delete_json["id"];
        connection->prepare("delete_config", "delete from config_queries where id = $1");
        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("delete_config", id);
        int status = result.affected_rows();
        transaction.commit();
        connection->unprepare("delete_config");

        if(status>0)
        {
            response.body = "{\n"
                            "  \"status\":\"Success\",\n"
                            "  \"message\":\"Configuration DELETED.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        else
        {
            response.status = boost::beast::http::status::bad_request;
            response.body = "{\n"
                            "  \"status\":\"Fail\",\n"
                            "  \"message\":\"Failed to delete configuration\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }

        // response.status = boost::beast::http::status::ok;
        // response.body = config_delete_json.dump();
        // response.header["Content-Type"] = "application/json";
        // connection->unprepare("delete_config");
        // return response;

    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to delete a configuration failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch(...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not delete configuration";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to delete configuration\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
        
    }
}

HTTPMessage config_update(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json config_update_json, update_json;
    std::string id, config_update;
    std::string body = request.body;
    std::string query = body;
    std::string query_name; 
    try
    {
        nlohmann::json valid_json = nlohmann::json::parse(query);
        config_update_json = nlohmann::json::parse(body);
        id = config_update_json["id"];
        query_name = config_update_json["query_name"];
        config_update = config_update_json["schedule"].dump();
        update_json["schedule"] = nlohmann::json::parse(config_update);
        query = update_json.dump();
        connection->prepare("config_update", "update config_queries set query_name = $3,query = $2 where id = $1");
        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("config_update", id, query,query_name);
        int status = result.affected_rows();
        transaction.commit();
        connection->unprepare("config_update");

        if(status>0)
        {
            response.status = boost::beast::http::status::ok;
            response.body = "{\n"
                            "  \"status\":\"Success\",\n"
                            "  \"message\":\"Configuration UPDATED.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        else
        {
            response.status = boost::beast::http::status::bad_request;
            response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to update config, NOT a valid JSON\"\n"
                        "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
    }
    catch (const nlohmann::json::parse_error& error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: could not parse JSON. Parse error: " << error.what();
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Could not parse JSON due to bad request\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (const nlohmann::json::type_error& error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: invalid type encountered in JSON. Parse error: " << error.what();
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"invalid type encountered in JSON\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to update configuration failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Query to update configuration contained malformed JSON";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Enrollment failed due to bad request\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }    
}

HTTPMessage config_add(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json config_add_json, config_json;
    std::string query;
    std::string query_name ;

    try
    {
        std::string body = request.body;
        config_add_json = nlohmann::json::parse(body);
        query_name = config_add_json["query_name"];
        config_json = config_add_json["config"];
        query = config_json.dump();
        connection->prepare("add_config", "insert into config_queries (query,query_name) values ($1,$2)");
        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("add_config", query, query_name);
        transaction.commit();
        connection->unprepare("add_config");
        int status = result.affected_rows();

        if(status>0)
        {
            response.status = boost::beast::http::status::ok;
            response.body = "{\n"
                            "  \"status\":\"Success\",\n"
                            "  \"message\":\"Configuration ADDED successfully.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        else
        {
            response.status = boost::beast::http::status::bad_request;
            response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to add config, NOT a valid JSON\"\n"
                        "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
    }
    catch (const nlohmann::json::parse_error& error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: could not parse JSON. Parse error: " << error.what();
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Could not parse JSON\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (const nlohmann::json::type_error& error)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Error: invalid type encountered in JSON. Parse error: " << error.what();
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Invalid type encountered in JSON\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to add configuration failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Query for adding configuration contained malformed JSON";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to add config, NOT a valid JSON\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}