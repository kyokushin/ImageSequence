#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cv.h>
#include <highgui.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

namespace ys {
	using namespace std;
	using namespace cv;

	class ImageSequence {
		public:
			virtual ~ImageSequence(){}

			virtual bool get( Mat &image, int channel=0 ) = 0;
			virtual bool next() = 0;
			virtual int currentNum() = 0;

			virtual bool open(const string &filename) = 0;

			virtual bool isOpen() = 0;
			virtual void release() = 0;

			virtual string getSourceName() = 0;

	};

	class VideoCapture : public ImageSequence {
		public:
			VideoCapture()
				:_counter(0)
			{}
			virtual ~VideoCapture(){}

			virtual bool get( Mat &image, int channel = 0 ){
				return _cap.retrieve(image, channel);
			}
			virtual bool next(){
				if( _cap.grab()){
					++_counter;
					return true;
				}

				return false;
			}
			virtual int currentNum(){
				return _counter;
			}

			virtual bool open(const string &filename ){
				_cap.open( filename );
			}
			virtual bool isOpen(){
				return _cap.isOpened();
			}
			virtual void release(){
				_cap.release();
				_counter = 0;
			}

			virtual string getSourceName(){
				return "VideoSource";
			}

		protected:
			cv::VideoCapture _cap;
			int _counter;
	};

	class CameraCapture : public VideoCapture {
		public:
			virtual bool open( const string &filename ){
				stringstream sstr(filename);
				int device;

				sstr>> device;

				return _cap.open(device);
			}
	};

	namespace fs = boost::filesystem;
	class ImageList : public ImageSequence {
		public:


			ImageList()
				:_counter(0),_open(false)
			{}
			virtual ~ImageList(){}

			virtual bool get( Mat &image, int channel = 0){

				if( _counter == _image_list.size() ){
					return false;
				}

				Mat img = imread( _image_list[_counter], channel );
				if( img.data == NULL ){
					return false;
				}

				_counter++;
				image = img;
				return true;
			}
			virtual bool next(){
				_counter++;
			}
			virtual int currentNum(){
				return _counter;
			}

			virtual bool open(const string &filename){

				const fs::path read_path(filename);
				boost::system::error_code error;

				const bool result = fs::exists(read_path);
				if( !result || error ){
					cerr << "not exist file " <<endl;
					return false;
				}
				
				int read_num ;
				if( fs::is_directory(filename)){
					read_num = _readDir2List( filename );
				}
				else {
					read_num = _readFile2List( filename );
				}
				
				return read_num > 0;
			}

			virtual bool isOpen(){
				return _open;
			}
			virtual void release(){
				_image_list.clear();
				_counter = 0;
				_open = false;
			}

			virtual string getSourceName(){
				return "ImageList";
			}

		private:
			vector<string> _image_list;
			int _counter;
			bool _open;

			int _readDir2List(const string &name ){

				fs::path read_path(name);
				fs::directory_iterator end;
				for( fs::directory_iterator itr(read_path); itr != end; ++itr ){
					if( fs::is_directory(*itr) ) continue;

					_image_list.push_back(itr->filename());
				}

				return _image_list.size();
			}

			int _readFile2List( const string &filename ){

				ifstream ifs(filename.c_str());
				string line;
				while( ifs ){
					ifs>> line;
					fs::path read_path(line);
					boost::system::error_code error;

					const bool result = fs::exists(read_path);
					if( !result || error ){
						cerr<< "not exist \""
							<< line
							<< "\". skip." <<endl;
						continue;
					}

					_image_list.push_back( read_path.filename() );
				}
			}
	};
}
