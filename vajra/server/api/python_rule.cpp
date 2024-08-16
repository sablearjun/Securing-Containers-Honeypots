#include "../../3rdparty/json.hpp"
#include "python_rule.h"
#include "../../common/logger.h"



std::string transform_to_camel_case(std::string s)
{
    int n = s.length();

    int res_ind = 0;
    s[0] = toupper(s[0]);
    for (int i = 0; i < n; i++) {

        // check for spaces in the sentence
        if (s[i] == '_') {
            s[i] = ' ';
            // conversion into upper case
            s[i + 1] = toupper(s[i + 1]);
            continue;
        }     
    }
    return s;
}


HTTPMessage rule_add(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json rule_json, rule_add_json;
    std::string rule, name, technique, technique_id, link, comment, os_name, rule_written_by;
    int severity;

    try
    {
        std::string body = request.body;
        rule_add_json = nlohmann::json::parse(body);
        name = rule_add_json["name"];
        rule_json = rule_add_json["rule"];
        rule = rule_json.dump();
        severity = rule_add_json["severity"];
        technique = rule_add_json["technique"];
        technique_id = rule_add_json["technique_id"];
        link = rule_add_json["link"];
        comment = rule_add_json["comment"];
        os_name = rule_add_json["os_name"];
        std::time_t t = std::time(0);
        int created = t;
        int last_modified = t;
        rule_written_by = rule_add_json["rule_written_by"];
        connection->prepare("add_rule", "insert into python_rules (rule, name, severity, technique, technique_id, link, comment, os_name, created, last_modified, rule_written_by) values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)");
        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("add_rule", rule, name, severity, technique, technique_id, link, comment, os_name, created, last_modified, rule_written_by);
        transaction.commit();
        connection->unprepare("add_rule");
        int status = result.affected_rows();
        if (status > 0)
        {
            response.status = boost::beast::http::status::ok;
            response.body = "{\n"
                            "  \"status\":\"Success\",\n"
                            "  \"message\":\"Rule ADDED successfully.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        else
        {
            response.status = boost::beast::http::status::bad_request;
            response.body = "{\n"
                            "  \"status\":\"Fail\",\n"
                            "  \"message\":\"Failed to add rule, NOT a valid JSON\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
    }
    catch (const nlohmann::json::parse_error &error)
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
    catch (const nlohmann::json::type_error &error)
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
                        "  \"message\":\"Query to add rule failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Query for adding rule contained malformed JSON";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to add rule, NOT a valid JSON\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage rule_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json rule_list_json, rule_list;
    std::string id, os_name;
    try
    {
        rule_list = request.query;
        if (rule_list.contains("os_name"))
        {
        os_name = rule_list["os_name"];
        }
        pqxx::result result;
        if (rule_list.contains("id"))
        {
            id = rule_list["id"];
        }
        pqxx::work transaction{*connection};
        if (id.empty() && !os_name.empty())
        {
            connection->prepare("get_rule_list", "select * from python_rules where os_name = $1 order by id");
            result = transaction.exec_prepared("get_rule_list", os_name);
        }
        else if (!id.empty())
        {
            connection->prepare("get_rule_list", "select * from python_rules where id = $1");
            result = transaction.exec_prepared("get_rule_list", id);
        }
        else
        {
            connection->prepare("get_rule_list", "select * from python_rules order by id");
            result = transaction.exec_prepared("get_rule_list");
        }
        connection->unprepare("get_rule_list");
        for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
        {
            rule_list_json["rule_list"] += {
                {"id", c["id"].as<long>()},
                {"name", c["name"].as<std::string>()},
                {"rule", c["rule"].as<std::string>()},
                {"severity", c["severity"].as<int>()},
                {"technique", c["technique"].as<std::string>()},
                {"technique_id", c["technique_id"].as<std::string>()},
                {"link", c["link"].as<std::string>()},
                {"comment", c["comment"].as<std::string>()},
                {"os_name", c["os_name"].as<std::string>()},
                {"created", c["created"].as<std::string>()},
                {"last_modified", c["last_modified"].as<std::string>()},
                {"rule_written_by", c["rule_written_by"].as<std::string>()}};
        }
        if (result.size() <= 0)
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
            response.body = rule_list_json.dump();
            response.header["Content-Type"] = "application/json";
            transaction.commit();
            return response;
        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to get list of rule failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get rule list";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to get rule list\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage rule_update(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json rule_update_json, update_json;
    std::string rule_update;
    std::string body = request.body;
    std::string query = body;
    std::string name, rule, technique, technique_id, link, comment, os_name, rule_written_by;
    int severity;
    long id;
    try
    {
        nlohmann::json valid_json = nlohmann::json::parse(query);
        rule_update_json = nlohmann::json::parse(body);
        id = rule_update_json["id"];
        name = rule_update_json["name"];
        rule = rule_update_json["rule"];
        severity = rule_update_json["severity"];
        technique = rule_update_json["technique"];
        technique_id = rule_update_json["technique_id"];
        link = rule_update_json["link"];
        comment = rule_update_json["comment"];
        os_name = rule_update_json["os_name"];
        std::time_t t = std::time(0);
        int last_modified = t;
        rule_written_by = rule_update_json["rule_written_by"];
        rule_update = rule_update_json["schedule"].dump();
        update_json["schedule"] = nlohmann::json::parse(rule_update);
        query = update_json.dump();
        connection->prepare("rule_update", "update python_rules set name = $2, rule = $3, severity = $4, technique = $5, technique_id = $6, link = $7, comment = $8, os_name = $9, last_modified = $10, rule_written_by = $11 where id = $1");
        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("rule_update", id, name, rule, severity, technique, technique_id, link, comment, os_name, last_modified, rule_written_by);
        int status = result.affected_rows();
        transaction.commit();
        connection->unprepare("rule_update");

        if (status > 0)
        {
            response.status = boost::beast::http::status::ok;
            response.body = "{\n"
                            "  \"status\":\"Success\",\n"
                            "  \"message\":\"RULE UPDATED.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        else
        {
            response.status = boost::beast::http::status::bad_request;
            response.body = "{\n"
                            "  \"status\":\"Fail\",\n"
                            "  \"message\":\"Failed to update rule, NOT a valid JSON\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
    }
    catch (const nlohmann::json::parse_error &error)
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
    catch (const nlohmann::json::type_error &error)
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
                        "  \"message\":\"Query to update rule failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Query to update rule contained malformed JSON";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed due to bad request\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage rule_delete(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json rule_delete_json;
    int id;
    std::string body = request.body;
    try
    {
        rule_delete_json = nlohmann::json::parse(body);
        id = rule_delete_json["id"];
        connection->prepare("delete_rule", "delete from python_rules where id = $1");
        pqxx::work transaction{*connection};
        pqxx::result result = transaction.exec_prepared("delete_rule", id);
        int status = result.affected_rows();
        transaction.commit();
        connection->unprepare("delete_rule");

        if (status > 0)
        {
            response.body = "{\n"
                            "  \"status\":\"Success\",\n"
                            "  \"message\":\"RULE DELETED.\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
        else
        {
            response.status = boost::beast::http::status::bad_request;
            response.body = "{\n"
                            "  \"status\":\"Fail\",\n"
                            "  \"message\":\"Failed to delete rule\"\n"
                            "}";
            response.header["Content-Type"] = "application/json";
            return response;
        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to delete a rule failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not delete rule";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to delete rule\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

HTTPMessage tables_list(HTTPMessage request, std::shared_ptr<pqxx::connection> connection)
{
    HTTPMessage response;
    nlohmann::json rule_list_json, rule_list;
    std::string id, os_name;
    nlohmann::json table_list, final_list, columns_list;

    try
    {
        pqxx::result result, result_label;
        pqxx::work transaction{*connection};

        std::string tables[3]
        = { "bpf_process_events", "bpf_socket_events", "bpf_file_events" };

    // Print Strings
        for (int i = 0; i < 3; i++)
        {

        connection->prepare("get_table_list", "select column_name from information_schema.columns where table_name = $1;");
        result = transaction.exec_prepared("get_table_list",tables[i]);
        connection->unprepare("get_table_list");

        int const num_rows = std::size(result);
        nlohmann::json parents_list = nlohmann::json::array();

        for (int rownum = 0; rownum < num_rows; ++rownum)
        {

        connection->prepare("get_table_label", "SELECT col_description($1::regclass, $2);");
        result_label = transaction.exec_prepared("get_table_label",tables[i],rownum+1);
        connection->unprepare("get_table_label");


            pqxx::row const row = result[rownum];
            int const num_cols = std::size(row);
            nlohmann::json obj;

            for (int colnum = 0; colnum < num_cols; ++colnum)
            {
                pqxx::field const field = row[colnum];

                if (row[row[colnum].name()].is_null())
                {
                    obj[row[colnum].name()] = "NULL";
                }
                else
                {
                    obj["column_name"] = row[row[colnum].name()].as<std::string>();
                    if(!result_label.at(0)["col_description"].is_null())
                    {
                        obj["column_label"] = result_label.at(0)["col_description"].as<std::string>();
                    }
                    else{
                        obj["column_label"] = transform_to_camel_case(row[row[colnum].name()].as<std::string>());
                    }


                }
            }
            parents_list.push_back(obj);
        }
        columns_list["table_name"]=tables[i];
        columns_list["table_label"]=transform_to_camel_case(tables[i]);
        columns_list["columns"]=parents_list;
        // table_list[tables[i]]=columns_list;
        table_list.push_back(columns_list);


        }
        final_list["data"]=table_list;
        if (result.size() <= 0)
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
            response.body = final_list.dump();
            response.header["Content-Type"] = "application/json";
            transaction.commit();
            return response;
        }
    }
    catch (pqxx::sql_error const &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << __SRCREF__ << "Database error: " << e.what() << ". Query was: " << e.query();
        response.status = boost::beast::http::status::internal_server_error;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Query to get list of rule failed due to database error.\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << __SRCREF__ << "Unknown error: could not get rule list";
        response.status = boost::beast::http::status::bad_request;
        response.body = "{\n"
                        "  \"status\":\"Fail\",\n"
                        "  \"message\":\"Failed to get rule list\"\n"
                        "}";
        response.header["Content-Type"] = "application/json";
        return response;
    }
}

