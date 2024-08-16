#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <boost/program_options.hpp>

#include "../common/ConfigurationParser.h"
#include "../common/ConnectionPool.h"
#include "../3rdparty/jwtpp/include/export/jwtpp/jwtpp.hh"

int main(int argc, char **argv)
{
    bool add_user = false, remove_user = false, list_users = false;
    std::string username, secret, token, issuer, iat, exp;

    ConfigurationParser config_parser("config.json");
    if (!config_parser.parse())
    {
        std::cerr << "Cannot read configuration" << std::endl;
        return 1;
    }

    ApiServerConfiguration api = config_parser.GetApiServerConfiguration();
    std::shared_ptr<ConnectionPool> connection_pool(
        new ConnectionPool(api.database.ConnectionString(), api.database_pool.maximum_connections));

    try
    {
        boost::program_options::options_description opt("Allowed options");
        opt.add_options()("username", boost::program_options::value<std::string>(), "username for access")("secret", boost::program_options::value<std::string>(), "shared secret to sign the token")("token", boost::program_options::value<std::string>(), "API access token")("issuer", boost::program_options::value<std::string>(), "issuer of authentication parameters")("iat", boost::program_options::value<std::string>(), "issued at of authentication parameters")("exp", boost::program_options::value<std::string>(), "expiry of authentication parameters")("help", "Help Options")("add", "Add new user")("remove", "Remove existing user")("list", "List existing users");

        boost::program_options::variables_map opt_map;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, opt), opt_map);
        boost::program_options::notify(opt_map);

        if (opt_map.count("help"))
        {
            std::cout << std::endl << "Sample :" << std::endl;
            std::cout << "$ ./userctl --add --username kni9ht --secret yogesh@123 --token qwerty --issuer manager --iat 1637342164 --exp 1952873712" << std::endl<<std::endl;
            std::cout << "$ ./userctl --add       'Query'           # To generate a new token and register it" << std::endl;
            std::cout << "$ ./userctl --username  'username'        # Usrename for access" << std::endl;
            std::cout << "$ ./userctl --secret    'secret passcode' # Shared secret to sign the token" << std::endl;
            std::cout << "$ ./userctl --token     'token'           # API access token" << std::endl;
            std::cout << "$ ./userctl --issuer    'name'            # Issuer of Authentication parameter" << std::endl;
            std::cout << "$ ./userctl --iat       'issued at'       # Time at token issued" << std::endl;
            std::cout << "$ ./userctl --exp       'expiration'      # Time at token expires" << std::endl;
            std::cout << "$ ./userctl --help                        # For help" << std::endl << std::endl; 
            return 0;
        }
        else if (opt_map.count("add"))
            add_user = true;
        else if (opt_map.count("remove"))
            remove_user = true;
        else if (opt_map.count("list"))
            list_users = true;
        else
        {
            std::cout << std::endl << "Sample :" << std::endl;
            std::cout << "$ ./userctl --add --username kni9ht --secret yogesh@123 --token qwerty --issuer manager --iat 1637342164 --exp 1952873712" << std::endl<<std::endl;
            std::cout << "$ ./userctl --add       'Query'           # To generate a new token and register it" << std::endl;
            std::cout << "$ ./userctl --username  'username'        # Usrename for access" << std::endl;
            std::cout << "$ ./userctl --secret    'secret passcode' # Shared secret to sign the token" << std::endl;
            std::cout << "$ ./userctl --token     'token'           # API access token" << std::endl;
            std::cout << "$ ./userctl --issuer    'name'            # Issuer of Authentication parameter" << std::endl;
            std::cout << "$ ./userctl --iat       'issued at'       # Time at token issued" << std::endl;
            std::cout << "$ ./userctl --exp       'expiration'      # Time at token expires" << std::endl;
            std::cout << "$ ./userctl --help                        # For help" << std::endl << std::endl; 
            return 0;
        }

        if (opt_map.count("username"))
            username = opt_map["username"].as<std::string>();
        if (opt_map.count("secret"))
            secret = opt_map["secret"].as<std::string>();
        if (opt_map.count("token"))
            token = opt_map["token"].as<std::string>();
        if (opt_map.count("issuer"))
            issuer = opt_map["issuer"].as<std::string>();
        if (opt_map.count("iat"))
            iat = opt_map["iat"].as<std::string>();
        if (opt_map.count("exp"))
            exp = opt_map["exp"].as<std::string>();
        if (add_user && (username.empty() || secret.empty() || token.empty() || issuer.empty() || iat.empty() || exp.empty()))
        {
            std::cerr << "All parameters (username, secret, token, issuer, iat and exp) are needed to add a new user" << std::endl;
            return 3;
        }
        if ((remove_user || list_users) && (!secret.empty() || !token.empty() || !issuer.empty() || !iat.empty() || !exp.empty()))
        {
            std::cerr << "Secret, token, issuer, iat and exp should not be supplied with action (remove, list)" << std::endl;
            return 4;
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 5;
    }

    // case 1: add new user
    if (add_user)
    {
        try
        {
            auto connection = connection_pool->GetConnection();
            connection->prepare("add_user", "insert into api_users (username, secret, token, issuer) values ($1, $2, $3, $4)");
            pqxx::work transaction{*connection};
            transaction.exec_prepared("add_user", username, secret, token, issuer);
            connection->unprepare("add_user");
            transaction.commit();
            connection_pool->ReturnConnection(connection);
            jwtpp::claims claims;
            claims.set().iss(issuer);
            claims.set().any("token", token);
            claims.set().any("user", username);
            claims.set().iat(iat);
            claims.set().exp(exp);
            jwtpp::sp_crypto h256 = std::make_shared<jwtpp::hmac>(secret.c_str(), jwtpp::alg_t::HS256);
            std::string bearer = jwtpp::jws::sign_bearer(claims, h256);

            std::cout << bearer << std::endl;

            return 0;
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return 6;
        }
    }

    if (remove_user)
    {
        try
        {
            auto connection = connection_pool->GetConnection();
            connection->prepare("remove_user", "delete from api_users where username = $1");
            pqxx::work transaction{*connection};
            transaction.exec_prepared("remove_user", username);
            connection->unprepare("remove_user");
            transaction.commit();
            connection_pool->ReturnConnection(connection);
            return 0;
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return 7;
        }
    }

    if (list_users)
    {
        try
        {
            auto connection = connection_pool->GetConnection();
            connection->prepare("list_user", "select * from api_users");
            pqxx::work transaction{*connection};
            pqxx::result result = transaction.exec_prepared("list_user");
            for (pqxx::result::const_iterator c = result.begin(); c != result.end(); ++c)
            {
                std::cout << std::endl << "Username : " << c["username"] << " , " <<"Issuer : " << c["issuer"] << std::endl;
            }
            connection->unprepare("list_user");
            transaction.commit();
            connection_pool->ReturnConnection(connection);
            return 0;
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return 7;
        }
    }
    return 0;
}
