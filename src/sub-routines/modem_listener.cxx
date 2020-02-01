#include "modem_listener.hpp"
#include "../formatters/helpers.hpp"
#include "../sys_calls/sys_calls.hpp"
#include <mutex>

using namespace std;

std::mutex mutex;
//class Modems
Modems::Modems() {}

void Modems::__INIT__() {
	string list_of_modem_indexes = sys_calls::terminal_stdout("../../scripts/modem_information_extraction.sh list");
	//logger::logger(__FUNCTION__, list_of_modem_indexes );
	vector<string> modem_indexes = helpers::split(list_of_modem_indexes, '\n', true);
	
	for(auto index : modem_indexes) {
		//logger::logger(__FUNCTION__, "working with index: " + index );
		index = helpers::remove_char( index, ' ', 'E');
		string modem_information = sys_calls::terminal_stdout("../../scripts/modem_information_extraction.sh extract " + index );
		vector<string> ln_modem_information = helpers::split(modem_information, '\n', true);

		Modem modem;
		modem.setIndex( index );
		for(auto ln : ln_modem_information) {
			//logger::logger(__FUNCTION__, "line: " + ln);
			vector<string> component = helpers::split(ln, ':', true);
			if((component.size() != 2 or component[1].empty())) {
				logger::logger(__FUNCTION__, "Incomplete data for modem", "stderr");
				continue;
			}
			else if(component[0] == "equipment_id") modem.setIMEI( component[1]);
			else if(component[0] == "operator_name") {
				//logger::logger(__FUNCTION__, "Setting ISP: " + component[1]);
				modem.setISP( component[1]);
			}
		}
		if(modem) {
			//logger::logger(__FUNCTION__, "Adding modem to list");
			this->modemCollection.push_back( modem );
		}
	}
	//logger::logger(__FUNCTION__, "Exited..");
}
vector<string> Modems::getAllIndexes() {
	vector<string> list;
	for(auto modem : this->modemCollection ) {
		list.push_back( modem.getIndex() );
	}
	return list;
}

vector<string> Modems::getAllISP() {
	vector<string> list;
	for(auto modem : this->modemCollection) {
		list.push_back( modem.getISP() );
	}
	return list;
}

vector<string> Modems::getAllIMEI() {
	vector<string> list;
	for(auto modem: this->modemCollection) {
		list.push_back( modem.getIMEI() );
	}
	return list;
}

vector<Modem> Modems::getAllModems() {
	return this->modemCollection;
}

bool Modems::start( Modem modem ) {}
