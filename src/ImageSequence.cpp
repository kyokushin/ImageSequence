#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cv.h>
#include <highgui.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include "ImageSequence.h"

using namespace std;
using namespace cv;



bool ys::VideoCapture::get( Mat &image, int channel ){
	return _cap.retrieve(image, channel);
}
bool ys::VideoCapture::next(){
	if( _cap.grab()){
		++_counter;
		return true;
	}

	return false;
}
int ys::VideoCapture::currentNum(){
	return _counter;
}

bool ys::VideoCapture::open(const string &filename ){
	_filename = filename;
	_cap.open( filename );
}
bool ys::VideoCapture::isOpen(){
	return _cap.isOpened();
}
void ys::VideoCapture::release(){
	_cap.release();
	_counter = 0;
}

string ys::VideoCapture::getSourceName(){
	return "Video";
}

string ys::VideoCapture::getFileName(){
	return _filename;
}

bool ys::CameraCapture::open(const string& filename ){

	stringstream sstr(filename);
	int device;

	sstr>> device;

	return _cap.open(device);
}

string ys::CameraCapture::getSourceName(){
	return "Camera";
}


namespace fs = boost::filesystem;

bool ys::ImageList::get( Mat &image, int channel ){

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
bool ys::ImageList::next(){
	if( _counter + 1 >= _image_list.size() ){
		return false;
	}
	++_counter;
	return true;
	
}
int ys::ImageList::currentNum(){
	return _counter;
}

bool ys::ImageList::open(const string &filename){
	_filename = filename;

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

bool ys::ImageList::isOpen(){
	return _open;
}
void ys::ImageList::release(){
	_image_list.clear();
	_counter = 0;
	_open = false;
}

string ys::ImageList::getSourceName(){
	return "ImageList";
}

string ys::ImageList::getFileName(){
	return _filename;
}

int ys::ImageList::_readDir2List(const string &name ){

	fs::path read_path(name);
	fs::directory_iterator end;

	for( fs::directory_iterator itr(read_path); itr != end; ++itr ){
		if( fs::is_directory(*itr) ) continue;

		_image_list.push_back(itr->filename());
	}

	return _image_list.size();
}


int ys::ImageList::_readFile2List( const string &filename ){

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

ys::AbstractCapture* ys::getCapture( std::string &type ){
	if( type == "video" ){
		cout<< "open video" <<endl;
		return new ys::VideoCapture();
	}
	else if( type == "camera" ){
		cout<< "open camera" <<endl;
		return new ys::CameraCapture();
	}
	else if( type == "list" ){
		cout<< "open image list" <<endl;
		return new ys::ImageList();
	}
	else {
		cerr<< "illegal error: invalid source > " << type <<endl;
		cerr<< "exit program" <<endl;
		return 0;
	}
}

ys::Capture::Capture( std::string &type ){
	_cap = getCapture(type);
}

ys::Capture::Capture( int type ){
	string str_type;
	if( type == 0 )
		str_type = "video";
	else if( type == 1 )
		str_type = "camera";
	else if( type == 2 )
		str_type = "images";

	_cap = getCapture( str_type );
}

ys::Capture::~Capture(){
	if( _cap != NULL )
		delete _cap;
}

ys::AbstractCapture* ys::Capture::_getSource( std::string &type ){

	if( type == "video" ){
		cout<< "open video" <<endl;
		return new ys::VideoCapture();
	}
	else if( type == "camera" ){
		cout<< "open camera" <<endl;
		return new ys::CameraCapture();
	}
	else if( type == "list" ){
		cout<< "open image list" <<endl;
		return new ys::ImageList();
	}
	else {
		cerr<< "illegal error: invalid source > " << type <<endl;
		cerr<< "exit program" <<endl;
		return 0;
	}
}

bool ys::Capture::get( cv::Mat &image, int channel ){
	return _cap->get( image, channel );
}

bool ys::Capture::next(){
	return _cap->next();
}

int ys::Capture::currentNum(){
	return _cap->currentNum();
}

bool ys::Capture::open( const std::string &filename ){
	return _cap->open( filename );
}

bool ys::Capture::isOpen(){
	return _cap->isOpen();
}

void ys::Capture::release(){
	_cap->release();
}

string ys::Capture::getSourceName(){
	return _cap->getSourceName();
}

ys::ImageSequence::ImageSequence()
	:_cap_type("video"), _processor(NULL), _interval(1)
	 ,_show_progress(false), _count(0),_size(-1,-1)
	 ,_process_size(-1,-1),_point(-1,-1),_rect(-1,-1,-1,-1)
	 ,_window_name("ImageSequence") ,_use_gui(false){
	 }

void ys::ImageSequence::setCaptureType( string type ){
	_cap_type = type;
}

void ys::ImageSequence::setSize( cv::Size &size ){
	_size = size;
}

void ys::ImageSequence::setSize( int width, int height ){
	_size = Size(width, height);
}

void ys::ImageSequence::setInterval( int interval ){
	_interval = interval;
}

void ys::ImageSequence::setWindowName( string name ){
	_window_name = name;
}

void ys::ImageSequence::showProgress( bool show ){
	_show_progress = show;
}
void ys::ImageSequence::useGUI(bool use){
	_use_gui = use;
}
void ys::ImageSequence::setImageProcessor( ImageProcessorInterface& processor ){
	_processor = &processor;
}

void ys::ImageSequence::setFileName( const std::string fname ){
	_filename = fname;
}

void ys::ImageSequence::runGuiMode(){
	Capture cap(_cap_type);
	cv::Mat frame;

	if( _processor == NULL ){
		cerr<< "error: empty ImageProcessorInterface in "
			<< __FILE__ << " at " << __LINE__ <<endl;
		return;
	}

	cap.open(_filename);
	if( !cap.isOpen() ){
		cerr<< "error: failed to open Capture in "
			<< __FILE__ << " at " << __LINE__ <<endl;
		return;
	}

	if( !cap.next() ){
		cerr<< "error: failed to first image in "
			<< __FILE__ << " at " << __LINE__ <<endl;
		return;
	}

	cv::namedWindow(_window_name);

	cap.get( frame );
	
	bool now_processing_image = false;
	do{
		cap.get(frame);

		if( now_processing_image ){
			_processor->processImage( frame, cap.currentNum() );
			_processor->drawImage( frame );

		}
		else {
			//TODO
			//guiによる選択処理
			cv::Rect rect;
			_processor->init( frame, rect, _point );
		}
		cv::imshow( _window_name, frame );
		char key = (char)cv::waitKey(10);
		if( key == 'q' ){
			break;
		}

	} while( cap.next() );

	cout<< "finish program" <<endl;

}

void ys::ImageSequence::runCuiMode(){

	Capture cap(_cap_type);
	cv::Mat frame;

	if( _processor == NULL ){
		cerr<< "error: empty ImageProcessorInterface in "
			<< __FILE__ << " at " << __LINE__ <<endl;
		return;
	}

	cap.open(_filename);
	if( !cap.isOpen() ){
		cerr<< "error: failed to open Capture in "
			<< __FILE__ << " at " << __LINE__ <<endl;
		return;
	}

	if( !cap.next() ){
		cerr<< "error: failed to first image in "
			<< __FILE__ << " at " << __LINE__ <<endl;
		return;
	}

	cap.get( frame );
	_processor->init( frame, _rect, _point );

	do{
		cap.get(frame);

		_processor->processImage( frame, cap.currentNum() );
		_processor->drawImage( frame );

		cv::imshow( _window_name, frame );
		int key = cv::waitKey(10);
		cv::Rect rect;

	} while( cap.next() );

	cout<< "finish program";
}

void ys::ImageSequence::run(){
	if( _use_gui )
		runGuiMode();
	else
		runCuiMode();
}
