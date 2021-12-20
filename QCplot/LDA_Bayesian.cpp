#include "LDA_Bayesian.h"

LDA_Bayesian::LDA_Bayesian()
{

}

LDA_Bayesian::~LDA_Bayesian()
{

}

bool LDA_Bayesian::Bayestrain(vector<vector<double> >& feature, vector<int>& label)
{
	using namespace Eigen;
	//number of features and dimension of features
	int feat_num = feature.size();
	int feat_dim = feature[0].size();
	//number of labels
	int lab_num = label.size();

	if(lab_num!=feat_num)
	{
		std::cout << "the number of features is not equal to the number of labels"<< std::endl;
		return false;
	}
	//unique label of labels
	_mClassLabel = label;
	sort(_mClassLabel.begin(), _mClassLabel.end());
	_mClassLabel.erase(unique(_mClassLabel.begin(), _mClassLabel.end()), _mClassLabel.end());

	//Eigen matrix class
	MatrixXd featmat(feat_num,feat_dim);
	for(int i=0;i<feat_num;i++)
	{
		for (int j=0;j<feat_dim;j++)
		{
			featmat(i,j)=feature[i][j];
		}
	}
	//number of classes
	int cnum = _mClassLabel.size();
	// mean matrix and covariance matrix for each class
	MatrixXd MeanMat = MatrixXd::Zero(cnum,feat_dim);
	MatrixXd CovMat = MatrixXd::Zero(feat_dim*cnum,feat_dim);
	// pool covariance matrix
	MatrixXd PoolCovMat = MatrixXd::Zero(feat_dim,feat_dim);
	// sample number for each class
	VectorXd feat_num_perclass = VectorXd::Zero(cnum);
	// compute the mean vector for each class
	for(int i=0;i<feat_num;i++)
	{
		for(int j=0;j<cnum;j++)
		{
			if(label[i]==_mClassLabel[j])
			{
				MeanMat.row(j)=MeanMat.row(j)+ featmat.row(i);
				feat_num_perclass(j) = feat_num_perclass(j)+1;
			}
		}
	}
	for(int i=0;i<cnum;i++)
	{
		MeanMat.row(i)=MeanMat.row(i)/feat_num_perclass(i);
	}
	//compute the covariance matrix for each class and pool covariance matrix
	for(int i=0;i<feat_num;i++)
	{
		for(int j=0;j<cnum;j++)
		{
			if(label[i]==_mClassLabel[j])
			{
				CovMat.block(j*feat_dim,0,feat_dim,feat_dim) += (featmat.row(i)-MeanMat.row(j)).transpose()*(featmat.row(i)-MeanMat.row(j));
			}
		}
	}
	for(int i=0;i<cnum;i++)
	{
		PoolCovMat+=CovMat.block(i*feat_dim,0,feat_dim,feat_dim);
		CovMat.block(i*feat_dim,0,feat_dim,feat_dim)=CovMat.block(i*feat_dim,0,feat_dim,feat_dim)/(feat_num_perclass(i)-1);
	}
	PoolCovMat/=(feat_num-cnum);
	PoolCovMat = PoolCovMat.inverse();

	//transform the data format from Eigen to member vectors
	vector<double> temp;

	for(int i=0;i<cnum;i++)
	{
		for(int j=0;j<feat_dim;j++)
		{
			temp.push_back(MeanMat(i,j));
		}
		ModelMean.push_back(temp);
		temp.clear();
	}
	for(int i=0;i<feat_dim;i++)
	{
		for(int j=0;j<feat_dim;j++)
		{
			temp.push_back(PoolCovMat(i,j));
		}
		ModelCov.push_back(temp);
		temp.clear();
	}

	return true;
}

int LDA_Bayesian::Bayespredict( vector<double>& x )
{
	using namespace Eigen; 

	//int feat_dim = PoolCovMat.size();
	int feat_dim = ModelCov.size();
	//int cnum = MeanMat.size();
	int cnum = ModelMean.size();
	int dim_x = x.size();
	if(dim_x!=feat_dim)
	{
		std::cerr<<"the dimension of features is not equal to the dimension of Gaussian Model"<<std::endl;
		return -1;
	}
	MatrixXd feature = MatrixXd::Zero(1,feat_dim);
	MatrixXd Cov = MatrixXd::Zero(feat_dim,feat_dim);
	MatrixXd Mean = MatrixXd::Zero(cnum,feat_dim);
	//MatrixXd Imat = MatrixXd::Identity(feat_dim,feat_dim);
	int i,j;
	for(i=0;i<feat_dim;i++)
	{
		feature(0,i)=x[i];
	}
	for(i=0;i<feat_dim;i++)
	{
		for(j=0;j<feat_dim;j++)
		{
			//Cov(i,j)=PoolCovMat[i][j];
			Cov(i,j)=ModelCov[i][j];
		}
	}
	for(i=0;i<cnum;i++)
	{
		for(j=0;j<feat_dim;j++)
		{
			//Mean(i,j)=MeanMat[i][j];
			Mean(i,j)=ModelMean[i][j];
		}
	}
	double gx;
	double mingx;
	int predict;
	for(i=0;i<cnum;i++)
	{
		// 马氏距离，Cov是协方差的逆矩阵
		gx = (feature.row(0)-Mean.row(i))*Cov*(feature.row(0)-Mean.row(i)).transpose();
		if(i==0)
		{
			mingx = gx;
			predict = _mClassLabel[i];
		}
		else
		{
			if(gx<mingx)
			{
				mingx=gx;
				predict = _mClassLabel[i];
			}
		}
	}

	return predict;
}

bool LDA_Bayesian::Bayestrain(Mat& feature, Mat &label)
{
	using namespace Eigen;
	//number of features and dimension of features
	int feat_num = feature.rows;
	int feat_dim = feature.cols;
	//number of labels
	int lab_num = label.rows;

	if(lab_num!=feat_num)
	{
		std::cout << "the number of features is not equal to the number of labels"<< std::endl;
		return false;
	}
	//unique label of labels, transfer label to vector
	vector<int> label_LDA;
	for (int i = 0; i< lab_num; i++ )
	{
		label_LDA.push_back(label.at<float>(i ,0));
	}
	_mClassLabel = label_LDA;
	sort(_mClassLabel.begin(), _mClassLabel.end());
	_mClassLabel.erase(unique(_mClassLabel.begin(), _mClassLabel.end()), _mClassLabel.end());

	//Eigen matrix class
	MatrixXd featmat(feat_num,feat_dim);
	for(int i=0;i<feat_num;i++)
	{
		for (int j=0;j<feat_dim;j++)
		{
			featmat(i,j)=feature.at<float>(i ,j);
		}
	}
	//number of classes
	int cnum = _mClassLabel.size();
	// mean matrix and covariance matrix for each class
	MatrixXd MeanMat = MatrixXd::Zero(cnum,feat_dim);
	MatrixXd CovMat = MatrixXd::Zero(feat_dim*cnum,feat_dim);
	// pool covariance matrix
	MatrixXd PoolCovMat = MatrixXd::Zero(feat_dim,feat_dim);
	// sample number for each class
	VectorXd feat_num_perclass = VectorXd::Zero(cnum);
	// compute the mean vector for each class
	for(int i=0;i<feat_num;i++)
	{
		for(int j=0;j<cnum;j++)
		{
			if(label_LDA[i]==_mClassLabel[j])
			{
				MeanMat.row(j)=MeanMat.row(j)+ featmat.row(i);
				feat_num_perclass(j) = feat_num_perclass(j)+1;
			}
		}
	}
	for(int i=0;i<cnum;i++)
	{
		MeanMat.row(i)=MeanMat.row(i)/feat_num_perclass(i);
	}
	//compute the covariance matrix for each class and pool covariance matrix
	for(int i=0;i<feat_num;i++)
	{
		for(int j=0;j<cnum;j++)
		{
			if(label_LDA[i]==_mClassLabel[j])
			{
				CovMat.block(j*feat_dim,0,feat_dim,feat_dim) += (featmat.row(i)-MeanMat.row(j)).transpose()*(featmat.row(i)-MeanMat.row(j));
			}
		}
	}
	for(int i=0;i<cnum;i++)
	{
		PoolCovMat+=CovMat.block(i*feat_dim,0,feat_dim,feat_dim);
		CovMat.block(i*feat_dim,0,feat_dim,feat_dim)=CovMat.block(i*feat_dim,0,feat_dim,feat_dim)/(feat_num_perclass(i)-1);
	}
	PoolCovMat/=(feat_num-cnum);
	PoolCovMat = PoolCovMat.inverse();
	//transform the data format from Eigen to member vectors
	vector<double> temp;
	//转换为vector<vector<double>>,可以不转换，暂时不删除
	for(int i=0;i<cnum;i++)
	{
		for(int j=0;j<feat_dim;j++)
		{
			temp.push_back(MeanMat(i,j));
		}
		ModelMean.push_back(temp);
		temp.clear();
	}
	for(int i=0;i<feat_dim;i++)
	{
		for(int j=0;j<feat_dim;j++)
		{
			temp.push_back(PoolCovMat(i,j));
		}
		ModelCov.push_back(temp);
		temp.clear();
	}

	return true;
}

int LDA_Bayesian::Bayespredict( Mat& x )
{
	using namespace Eigen; 
	int feat_dim = ModelCov.size();
	int cnum = ModelMean.size();
	//特征维数
	int dim_x = x.cols;
	if(dim_x!=feat_dim)
	{
		std::cerr<<"the dimension of features is not equal to the dimension of Gaussian Model"<<std::endl;
		return -1;
	}
	MatrixXd feature = MatrixXd::Zero(1,feat_dim);
	MatrixXd Cov = MatrixXd::Zero(feat_dim,feat_dim);
	MatrixXd Mean = MatrixXd::Zero(cnum,feat_dim);
	//MatrixXd Imat = MatrixXd::Identity(feat_dim,feat_dim);
	int i,j;
	for(i=0;i<feat_dim;i++)
	{
		feature(0,i)=x.at<float>(0, i);
	}
	for(i=0;i<feat_dim;i++)
	{
		for(j=0;j<feat_dim;j++)
		{
			Cov(i,j)=ModelCov[i][j];
		}
	}
	for(i=0;i<cnum;i++)
	{
		for(j=0;j<feat_dim;j++)
		{
			Mean(i,j)=ModelMean[i][j];
		}
	}
	double gx;
	double mingx;
	int predict;
	for(i=0;i<cnum;i++)
	{
		// 马氏距离，Cov是协方差的逆矩阵
		gx = (feature.row(0)-Mean.row(i))*Cov*(feature.row(0)-Mean.row(i)).transpose();
		if(i==0)
		{
			mingx = gx;
			predict = _mClassLabel[i];
		}
		else
		{
			if(gx<mingx)
			{
				mingx=gx;
				predict = _mClassLabel[i];
			}
		}
	}

	return predict;
}