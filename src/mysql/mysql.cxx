// TODO: build test scripts to make sure this works as planned
#include "../formatters/helpers.hpp"
#include "mysql.hpp"
#include <cstring>
#include <iostream>

using namespace std;

MySQL::MySQL(string server, string user, string password, string database = "" ) {
	this->server = server;
	this->user = user;
	this->password = password;
	this->database = database;

	this->mysqlConnection = mysql_init( NULL );
}

void MySQL::setConnectionDetails( string server, string user, string password, string database = "") {
	this->server = server;
	this->user = user;
	this->password = password;
	this->database = database;
}

bool MySQL::connect() {
	this->initialized = true;
	auto mysql_connection_state_good = mysql_real_connect( this->mysqlConnection, this->server.c_str(), this->user.c_str(), this->password.c_str(), this->database.c_str(), 0, NULL, 0);

	if( !mysql_connection_state_good ) {
		const char *mysql_error_msg = mysql_error( this->mysqlConnection );
		logger::logger(__FUNCTION__, "Failed to connect to database: " + string( mysql_error_msg, strlen(mysql_error_msg)), "stderr");

		return false;
	}

	return true;
}

bool MySQL::set_database( string database ) {
	auto selected_database = mysql_select_db(this->mysqlConnection, database.c_str());

	return selected_database == 0 ? true : false;
}

bool MySQL::create_database( string database ) {
	// auto created_database = mysql_create_db(this->mysqlConnection, database.c_str());
	// TODO: Above function is deprecated!!
	
	string create_db_query = "CREATE DATABASE " + database;
	bool create_db_state = this->query( create_db_query );

	return create_db_state;
}

bool MySQL::has_database( string database ) const {
	auto list_of_databases = mysql_list_dbs(this->mysqlConnection, database.c_str() );

	if( list_of_databases == NULL ) return false;

	bool database_found = false;
	for(MYSQL_ROW mysqlRow = mysql_fetch_row( list_of_databases ); mysqlRow != NULL ; mysqlRow = mysql_fetch_row( list_of_databases ) ) {
		logger::logger(__FUNCTION__, mysqlRow[0]);
	}

	return database_found;
}

bool MySQL::delete_database( string database ) {
	//TODO: Default method is already deprecated!
	string delete_db_query = "DROP DATABASE " + database;
	auto drop_db_state = this->query( delete_db_query );
	return false;
}

string MySQL::get_server() const {
	return this->server;
}

string MySQL::get_user() const {
	return this->user;
}

string MySQL::get_password() const {
	return this->password;
}

string MySQL::get_database() const {
	return this->database;
}


bool MySQL::is_init() const {
	return this->initialized;
}

MySQL::MySQL() {
	this->mysqlConnection = mysql_init( NULL );
}

map<string,vector<string>> MySQL::get_results() {
	map<string, vector<string>> query_results;

	size_t num_fields = mysql_num_fields( this->mysqlResult );
	logger::logger(__FUNCTION__, "Number of SQL results fields: " + to_string( num_fields ));

	if( num_fields < 1 ) {
		return query_results;
	}

	MYSQL_FIELD *fields = mysql_fetch_fields( this->mysqlResult );
	for(MYSQL_ROW mysqlRow = mysql_fetch_row( this->mysqlResult ); mysqlRow != NULL ; mysqlRow = mysql_fetch_row( mysqlResult ) ) {
		auto num_of_fields = mysql_num_fields( this->mysqlResult );
		for( size_t i = 0; i< num_of_fields; ++i) {
			string row_header = fields[i].name;
			if( query_results.find( row_header ) != query_results.end() ) 
				query_results.insert(make_pair(row_header, vector<string>{}));
			query_results[row_header].push_back( mysqlRow[i] );
		}
	}
	mysql_free_result ( this->mysqlResult );

	return query_results;
}

void MySQL::set_error_message( const char* error_msg ) {
	this->error_message = string( error_msg, strlen( error_msg ) );
}

bool MySQL::query( string query ) {
	// logger::logger(__FUNCTION__, "Querying with: " + query );
	auto mysql_query_state = mysql_query( this->mysqlConnection, query.c_str() );

	if( mysql_query_state != 0 ) {
		const char *mysql_error_msg = mysql_error( this->mysqlConnection );
		this->set_error_message( mysql_error_msg );
		// logger::logger(__FUNCTION__, "Failed to query database: " + string( mysql_error_msg, strlen(mysql_error_msg)), "stderr");
		return false;
	}

	this->mysqlResult = mysql_use_result( this->mysqlConnection );
	
	/*
	if( !mysqlResult) {
		// TODO: https://dev.mysql.com/doc/refman/8.0/en/mysql-field-count.html
		logger::logger(__FUNCTION__, "Number of Affected Rows: " + to_string(mysql_affected_rows(this->mysqlConnection)));
		return query_results;
	}
	*/

	return true;
}

void MySQL::close() {
	mysql_close( this->mysqlConnection );
}

MySQL::~MySQL() {
}
