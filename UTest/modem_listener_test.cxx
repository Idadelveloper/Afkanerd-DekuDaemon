// Custom header files
#include "../src/sub-routines/modem.cxx"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

MySQL mysqlConnection;

string logger::show_state = "TESTING";

std::string imei = "test_imei";
std::string isp = "test_isp";
std::string type = "test_type";
std::string _index = "test_index";

std::string DIR_ISP = "/home/sherlock/Deku/Utest/isp";
std::string DIR_REQUEST_FILE = "/home/sherlock/Deku/Utest";
std::string DIR_SUCCESS = "/home/sherlock/Deku/200";
std::string DIR_ERROR = "/home/sherlock/Deku/404";
std::string DIR_SCRIPTS = "../scripts";
std::string STD_NAME_REQUEST_FILE = "request_file.txt";
std::string MYSQL_SERVER = "";
std::string MYSQL_USER = "";
std::string MYSQL_PASSWORD = "";
std::string MYSQL_DATABASE = "";

std::map<string,string> configs {
	{"DIR_ISP", DIR_ISP},
	{"DIR_ERROR", DIR_ERROR},
	{"DIR_SUCCESS", DIR_SUCCESS},
	{"DIR_SCRIPTS", DIR_SCRIPTS},
	{"DIR_REQUEST_FILE", DIR_REQUEST_FILE},
	{"STD_NAME_REQUEST_FILE", STD_NAME_REQUEST_FILE},
	{"MYSQL_SERVER", MYSQL_SERVER},
	{"MYSQL_USER", MYSQL_USER},
	{"MYSQL_PASSWORD", MYSQL_PASSWORD},
	{"MYSQL_DATABASE", MYSQL_DATABASE}	
};

TEST_GROUP( Modem ) {};

TEST( Modems, get_all_indexes ) {
}

TEST( Modems, get_all_isp ) {
}

TEST( Modems, get_all_imei ) {
}

TEST( Modems, find_modem_type ) {
}

TEST( Modems, get_all_modems ) {
}

TEST( Modems, get_available_modems ) {
}

TEST( Modems, get_modem_details ) {
}

TEST( Integration, db_insert_modems ) {
	// db_insert_modems( map<string,string> modems )
	Modems modems( configs );

	bool insert_into_db_state = modems.db_insert_modems( modems );
	
	CHECK( insert_into_db_state == true );
}

int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
