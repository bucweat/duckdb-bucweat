#include "../common.h"

using namespace odbc_test;

TEST_CASE("Test Select Statement", "[odbc][select]") {
	SQLHANDLE env;
	SQLHANDLE dbc;

	HSTMT hstmt = SQL_NULL_HSTMT;

	// Connect to the database using SQLConnect
	CONNECT_TO_DATABASE(env, dbc);

	// Allocate a statement handle
	EXECUTE_AND_CHECK("SQLAllocHandle (HSTMT)", SQLAllocHandle, SQL_HANDLE_STMT, dbc, &hstmt);

	// Execute a simple query
	EXECUTE_AND_CHECK("SQLExecDirect (SELECT 1 UNION ALL SELECT 2)", SQLExecDirect, hstmt,
	                  ConvertToSQLCHAR("SELECT 1 UNION ALL SELECT 2"), SQL_NTS);

	// Fetch the first row
	EXECUTE_AND_CHECK("SQLFetch (SELECT 1 UNION ALL SELECT 2)", SQLFetch, hstmt);
	// Check the data
	DATA_CHECK(hstmt, 1, "1");

	// Fetch the second row
	EXECUTE_AND_CHECK("SQLFetch (SELECT 1 UNION ALL SELECT 2)", SQLFetch, hstmt);
	// Check the data
	DATA_CHECK(hstmt, 1, "2");

	EXECUTE_AND_CHECK("SQLFreeStmt (SQL_CLOSE)", SQLFreeStmt, hstmt, SQL_CLOSE);

	// Create a query with 1600 columns
	std::string query = "SELECT ";
	for (int i = 1; i < 1600; i++) {
		query += std::to_string(i);
		if (i < 1599) {
			query += ", ";
		}
	}

	EXECUTE_AND_CHECK("SQLExecDirect (SELECT 1600 columns)", SQLExecDirect, hstmt, ConvertToSQLCHAR(query.c_str()),
	                  SQL_NTS);

	// Fetch the first row
	EXECUTE_AND_CHECK("SQLFetch (SELECT 1600 columns)", SQLFetch, hstmt);

	// Check the data
	for (int i = 1; i < 1600; i++) {
		DATA_CHECK(hstmt, i, std::to_string(i));
	}

	// when connecting via platform ODBC plumbing, you might get "Invalid cursor state"
	// which may happen due to cursor being maintained in handle and is left in a
	// invalid state from previous read. Solution is to close a cursor that has been opened
	// on a statement and discards pending results.
	// https://stackoverflow.com/questions/1752548/invalid-cursor-state-sql-state-24000-in-sqlexecdirect
	// https://www.ibm.com/docs/en/db2-for-zos/11?topic=functions-sqlclosecursor-close-cursor-discard-pending-results
	EXECUTE_AND_CHECK("SQLCloseCursor", SQLCloseCursor, hstmt);

	// SELECT $x; should throw error
	SQLRETURN ret = SQLExecDirect(hstmt, ConvertToSQLCHAR("SELECT $x"), SQL_NTS);
	REQUIRE(ret == SQL_ERROR);
	std::string state;
	std::string message;
	ACCESS_DIAGNOSTIC(state, message, hstmt, SQL_HANDLE_STMT);
	REQUIRE(state == "42000");
	REQUIRE_THAT(message, Catch::Matchers::Contains("Not all parameters are bound"));

	// Free the statement handle
	EXECUTE_AND_CHECK("SQLFreeStmt (HSTMT)", SQLFreeStmt, hstmt, SQL_CLOSE);
	EXECUTE_AND_CHECK("SQLFreeHandle (HSTMT)", SQLFreeHandle, SQL_HANDLE_STMT, hstmt);

	DISCONNECT_FROM_DATABASE(env, dbc);
}
