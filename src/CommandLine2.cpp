#include "CommandLine.h"

#include <sstream>
#include <getopt.h>

using namespace std;
using namespace ys;


int OptionSetting::_count = 0;

bool CommandLine::_checkRefCount( const std::string &opt ){
	int ref_num = _options_value[opt].second;
	if( ref_num != 0 && ref_num != 1 ){
		cerr<< "not setting option > " << opt <<endl;
		return false;
	}
	return true;

}

void CommandLine::addOption( const std::string &opt, bool has_value, char alias ){
	_options.addOption( opt, has_value, alias );
}

char CommandLine::getChar( const std::string &opt ){
	if( _checkRefCount(opt) ) return 0;

	char tmp;
	stringstream sstr(_options_value[opt].first);
	sstr>> tmp;
	return tmp;
}
unsigned char CommandLine::getUChar( const std::string &opt ){
	if( _checkRefCount(opt) ) return 0;

	unsigned char tmp;
	stringstream sstr(_options_value[opt].first);
	sstr>> tmp;
	return tmp;
}
int CommandLine::getInt( const std::string &opt ){
	if( _checkRefCount(opt) ) return 0;

	int tmp;
	stringstream sstr(_options_value[opt].first);
	sstr>> tmp;
	return tmp;
}
float CommandLine::getFloat( const std::string &opt ){
	if( _checkRefCount(opt) ) return 0;

	float tmp;
	stringstream sstr(_options_value[opt].first);
	sstr>> tmp;
	return tmp;
}
double CommandLine::getDouble( const std::string &opt ){
	if( _checkRefCount(opt) ) return 0;

	double tmp;
	stringstream sstr(_options_value[opt].first);
	sstr>> tmp;
	return tmp;
}
string& CommandLine::getString( const std::string &opt ){
	if( _checkRefCount(opt) ) return 0;

	return _options_value[opt].first;
}

void CommandLine::parse( int argc, char **argv ){

	option *long_opts = new option[ _options_setting.size() + 1 ];
	int count = 0;
	string opt_list;

	for( OptionSettingAliasMap::iterator itr = _options_setting.begin();
			itr != _options_setting.end(); itr++){

		OptionSetting tmp_opt = itr->first;

		//getopt_longの設定
		option opt = { tmp_opt.name.c_str(), tmp_opt.has_value, NULL, tmp_opt.alias };
		long_opts[ count ] = opt;
		if( 0 <= tmp_opt.alias && tmp_opt.alias <= 255 )
			opt_list += tmp_opt.alias;
		if( tmp_opt.has_value )
			opt_list += ':';

		count++;
	}

	long_opts[_options.size()]  = {0,0,0,0};

	int opt_index = 0;
	while( (int result = getopt_long(argc, argv, opt_list.c_str(), long_opts, &opt_index)) != -1 ){
		OptionSetting tmp_opt = _options[(char)result];
		_options_value[tmp_opt._name] = pair<string,int>(optarg,0);
	}


	delete long_opts;
}
