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

TEST_CASE("System.Data.ODBC", "test .NET OdbcDataAdapter functionality") {

	OdbcConnection ^ Conn = nullptr;
	try {
		int rtnVal_i = 0;
		Int16 rtnVal_i16 = 0;
		Int32 rtnVal_i32 = 0;
		Int64 rtnVal_i64 = 0;
		UInt16 rtnVal_ui16 = 0;
		UInt32 rtnVal_ui32 = 0;
		UInt64 rtnVal_ui64 = 0;
		Decimal rtnVal_dec = 0;
		double rtnVal_d = 0;
		System::String ^ connStr = "Driver=DuckDB Driver;Database=test.duckdb;";
		Conn = gcnew OdbcConnection(connStr);
		Conn->Open();

		OdbcCommand ^ DbCmd = Conn->CreateCommand();


		//https://duckdb.org/docs/sql/data_types/overview#nested--composite-types
		//LIST		An ordered sequence of data values of the same type.
		//			Each row must have the same data type within each LIST, but can have any number of elements.
		//			[1, 2, 3]	INT[ ]
		//STRUCT	A dictionary of multiple named values, where each key is a string, but the value can be a different type for each key.
		//			Each row must have the same keys.
		//			{'i': 42, 'j': 'a'}	STRUCT(i INT, j VARCHAR)
		//MAP		A dictionary of multiple named values, each key having the same type and each value having the same type.
		//			Keys and values can be any type and can be different types from one another.
		//			Rows may have different keys.
		//			map([1, 2], ['a', 'b'])	MAP(INT, VARCHAR)
		//UNION		A union of multiple alternative data types, storing one of them in each value at a time.
		//			A union also contains a discriminator “tag” value to inspect and access the currently set member type.
		//			Rows may be set to different member types of the union.	union_value(num := 2)
		//			UNION(num INT, text VARCHAR)

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
		REQUIRE(-1 == DbCmd->ExecuteNonQuery());

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
		REQUIRE(1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
			select count(1) from weather;
		)";
		Int64 weatherCount = static_cast<Int64>(DbCmd->ExecuteScalar());
		REQUIRE(weatherCount == 1);

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

		REQUIRE(dt->Rows->Count == 0);
		REQUIRE(dt->Columns->Count == 4);
		REQUIRE(toss(dt->Columns[0]->ColumnName) == "city");
		REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.String");
		REQUIRE(toss(dt->Columns[1]->ColumnName) == "temp_lo");
		REQUIRE(toss(dt->Columns[1]->DataType->ToString()) == "System.Int32");
		REQUIRE(toss(dt->Columns[2]->ColumnName) == "prcp");
		REQUIRE(toss(dt->Columns[2]->DataType->ToString()) == "System.Double");
		REQUIRE(toss(dt->Columns[3]->ColumnName) == "date");
		REQUIRE(toss(dt->Columns[3]->DataType->ToString()) == "System.DateTime");

		/////////////////////////////////////////////////////////////////
		// Fill() DataTable directly
		delete dt;
		dt = gcnew DataTable();
		adapter->Fill(dt);
		REQUIRE(dt->Rows->Count == 1);
		REQUIRE(dt->Columns->Count == 4);
		REQUIRE(toss(dt->Columns[0]->ColumnName) == "city");
		REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.String");
		REQUIRE(toss(dt->Columns[1]->ColumnName) == "temp_lo");
		REQUIRE(toss(dt->Columns[1]->DataType->ToString()) == "System.Int32");
		REQUIRE(toss(dt->Columns[2]->ColumnName) == "prcp");
		REQUIRE(toss(dt->Columns[2]->DataType->ToString()) == "System.Double");
		REQUIRE(toss(dt->Columns[3]->ColumnName) == "date");
		REQUIRE(toss(dt->Columns[3]->DataType->ToString()) == "System.DateTime");
		REQUIRE(toss(dt->Rows[0]->ItemArray[0]->ToString()) == "San Francisco");
		REQUIRE(toss(dt->Rows[0]->ItemArray[1]->ToString()) == "46");
		REQUIRE(toss(dt->Rows[0]->ItemArray[2]->ToString()) == "0.25");
		REQUIRE(toss(dt->Rows[0]->ItemArray[3]->ToString()) == "11/27/1994 12:00:00 AM");

		/////////////////////////////////////////////////////////////////
        // Fill() Dataset then look at the Datatable
		delete dt;
		dt = gcnew DataTable();
        DataSet ^ ds = gcnew DataSet();
        adapter->Fill(ds);
        dt = ds->Tables[0];
		REQUIRE(dt->Rows->Count == 1);
		REQUIRE(dt->Columns->Count == 4);
		REQUIRE(toss(dt->Columns[0]->ColumnName) == "city");
		REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.String");
		REQUIRE(toss(dt->Columns[1]->ColumnName) == "temp_lo");
		REQUIRE(toss(dt->Columns[1]->DataType->ToString()) == "System.Int32");
		REQUIRE(toss(dt->Columns[2]->ColumnName) == "prcp");
		REQUIRE(toss(dt->Columns[2]->DataType->ToString()) == "System.Double");
		REQUIRE(toss(dt->Columns[3]->ColumnName) == "date");
		REQUIRE(toss(dt->Columns[3]->DataType->ToString()) == "System.DateTime");
		REQUIRE(toss(dt->Rows[0]->ItemArray[0]->ToString()) == "San Francisco");
		REQUIRE(toss(dt->Rows[0]->ItemArray[1]->ToString()) == "46");
		REQUIRE(toss(dt->Rows[0]->ItemArray[2]->ToString()) == "0.25");
		REQUIRE(toss(dt->Rows[0]->ItemArray[3]->ToString()) == "11/27/1994 12:00:00 AM");
        
		delete dt;
		delete adapter;

		DbCmd->CommandText = R"(
			CREATE OR REPLACE TABLE cities(
				name VARCHAR,
				lat DECIMAL,
				lon DECIMAL
			);
		)";
		REQUIRE(-1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
			INSERT INTO
				cities
			VALUES(
				'San Francisco',
				-194.0,
				53.0
			);
		)";
		REQUIRE(1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
			select count(1) from cities;
		)";
		Int64 citiesCount = static_cast<Int64>(DbCmd->ExecuteScalar());
		REQUIRE(1 == citiesCount);

		/////////////////////////////////////////////////////////////////
		// look at how ODBC handles all of the DuckDB supported data types
		// https://duckdb.org/docs/sql/data_types/overview#general-purpose-data-types
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
	    REQUIRE(-1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
		    INSERT INTO biginttest VALUES
				(-9223372036854775808, -9223372036854775808, -9223372036854775808),
				(9223372036854775807,9223372036854775807,9223372036854775807),
				(0,0,0);
		)";
		REQUIRE(1 == DbCmd->ExecuteNonQuery());

		adapter = gcnew OdbcDataAdapter("select * from biginttest;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		adapter->Fill(dt);
		std::cout.rdbuf(old_buf);

		REQUIRE(dt->Rows->Count == 3);
		REQUIRE(dt->Columns->Count == 3);
		REQUIRE(toss(dt->Columns[0]->ColumnName) == "bi");
		REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.Int64");
		REQUIRE(toss(dt->Columns[1]->ColumnName) == "i8");
		REQUIRE(toss(dt->Columns[1]->DataType->ToString()) == "System.Int64");
		REQUIRE(toss(dt->Columns[2]->ColumnName) == "l");
		REQUIRE(toss(dt->Columns[2]->DataType->ToString()) == "System.Int64");

		delete dt;
		delete adapter;

		/////////////////////////////////////////////////////////////////
		// BIT			BITSTRING					string of 1’s and 0’s
		// BOOLEAN		BOOL, LOGICAL				logical boolean (true/false)
		// BLOB			BYTEA, BINARY, VARBINARY	variable-length binary data
		// DATE	 									calendar date (year, month day)
		// DECIMAL(prec, scale)	 					fixed-precision number with the given width (precision) and scale

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
		REQUIRE(-1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
			INSERT INTO
				doubletest
			VALUES
				(2.2250738585072014e-308,2.2250738585072014e-308,1.0, 123456789012345.123, 123456789012345678901.123),
				(1.7976931348623158e+308,1.7976931348623158e+308,1.0, 123456789012345.123, 123456789012345678901.123),
				('-Infinity','-Infinity',1.0, 123456789012345.123, 123456789012345678901.123),
				('Infinity','Infinity',1.0, 123456789012345.123, 123456789012345678901.123),
				('NaN','NaN',1.0, 123456789012345.123, 123456789012345678901.123),
				(0,0,1.0, 123456789012345.123, 123456789012345678901.123);
		)";
		rtnVal_i = DbCmd->ExecuteNonQuery();
		REQUIRE(1 == rtnVal_i);

		adapter = gcnew OdbcDataAdapter("select * from doubletest;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		adapter->Fill(dt);
		std::cout.rdbuf(old_buf);

		REQUIRE(dt->Rows->Count == 6);
		REQUIRE(dt->Columns->Count == 5);
		REQUIRE(toss(dt->Columns[0]->ColumnName) == "dbl");
		REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.Double");
		REQUIRE(toss(dt->Columns[1]->ColumnName) == "f8");
		REQUIRE(toss(dt->Columns[1]->DataType->ToString()) == "System.Double");
		REQUIRE(toss(dt->Columns[2]->ColumnName) == "num");
		REQUIRE(toss(dt->Columns[2]->DataType->ToString()) == "System.Double");
		REQUIRE(toss(dt->Columns[3]->ColumnName) == "dec");
		REQUIRE(toss(dt->Columns[3]->DataType->ToString()) == "System.Double");
		REQUIRE(toss(dt->Columns[4]->ColumnName) == "bigdec");
		REQUIRE(toss(dt->Columns[4]->DataType->ToString()) == "System.Double");

		rtnVal_d = static_cast<double>(dt->Rows[0]->ItemArray[0]);
		REQUIRE_THAT(rtnVal_d, Catch::Matchers::WithinAbs(2.2250738585072014e-308, 1.0));
		rtnVal_d = static_cast<double>(dt->Rows[0]->ItemArray[1]);
		REQUIRE_THAT(rtnVal_d, Catch::Matchers::WithinAbs(2.2250738585072014e-308, 1.0));

		rtnVal_d = static_cast<double>(dt->Rows[1]->ItemArray[0]);
		REQUIRE_THAT(rtnVal_d, Catch::Matchers::WithinAbs(1.7976931348623158e+308, 1.0));
		rtnVal_d = static_cast<double>(dt->Rows[1]->ItemArray[1]);
		REQUIRE_THAT(rtnVal_d, Catch::Matchers::WithinAbs(1.7976931348623158e+308, 1.0));

		rtnVal_d = static_cast<double>(dt->Rows[0]->ItemArray[3]);
		REQUIRE_THAT(rtnVal_d, Catch::Matchers::WithinAbs(123456789012345.123, 0.001));

		// ODBC only has double, so loss of precision makes this the best we can do...
		rtnVal_d = static_cast<double>(dt->Rows[0]->ItemArray[4]);
		REQUIRE_THAT(rtnVal_d, Catch::Matchers::WithinAbs(123456789012345683968.0, 1.0));

		delete dt;
		delete adapter;

		/////////////////////////////////////////////////////////////////
		// HUGEINT	 								signed sixteen-byte integer
		DbCmd->CommandText = R"(
			CREATE OR REPLACE TABLE huge_int(
				hi HUGEINT
			);
		)";
		REQUIRE(-1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
			INSERT INTO
				huge_int
			VALUES
				(-170141183460469231731687303715884105727),
				(170141183460469231731687303715884105727);
		)";
		rtnVal_i = DbCmd->ExecuteNonQuery();
		REQUIRE(1 == rtnVal_i);

		adapter = gcnew OdbcDataAdapter("select * from huge_int;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		adapter->Fill(dt);
		std::cout.rdbuf(old_buf);

		REQUIRE(dt->Rows->Count == 2);
		REQUIRE(dt->Columns->Count == 1);
		REQUIRE(toss(dt->Columns[0]->ColumnName) == "hi");
		//INFO(toss(dt->Columns[0]->DataType->ToString()));

		// ODBC largest int type is SQL_BIGINT which is 8 bytes, 
        // so huge_int converted to double with loss of precision
		// since it is a double, no way to compare
		REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.Double");

		/////////////////////////////////////////////////////////////////
		// INTEGER		INT4, INT, SIGNED			signed four-byte integer
		DbCmd->CommandText = R"(
			CREATE OR REPLACE TABLE int4b(
				i4 INT4,
				i INT,
				s SIGNED
			);
		)";
		REQUIRE(-1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
			INSERT INTO
				int4b
			VALUES
				(-2147483648,-2147483648,-2147483648),
				(2147483647,2147483647,2147483647);
		)";
		rtnVal_i = DbCmd->ExecuteNonQuery();
		REQUIRE(1 == rtnVal_i);

		adapter = gcnew OdbcDataAdapter("select * from int4b;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		adapter->Fill(dt);
		std::cout.rdbuf(old_buf);

		REQUIRE(dt->Rows->Count == 2);
		REQUIRE(dt->Columns->Count == 3);
		REQUIRE(toss(dt->Columns[0]->ColumnName) == "i4");
		REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.Int32");
		REQUIRE(toss(dt->Columns[1]->ColumnName) == "i");
		REQUIRE(toss(dt->Columns[1]->DataType->ToString()) == "System.Int32");
		REQUIRE(toss(dt->Columns[2]->ColumnName) == "s");
		REQUIRE(toss(dt->Columns[2]->DataType->ToString()) == "System.Int32");

		rtnVal_i32 = static_cast<Int32>(dt->Rows[0]->ItemArray[0]);
		REQUIRE(rtnVal_i32 == -2147483648);
		rtnVal_i32 = static_cast<Int32>(dt->Rows[0]->ItemArray[1]);
		REQUIRE(rtnVal_i32 == -2147483648);
		rtnVal_i32 = static_cast<Int32>(dt->Rows[0]->ItemArray[2]);
		REQUIRE(rtnVal_i32 == -2147483648);

		rtnVal_i32 = static_cast<Int32>(dt->Rows[1]->ItemArray[0]);
		REQUIRE(rtnVal_i32 == 2147483647);
		rtnVal_i32 = static_cast<Int32>(dt->Rows[1]->ItemArray[1]);
		REQUIRE(rtnVal_i32 == 2147483647);
		rtnVal_i32 = static_cast<Int32>(dt->Rows[1]->ItemArray[2]);
		REQUIRE(rtnVal_i32 == 2147483647);

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
		REQUIRE(-1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
			INSERT INTO
				spfp4
			VALUES
				(1.175494351e-38,1.175494351e-38),
				(3.402823466e+38,3.402823466e+38);
		)";
		rtnVal_i = DbCmd->ExecuteNonQuery();
		REQUIRE(1 == rtnVal_i);

		adapter = gcnew OdbcDataAdapter("select * from spfp4;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		adapter->Fill(dt);
		std::cout.rdbuf(old_buf);

		REQUIRE(dt->Rows->Count == 2);
		REQUIRE(dt->Columns->Count == 2);
		REQUIRE(toss(dt->Columns[0]->ColumnName) == "f4");
		REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.Double");
		REQUIRE(toss(dt->Columns[1]->ColumnName) == "f");
		REQUIRE(toss(dt->Columns[1]->DataType->ToString()) == "System.Double");

		rtnVal_d = static_cast<double>(dt->Rows[0]->ItemArray[0]);
		REQUIRE_THAT(rtnVal_d, Catch::Matchers::WithinAbs(1.175494351e-38F, 1.0));
		rtnVal_d = static_cast<double>(dt->Rows[0]->ItemArray[1]);
		REQUIRE_THAT(rtnVal_d, Catch::Matchers::WithinAbs(1.175494351e-38F, 1.0));

		rtnVal_d = static_cast<double>(dt->Rows[1]->ItemArray[0]);
		REQUIRE_THAT(rtnVal_d, Catch::Matchers::WithinAbs(3.402823466e+38F, 1.0));
		rtnVal_d = static_cast<double>(dt->Rows[1]->ItemArray[1]);
		REQUIRE_THAT(rtnVal_d, Catch::Matchers::WithinAbs(3.402823466e+38F, 1.0));

		/////////////////////////////////////////////////////////////////
		// SMALLINT		INT2, SHORT					signed two-byte integer
		DbCmd->CommandText = R"(
			CREATE OR REPLACE TABLE int2b(
				i2 INT2,
				s SHORT
			);
		)";
		REQUIRE(-1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
			INSERT INTO
				int2b
			VALUES
				(-32768,-32768),
				(32767,32767);
		)";
		rtnVal_i = DbCmd->ExecuteNonQuery();
		REQUIRE(1 == rtnVal_i);

		adapter = gcnew OdbcDataAdapter("select * from int2b;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		adapter->Fill(dt);
		std::cout.rdbuf(old_buf);

		REQUIRE(dt->Rows->Count == 2);
		REQUIRE(dt->Columns->Count == 2);
		REQUIRE(toss(dt->Columns[0]->ColumnName) == "i2");
		REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.Int16");
		REQUIRE(toss(dt->Columns[1]->ColumnName) == "s");
		REQUIRE(toss(dt->Columns[1]->DataType->ToString()) == "System.Int16");

		rtnVal_i16 = static_cast<Int16>(dt->Rows[0]->ItemArray[0]);
		REQUIRE(rtnVal_i16 == -32768);
		rtnVal_i16 = static_cast<Int16>(dt->Rows[0]->ItemArray[1]);
		REQUIRE(rtnVal_i16 == -32768);

		rtnVal_i16 = static_cast<Int16>(dt->Rows[1]->ItemArray[0]);
		REQUIRE(rtnVal_i16 == 32767);
		rtnVal_i16 = static_cast<Int16>(dt->Rows[1]->ItemArray[1]);
		REQUIRE(rtnVal_i16 == 32767);


		// TIME	 									time of day (no time zone)
		// TIMESTAMP		DATETIME					combination of time and date
		// TIMESTAMP WITH TIME ZONE	TIMESTAMPTZ		combination of time and date that uses the current time zone

		/////////////////////////////////////////////////////////////////
		// TINYINT		INT1						signed one-byte integer
		
		/////////////////////////////////////////////////////////////////
		// UBIGINT	 								unsigned eight-byte integer
		DbCmd->CommandText = R"(
			CREATE OR REPLACE TABLE ubi(
				ubi UBIGINT
			);
		)";
		REQUIRE(-1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
			INSERT INTO
				ubi
			VALUES
				(18446744073709551615);
		)";
		rtnVal_i = DbCmd->ExecuteNonQuery();
		REQUIRE(1 == rtnVal_i);

		adapter = gcnew OdbcDataAdapter("select * from ubi;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		adapter->Fill(dt);
		std::cout.rdbuf(old_buf);

		REQUIRE(dt->Rows->Count == 1);
		REQUIRE(dt->Columns->Count == 1);
		REQUIRE(toss(dt->Columns[0]->ColumnName) == "ubi");

		// expecting UInt64 but received Decimal (floating point number)
		//REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.UInt64");
		REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.Decimal");

		rtnVal_dec = static_cast<Decimal>(dt->Rows[0]->ItemArray[0]);
		REQUIRE(rtnVal_dec == 18446744073709551615UL);

		/////////////////////////////////////////////////////////////////
		// UINTEGER	 								unsigned four-byte integer
		DbCmd->CommandText = R"(
			CREATE OR REPLACE TABLE uint(
				ui UINTEGER
			);
		)";
		REQUIRE(-1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
			INSERT INTO
				uint
			VALUES
				(4294967295);
		)";
		rtnVal_i = DbCmd->ExecuteNonQuery();
		REQUIRE(1 == rtnVal_i);

		adapter = gcnew OdbcDataAdapter("select * from uint;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		adapter->Fill(dt);
		std::cout.rdbuf(old_buf);

		REQUIRE(dt->Rows->Count == 1);
		REQUIRE(dt->Columns->Count == 1);
		REQUIRE(toss(dt->Columns[0]->ColumnName) == "ui");
		// expecting UInt32 but get Int64
		//REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.UInt32");
		REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.Int64");

		rtnVal_i64 = static_cast<Int64>(dt->Rows[0]->ItemArray[0]);
		REQUIRE(rtnVal_i64 == 4294967295);

		/////////////////////////////////////////////////////////////////
		// USMALLINT	 								unsigned two-byte integer
		DbCmd->CommandText = R"(
			CREATE OR REPLACE TABLE ui2b(
				usi USMALLINT
			);
		)";
		REQUIRE(-1 == DbCmd->ExecuteNonQuery());

		DbCmd->CommandText = R"(
			INSERT INTO
				ui2b
			VALUES
				(65535);
		)";
		rtnVal_i = DbCmd->ExecuteNonQuery();
		REQUIRE(1 == rtnVal_i);

		adapter = gcnew OdbcDataAdapter("select * from ui2b;", Conn);
		dt = gcnew DataTable();
		old_buf = std::cout.rdbuf(ss.rdbuf());
		adapter->Fill(dt);
		std::cout.rdbuf(old_buf);

		REQUIRE(dt->Rows->Count == 1);
		REQUIRE(dt->Columns->Count == 1);
		REQUIRE(toss(dt->Columns[0]->ColumnName) == "usi");
		// expecting UInt16 but get Int32
		// REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.UInt16");
		REQUIRE(toss(dt->Columns[0]->DataType->ToString()) == "System.Int32");

		rtnVal_i32 = static_cast<Int32>(dt->Rows[0]->ItemArray[0]);
		REQUIRE(rtnVal_i32 == 65535);

		/////////////////////////////////////////////////////////////////
		// UTINYINT	 								unsigned one-byte integer

		/////////////////////////////////////////////////////////////////
		// UUID	 									UUID data type

		/////////////////////////////////////////////////////////////////
		// VARCHAR		CHAR, BPCHAR, TEXT, STRING	variable-length character string



	} catch (OdbcException ^ ex) {

		FAIL("OdbcException: {" << toss(ex->Message) << "}");

	} catch (Exception ^ ex) {

		FAIL("Exception: {" << toss(ex->Message) << "}");

	} finally {

		if (Conn != nullptr) {
			delete Conn;
		}
	}
}
