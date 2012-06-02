#include <cv.h>
#include <highgui.h>

#include <iostream>
#include <sstream>

#include "ImageSequence.h"
#include "CommandLine.h"
#include "usegui.h"

#ifdef USE_GUI
#include "gui.h"
#endif

using namespace std;
using namespace cv;

int main(int argc, char **argv){

	ys::Params params;
	if( !commandLine( argc, argv, params )){
		return 1;
	}

	ys::AbstractCapture *source = ys::getCapture( params.source_name );

	//GUIの設定
#ifdef USE_GUI 
	string main_window("main window");
	namedWindow( main_window );
	ys::MouseParam mparam;
	setMouseCallback( main_window, &ys::mouseCallback, &mparam );
#endif

	//画像の保存用
#ifdef USE_GUI
	bool now_saving_image = false;
#else
	bool now_saving_image = true;
#endif
	unsigned int save_image_count = 0;
	string save_image_ext = ".jpg";

	//矩形表示
	Scalar red(0,0,255);
#ifdef USE_GUI
	Rect rect;
	bool now_drawing = false;
#else
	Rect rect( params.sx, params.sy, params.ex - params.sx, params.ey - params.sy );
	bool now_drawing = true;
#endif

	Mat frame;//表示用
	Mat image;//画像処理用

	//初期画像の取り込み
	source->open(params.filename);
	if( !source->next()){
		cout<< "failed to read video source" <<endl;
		return 1;
	}
	source->get(frame);
	frame.copyTo(image);

	while( 1 ){

#ifdef USE_GUI
		imshow(main_window, frame);
		int key = waitKey(10);
		if( (char)key == 'q' ){
			break;
		}
		else if( (char)key == 's' ){
			now_saving_image = !now_saving_image;
			if( now_saving_image )
				cout<< endl
					<< "start save image" <<endl;
			else
				cout<< endl
					<< "stop save image" <<endl;
		}
		else if( (char)key == 'd' ){
			now_drawing = false;
			cout<< "stop draw rect" <<endl;
		}
#endif

		if( !source->next()) break;
		source->get(frame);

		//表示用画像への描画
		if( now_drawing ){
			rectangle( frame, rect.tl(), rect.br(), red );
		}

		//矩形選択
#ifdef USE_GUI
		if( mparam.state == ys::MouseParam::STATE_DOWN ){
			now_drawing = false;
			initMouseParam2Rect( mparam, rect );
		}
		else if( mparam.state == ys::MouseParam::STATE_DRAG ){
			updateMouseParam2Rect( mparam, rect );
			rectangle( frame, rect.tl(), rect.br(), red);
		}
		else if( mparam.state == ys::MouseParam::STATE_UP ){
			updateMouseParam2Rect( mparam, rect );
			rectangle( frame, rect.tl(), rect.br(), red);

			now_drawing = true;
			cout<< "draw rect" <<endl;
		}
#endif

		//表示用画像の保存
		if( now_saving_image ){
			stringstream sstr;
			sstr<< setw(6) << setfill('0') << save_image_count << save_image_ext<<flush;
			if( !imwrite( sstr.str(), frame )){
				cerr<< "failed to save image>" << sstr.str() <<endl;
			}
			else {
				save_image_count++;
				cout<< "\rnow saving>" << sstr.str() <<flush;
			}
		}
	}

	cout<< endl
		<< "finish program" <<endl;

	delete source;
	return 0;
}
