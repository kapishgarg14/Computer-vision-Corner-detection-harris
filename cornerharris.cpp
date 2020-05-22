#include<iostream>
#include<opencv2/opencv_modules.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include<string>
using namespace std;
using namespace cv;
using namespace xfeatures2d;
float single_convol(Mat a,Mat b){
float sum=0;
for(int i=0;i<a.rows;i++){
    for(int j=0;j<a.cols;j++) sum+=a.at<float>(i,j)*b.at<float>(i,j);
}
return sum;
}
void sobelme(Mat img,Mat kernel,Mat& ans){
    for(int i=1;i<img.rows-1;i++){
        for(int j=1;j<img.cols-1;j++){
           Mat a(img,Range(i-1,i+2),Range(j-1,j+2));
           ans.at<float>(i,j)=abs(single_convol(a,kernel));
        }}
}
void harriscorner(Mat img_gray,Mat& out,int size,double k){
Mat IX=Mat::zeros(img_gray.size(),CV_32FC1),Iy=Mat::zeros(img_gray.size(),CV_32FC1);
   float kx[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
    Mat kernel1=Mat(3,3,CV_32FC1,kx);  
    float ky[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};
    Mat kernel2=Mat(3,3,CV_32FC1,ky);
    sobelme(img_gray,kernel1,IX);
    sobelme(img_gray,kernel2,Iy);
     Mat ixx,iyy,ixy,ixg,iyg,ixyg,x2y2,xy,mtrace;
     pow(IX,2.0,ixx);
    pow(Iy,2.0,iyy);
    multiply(IX,Iy,ixy);
    GaussianBlur(ixx,ixg,Size(size,size),2.0,0.0,BORDER_DEFAULT);
    GaussianBlur(iyy,iyg,Size(size,size),0.0,2.0,BORDER_DEFAULT);
    GaussianBlur(ixy,ixyg,Size(size,size),2.0,2.0,BORDER_DEFAULT);
    multiply(ixg,iyg,x2y2);
    multiply(ixyg,ixyg,xy);
    pow((ixg+ iyg),2.0,mtrace);
    out=(x2y2-xy)-k*mtrace;
}
int main(){
    string inp;cin>>inp;
    int k;cin>>k;
    Mat img=imread(inp);
    cout<<img.size()<<endl;
    Mat img2,img_gray;
    cvtColor(img,img2,COLOR_BGR2GRAY);
    img2.convertTo(img_gray,CV_32FC1);
    Mat out;
     harriscorner(img_gray,out,k,0.04);
    Mat out_norm;
    normalize(out, out_norm, 0, 255, NORM_MINMAX);
    float a=128;

     for(int i=1;i<img_gray.rows-1;i++){
         for(int j=1;j<img_gray.cols-1;j++)
         {     double max1,min1;
         Mat check(out_norm,Range(i-1,i+2),Range(j-1,j+2));
          minMaxLoc(check,&min1,&max1);
             if(max1==out_norm.at<float>(i,j)&&out_norm.at<float>(i,j)>113){
             circle(img,Point(j,i),5,Scalar(255,0,255),2,8,0);
             } }}
     namedWindow("corner");
    imshow("corner",img);
    waitKey(0);
    return 0;
}