#include <iostream>
#include <pqxx/pqxx>

int main(int, char *argv[])
{
    pqxx::connection c{"postgresql://test@localhost/test"};
    pqxx::work txn{c};

    // Normally we'd query the DB using txn.exec().  But for querying just one
    // single value, we can use txn.query_value() as a shorthand.
    //
    // Use txn.quote() to escape and quote a C++ string for use as an SQL string
    // in a query's text.
    int employee_id = txn.query_value<int>(
            "SELECT id "
            "FROM Employee "
           /* "WHERE name ="
            " \"Egor Suvorov\""*/);

    std::cout << "Updating employee #" << employee_id << '\n';

    // Update the employee's salary.  Use exec0() to perform a query and check
    // that it produces an empty result.  If the result does contain data, it
    // will throw an exception.
    //
    // The ID is an integer, so we don't need to escape and quote it when using
    // it in our query text.  Just convert it to its PostgreSQL string
    // representation using to_string().
    txn.exec0(
            "UPDATE EMPLOYEE "
            "SET salary = salary + 1 "
            "WHERE id = " + pqxx::to_string(employee_id));

    // Make our change definite.
    txn.commit();
}
