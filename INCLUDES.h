#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/text.hpp"
#include "opencv2/core/utility.hpp"
#include <regex>
#include <time.h>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;
using namespace cv;
using namespace cv::text;