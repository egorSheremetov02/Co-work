#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>

bool auth(std::string const &account_name, std::string const &password) {

    try {
        pqxx::connection C("dbname=test_bd1 user=egor");
        std::cout << "Connected to " << C.dbname() << std::endl;
        pqxx::work W{C};

        pqxx::result R{
                W.exec("SELECT * FROM users WHERE account_name=\'" + account_name + "\' AND password=\'" + password +
                       "\'")};

        return R.size() == 1;

    }
    catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;
    }

    return false;
}
