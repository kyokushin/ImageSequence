#ifndef CommandLine_h
#define CommandLine_h

#include <string>
#include <map>

namespace ys {
	class OptionSetting{
		public:
			OptionSetting( std::string name, bool has_value = true, char alias = -1)
				:_name(name),_has_value(has_value) {
					if( _alias == -1 ){
						this->_alias = _count;
						_count++;
					}
					else {
						this->alias = alias;
					}
				}
		std::string _name;
		bool _has_value;
		char _alias;
		static int _count;
	}
	class OptionSettingAliasMap : std::map<char, OptionSetting> {
		public:
			void addOption( std::string name, bool has_value = true, char alias = -1 ){
				OptionSetting opt( name, has_value, alias );
				(*this)[opt._alias] = opt;
			}
	}

	class CommandLine {
		public:
			CommandLine();
			void setOptionSetting( OptionsSetting &setting );
			void addOption( const std::string &opt, bool has_value = true, char alias = -1 );
			unsigned char getUChar( const std::string &opt );
			char getChar( const std::string &opt );
			int getInt( const std::string &opt );
			float getFloat( const std::string &opt );
			double getDouble( const std::string &opt );
			std::string& getString( const std::string &opt );
			void parse( int argc, char**argv );

		private:
			OptionSettingAliasMap _options;
			//first:OptionSettingAliasMap,
			//second.first:option_name
			//second.second:reference_count
			std::map<std::string, std::pair< std::string, int> > _options_value;
			bool _checkRefCount( const std::string &opt );
	}

}
#endif
