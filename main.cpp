#include "skew.h"
#include "cropper.h"

struct TextData {
	vector<string> Text;
	vector<Rect> BoundingBoxes;
	vector<float> Confidence;
};

vector<Rect> Crop(Mat imageData);
TextData getTextFromImage(string ImagePath);
/////////////////////////////////////////////////////////////////////////////////////



//pattern matching using regular expressions
static void pattern_matching(std:: string file_contents, std:: regex reg) {       
        std::sregex_iterator currentMatch(file_contents.begin(), file_contents.end(), reg);
        std::sregex_iterator lastMatch;
        while(currentMatch != lastMatch){
            std:: smatch match = *currentMatch;
            std:: cout << match.str() << "\n";
            currentMatch++;
         }
         std::cout << std::endl;
}

static void pattern_matching1(std:: string file_contents, std:: regex dl) {       
        std::sregex_iterator currentMatch(file_contents.begin(), file_contents.end(), dl);
        std::sregex_iterator lastMatch;
        while(currentMatch != lastMatch){
            std:: smatch matc = *currentMatch;
            std:: cout << matc.str() << "\n";
            currentMatch++;
         }
         std::cout << std::endl;
}

static void pattern_matching2(std:: string file_contents, std:: regex capt) {       
        std::sregex_iterator currentMatch(file_contents.begin(), file_contents.end(), capt);
        std::sregex_iterator lastMatch;
        while(currentMatch != lastMatch){
            std:: smatch mat = *currentMatch;
            std:: cout << mat.str() << "\n";
            currentMatch++;
         }
         std::cout << std::endl;
}

static void pattern_matching3(std:: string file_contents, std:: regex pan) {       
        std::sregex_iterator currentMatch(file_contents.begin(), file_contents.end(), pan);
        std::sregex_iterator lastMatch;
        while(currentMatch != lastMatch){
            std:: smatch panp = *currentMatch;
            std:: cout << panp.str() << "\n";
            currentMatch++;
         }
         std::cout << std::endl;
}


static void pattern_matching4(std:: string file_contents, std:: regex aad) {       
        std::sregex_iterator currentMatch(file_contents.begin(), file_contents.end(), aad);
        std::sregex_iterator lastMatch;
        while(currentMatch != lastMatch){
            std:: smatch aada = *currentMatch;
            std:: cout << aada.str() << "\n";
            currentMatch++;
         }
         std::cout << std::endl;
}


//////////////////////////////////////////////////////////////////////////////////////////////
int main() {
        int i;
        ofstream file;
        file.open("tessdata.txt");
	string imgPath = "TestCases/IMG_20190912_110454.jpg";
	TextData data = getTextFromImage(imgPath);
	cout << "__Text Output__" << endl;
	for (int i = 0; i < data.Text.size(); ++i) {
		cout << data.Text[i];
	}
	cout << endl;      
        file << data.Text[i];
        file.close();
//////////////////////////////////////////////////////////////////////////////////////////        
        ifstream infile { "tessdata.txt" };
        string file_contents { istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };
        



        cout << file_contents << "!!!!!!!!!!!!!! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA !!!!!!!!!!!!!!"<< file_contents ;
        std:: regex reg ("([0-9]{2}\\/[0-9]{2}\\/[0-9]{4})");
        std:: regex dl("([A-Z]{2}[0-9]{2}(\\s)[0-9]{11})");
        std:: regex capt("(\b[A-Z]{4}[A-Z\\s]+\b)");
        std:: regex pan("([A-Z]{5}[0-9]{4}[A-Z]{1})");
        std:: regex aad("([0-9]{4}(\\s)[0-9]{4}(\\s)[0-9]{4})");



        pattern_matching(file_contents, reg);
        pattern_matching1(file_contents, dl);
        pattern_matching2(file_contents, capt);
	pattern_matching3(file_contents, pan);
        pattern_matching4(file_contents, aad);







         return 0;
         //exit(1);
        
}



TextData getTextFromImage(string ImagePath) {
	Skew* skewObj = new Skew();
  
 // Avoid memory leak caused by auto variable when return is called. 
        //static tesseract::TessBaseAPI api;
	static tesseract::TessBaseAPI *tess = new tesseract::TessBaseAPI();
	tess->SetPageSegMode(tesseract::PSM_SINGLE_WORD);
	if (tess->Init(NULL, "eng")) {
		cout << "!!english language file not found!!" << endl;
		exit(1);
	}
	imshow("Original Image", imread(ImagePath));
	Mat skewedImage = (skewObj->GetSkewedImage(ImagePath)).image;
	Mat copyTemp = skewedImage.clone();
	imwrite("Deskewed.jpg", copyTemp);
	imshow("Deskewed Image", skewedImage);
	//waitKey(0);
	Mat gray;
	vector<Rect> TextBoxes = Crop(copyTemp);
	vector<string> TextVec;
	vector<float> confidence;
	string tempFileName = "TEMP/temp.tiff";
	for (int i = 0; i < TextBoxes.size(); ++i) {
		Mat tempImage = skewedImage(TextBoxes[i]);
		resize(tempImage, tempImage, Size(tempImage.size().width*3,tempImage.size().height*3));
		imwrite(tempFileName, tempImage);
		//string TessCommand = "tesseract " + tempFileName + " " + "TEMP/TXToutput" + " --oem 1 -l eng"; 
		//system(TessCommand.c_str());
		
		string command = "./textcleaner -g -e normalize -f 100	 -o 12 -s 2 " + tempFileName + " " + tempFileName;
		system(command.c_str());
		tempImage = imread(tempFileName.c_str());

		cvtColor(tempImage, tempImage, CV_BGR2GRAY);
		threshold(tempImage, tempImage, 0.0, 255.0, THRESH_BINARY | THRESH_OTSU);
		imwrite(tempFileName, tempImage);
		try {	
			tess->SetImage((uchar*)tempImage.data, tempImage.size().width, tempImage.size().height, tempImage.channels(), tempImage.step1());
			tess->Recognize(0);
			TextVec.push_back(tess->GetUTF8Text());
			confidence.push_back(tess->MeanTextConf());
			cout << TextVec.back() << " Confidence: " << confidence.back() << endl;
			imshow(tempFileName, tempImage);
			//waitKey(0);
		} catch (const string& TesseractException) {
			cout << "Caught Exception: " << TesseractException << endl;
		}
		remove(tempFileName.c_str());
		bool failed = !std::ifstream(tempFileName.c_str());
		if (!failed) { cout << "FAILED TO DELETE FILE!!!|  " << i << endl; }
		//GetTextFromCroppedImage(skewedImage(TextBoxes[i]).clone());
	}
        TextData data = { TextVec , TextBoxes , confidence };
	return data;
}

vector<Rect> Crop(Mat imageData) {
	vector<Rect> BoundingBoxes;
	Mat large = imageData;
	Mat rgb;
	pyrDown(large, rgb);
	rgb = large;
	Mat small;
	cvtColor(rgb, small, CV_BGR2GRAY);
	imwrite("bw.png", small);
	Mat grad;
	//10,4 - ||Optimal For Now||
	//Mat morphKernel = getStructuringElement(MORPH_ELLIPSE, Size(30,30));
	//Mat morphKernel = getStructuringElement(MORPH_ELLIPSE, Size(15,8));
	Mat morphKernel = getStructuringElement(MORPH_ELLIPSE, Size(3,3));
	morphologyEx(small, grad, MORPH_GRADIENT, morphKernel);
	imwrite("morfex1.png", grad);
	Mat bw;
	threshold(~grad, bw, 0.0, 255.0, THRESH_BINARY | THRESH_OTSU);
	Mat connected;
	//morphKernel = getStructuringElement(MORPH_RECT, Size(4,2));
	morphKernel = getStructuringElement(MORPH_RECT, Size(9,1));
	//morphKernel = getStructuringElement(MORPH_ELLIPSE, Size(30,30));
	morphologyEx(bw, connected, MORPH_CLOSE, morphKernel);
	imwrite("morfex2.png", connected);
	Mat mask = Mat::zeros(bw.size(), CV_8UC1);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(connected, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	for(int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {
		Rect rect = boundingRect(contours[idx]);
		Mat maskROI(mask, rect);
		maskROI = Scalar(0, 0, 0);
		drawContours(mask, contours, idx, Scalar(255, 255, 255), CV_FILLED);
		double r = (double)countNonZero(maskROI)/(rect.width*rect.height);
		if (r > .45 && (rect.height > 8 && rect.width > 8)) {
			if ((rect.y-5>0) && (rect.x-5>0)) {
				rect.y-=5;
				rect.x-=5;
			}
			rect+=Size(7,7);
			if (0 <= rect.x
				&& 0 <= rect.width
				&& rect.x + rect.width <= rgb.cols
				&& 0 <= rect.y
				&& 0 <= rect.height
				&& rect.y + rect.height <= rgb.rows){
				BoundingBoxes.push_back(rect);
			} else {
				rect-=Size(7,7);
				BoundingBoxes.push_back(rect);
			}
			rectangle(rgb, rect, Scalar(0, 255, 0), 2);
		}
	}
	imwrite("final.png", rgb);
	imshow("Text Detection", rgb);
	//waitKey(0);
	return BoundingBoxes;
}
//g++ *.cpp -std=gnu++11 -o OCR-OpenCV-Tesseract -llept -ltesseract `pkg-config --cflags --libs opencv`
