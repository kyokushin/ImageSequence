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
	return "VideoSource";
}


bool ys::CameraCapture::open( const string &filename ){
	stringstream sstr(filename);
	int device;

	sstr>> device;

	return _cap.open(device);
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
	_counter++;
}
int ys::ImageList::currentNum(){
	return _counter;
}

bool ys::ImageList::open(const string &filename){

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
	return "ys::ImageList";
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


