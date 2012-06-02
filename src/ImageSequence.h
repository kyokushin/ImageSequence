#include <string>

#include <cv.h>
#include <highgui.h>

namespace ys {

	class AbstractCapture {
		public:
			virtual ~AbstractCapture(){}

			virtual bool get( cv::Mat &image, int channel=0 ) = 0;
			virtual bool next() = 0;
			virtual int currentNum() = 0;

			virtual bool open(const std::string &filename) = 0;

			virtual bool isOpen() = 0;
			virtual void release() = 0;

			virtual std::string getSourceName() = 0;

	};

	class VideoCapture : public AbstractCapture {
		public:
			VideoCapture()
				:_counter(0)
			{}
			virtual ~VideoCapture(){}

			virtual bool get( cv::Mat &image, int channel = 0 );
			virtual bool next();
			virtual int currentNum();

			virtual bool open(const std::string &filename );
			virtual bool isOpen();
			virtual void release();

			virtual std::string getSourceName();

		protected:
			cv::VideoCapture _cap;
			int _counter;
	};

	class CameraCapture : public VideoCapture {
		public:
			virtual bool open( const std::string &filename );
	};

	class ImageList : public AbstractCapture {
		public:


			ImageList()
				:_counter(0),_open(false)
			{}
			virtual ~ImageList(){}

			virtual bool get( cv::Mat &image, int channel = 0);
			virtual bool next();
			virtual int currentNum();

			virtual bool open(const std::string &filename);

			virtual bool isOpen();
			virtual void release();

			virtual std::string getSourceName();

		private:
			std::vector<std::string> _image_list;
			int _counter;
			bool _open;

			int _readDir2List(const std::string &name );

			int _readFile2List( const std::string &filename );
	};


	AbstractCapture* getCapture(std::string &type);

	class Capture {
		public:
			Capture( std::string &type );
			Capture( int type );
			~Capture();
			virtual bool get( cv::Mat &image, int channel = 0);
			virtual bool next();
			virtual int currentNum();

			virtual bool open(const std::string &filename);

			virtual bool isOpen();
			virtual void release();

			virtual std::string getSourceName();
			
		private:
			AbstractCapture *_cap;
	};

	class ImageProcessorInterface {
		public:
			virtual ~ImageProcessorInterface() = 0;
			virtual void init( cv::Mat &image, cv::Rect &area ) = 0;
			//処理内容を記述
			virtual void processImage( cv::Mat &image, int count ) = 0;
			//処理を終了する条件
			virtual bool stopCondition();
	};

	class ImageSequence {
		public:
			ImageSequence();
			void setSize( cv::Size &size );
			void setInterval( int interval );
			void setShowProgress( bool show );
			void setImageProcessor( ImageProcessorInterface &processor );
			void run();

		private:
			Capture _cap;
			ImageProcessorInterface *_processor;
			cv::Size _image_size;
			int _interval;
			bool _show_progress;
			int _count;
	};
}
