#include "ImageSequence.h"

#include <iostream>

using namespace std;

class Processor : public ys::ImageProcessorInterface {

	public:
	virtual ~Processor(){};
	virtual void init( const cv::Mat &image, cv::Rect &area, cv::Point &point ){

	}
	virtual void init( const cv::Mat &image, const cv::Rect &rect, const cv::Point &point ){
		cout<< "first init" <<endl;
	}
	//処理内容を記述
	//引数より与えられた画像データは書き換え不可
	//処理結果を画像に書き加える時は、drawImageを使うこと
	virtual void processImage( const cv::Mat &image, const int count ){
		cout<< "image number >" << count <<endl;
	}

	//処理結果を画像に書き加えるためのメソッド
	virtual void drawImage( cv::Mat &image ){}
	//処理を終了する条件
	virtual bool stopCriteria() { return false; }
	virtual std::string getClassName(){ return "Processor"; }
};

int main( int argc, char **argv ){
	Processor proc;

	ys::ImageSequence seq;
	seq.setCaptureType((string)"camera");
	seq.setImageProcessor( proc );
	seq.setFileName("0");
	seq.setWindowName( (string)"main2" );
	
	seq.useGUI( true );

	seq.run();
}
