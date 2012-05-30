#include <string>

#include <cv.h>
#include <highgui.h>

namespace ys {

	class ImageSequence {
		public:
			virtual ~ImageSequence(){}

			virtual bool get( cv::Mat &image, int channel=0 ) = 0;
			virtual bool next() = 0;
			virtual int currentNum() = 0;

			virtual bool open(const std::string &filename) = 0;

			virtual bool isOpen() = 0;
			virtual void release() = 0;

			virtual std::string getSourceName() = 0;

	};

	class VideoCapture : public ImageSequence {
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

	class ImageList : public ImageSequence {
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


	ImageSequence* getImageSequence(std::string &type);
}
