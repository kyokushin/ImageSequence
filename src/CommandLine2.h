#ifndef CommandLine_h
#define CommandLine_h

#include <string>
#include <map>

namespace ys {
	class OptionSetting{
		public:
			OptionSetting(bool has_value = true, char alias = -1)
				:has_value(has_value) {
					if( alias == -1 ){
						this->alias = count;
						count++;
					}
					else {
						this->alias = alias;
					}
				}
		bool has_value;
		char alias;
		static int count;
	}
	class OptionsSetting : std::map<std::string, OptionSetting> {
		public:
			void addOption( std::string name, bool has_value = true, char alias = -1 );
			std::string name;
			bool has_value;
			char alias;
	}

	class CommandLine {
		public:
			CommandLine( OptionsSetting &setting );
			void setOptionSetting( OptionsSetting &setting );
			void addOption( const std::string &opt, bool has_value = true, char alias = 0 );
			unsigned char getUChar( const std::string &opt );
			char getChar( const std::string &opt );
			int getInt( const std::string &opt );
			float getFloat( const std::string &opt );
			double getDouble( const std::string &opt );
			std::string& getString( const std::string &opt );
			void parse( int argc, char**argv );

		private:
			OptionsSetting _options_setting;
			std::map<char, std::pair< int, std::string> > _options_value;
			bool _checkRefCount( const std::string &opt );
	}

}
#endif
