/*
* A Demo to OpenCV Implementation of SURF
* Further Information Refer to "SURF: Speed-Up Robust Feature"
* Author: Liu Liu
* liuliu.1987+opencv@gmail.com
*/
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include <iostream>
#include <vector>

using namespace std;

// define whether to use approximate nearest-neighbor search
#define USE_FLANN


IplImage *image = 0;

double
compareSURFDescriptors( const float* d1, const float* d2, double best, int length )
{
	double total_cost = 0;
	assert( length % 4 == 0 );
	for( int i = 0; i < length; i += 4 )
	{
		double t0 = d1[i] - d2[i];
		double t1 = d1[i+1] - d2[i+1];
		double t2 = d1[i+2] - d2[i+2];
		double t3 = d1[i+3] - d2[i+3];
		total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
		if( total_cost > best )
			break;
	}
	return total_cost;
}


int
naiveNearestNeighbor( const float* vec, int laplacian,
					 const CvSeq* model_keypoints,
					 const CvSeq* model_descriptors )
{
	int length = (int)(model_descriptors->elem_size/sizeof(float));
	int i, neighbor = -1;
	double d, dist1 = 1e6, dist2 = 1e6;
	CvSeqReader reader, kreader;
	cvStartReadSeq( model_keypoints, &kreader, 0 );
	cvStartReadSeq( model_descriptors, &reader, 0 );

	for( i = 0; i < model_descriptors->total; i++ )
	{
		const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
		const float* mvec = (const float*)reader.ptr;
		CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
		CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
		if( laplacian != kp->laplacian )
			continue;
		d = compareSURFDescriptors( vec, mvec, dist2, length );
		if( d < dist1 )
		{
			dist2 = dist1;
			dist1 = d;
			neighbor = i;
		}
		else if ( d < dist2 )
			dist2 = d;
	}
	if ( dist1 < 0.6*dist2 )
		return neighbor;
	return -1;
}

void
findPairs( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
		  const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, vector<int>& ptpairs )
{
	int i;
	CvSeqReader reader, kreader;
	cvStartReadSeq( objectKeypoints, &kreader );
	cvStartReadSeq( objectDescriptors, &reader );
	ptpairs.clear();

	for( i = 0; i < objectDescriptors->total; i++ )
	{
		const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
		const float* descriptor = (const float*)reader.ptr;
		CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
		CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
		int nearest_neighbor = naiveNearestNeighbor( descriptor, kp->laplacian, imageKeypoints, imageDescriptors );
		if( nearest_neighbor >= 0 )
		{
			ptpairs.push_back(i);
			ptpairs.push_back(nearest_neighbor);
		}
	}
}


void
flannFindPairs( const CvSeq*, const CvSeq* objectDescriptors,
			   const CvSeq*, const CvSeq* imageDescriptors, vector<int>& ptpairs )
{
	int length = (int)(objectDescriptors->elem_size/sizeof(float));

	cv::Mat m_object(objectDescriptors->total, length, CV_32F);
	cv::Mat m_image(imageDescriptors->total, length, CV_32F);


	// copy descriptors
	CvSeqReader obj_reader;
	float* obj_ptr = m_object.ptr<float>(0);
	cvStartReadSeq( objectDescriptors, &obj_reader );
	for(int i = 0; i < objectDescriptors->total; i++ )
	{
		const float* descriptor = (const float*)obj_reader.ptr;
		CV_NEXT_SEQ_ELEM( obj_reader.seq->elem_size, obj_reader );
		memcpy(obj_ptr, descriptor, length*sizeof(float));
		obj_ptr += length;
	}
	CvSeqReader img_reader;
	float* img_ptr = m_image.ptr<float>(0);
	cvStartReadSeq( imageDescriptors, &img_reader );
	for(int i = 0; i < imageDescriptors->total; i++ )
	{
		const float* descriptor = (const float*)img_reader.ptr;
		CV_NEXT_SEQ_ELEM( img_reader.seq->elem_size, img_reader );
		memcpy(img_ptr, descriptor, length*sizeof(float));
		img_ptr += length;
	}

	// find nearest neighbors using FLANN
	cv::Mat m_indices(objectDescriptors->total, 2, CV_32S);
	cv::Mat m_dists(objectDescriptors->total, 2, CV_32F);
	cv::flann::Index flann_index(m_image, cv::flann::KDTreeIndexParams(4));  // using 4 randomized kdtrees
	flann_index.knnSearch(m_object, m_indices, m_dists, 2, cv::flann::SearchParams(64) ); // maximum number of leafs checked

	int* indices_ptr = m_indices.ptr<int>(0);
	float* dists_ptr = m_dists.ptr<float>(0);
	for (int i=0;i<m_indices.rows;++i) {
		if (dists_ptr[2*i]<0.6*dists_ptr[2*i+1]) {
			ptpairs.push_back(i);
			ptpairs.push_back(indices_ptr[2*i]);
		}
	}
}


/* a rough implementation for object location */
int
locatePlanarObject( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
				   const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
				   const CvPoint src_corners[4], CvPoint dst_corners[4] )
{

	double h[9];
	CvMat _h = cvMat(3, 3, CV_64F, h);
	vector<int> ptpairs;
	vector<CvPoint2D32f> pt1, pt2;
	CvMat _pt1, _pt2;
	int i, n;

#ifdef USE_FLANN
	flannFindPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#else
	findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#endif

	n = (int)(ptpairs.size()/2);
	if( n < 4 )
		return 0;

	pt1.resize(n);
	pt2.resize(n);
	for( i = 0; i < n; i++ )
	{
		pt1[i] = ((CvSURFPoint*)cvGetSeqElem(objectKeypoints,ptpairs[i*2]))->pt;
		pt2[i] = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,ptpairs[i*2+1]))->pt;
	}

	_pt1 = cvMat(1, n, CV_32FC2, &pt1[0] );
	_pt2 = cvMat(1, n, CV_32FC2, &pt2[0] );
	if( !cvFindHomography( &_pt1, &_pt2, &_h, CV_RANSAC, 5 ))
		return 0;

	for( i = 0; i < 4; i++ )
	{
		double x = src_corners[i].x, y = src_corners[i].y;
		double Z = 1./(h[6]*x + h[7]*y + h[8]);
		double X = (h[0]*x + h[1]*y + h[2])*Z;
		double Y = (h[3]*x + h[4]*y + h[5])*Z;
		dst_corners[i] = cvPoint(cvRound(X), cvRound(Y));
	}

	return 1;
}


/* a rough implementation for object location */
int
zoglocatePlanarObject( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
					  const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
					  CvMat *_h,vector<int>& ptpairs,
					  const CvPoint2D32f src_corners[4], CvPoint2D32f dst_corners[4] )
{
	vector<CvPoint2D32f> pt1, pt2;
	CvMat _pt1, _pt2;
	int i, n;

#ifdef USE_FLANN
	flannFindPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#else
	findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#endif

	n = (int)(ptpairs.size()/2);
	if( n < 4 )
		return 0;

	pt1.resize(n);
	pt2.resize(n);
	for( i = 0; i < n; i++ )
	{
		pt1[i] = ((CvSURFPoint*)cvGetSeqElem(objectKeypoints,(ptpairs)[i*2]))->pt;
		pt2[i] = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,(ptpairs)[i*2+1]))->pt;
	}

	_pt1 = cvMat(1, n, CV_32FC2, &pt1[0] );
	_pt2 = cvMat(1, n, CV_32FC2, &pt2[0] );
	if( !cvFindHomography( &_pt1, &_pt2, _h, CV_RANSAC, 5 ))
		return 0;

	for( i = 0; i < 4; i++ )
	{
		double x = src_corners[i].x, y = src_corners[i].y;
		double Z = 1./(cvGetReal1D(_h,6)*x + cvGetReal1D(_h,7)*y + cvGetReal1D(_h,8));
		double X = (cvGetReal1D(_h,0)*x + cvGetReal1D(_h,1)*y + cvGetReal1D(_h,2))*Z;
		double Y = (cvGetReal1D(_h,3)*x + cvGetReal1D(_h,4)*y + cvGetReal1D(_h,5))*Z;
		dst_corners[i] = cvPoint2D32f(cvRound(X), cvRound(Y));
	}

	return 1;
}

//同通道 两个图片相融合 支持ROI SRC拷贝到DST中
void zogAlpha(IplImage *src,IplImage *dst,float alpha,float beta)
{
	int i,j,m,n,k;
	int dsty,dsth,dstx,dstw;
	//设置ROI
	if(dst->roi)
	{
		dsty=dst->roi->yOffset;
		dsth=dst->roi->yOffset + dst->roi->height;
		dstx=dst->roi->xOffset;
		dstw=dst->roi->xOffset + dst->roi->width;
	}
	else
	{
		dsty=0;
		dsth=dst->height;
		dstx=0;
		dstw=dst->width;
	}

	unsigned char *srcp,*dstp;
	int nullflag=0;

	for(i=dsty,m=0;i<dsth && m< src->height ;i++,m++)
	{
		if(i< 0 || i>= dst->height)
			continue;

		srcp=(unsigned char *)(src->imageData + m*src->widthStep);
		dstp=(unsigned char *)(dst->imageData + i*dst->widthStep);

		for(j=dstx,n=0;j<dstw && n< src->width ;j++,n++)
		{
			if(j< 0 || j>= dst->width)
				continue;

			//判断是否有值
			nullflag=0;
			for(k=0;k<dst->nChannels;k++)
				nullflag=dstp[dst->nChannels*j+k];
			//有值用融合
			if(nullflag)
			{
				//如果原始无值 则不做任何事
				nullflag=0;
				for(k=0;k<src->nChannels;k++)
					nullflag=srcp[src->nChannels*n+k];

				if(nullflag)
				{
					for(k=0;k<dst->nChannels;k++)
						dstp[dst->nChannels*j+k]=(unsigned char)(int)(dstp[dst->nChannels*j+k]*alpha + srcp[src->nChannels*n+k]*beta);
				}
			}
			else
			{
				//无值就直接拷贝
				for(k=0;k<dst->nChannels;k++)
					dstp[dst->nChannels*j+k]=srcp[src->nChannels*n+k];
			}
		}
	}
}

//对齐，输入未对齐到00点的数组 输出对齐到00点的数组
void zogAlignment(const CvPoint2D32f src_corners[4], CvPoint2D32f dst_corners[4],int *x,int *y,int *w,int *h)
{
	int i;
	float minx=60000,miny=60000;
	float maxx=-60000,maxy=-60000;

	for( i = 0; i < 4; i++ )
	{
		//找最小值
		if(minx>src_corners[i].x)
			minx=src_corners[i].x;
		if(miny>src_corners[i].y)
			miny=src_corners[i].y;

		//找最大值
		if(maxx<src_corners[i].x)
			maxx=src_corners[i].x;
		if(maxy<src_corners[i].y)
			maxy=src_corners[i].y;
	}
	//偏移
	for( i = 0; i < 4; i++ )
	{
		dst_corners[i].x=src_corners[i].x-minx;
		dst_corners[i].y=src_corners[i].y-miny;
	}
	//图像偏移值
	*x=(int)minx;
	*y=(int)miny;

	//图像宽度和高度
	*w=(int)(maxx-minx);
	*h=(int)(maxy-miny);

}

int main(int argc, char** argv)
{

	IplImage* src_image[100];
	IplImage* gray_image[100];
	IplImage* dst_image,*dstgray_image;

	int i;

	CvMemStorage* storage = cvCreateMemStorage(0);

	int dstw=0,dsth=0;

	for( i=1;i<argc;i++)
	{
		src_image[i-1] = cvLoadImage( argv[i], CV_LOAD_IMAGE_COLOR );
		gray_image[i-1] = cvCreateImage(cvGetSize(src_image[i-1]), 8, 1);
		cvCvtColor( src_image[i-1], gray_image[i-1], CV_BGR2GRAY );
		dstw+=src_image[i-1]->width;
		dsth+=src_image[i-1]->height;
	}
	dst_image = cvCreateImage(cvSize(dstw,dsth), 8, 3);
	dstgray_image = cvCreateImage(cvSize(dstw,dsth), 8, 1);
	cvZero(dst_image);
	cvZero(dstgray_image);

	// 特征点选取的 hessian 阈值 500  
	// 是否扩展，1 - 生成128维描述符，0 - 64维描述符
	CvSURFParams params = cvSURFParams(500, 1);
	CvSeq *objectKeypoints = 0, *objectDescriptors = 0;
	CvSeq *imageKeypoints = 0, *imageDescriptors = 0;

	//拷贝第一张图到原始位置
	cvSetImageROI( dst_image, cvRect( 0,0,  src_image[0]->width,  src_image[0]->height ) );
	cvCopy( src_image[0], dst_image );
	cvResetImageROI( dst_image );

	cvSetImageROI( dstgray_image, cvRect( 0,0,  gray_image[0]->width,  gray_image[0]->height ) );
	cvCopy( gray_image[0], dstgray_image );
	cvResetImageROI( dstgray_image );


	for( i=2;i<argc;i++)
	{

		//计算两个图的特征 返回特征点总数。 特征点和特征点描述符
		cvExtractSURF( gray_image[i-1], 0, &objectKeypoints, &objectDescriptors, storage, params );

		cvExtractSURF( dstgray_image, 0, &imageKeypoints, &imageDescriptors, storage, params );


#if 0
		CvPoint src_corners[4] = {{0,0}, {gray_image[i-1]->width,0}, {gray_image[i-1]->width, gray_image[i-1]->height}, {0, gray_image[i-1]->height}};
		CvPoint dst_corners[4];

		CvPoint temp_corners[4];
		int tempw,temph;
		CvMat* warp_matrix=cvCreateMat(3,3,CV_32FC1);

		if( locatePlanarObject( objectKeypoints, objectDescriptors, imageKeypoints,
			imageDescriptors, src_corners, dst_corners ))
		{
			CvPoint2D32f srcQuad[4]={cvPointTo32f(src_corners[0]),cvPointTo32f(src_corners[1]),cvPointTo32f(src_corners[2]),cvPointTo32f(src_corners[3])};
			CvPoint2D32f dstQuad[4]={cvPointTo32f(dst_corners[0]),cvPointTo32f(dst_corners[1]),cvPointTo32f(dst_corners[2]),cvPointTo32f(dst_corners[3])};


			zogAlignment(dst_corners,temp_corners,&tempw,&temph);

			cvGetPerspectiveTransform(srcQuad,dstQuad,warp_matrix);

			cvWarpPerspective (gray_image[i-1],dstgray_image,warp_matrix);


		}
		cvReleaseMat(&warp_matrix);
#else

		double h[9];
		CvMat _h = cvMat(3, 3, CV_64F, h);
	
		vector<int> ptpairs;
		CvPoint2D32f src_corners[4] = {{0,0}, {gray_image[i-1]->width,0}, {gray_image[i-1]->width, gray_image[i-1]->height}, {0, gray_image[i-1]->height}};
		CvPoint2D32f dst_corners[4];
		CvPoint2D32f temp_corners[4];
		int tempw,temph;
		int tempx,tempy;

		CvMat* warp_matrix=cvCreateMat(3,3,CV_32FC1);
		//得到匹配点和透视变换矩阵
		if( zoglocatePlanarObject( objectKeypoints, objectDescriptors, imageKeypoints,imageDescriptors,
			&_h,ptpairs ,src_corners,dst_corners))
		{

			//对齐到00
			zogAlignment(dst_corners,temp_corners,&tempx,&tempy,&tempw,&temph);

			cvGetPerspectiveTransform(src_corners,temp_corners,warp_matrix);
			//临时存放的彩色图像 灰色
			IplImage*	temp_image = cvCreateImage(cvSize(tempw,temph), 8, 3);
			IplImage* tempgray_image = cvCreateImage(cvSize(tempw,temph), 8, 1);
			cvZero(temp_image);
			cvZero(tempgray_image);

			cvWarpPerspective (src_image[i-1],temp_image,warp_matrix);
			cvWarpPerspective (gray_image[i-1],tempgray_image,warp_matrix);
			//拷贝彩色和灰色图像
			cvSetImageROI( dst_image, cvRect( tempx,tempy, tempw, temph ) );
			zogAlpha(temp_image,dst_image,0.5,0.5);
			cvResetImageROI( dst_image );

			cvSetImageROI( dstgray_image, cvRect( tempx,tempy, tempw, temph ) );
			zogAlpha(tempgray_image,dstgray_image,0.5,0.5);
			cvResetImageROI( dstgray_image );

			cvReleaseImage(&temp_image);
			cvReleaseImage(&tempgray_image);

		}

		cvReleaseMat(&warp_matrix);
#endif 
	}

	cvNamedWindow("warp", 1);

	cvShowImage( "warp", dstgray_image );
	cvWaitKey(0);

	cvDestroyWindow("warp");

	cvSaveImage( "dst.jpg", dst_image );

	for( i=1;i<argc;i++)
	{
		cvReleaseImage(&src_image[i-1] );
		cvReleaseImage(&gray_image[i-1] );
	}
	cvReleaseImage(&dst_image);
	cvReleaseImage(&dstgray_image);

	return 0;
}
