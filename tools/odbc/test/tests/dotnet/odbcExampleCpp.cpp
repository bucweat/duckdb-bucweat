#include "pch.h"
#include <sstream>
#include <iostream>
#include <string>
#include <exception>
#include <cstdarg>
#include <vector>
#include <limits>
#include <regex>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace System;
using namespace System::Data;
using namespace System::Data::Odbc;

//*************************************************************
// convert .NET System::String to std::string
static std::string toss(System::String ^ s) {
	using Runtime::InteropServices::Marshal;
	System::IntPtr pointer = Marshal::StringToHGlobalAnsi(s);
	char *charPointer = reinterpret_cast<char *>(pointer.ToPointer());
	std::string returnString(charPointer, s->Length);
	Marshal::FreeHGlobal(pointer);
	return returnString;
}

//*************************************************************
// CHECKs at least C++11
const std::string vformat(const char *const zcFormat, ...) {

	// initialize use of the variable argument array
	va_list vaArgs;
	va_start(vaArgs, zcFormat);

	// reliably acquire the size
	// from a copy of the variable argument array
	// and a functionally reliable call to mock the formatting
	va_list vaArgsCopy;
	va_copy(vaArgsCopy, vaArgs);
	const int iLen = std::vsnprintf(NULL, 0, zcFormat, vaArgsCopy);
	va_end(vaArgsCopy);

	// return a formatted string without risking memory mismanagement
	// and without assuming any compiler or platform specific behavior
	std::vector<char> zc(iLen + 1);
	std::vsnprintf(zc.data(), zc.size(), zcFormat, vaArgs);
	va_end(vaArgs);
	return std::string(zc.data(), iLen);
}

TEST_CASE("System.Data.ODBC .NET tests written in C++/CLI", "[dotnet][odbc]") {

	int rtnVal_i = 0;
	INT8 rtnVal_i8 = 0;
	Int16 rtnVal_i16 = 0;
	Int32 rtnVal_i32 = 0;
	Int64 rtnVal_i64 = 0;
	UINT8 rtnVal_ui8 = 0;
	UInt16 rtnVal_ui16 = 0;
	UInt32 rtnVal_ui32 = 0;
	UInt64 rtnVal_ui64 = 0;
	Decimal rtnVal_dec = 0;
	DateTime rtnVal_dt;
	TimeSpan rtnVal_ts;
	double rtnVal_d = 0;
	System::String ^ connStr = "Driver=DuckDB Driver;Database=test.duckdb;";

	OdbcConnection ^ Conn = nullptr;
	try {
		Conn = gcnew OdbcConnection(connStr);
		Conn->Open();
	} catch (OdbcException ^ ex) {

		INFO("OdbcException: {" << toss(ex->Message) << "}");

	} catch (Exception ^ ex) {

		INFO("Exception: {" << toss(ex->Message) << "}");
	}

	OdbcCommand ^ DbCmd = Conn->CreateCommand();

	/////////////////////////////////////////////////////////////////
	// create weather table
	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE weather(
			city	VARCHAR,
			temp_lo INTEGER,
			temp_hi INTEGER,
			prcp	FLOAT,
			date	DATE
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		INSERT INTO
			weather
		VALUES(
			'San Francisco',
			46,
			50,
			0.25,
			'1994-11-27'
		);
	)";
	CHECK(1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		select count(1) from weather;
	)";
	Int64 weatherCount = static_cast<Int64>(DbCmd->ExecuteScalar());
	CHECK(weatherCount == 1);

	/////////////////////////////////////////////////////////////////
	// FillSchema() a DataTable
	DataTable ^ dt = gcnew DataTable();
	OdbcDataAdapter ^ adapter =
		gcnew OdbcDataAdapter("select city, temp_lo, prcp, date from weather limit 1;", Conn);

	// FillSchema() .NET code outputs some cruft so set up to capture it to a string and ignore
	std::stringstream ss;
	auto old_buf = std::cout.rdbuf(ss.rdbuf());
	adapter->FillSchema(dt, SchemaType::Source);
	std::cout.rdbuf(old_buf);

	CHECK(dt->Rows->Count == 0);
	CHECK(dt->Columns->Count == 4);
	CHECK(toss(dt->Columns[0]->ColumnName) == "city");
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.String");
	CHECK(toss(dt->Columns[1]->ColumnName) == "temp_lo");
	CHECK(toss(dt->Columns[1]->DataType->ToString()) == "System.Int32");
	CHECK(toss(dt->Columns[2]->ColumnName) == "prcp");
	CHECK(toss(dt->Columns[2]->DataType->ToString()) == "System.Double");
	CHECK(toss(dt->Columns[3]->ColumnName) == "date");
	CHECK(toss(dt->Columns[3]->DataType->ToString()) == "System.DateTime");

	/////////////////////////////////////////////////////////////////
	// Fill() DataTable directly
	delete dt;
	dt = gcnew DataTable();
	adapter->Fill(dt);
	CHECK(dt->Rows->Count == 1);
	CHECK(dt->Columns->Count == 4);
	CHECK(toss(dt->Columns[0]->ColumnName) == "city");
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.String");
	CHECK(toss(dt->Columns[1]->ColumnName) == "temp_lo");
	CHECK(toss(dt->Columns[1]->DataType->ToString()) == "System.Int32");
	CHECK(toss(dt->Columns[2]->ColumnName) == "prcp");
	CHECK(toss(dt->Columns[2]->DataType->ToString()) == "System.Double");
	CHECK(toss(dt->Columns[3]->ColumnName) == "date");
	CHECK(toss(dt->Columns[3]->DataType->ToString()) == "System.DateTime");
	CHECK(toss(dt->Rows[0]->ItemArray[0]->ToString()) == "San Francisco");
	CHECK(toss(dt->Rows[0]->ItemArray[1]->ToString()) == "46");
	CHECK(toss(dt->Rows[0]->ItemArray[2]->ToString()) == "0.25");
	CHECK(toss(dt->Rows[0]->ItemArray[3]->ToString()) == "11/27/1994 12:00:00 AM");

	/////////////////////////////////////////////////////////////////
	// Fill() Dataset then look at the Datatable
	delete dt;
	dt = gcnew DataTable();
	DataSet ^ ds = gcnew DataSet();
	adapter->Fill(ds);
	dt = ds->Tables[0];
	CHECK(dt->Rows->Count == 1);
	CHECK(dt->Columns->Count == 4);
	CHECK(toss(dt->Columns[0]->ColumnName) == "city");
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.String");
	CHECK(toss(dt->Columns[1]->ColumnName) == "temp_lo");
	CHECK(toss(dt->Columns[1]->DataType->ToString()) == "System.Int32");
	CHECK(toss(dt->Columns[2]->ColumnName) == "prcp");
	CHECK(toss(dt->Columns[2]->DataType->ToString()) == "System.Double");
	CHECK(toss(dt->Columns[3]->ColumnName) == "date");
	CHECK(toss(dt->Columns[3]->DataType->ToString()) == "System.DateTime");
	CHECK(toss(dt->Rows[0]->ItemArray[0]->ToString()) == "San Francisco");
	CHECK(toss(dt->Rows[0]->ItemArray[1]->ToString()) == "46");
	CHECK(toss(dt->Rows[0]->ItemArray[2]->ToString()) == "0.25");
	CHECK(toss(dt->Rows[0]->ItemArray[3]->ToString()) == "11/27/1994 12:00:00 AM");

	delete dt;
	delete adapter;

	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE cities(
			name VARCHAR,
			lat DECIMAL,
			lon DECIMAL
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		INSERT INTO
			cities
		VALUES(
			'San Francisco',
			-194.0,
			53.0
		);
	)";
	CHECK(1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		select count(1) from cities;
	)";
	Int64 citiesCount = static_cast<Int64>(DbCmd->ExecuteScalar());
	CHECK(1 == citiesCount);

	/////////////////////////////////////////////////////////////////
	// Use DataReader and access the data.
	DbCmd->CommandText = R"(
		select * from weather;
	)";
	OdbcDataReader ^ DbReader = DbCmd->ExecuteReader();

	CHECK(DbReader->HasRows == true);
	CHECK(DbReader->FieldCount == 5);
	CHECK(toss(DbReader->GetName(0)) == "city");
	CHECK(toss(DbReader->GetName(1)) == "temp_lo");
	CHECK(toss(DbReader->GetName(2)) == "temp_hi");
	CHECK(toss(DbReader->GetName(3)) == "prcp");
	CHECK(toss(DbReader->GetName(4)) == "date");

	DbReader->Read();
	CHECK(toss(DbReader->GetString(0)) == "San Francisco");
	CHECK(toss(DbReader->GetString(1)) == "46");
	CHECK(toss(DbReader->GetString(2)) == "50");
	CHECK(toss(DbReader->GetString(3)) == "0.25");
	CHECK(toss(DbReader->GetString(4)) == "1994-11-27");

	CHECK(DbReader->RecordsAffected == -1);

	DbReader->Close();
	delete DbReader;

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	// look at how ODBC handles all of the DuckDB supported data types
	// https://duckdb.org/docs/sql/data_types/overview#general-purpose-data-types
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// BIGINT		INT8, LONG					signed eight-byte integer
	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE biginttest(
			bi BIGINT,
			i8 INT8,
			l LONG
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		INSERT INTO biginttest VALUES
			(-9223372036854775808, -9223372036854775808, -9223372036854775808),
			(9223372036854775807,9223372036854775807,9223372036854775807),
			(0,0,0);
	)";
	CHECK(1 == DbCmd->ExecuteNonQuery());

	adapter = gcnew OdbcDataAdapter("select * from biginttest;", Conn);
	dt = gcnew DataTable();
	old_buf = std::cout.rdbuf(ss.rdbuf());
	adapter->Fill(dt);
	std::cout.rdbuf(old_buf);

	CHECK(dt->Rows->Count == 3);
	CHECK(dt->Columns->Count == 3);
	CHECK(toss(dt->Columns[0]->ColumnName) == "bi");
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.Int64");
	CHECK(toss(dt->Columns[1]->ColumnName) == "i8");
	CHECK(toss(dt->Columns[1]->DataType->ToString()) == "System.Int64");
	CHECK(toss(dt->Columns[2]->ColumnName) == "l");
	CHECK(toss(dt->Columns[2]->DataType->ToString()) == "System.Int64");

	delete dt;
	delete adapter;

	/////////////////////////////////////////////////////////////////
	// BIT			BITSTRING					string of 1’s and 0’s

	/////////////////////////////////////////////////////////////////
	// BOOLEAN		BOOL, LOGICAL				logical boolean (true/false)

	/////////////////////////////////////////////////////////////////
	// BLOB			BYTEA, BINARY, VARBINARY	variable-length binary data

	/////////////////////////////////////////////////////////////////
	// DATE	 									calendar date (year, month day)
	try {
		adapter = gcnew OdbcDataAdapter("SELECT DATE '1992-09-20' as d, DATE '1992-03-22' + 5 as d2, 'epoch'::DATE as e;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		adapter->Fill(dt);
		std::cout.rdbuf(old_buf);
		CHECK(dt->Rows->Count == 1);
		CHECK(dt->Columns->Count == 3);
		CHECK(toss(dt->Columns[0]->ColumnName) == "d");
		CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.DateTime");
		CHECK(toss(dt->Columns[1]->ColumnName) == "d2");
		CHECK(toss(dt->Columns[1]->DataType->ToString()) == "System.DateTime");
		CHECK(toss(dt->Columns[2]->ColumnName) == "e");
		CHECK(toss(dt->Columns[2]->DataType->ToString()) == "System.DateTime");
		rtnVal_dt = static_cast<DateTime>(dt->Rows[0]->ItemArray[0]);
		CHECK(toss(rtnVal_dt.ToString("MM/dd/yyyy")) == "09/20/1992");
		rtnVal_dt = static_cast<DateTime>(dt->Rows[0]->ItemArray[1]);
		CHECK(toss(rtnVal_dt.ToString("MM/dd/yyyy")) == "03/27/1992");
		rtnVal_dt = static_cast<DateTime>(dt->Rows[0]->ItemArray[2]);
		CHECK(toss(rtnVal_dt.ToString("MM/dd/yyyy")) == "01/01/1970");
	} catch (OdbcException ^ ex) {
		INFO("DATE OdbcException[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
		CHECK(false);
	} catch (Exception ^ ex) {
		INFO("DATE Exception[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
		CHECK(false);
	}

	/////////////////////////////////////////////////////////////////
	// DOUBLE		FLOAT8, NUMERIC, DECIMAL	double precision floating-point number (8 bytes)
	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE doubletest(
			dbl DOUBLE,
			f8 FLOAT8,
			num NUMERIC,
			dec DECIMAL(18, 3),
			bigdec DECIMAL(24, 3)
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	// note here that inserting oversized value (row 7) is converted to Infinity.
	DbCmd->CommandText = R"(
		INSERT INTO
			doubletest
		VALUES
			(2.2250738585072014e-308,2.2250738585072014e-308,1.0, 123456789012345.123, 123456789012345678901.123),
			(1.7976931348623158e+308,1.7976931348623158e+308,1.0, 123456789012345.123, 123456789012345678901.123),
			('-Infinity','-Infinity',1.0, 123456789012345.123, 123456789012345678901.123),
			('Infinity','Infinity',1.0, 123456789012345.123, 123456789012345678901.123),
			('NaN','NaN',1.0, 123456789012345.123, 123456789012345678901.123),
			(0,0,1.0, 123456789012345.123, 123456789012345678901.123),
			(1.8e+309,1.0,1.0,1.0,1.0);
	)";
	rtnVal_i = DbCmd->ExecuteNonQuery();
	CHECK(1 == rtnVal_i);

	adapter = gcnew OdbcDataAdapter("select * from doubletest;", Conn);
	dt = gcnew DataTable();
	old_buf = std::cout.rdbuf(ss.rdbuf());
	adapter->Fill(dt);
	std::cout.rdbuf(old_buf);

	CHECK(dt->Rows->Count == 7);
	CHECK(dt->Columns->Count == 5);
	CHECK(toss(dt->Columns[0]->ColumnName) == "dbl");
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.Double");
	CHECK(toss(dt->Columns[1]->ColumnName) == "f8");
	CHECK(toss(dt->Columns[1]->DataType->ToString()) == "System.Double");
	CHECK(toss(dt->Columns[2]->ColumnName) == "num");
	CHECK(toss(dt->Columns[2]->DataType->ToString()) == "System.Double");
	CHECK(toss(dt->Columns[3]->ColumnName) == "dec");
	CHECK(toss(dt->Columns[3]->DataType->ToString()) == "System.Double");
	CHECK(toss(dt->Columns[4]->ColumnName) == "bigdec");
	CHECK(toss(dt->Columns[4]->DataType->ToString()) == "System.Double");

	rtnVal_d = static_cast<double>(dt->Rows[0]->ItemArray[0]);
	CHECK_THAT(rtnVal_d, Catch::Matchers::WithinAbs(2.2250738585072014e-308, 1.0));
	rtnVal_d = static_cast<double>(dt->Rows[0]->ItemArray[1]);
	CHECK_THAT(rtnVal_d, Catch::Matchers::WithinAbs(2.2250738585072014e-308, 1.0));

	rtnVal_d = static_cast<double>(dt->Rows[1]->ItemArray[0]);
	CHECK_THAT(rtnVal_d, Catch::Matchers::WithinAbs(1.7976931348623158e+308, 1.0));
	rtnVal_d = static_cast<double>(dt->Rows[1]->ItemArray[1]);
	CHECK_THAT(rtnVal_d, Catch::Matchers::WithinAbs(1.7976931348623158e+308, 1.0));

	rtnVal_d = static_cast<double>(dt->Rows[0]->ItemArray[3]);
	CHECK_THAT(rtnVal_d, Catch::Matchers::WithinAbs(123456789012345.123, 0.001));

	rtnVal_d = static_cast<double>(dt->Rows[2]->ItemArray[0]);
	CHECK(rtnVal_d == -std::numeric_limits<double>::infinity());

	rtnVal_d = static_cast<double>(dt->Rows[3]->ItemArray[0]);
	CHECK(rtnVal_d == std::numeric_limits<double>::infinity());

	rtnVal_d = static_cast<double>(dt->Rows[4]->ItemArray[0]);
	CHECK(std::isnan(rtnVal_d) == true);

	rtnVal_d = static_cast<double>(dt->Rows[6]->ItemArray[0]);
	CHECK(rtnVal_d == std::numeric_limits<double>::infinity());

	// ODBC only has double, so loss of precision makes this the best we can do...
	rtnVal_d = static_cast<double>(dt->Rows[0]->ItemArray[4]);
	CHECK_THAT(rtnVal_d, Catch::Matchers::WithinAbs(123456789012345683968.0, 1.0));

	delete dt;
	delete adapter;

	/////////////////////////////////////////////////////////////////
	// HUGEINT	 								signed sixteen-byte integer
	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE huge_int(
			hi HUGEINT
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		INSERT INTO
			huge_int
		VALUES
			(-170141183460469231731687303715884105727),
			(170141183460469231731687303715884105727);
	)";
	rtnVal_i = DbCmd->ExecuteNonQuery();
	CHECK(1 == rtnVal_i);

	adapter = gcnew OdbcDataAdapter("select * from huge_int;", Conn);
	dt = gcnew DataTable();
	old_buf = std::cout.rdbuf(ss.rdbuf());
	adapter->Fill(dt);
	std::cout.rdbuf(old_buf);

	CHECK(dt->Rows->Count == 2);
	CHECK(dt->Columns->Count == 1);
	CHECK(toss(dt->Columns[0]->ColumnName) == "hi");
	// INFO(toss(dt->Columns[0]->DataType->ToString()));

	// ODBC largest int type is SQL_BIGINT which is 8 bytes,
	// so huge_int converted to double with loss of precision
	// since it is a double, no [easy] way to compare
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.Double");

	DbCmd->CommandText = R"(
		INSERT INTO
			huge_int
		VALUES
			(170141183460469231731687303715884105728);
	)";
	try {
		DbCmd->ExecuteNonQuery();
	} catch (OdbcException ^ ex) {
		CHECK_THAT(toss(ex->Message),
			            Catch::Matchers::Contains("is out of range for the destination type INT128"));
	}

	/////////////////////////////////////////////////////////////////
	// INTEGER		INT4, INT, SIGNED			signed four-byte integer
	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE int4b(
			i4 INT4,
			i INT,
			s SIGNED
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		INSERT INTO
			int4b
		VALUES
			(-2147483648,-2147483648,-2147483648),
			(2147483647,2147483647,2147483647);
	)";
	rtnVal_i = DbCmd->ExecuteNonQuery();
	CHECK(1 == rtnVal_i);

	adapter = gcnew OdbcDataAdapter("select * from int4b;", Conn);
	dt = gcnew DataTable();
	old_buf = std::cout.rdbuf(ss.rdbuf());
	adapter->Fill(dt);
	std::cout.rdbuf(old_buf);

	CHECK(dt->Rows->Count == 2);
	CHECK(dt->Columns->Count == 3);
	CHECK(toss(dt->Columns[0]->ColumnName) == "i4");
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.Int32");
	CHECK(toss(dt->Columns[1]->ColumnName) == "i");
	CHECK(toss(dt->Columns[1]->DataType->ToString()) == "System.Int32");
	CHECK(toss(dt->Columns[2]->ColumnName) == "s");
	CHECK(toss(dt->Columns[2]->DataType->ToString()) == "System.Int32");

	rtnVal_i32 = static_cast<Int32>(dt->Rows[0]->ItemArray[0]);
	CHECK(rtnVal_i32 == -2147483648);
	rtnVal_i32 = static_cast<Int32>(dt->Rows[0]->ItemArray[1]);
	CHECK(rtnVal_i32 == -2147483648);
	rtnVal_i32 = static_cast<Int32>(dt->Rows[0]->ItemArray[2]);
	CHECK(rtnVal_i32 == -2147483648);

	rtnVal_i32 = static_cast<Int32>(dt->Rows[1]->ItemArray[0]);
	CHECK(rtnVal_i32 == 2147483647);
	rtnVal_i32 = static_cast<Int32>(dt->Rows[1]->ItemArray[1]);
	CHECK(rtnVal_i32 == 2147483647);
	rtnVal_i32 = static_cast<Int32>(dt->Rows[1]->ItemArray[2]);
	CHECK(rtnVal_i32 == 2147483647);

	// OdbcException : {ERROR [42000] ODBC_DuckDB->PrepareStmt
	// Conversion Error: Type INT64 with value -2147483649 can't be cast because the
	// value is out of range for the destination type INT32}
	DbCmd->CommandText = R"(
		INSERT INTO
			int4b
		VALUES
			(-2147483649,-2147483649,-2147483649);
	)";
	try {
		DbCmd->ExecuteNonQuery();
	} catch (OdbcException ^ ex) {
		CHECK_THAT(toss(ex->Message),
			            Catch::Matchers::Contains("is out of range for the destination type INT32"));
	}

	// ERROR[42000] ODBC_DuckDB->PrepareStmt Binder Error
	//: table int4b has 3 columns but 1 values were supplied
	DbCmd->CommandText = R"(
		INSERT INTO
			int4b
		VALUES
			(1);
	)";
	try {
		DbCmd->ExecuteNonQuery();
	} catch (OdbcException ^ ex) {
		CHECK_THAT(toss(ex->Message),
			            Catch::Matchers::Contains("table int4b has 3 columns but 1 values were supplied"));
	}

	/////////////////////////////////////////////////////////////////
	// INTERVAL	 								date / time delta

	/////////////////////////////////////////////////////////////////
	// REAL			FLOAT4, FLOAT				single precision floating-point number (4 bytes)
	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE spfp4(
			f4 FLOAT4,
			f FLOAT
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		INSERT INTO
			spfp4
		VALUES
			(1.175494351e-38,1.175494351e-38),
			(3.402823466e+38,3.402823466e+38);
	)";
	rtnVal_i = DbCmd->ExecuteNonQuery();
	CHECK(1 == rtnVal_i);

	adapter = gcnew OdbcDataAdapter("select * from spfp4;", Conn);
	dt = gcnew DataTable();
	old_buf = std::cout.rdbuf(ss.rdbuf());
	adapter->Fill(dt);
	std::cout.rdbuf(old_buf);

	CHECK(dt->Rows->Count == 2);
	CHECK(dt->Columns->Count == 2);
	CHECK(toss(dt->Columns[0]->ColumnName) == "f4");
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.Double");
	CHECK(toss(dt->Columns[1]->ColumnName) == "f");
	CHECK(toss(dt->Columns[1]->DataType->ToString()) == "System.Double");

	rtnVal_d = static_cast<double>(dt->Rows[0]->ItemArray[0]);
	CHECK_THAT(rtnVal_d, Catch::Matchers::WithinAbs(1.175494351e-38F, 1.0));
	rtnVal_d = static_cast<double>(dt->Rows[0]->ItemArray[1]);
	CHECK_THAT(rtnVal_d, Catch::Matchers::WithinAbs(1.175494351e-38F, 1.0));

	rtnVal_d = static_cast<double>(dt->Rows[1]->ItemArray[0]);
	CHECK_THAT(rtnVal_d, Catch::Matchers::WithinAbs(3.402823466e+38F, 1.0));
	rtnVal_d = static_cast<double>(dt->Rows[1]->ItemArray[1]);
	CHECK_THAT(rtnVal_d, Catch::Matchers::WithinAbs(3.402823466e+38F, 1.0));

	/////////////////////////////////////////////////////////////////
	// SMALLINT		INT2, SHORT					signed two-byte integer
	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE int2b(
			i2 INT2,
			s SHORT
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		INSERT INTO
			int2b
		VALUES
			(-32768,-32768),
			(32767,32767);
	)";
	rtnVal_i = DbCmd->ExecuteNonQuery();
	CHECK(1 == rtnVal_i);

	adapter = gcnew OdbcDataAdapter("select * from int2b;", Conn);
	dt = gcnew DataTable();
	old_buf = std::cout.rdbuf(ss.rdbuf());
	adapter->Fill(dt);
	std::cout.rdbuf(old_buf);

	CHECK(dt->Rows->Count == 2);
	CHECK(dt->Columns->Count == 2);
	CHECK(toss(dt->Columns[0]->ColumnName) == "i2");
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.Int16");
	CHECK(toss(dt->Columns[1]->ColumnName) == "s");
	CHECK(toss(dt->Columns[1]->DataType->ToString()) == "System.Int16");

	rtnVal_i16 = static_cast<Int16>(dt->Rows[0]->ItemArray[0]);
	CHECK(rtnVal_i16 == -32768);
	rtnVal_i16 = static_cast<Int16>(dt->Rows[0]->ItemArray[1]);
	CHECK(rtnVal_i16 == -32768);

	rtnVal_i16 = static_cast<Int16>(dt->Rows[1]->ItemArray[0]);
	CHECK(rtnVal_i16 == 32767);
	rtnVal_i16 = static_cast<Int16>(dt->Rows[1]->ItemArray[1]);
	CHECK(rtnVal_i16 == 32767);

	// OdbcException : {ERROR [42000] ODBC_DuckDB->PrepareStmt
	// Conversion Error: Type INT32 with value -32769 can't be cast because the
	// value is out of range for the destination type INT16}
	DbCmd->CommandText = R"(
		INSERT INTO
			int2b
		VALUES
			(-32769,-32769);
	)";
	try {
		DbCmd->ExecuteNonQuery();
	} catch (OdbcException ^ ex) {
		CHECK_THAT(toss(ex->Message),
			            Catch::Matchers::Contains("is out of range for the destination type INT16"));
	}

	/////////////////////////////////////////////////////////////////
	// TIME	 									time of day (no time zone)

	// all of the time examples return TimeSpan which is incorrect
	// looking at the ODBC log it reports type SQL_TYPE_TIME which seems correct...
	// seems that .Net is not returning the correct typee

	//try {
	//	DbCmd->CommandText = R"(
	//	CREATE OR REPLACE TABLE time(
	//		t TIME
	//	);
	//	)";
	//	CHECK(-1 == DbCmd->ExecuteNonQuery());

	//	DbCmd->CommandText = R"(
	//		INSERT INTO
	//			time
	//		VALUES
	//			('11:30:00');
	//	)";
	//	rtnVal_i = DbCmd->ExecuteNonQuery();
	//	CHECK(1 == rtnVal_i);
	//	adapter = gcnew OdbcDataAdapter("select * from time;", Conn);
	//	dt = gcnew DataTable();
	//	old_buf = std::cout.rdbuf(ss.rdbuf());
	//	adapter->Fill(dt);
	//	std::cout.rdbuf(old_buf);
	//	CHECK(dt->Rows->Count == 1);
	//	CHECK(dt->Columns->Count == 1);
	//	CHECK(toss(dt->Columns[0]->ColumnName) == "t");
	//	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.TimeSpan");
	//	rtnVal_ts = static_cast<TimeSpan>(dt->Rows[0]->ItemArray[0]);
	//	CHECK(toss(rtnVal_dt.ToString("hh:mm:ss")) == "11:30:00");
	//} catch (OdbcException ^ ex) {
	//INFO("TIME OdbcException[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	//CHECK(false);
	//} catch (Exception ^ ex) {
	//INFO("TIME Exception[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	//CHECK(false);
	//}

	//try {
	//	adapter = gcnew OdbcDataAdapter("SELECT TIME '11:30:00' as t;", Conn);
	//	dt = gcnew DataTable();
	//	old_buf = std::cout.rdbuf(ss.rdbuf());
	//	adapter->Fill(dt);
	//	std::cout.rdbuf(old_buf);
	//	CHECK(dt->Rows->Count == 1);
	//	CHECK(dt->Columns->Count == 1);
	//	CHECK(toss(dt->Columns[0]->ColumnName) == "t");
	//	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.TimeSpan");
	//	rtnVal_ts = static_cast<TimeSpan>(dt->Rows[0]->ItemArray[0]);
	//	CHECK(toss(rtnVal_dt.ToString("hh:mm:ss")) == "11:30:00");
	//} catch (OdbcException ^ ex) {
	//	INFO("TIME OdbcException[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	// CHECK(false);
	// } catch (Exception ^ ex) {
	//	INFO("TIME Exception[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	// CHECK(false);
	// }

	//try {
	//	adapter = gcnew OdbcDataAdapter("SELECT get_current_time() as t;", Conn);
	//	dt = gcnew DataTable();
	//	old_buf = std::cout.rdbuf(ss.rdbuf());
	//	adapter->Fill(dt);
	//	std::cout.rdbuf(old_buf);
	//	CHECK(dt->Rows->Count == 1);
	//	CHECK(dt->Columns->Count == 1);
	//	CHECK(toss(dt->Columns[0]->ColumnName) == "t");
	//	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.TimeSpan");
	//	rtnVal_ts = static_cast<TimeSpan>(dt->Rows[0]->ItemArray[0]);
	//	CHECK(toss(rtnVal_dt.ToString("hh:mm:ss")) == "11:30:00");
	//} catch (OdbcException ^ ex) {
	//	INFO("get_current_time OdbcException[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	// CHECK(false);
	// } catch (Exception ^ ex) {
	//	INFO("get_current_time Exception[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	// CHECK(false);
	// }

	/////////////////////////////////////////////////////////////////
	// TIMESTAMP		DATETIME					combination of time and date
	try {
		adapter = gcnew OdbcDataAdapter("SELECT TIMESTAMP '1992-09-20 11:30:00.123456' as dt;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		adapter->Fill(dt);
		std::cout.rdbuf(old_buf);
		CHECK(dt->Rows->Count == 1);
		CHECK(dt->Columns->Count == 1);
		CHECK(toss(dt->Columns[0]->ColumnName) == "dt");
		CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.DateTime");
		// .Net does not handle fractional seconds...
		rtnVal_dt = static_cast<DateTime>(dt->Rows[0]->ItemArray[0]);
		CHECK(toss(rtnVal_dt.ToString("yyyy-MM-dd hh:mm:ss.fff")) == "1992-09-20 11:30:00.000");
	} catch (OdbcException ^ ex) {
		INFO("TIMESTAMP OdbcException[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
		CHECK(false);
	} catch (Exception ^ ex) {
		INFO("TIMESTAMP Exception[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
		CHECK(false);
	}

	try {
		// DATETIME with fractional sec
		adapter = gcnew OdbcDataAdapter("SELECT DATETIME '1992-09-20 11:30:00.123456' as dt;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		adapter->Fill(dt);
		std::cout.rdbuf(old_buf);
		CHECK(dt->Rows->Count == 1);
		CHECK(dt->Columns->Count == 1);
		CHECK(toss(dt->Columns[0]->ColumnName) == "dt");
		CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.DateTime");
		// .Net does not handle fractional seconds...
		rtnVal_dt = static_cast<DateTime>(dt->Rows[0]->ItemArray[0]);
		CHECK(toss(rtnVal_dt.ToString("yyyy-MM-dd hh:mm:ss.fff")) == "1992-09-20 11:30:00.000");
	} catch (OdbcException ^ ex) {
		INFO("DATETIME OdbcException[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
		CHECK(false);
	} catch (Exception ^ ex) {
		INFO("DATETIME Exception[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
		CHECK(false);
	}

	// currently this data type does not work
	//try {
	//	adapter = gcnew OdbcDataAdapter("SELECT TIMESTAMP_NS '1992-09-20 11:30:00.123456' as dt;", Conn);
	//	dt = gcnew DataTable();
	//	old_buf = std::cout.rdbuf(ss.rdbuf());
	//	adapter->Fill(dt);
	//	std::cout.rdbuf(old_buf);
	//	CHECK(dt->Rows->Count == 1);
	//	CHECK(dt->Columns->Count == 1);
	//	CHECK(toss(dt->Columns[0]->ColumnName) == "dt");
	//	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.DateTime");
	//	// .Net does not handle fractional seconds...
	//	CHECK(toss(rtnVal_dt.ToString("yyyy-MM-dd hh:mm:ss.ffffff")) == "1992-09-20 11:30:00.000000");
	//} catch (OdbcException ^ ex) {
	//	INFO("TIMESTAMP_NS OdbcException[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	// CHECK(false);
	//} catch (Exception ^ ex) {
	//	INFO("TIMESTAMP_NS Exception[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	// CHECK(false);
	//}

	// currently this data type does not work
	//try {
	//	adapter = gcnew OdbcDataAdapter("SELECT TIMESTAMP_MS '1992-09-20 11:30:00.123456' as dt;", Conn);
	//	dt = gcnew DataTable();
	//	old_buf = std::cout.rdbuf(ss.rdbuf());
	//	adapter->Fill(dt);
	//	std::cout.rdbuf(old_buf);
	//	CHECK(dt->Rows->Count == 1);
	//	CHECK(dt->Columns->Count == 1);
	//	CHECK(toss(dt->Columns[0]->ColumnName) == "dt");
	//	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.DateTime");
	//	// .Net does not handle fractional seconds...
	//	CHECK(toss(rtnVal_dt.ToString("yyyy-MM-dd hh:mm:ss.fff")) == "1992-09-20 11:30:00.000");
	//} catch (OdbcException ^ ex) {
	//	INFO("TIMESTAMP_MS OdbcException[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	// CHECK(false);
	//} catch (Exception ^ ex) {
	//	INFO("TIMESTAMP_MS Exception[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	// CHECK(false);
	//}

	// currently this data type does not work
	//try {
	//	adapter = gcnew OdbcDataAdapter("SELECT TIMESTAMP_S '1992-09-20 11:30:00.123456' as dt;", Conn);
	//	dt = gcnew DataTable();
	//	old_buf = std::cout.rdbuf(ss.rdbuf());
	//	adapter->Fill(dt);
	//	std::cout.rdbuf(old_buf);
	//	CHECK(dt->Rows->Count == 1);
	//	CHECK(dt->Columns->Count == 1);
	//	CHECK(toss(dt->Columns[0]->ColumnName) == "dt");
	//	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.DateTime");
	//	// .Net does not handle fractional seconds...
	//	rtnVal_dt = static_cast<DateTime>(dt->Rows[0]->ItemArray[0]);
	//	CHECK(toss(rtnVal_dt.ToString("yyyy-MM-dd hh:mm:ss.fff")) == "1992-09-20 11:30:00.000");
	//} catch (OdbcException ^ ex) {
	// CHECK(false);
	//	INFO("TIMESTAMP_S OdbcException[" << __LINE__<< "]: {" << toss(ex->Message) << "}");
	//} catch (Exception ^ ex) {
	//	INFO("TIMESTAMP_S Exception[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	// CHECK(false);
	//}

	/////////////////////////////////////////////////////////////////
	// TIMESTAMP WITH TIME ZONE	TIMESTAMPTZ		combination of time and date that uses the current time zone
	//try {
	//	adapter = gcnew OdbcDataAdapter("SELECT '1992-09-20 11:30:00.123-07'::TIMESTAMPTZ as dt;", Conn);
	//	dt = gcnew DataTable();
	//	old_buf = std::cout.rdbuf(ss.rdbuf());
	//	adapter->Fill(dt);
	//	std::cout.rdbuf(old_buf);
	//	CHECK(dt->Rows->Count == 1);
	//	CHECK(dt->Columns->Count == 1);
	//	CHECK(toss(dt->Columns[0]->ColumnName) == "dt");
	//	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.DateTime");
	//	// .Net does not handle fractional seconds...
	//	rtnVal_dt = static_cast<DateTime>(dt->Rows[0]->ItemArray[0]);
	//	CHECK(toss(rtnVal_dt.ToString("yyyy-MM-dd hh:mm:ss.fff")) == "1992-09-20 11:30:00.000");
	//} catch (OdbcException ^ ex) {
	// CHECK(false);
	//	INFO("TIMESTAMPTZ OdbcException[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	//} catch (Exception ^ ex) {
	//	INFO("TIMESTAMPTZ Exception[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
	// CHECK(false);
	//}

	/////////////////////////////////////////////////////////////////
	// TINYINT		INT1						signed one-byte integer
	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE sti(
			sti TINYINT,
			i1	INT1
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		INSERT INTO
			sti
		VALUES
			(-128,-128),
			(127,127);
	)";
	rtnVal_i = DbCmd->ExecuteNonQuery();
	CHECK(1 == rtnVal_i);

	adapter = gcnew OdbcDataAdapter("select * from sti;", Conn);
	dt = gcnew DataTable();
	old_buf = std::cout.rdbuf(ss.rdbuf());
	adapter->Fill(dt);
	std::cout.rdbuf(old_buf);

	CHECK(dt->Rows->Count == 2);
	CHECK(dt->Columns->Count == 2);
	CHECK(toss(dt->Columns[0]->ColumnName) == "sti");
	CHECK(toss(dt->Columns[1]->ColumnName) == "i1");

	// expected "System.Byte" but got "System.Int16"
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.Int16");
	CHECK(toss(dt->Columns[1]->DataType->ToString()) == "System.Int16");

	rtnVal_i16 = static_cast<INT16>(dt->Rows[0]->ItemArray[0]);
	CHECK(rtnVal_i16 == -128);
	rtnVal_i16 = static_cast<INT16>(dt->Rows[0]->ItemArray[1]);
	CHECK(rtnVal_i16 == -128);

	rtnVal_i16 = static_cast<INT16>(dt->Rows[1]->ItemArray[0]);
	CHECK(rtnVal_i16 == 127);
	rtnVal_i16 = static_cast<INT16>(dt->Rows[1]->ItemArray[1]);
	CHECK(rtnVal_i16 == 127);

	// ERROR [42000] ODBC_DuckDB->PrepareStmt
	// Conversion Error: Type INT32 with value -129 can't be cast because the value
	// is out of range for the destination type INT8
	DbCmd->CommandText = R"(
		INSERT INTO
			sti
		VALUES
			(-129,-129);
	)";
	try {
		DbCmd->ExecuteNonQuery();
	} catch (OdbcException ^ ex) {
		CHECK_THAT(toss(ex->Message), Catch::Matchers::Contains("is out of range for the destination type INT8"));
	}

	/////////////////////////////////////////////////////////////////
	// UBIGINT	 								unsigned eight-byte integer
	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE ubi(
			ubi UBIGINT
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		INSERT INTO
			ubi
		VALUES
			(18446744073709551615);
	)";
	rtnVal_i = DbCmd->ExecuteNonQuery();
	CHECK(1 == rtnVal_i);

	adapter = gcnew OdbcDataAdapter("select * from ubi;", Conn);
	dt = gcnew DataTable();
	old_buf = std::cout.rdbuf(ss.rdbuf());
	adapter->Fill(dt);
	std::cout.rdbuf(old_buf);

	CHECK(dt->Rows->Count == 1);
	CHECK(dt->Columns->Count == 1);
	CHECK(toss(dt->Columns[0]->ColumnName) == "ubi");

	// expecting UInt64 but received Decimal (floating point number)
	// CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.UInt64");
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.Decimal");

	rtnVal_dec = static_cast<Decimal>(dt->Rows[0]->ItemArray[0]);
	CHECK(rtnVal_dec == 18446744073709551615UL);

	/////////////////////////////////////////////////////////////////
	// UINTEGER	 								unsigned four-byte integer
	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE uint(
			ui UINTEGER
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		INSERT INTO
			uint
		VALUES
			(4294967295);
	)";
	rtnVal_i = DbCmd->ExecuteNonQuery();
	CHECK(1 == rtnVal_i);

	adapter = gcnew OdbcDataAdapter("select * from uint;", Conn);
	dt = gcnew DataTable();
	old_buf = std::cout.rdbuf(ss.rdbuf());
	adapter->Fill(dt);
	std::cout.rdbuf(old_buf);

	CHECK(dt->Rows->Count == 1);
	CHECK(dt->Columns->Count == 1);
	CHECK(toss(dt->Columns[0]->ColumnName) == "ui");
	// expecting UInt32 but get Int64
	// CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.UInt32");
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.Int64");

	rtnVal_i64 = static_cast<Int64>(dt->Rows[0]->ItemArray[0]);
	CHECK(rtnVal_i64 == 4294967295);

	/////////////////////////////////////////////////////////////////
	// USMALLINT	 								unsigned two-byte integer
	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE ui2b(
			usi USMALLINT
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		INSERT INTO
			ui2b
		VALUES
			(65535);
	)";
	rtnVal_i = DbCmd->ExecuteNonQuery();
	CHECK(1 == rtnVal_i);

	adapter = gcnew OdbcDataAdapter("select * from ui2b;", Conn);
	dt = gcnew DataTable();
	old_buf = std::cout.rdbuf(ss.rdbuf());
	adapter->Fill(dt);
	std::cout.rdbuf(old_buf);

	CHECK(dt->Rows->Count == 1);
	CHECK(dt->Columns->Count == 1);
	CHECK(toss(dt->Columns[0]->ColumnName) == "usi");
	// expecting UInt16 but get Int32
	// CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.UInt16");
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.Int32");

	rtnVal_i32 = static_cast<Int32>(dt->Rows[0]->ItemArray[0]);
	CHECK(rtnVal_i32 == 65535);

	/////////////////////////////////////////////////////////////////
	// UTINYINT	 								unsigned one-byte integer
	DbCmd->CommandText = R"(
		CREATE OR REPLACE TABLE usti(
			usti UTINYINT
		);
	)";
	CHECK(-1 == DbCmd->ExecuteNonQuery());

	DbCmd->CommandText = R"(
		INSERT INTO
			usti
		VALUES
			(255);
	)";
	rtnVal_i = DbCmd->ExecuteNonQuery();
	CHECK(1 == rtnVal_i);

	adapter = gcnew OdbcDataAdapter("select * from usti;", Conn);
	dt = gcnew DataTable();
	old_buf = std::cout.rdbuf(ss.rdbuf());
	adapter->Fill(dt);
	std::cout.rdbuf(old_buf);

	CHECK(dt->Rows->Count == 1);
	CHECK(dt->Columns->Count == 1);
	CHECK(toss(dt->Columns[0]->ColumnName) == "usti");
	CHECK(toss(dt->Columns[0]->DataType->ToString()) == "System.Byte");
	rtnVal_ui8 = static_cast<UINT8>(dt->Rows[0]->ItemArray[0]);
	CHECK(rtnVal_ui8 == 255);

	/////////////////////////////////////////////////////////////////
	// UUID	 									UUID data type
	try {
		DbCmd->CommandText = R"(
			CREATE OR REPLACE TABLE uuid(
				id UUID
			);
		)";
		CHECK(-1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
			INSERT INTO
				uuid
			VALUES
				(uuid());
		)";
		rtnVal_i = DbCmd->ExecuteNonQuery();
		CHECK(1 == rtnVal_i);
		adapter = gcnew OdbcDataAdapter("select * from uuid;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		try {
			adapter->Fill(dt);
		} catch (Exception ^ ex) {
				CHECK_THAT(toss(ex->Message),
						   Catch::Matchers::Contains("Unknown SQL type"));
		}
		std::cout.rdbuf(old_buf);
	} catch (OdbcException ^ ex) {
		std::string s;
		for (int i = 0; i < ex->Errors->Count; i++) {
			s += "Index #" + toss(i.ToString()) + "\n";
			s += "Message: " + toss(ex->Errors[i]->Message) + "\n";
			s += "Native: " + toss(ex->Errors[i]->NativeError.ToString()) + "\n" ;
			s += "Source: " + toss(ex->Errors[i]->Source) + "\n";
			s += "SQL: " + toss(ex->Errors[i]->SQLState) + "\n";
		}
		INFO("UUID OdbcException[" << __LINE__ << "]: {" << s << "}");
		CHECK(false);
	} catch (Exception ^ ex) {
		INFO("UUID Exception[" << __LINE__ << "]: {" << toss(ex->Message) << "}");
		CHECK(false);
	}

	/////////////////////////////////////////////////////////////////
	// VARCHAR		CHAR, BPCHAR, TEXT, STRING	variable-length character string


	if (Conn != nullptr) {
		delete Conn;
	}
	
}
