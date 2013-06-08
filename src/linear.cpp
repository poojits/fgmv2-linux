#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "linear.h"
#include "FGM.h"
#include <malloc.h>
#include "tron.h"
typedef signed char schar;
//using namespace std;
//#include<iostream>

//template <class T> inline void swap(T& x, T& y) { T t=x; x=y; y=t; }
#ifndef min
template <class T> inline T min(T x,T y) { return (x<y)?x:y; }
#endif

#ifndef max
template <class T> inline T max(T x,T y) { return (x>y)?x:y; }
#endif
template <class S, class T> inline void clone(T*& dst, S* src, int n)
{
	dst = new T[n];
	memcpy((void *)dst,(void *)src,sizeof(T)*n);
}
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
#define INF HUGE_VAL

#if 1
static void info(const char *fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	vprintf(fmt,ap);
	va_end(ap);
}
static void info_flush()
{
	fflush(stdout);
}
#else
static void info(char *fmt,...) {}
static void info_flush() {}
#endif

const char *solver_type_table[]=
{
	"L2R_LR", "L2R_L2LOSS_SVC_DUAL", "L2R_L2LOSS_SVC", "L2R_L1LOSS_SVC_DUAL", "MCSVM_CS", "L1R_L2LOSS_SVC", "L1R_LR", "L2R_LR_DUAL", "SVMFGM", "LRFGM", "MINLR", "PSVMFGM", "PROXFGM", "PROXFGMLR","STOCHFGM","STOCHLR","OMPLR","OMPL2",NULL
};


void destroy_model(struct model *model_)
{
	if(model_->w != NULL)
		free(model_->w);
	if(model_->label != NULL)
		free(model_->label);
	if(model_->alpha != NULL)
		free(model_->alpha);
	if(model_->sigma != NULL)
		free(model_->sigma);
	if (model_->count != NULL)
		free(model_->count);

	if (model_->solution_->w_FGM !=NULL)
		free(model_->solution_->w_FGM);
	if (model_->solution_->w_FGM_retrain !=NULL)
		free(model_->solution_->w_FGM_retrain);
	if (model_->solution_->w_FGM_B !=NULL)
		free(model_->solution_->w_FGM_B);
	if (model_->solution_ != NULL)
	{
		free(model_->solution_);
	}

	free(model_);
}

//double SearchSparseElement(feature_node *xi,int index)
//{
//	double node_value = 0;
//	while(xi->index!=-1)
//	{
//		if (xi->index == index+1)
//		{
//			node_value = xi->value;
//			break;
//		}
//		xi++;
//	}
//	return node_value;
//}
//
//void svm_retrain(problem *prob, double eps,
//					  double Cp, double Cn, int solver_type, weight *w2, int feature_num, int bias)
//{
//
//	int l = prob->l;
//	int n = feature_num;
//	int w_size = feature_num;
//
//	int i, s,iter = 0;//,
//	double C, d, G;
//	double *QD = new double[l];
//	int max_iter = 1000;
//	int *index = new int[l];
//	double temp = 0;
//	double *alpha = new double[l];
//	schar *y = new schar[l];
//	int active_size = l;
//
//	// PG: projected gradient, for shrinking and stopping
//	double PG;
//	double PGmax_old = INF;
//	double PGmin_old = -INF;
//	double PGmax_new, PGmin_new;
//
//	// default solver_type: L2LOSS_SVM_DUAL
//	double diag_p = 0.5/Cp, diag_n = 0.5/Cn;
//	double upper_bound_p = INF, upper_bound_n = INF;
//
//	// w: 1: bias, n:linear, (n+1)n/2: quadratic
//	double tmp_value;
//	int j = 0;
//
//	if(solver_type == L2R_L1LOSS_SVC_DUAL)
//	{
//		diag_p = 0; diag_n = 0;
//		upper_bound_p = Cp; upper_bound_n = Cn;
//	}
//
//	for(i=0; i<l; i++)
//	{
//		{
//			alpha[i] =0;
//		}
//
//		if(prob->y[i] > 0)
//		{
//			y[i] = +1;
//			QD[i] = diag_p;
//		}
//		else
//		{
//			y[i] = -1;
//			QD[i] = diag_n;
//		}
//		if (bias)// add bias here
//		{
//			QD[i] = QD[i]+1;
//		}
//		j = 0;
//		if (bias)
//		{
//			while (w2[j].index1!=-2)
//			{
//				tmp_value = SearchSparseElement(&prob->x[i][0],w2[j].indexj);
//				QD[i] += tmp_value *tmp_value;
//				j++;
//			}
//		}else
//		{
//			while (w2[j].index1!=-1)
//			{
//				tmp_value = SearchSparseElement(&prob->x[i][0],w2[j].indexj);
//				QD[i] += tmp_value *tmp_value;
//				j++;
//			}
//		}
//		index[i] = i;
//	}
//
//
//	for(i=0; i<w_size; i++)
//	{
//		w2[i].value = 0;
//	}
//
//	while (iter < max_iter)
//	{
//		PGmax_new = -INF;
//		PGmin_new = INF;
//
//		for (i=0; i<active_size; i++)
//		{
//			int j = i+rand()%(active_size-i);
//			swap(index[i], index[j]);
//		}
//
//		for (s=0;s<active_size;s++)
//		{
//			i = index[s];
//			G = 0;
//			schar yi = y[i];
//
//			j = 0;
//			if (bias)
//			{
//				while (w2[j].index1!=-2)
//				{
//					tmp_value =  SearchSparseElement(&prob->x[i][0],w2[j].indexj);
//					G += w2[j].value*tmp_value;
//					j++;
//				}
//				G += w2[j].value*y[i];
//			}else
//			{
//				while (w2[j].index1!=-1)
//				{
//					tmp_value =  SearchSparseElement(&prob->x[i][0],w2[j].indexj);
//					G += w2[j].value*tmp_value;
//					j++;
//				}
//			}
//			G = G*yi-1;
//
//			if(y[i] == 1)
//			{
//				C = upper_bound_p;
//				G += alpha[i]*diag_p;
//			}
//			else
//			{
//				C = upper_bound_n;
//				G += alpha[i]*diag_n;
//			}
//
//			PG = 0;
//			if (alpha[i] == 0)
//			{
//				if (G > PGmax_old)
//				{
//					active_size--;
//					swap(index[s], index[active_size]);
//					s--;
//					continue;
//				}
//				else if (G < 0)
//					PG = G;
//			}
//			else if (alpha[i] == C)
//			{
//				if (G < PGmin_old)
//				{
//					active_size--;
//					swap(index[s], index[active_size]);
//					s--;
//					continue;
//				}
//				else if (G > 0)
//					PG = G;
//			}
//			else
//				PG = G;
//
//			PGmax_new = max(PGmax_new, PG);
//			PGmin_new = min(PGmin_new, PG);
//
//			if(fabs(PG) > 1.0e-12)
//			{
//				double alpha_old = alpha[i];
//				alpha[i] = min(max(alpha[i] - G/QD[i], 0.0), C);
//				d = (alpha[i] - alpha_old)*yi;
//
//				j = 0;
//				if (bias)
//				{
//					while (w2[j].index1!=-2)
//					{
//						tmp_value =  SearchSparseElement(&prob->x[i][0],w2[j].indexj);
//						w2[j].value += d*tmp_value;//dt[xi->index-1].value*dt[xi->index-1].value;
//						j++;
//					}
//					w2[j].value += d*1*y[i];
//				}
//				else
//				{
//					while (w2[j].index1!=-1)
//					{
//						tmp_value =  SearchSparseElement(&prob->x[i][0],w2[j].indexj);
//						w2[j].value += d*tmp_value;//dt[xi->index-1].value*dt[xi->index-1].value;
//						j++;
//					}
//				}
//
//			}
//		}
//
//		iter++;
//		if(iter % 10 == 0)
//		{
//			info(".");
//		}
//
//		if(PGmax_new - PGmin_new <= eps)
//		{
//			if(active_size == l)
//				break;
//			else
//			{
//				active_size = l;
//				printf("*");
//				PGmax_old = INF;
//				PGmin_old = -INF;
//				continue;
//			}
//		}
//		PGmax_old = PGmax_new;
//		PGmin_old = PGmin_new;
//		if (PGmax_old <= 0)
//			PGmax_old = INF;
//		if (PGmin_old >= 0)
//			PGmin_old = -INF;
//	}
//
//
//	// calculate objective value
//
//	double v = 0;
//	double tmp = 0;
//	int nSV = 0;
//
//	tmp = 0;
//	for(i=0; i<w_size; i++)
//	{
//		tmp += w2[i].value*w2[i].value;
//	}
//	v += tmp;
//
//	for(i=0; i<l; i++)
//	{
//		if (y[i] == 1)
//			v += alpha[i]*(alpha[i]*diag_p - 2);
//		else
//			v += alpha[i]*(alpha[i]*diag_n - 2);
//		if(alpha[i] > 0)
//			++nSV;
//	}
//
//	printf("Objective value = %lf\n",v/2);
//	printf("nSV = %d\n",nSV);
//
//
//	delete [] QD;
//	delete [] alpha;
//	delete [] y;
//	delete [] index;
//}
feature_node		**sub_x_space;//
int count_element(problem *&prob, weight *w2b, int B,int iteration)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int iw = 0;
	int jw = 0;

	int i_start = 0;
	int j_start = 0;
	int iw_temp=0;
	feature_node *xi;

	long int element=0;

	for(k=0;k<prob->l;k++)
	{
		i_start = 0;
		xi = prob->x[k];
		j = 0;
		for(i=0; i<B; i++)
		{
			iw = w2b[i].index1;
			if(xi[i_start].index==-1)
			{
				break;
			}
			while(xi[i_start].index!=-1&&xi[i_start].index-1<iw)
			{
				i_start++;
			}
			if(xi[i_start].index-1==iw)
			{
				element++;
				j++;
			}
		}

	}
	printf("element number is %d\n",element);
	return element;

}

void record_subfeature_sparse(problem *&prob,weight *w2b, int B, int iteration)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int iw = 0;
	int jw = 0;

	int i_start = 0;
	int j_start = 0;
	int iw_temp=0;
	feature_node *xi;
	feature_node *xj;

	long int element = 0;

	element = count_element(prob, w2b, B,iteration);
	sub_x_space[iteration] = Malloc(feature_node , element+prob->l);
	element = 0;


	for(k=0;k<prob->l;k++)
	{
		prob->xsp[iteration][k] = &sub_x_space[iteration][element];
		i_start = 0;
		xi = prob->x[k];
		j = 0;
		for(i=0; i<B; i++)
		{
			iw = w2b[i].index1;
			if(xi[i_start].index==-1)
			{
				break;
			}
			while(xi[i_start].index!=-1&&xi[i_start].index-1<iw)
			{
				i_start++;
			}
			if(xi[i_start].index-1==iw)
			{
				prob->xsp[iteration][k][j].index = i+1;
				prob->xsp[iteration][k][j].value = xi[i_start].value;
				j++;
				element++;
			}
		}
		prob->xsp[iteration][k][j].index = -1;
		element++;

	}


}


void record_subfeature_sparse_(problem *&prob,weight *w2b, int B, int iteration)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int iw = 0;
	int jw = 0;

	int i_start = 0;
	int j_start = 0;
	int iw_temp=0;
	feature_node *xi;
	feature_node *xj;

	long int element = 0;

	element = count_element(prob, w2b, B,iteration);
	sub_x_space[iteration] = Malloc(feature_node , element+prob->l);
	element = 0;


	for(k=0;k<prob->l;k++)
	{
		prob->xsp_temp[iteration][k] = &sub_x_space[iteration][element];
		i_start = 0;
		xi = prob->x[k];
		j = 0;
		for(i=0; i<B; i++)
		{
			iw = w2b[i].index1;
			if(xi[i_start].index==-1)
			{
				break;
			}
			while(xi[i_start].index!=-1&&xi[i_start].index-1<iw)
			{
				i_start++;
			}
			if(xi[i_start].index-1==iw)
			{
				prob->xsp_temp[iteration][k][j].index = i+1;
				prob->xsp_temp[iteration][k][j].value = xi[i_start].value;
				j++;
				element++;
			}
		}
		prob->xsp_temp[iteration][k][j].index = -1;
		element++;

	}


}
//void record_subfeature_sparse(problem *&prob,weight *w2b, int B, int iteration)
//{
//	int i = 0;
//	int j = 0;
//	int k = 0;
//	int iw = 0;
//	int jw = 0;
//
//	int i_start = 0;
//	int j_start = 0;
//	int iw_temp=0;
//	feature_node *xi;
//	feature_node *xj;
//
//	long int element = 0;
//
//	element = count_element(prob, w2b, B,iteration);
//	sub_x_space[iteration] = Malloc(feature_node , element+prob->l);
//	element = 0;
//
//
//		for(k=0;k<prob->l;k++)
//		{
//			prob->xsp[iteration][k] = &sub_x_space[iteration][element];
//			i_start = 0;
//			xi = prob->x[k];
//			j = 0;
//			for(i=0; i<B; i++)
//			{
//				iw = w2b[i].index1;
//				if(xi[i_start].index==-1)
//				{
//					break;
//				}
//				while(xi[i_start].index!=-1&&xi[i_start].index-1<iw)
//				{
//					i_start++;
//				}
//				if(xi[i_start].index-1==iw)
//				{
//					prob->xsp[iteration][k][j].index = i+1;
//					prob->xsp[iteration][k][j].value = xi[i_start].value;
//					j++;
//					element++;
//				}
//			}
//			prob->xsp[iteration][k][j].index = -1;
//			element++;
//		}
//
//		return;
//
//}


void retrain_linear_solver_svc(problem *&prob,const parameter *param,double *w,int w_size,double eps,
							   double Cp, double Cn, int solver_type)
{
	int l = prob->l;
	int n = w_size;
	int num_kernel=1;

	int i, s, p,iter = 0;//,
	double C, d, G;

	int max_iter = 1000;
	double temp = 0;
	int active_size = l;

	double *QD = new double[l];
	int *index = new int[l];
	double *alpha = new double[l];
	schar *y = new schar[l];

	// PG: projected gradient, for shrinking and stopping
	double PG;
	double PGmax_old = INF;
	double PGmin_old = -INF;
	double PGmax_new, PGmin_new;

	// default solver_type: L2LOSS_SVM_DUAL
	double diag_p = 0.5/Cp, diag_n = 0.5/Cn;
	double upper_bound_p = INF, upper_bound_n = INF;

	// w: 1: bias, n:linear

	if(solver_type == L2R_L1LOSS_SVC_DUAL)
	{
		diag_p = 0; diag_n = 0;
		upper_bound_p = Cp; upper_bound_n = Cn;
	}

	for(i=0; i<l; i++)
	{

		alpha[i] = 0;
		if(prob->y[i] > 0)
		{
			QD[i] = diag_p;
			y[i] = 1;
		}
		else
		{
			QD[i] = diag_n;
			y[i] = -1;
		}

		feature_node *xi = prob->xsp[0][i];
		while(xi->index!=-1)
		{
			{
				QD[i] += (xi->value)*(xi->value);
			}
			xi++;
		}

		index[i] = i;
	}

	for(i=0; i<w_size; i++)
	{
		w[i] = 0;
	}

	while (iter < max_iter)
	{
		PGmax_new = -INF;
		PGmin_new = INF;

		for (i=0; i<active_size; i++)
		{
			int j = i+rand()%(active_size-i);
			swap(index[i], index[j]);
		}

		for (s=0;s<active_size;s++)
		{
			i = index[s];
			G = 0;
			schar yi = y[i];

			for(p=0; p<num_kernel; p++)
			{
				feature_node *xi = prob->xsp[p][i];
				while(xi->index!=-1)
				{
					G += w[p*n+xi->index-1]*(xi->value);
					xi++;
				}
			}
			G = G*yi-1;

			if(y[i] == 1)
			{
				C = upper_bound_p;
				G += alpha[i]*diag_p;
			}
			else
			{
				C = upper_bound_n;
				G += alpha[i]*diag_n;
			}

			PG = 0;
			if (alpha[i] == 0)
			{
				if (G > PGmax_old)
				{
					active_size--;
					swap(index[s], index[active_size]);
					s--;
					continue;
				}
				else if (G < 0)
					PG = G;
			}
			else if (alpha[i] == C)
			{
				if (G < PGmin_old)
				{
					active_size--;
					swap(index[s], index[active_size]);
					s--;
					continue;
				}
				else if (G > 0)
					PG = G;
			}
			else
				PG = G;

			PGmax_new = max(PGmax_new, PG);
			PGmin_new = min(PGmin_new, PG);

			if(fabs(PG) > 1.0e-12)
			{
				double alpha_old = alpha[i];
				alpha[i] = min(max(alpha[i] - G/QD[i], 0.0), C);
				d = (alpha[i] - alpha_old)*yi;

				for(p=0; p<num_kernel; p++)
				{
					feature_node *xi = prob->xsp[p][i];
					while(xi->index!=-1)
					{
						w[p*n+xi->index-1] += d*(xi->value);
						xi++;
					}
				}
			}
		}

		iter++;
		if(iter % 10 == 0)
		{
			info(".");
			info_flush();
		}

		if(PGmax_new - PGmin_new <= eps)
		{
			if(active_size == l)
				break;
			else
			{
				active_size = l;
				info("*"); info_flush();
				PGmax_old = INF;
				PGmin_old = -INF;
				continue;
			}
		}
		PGmax_old = PGmax_new;
		PGmin_old = PGmin_new;
		if (PGmax_old <= 0)
			PGmax_old = INF;
		if (PGmin_old >= 0)
			PGmin_old = -INF;
	}


	// calculate objective value

	double v = 0;
	double tmp = 0;
	int nSV = 0;


	tmp = 0;
	for(i=0; i<w_size; i++)
	{
		tmp += w[i]*w[i];
	}
	v += tmp/2;


	for(i=0; i<l; i++)
	{
		if (y[i] == 1)
			v += alpha[i]*(alpha[i]*diag_p - 2);
		else
			v += alpha[i]*(alpha[i]*diag_n - 2);
		if(alpha[i] > 0)
			++nSV;
	}

	delete [] QD;
	delete [] alpha;
	delete [] y;
	delete [] index;
	//info("Objective value = %lf\n",v/2);
	//info("nSV = %d\n",nSV);
}

static double train_one(const problem *prob, const parameter *param, double *w, double Cp, double Cn);
void solve_l2r_lr_dual_retrain(problem *prob, double eps,
							   double Cp, double Cn, int solver_type, weight *w2, int feature_num, int bias);
void svm_retrain(problem *prob, double eps,
				 double Cp, double Cn, int solver_type, weight *w2, int feature_num, int bias);

model* FGM_train(struct problem *prob, const struct parameter *param)
{

	long int t_start = clock();
	long int t_finish;
	double run_time;
	//data dimension
	int n = prob->n;
	prob->bias = -1;// we consider here no bias problem;

	//set the number of iteration
	int max_iteration = param->max_iteraion;

	model *model_ = Malloc(model,1);
	if(prob->bias>=0)
		model_->nr_feature = n-1;
	else
		model_->nr_feature = n;

	//initialize model
	model_->param = *param;
	model_->bias = prob->bias;
	model_->l = prob->l;
	model_->nr_class = 2;
	model_->n_kernel = 1;

	model_->label = Malloc(int, model_->nr_class);
	model_->label[0] = 1;
	model_->label[1] = -1;
	model_->B = param->B;
	model_->count = NULL;
	model_->alpha = NULL;
	model_->sigma						= Malloc(double,max_iteration+1);
	model_->solution_					= Malloc(Solution,1); //EDIT-Poojit: Solution
	model_->alpha                       = Malloc(double,model_->l+1);

	printf("Initialization of model is complete inside FGM::train in linear.cpp \n");

	if(param->solver_type>7)
	{
		model_->w2s							= Malloc(weight,max_iteration*(param->B+1));
		model_->solution_->w_FGM			= Malloc(weight,max_iteration*param->B+1);
		model_->solution_->w_FGM_retrain	= Malloc(weight,max_iteration*param->B+1);
		model_->solution_->w_FGM_B			= Malloc(weight,max_iteration*param->B+1);
		model_->w						    = Malloc(double,(max_iteration)*(param->B+1));
		//construct FGM object:
		FGM FGM_inst(prob,model_,param,max_iteration);

		FGM_inst.FGM_init();

		//FGM training
		double run_time_ = 0;
		FGM_inst.FGM_train_one(run_time_);

		t_finish = clock();
		run_time = (double(t_finish-t_start)/CLOCKS_PER_SEC);
		printf("constraint elaps time is %f\n",run_time_);
		printf("elaps time is %f\n",run_time);
		model_->run_time = run_time;
		FGM_inst.set_model();
		//FGM_inst.~FGM();

		sub_x_space				= Malloc(feature_node *, 1);
		prob->xsp_temp			= Malloc(feature_node **, 1);
		prob->xsp_temp[0]       = Malloc(struct feature_node *,prob->l);

		record_subfeature_sparse(prob,model_->solution_->w_FGM_retrain,model_->feature_pair,0);
		//
		double *w = Malloc(double, model_->feature_pair);
		////retraining
		if(param->t>0)
		{
			retrain_linear_solver_svc(prob,param,&w[0],model_->feature_pair, 0.1,
				10, 10, L2R_L2LOSS_SVC_DUAL);
			printf("do retraining with all selected features using SVM\n");
			for (int i =0; i<model_->feature_pair; i++)
			{
				model_->solution_->w_FGM_retrain[i].value = w[i];
			}
		}
		free(w);
		free(sub_x_space[0]);
		free(sub_x_space);
		free(prob->xsp_temp[0]);
		free(prob->xsp_temp);
		//svm_retrain(prob,0.1, 5, 5, L2R_L2LOSS_SVC_DUAL,
		//model_->solution_->w_FGM_retrain,model_->feature_pair,0);

	}else
	{
		double runtime_actual = 0;
		model_->w						= Malloc(double,prob->n+3);
		runtime_actual = train_one(prob, param, &model_->w[0], param->C, param->C);

		//give the score
		int i;
		int count=0;
		for(i=0;i<prob->n;i++)
		{
			if(fabs(model_->w[i])>0)
			{
				count++;
			}
		}
		printf("count=  %d\n",count);
		model_->w2s							= Malloc(weight,count+1);
		model_->solution_->w_FGM			= Malloc(weight,count+1);
		model_->solution_->w_FGM_retrain	= Malloc(weight,count+1);
		model_->solution_->w_FGM_B			= Malloc(weight,count+1);

		model_->feature_pair = count;
		t_finish = clock();
		run_time = (double(t_finish-t_start)/CLOCKS_PER_SEC);
		printf("elaps time is %f\n",run_time);
		printf("runtime_actual time is %f\n",runtime_actual);
		model_->run_time = run_time;
		count = 0;
		for(i=0;i<prob->n;i++)
		{
			if(fabs(model_->w[i])>0)
			{
				model_->solution_->w_FGM[count].index1 = i;
				model_->solution_->w_FGM[count].index2 = -1;
				model_->solution_->w_FGM[count].value = model_->w[i];

				model_->solution_->w_FGM_retrain[count].index1 = i;
				model_->solution_->w_FGM_retrain[count].index2 = -1;
				model_->solution_->w_FGM_retrain[count].value = model_->w[i];

				model_->solution_->w_FGM_B[count].index1 = i;
				model_->solution_->w_FGM_B[count].index2 = -1;
				model_->solution_->w_FGM_B[count].value = model_->w[i];
				count++;
			}
		}
		printf("count=  %d\n",count);
		model_->solution_->w_FGM[count].index1 = -1;
		model_->solution_->w_FGM_B[count].index1 = -1;
		model_->solution_->w_FGM_retrain[count].index1 = -1;
		if(model_->feature_pair>1&&param->t>0)
		{
			sub_x_space				= Malloc(feature_node *, 1);
			prob->xsp				= Malloc(feature_node **, 1);
			prob->xsp[0] = Malloc(struct feature_node *,prob->l);

			record_subfeature_sparse(prob,model_->solution_->w_FGM_retrain,model_->feature_pair,0);

			double *w = Malloc(double, model_->feature_pair);
			//retraining
			if(param->t>0)
			{
				retrain_linear_solver_svc(prob,param,&w[0],model_->feature_pair, 0.1,
					5, 5, L2R_L2LOSS_SVC_DUAL);
			}
			printf("do retraining with all selected features using SVM\n");
			//svm_retrain(prob,0.1, 8, 8, L2R_L2LOSS_SVC_DUAL,
			//model_->solution_->w_FGM_retrain,model_->feature_pair,0);

			for (int i =0; i<model_->feature_pair; i++)
			{
				model_->solution_->w_FGM_retrain[i].value = w[i];
			}
			free(w);
			free(sub_x_space[0]);
			free(sub_x_space);
			free(prob->xsp[0]);
			free(prob->xsp);
		}
		//solve_l2r_lr_dual_retrain(prob,0.01, 8, 8, L2R_L2LOSS_SVC_DUAL,
		//model_->solution_->w_FGM_retrain,model_->feature_pair,0);

	}

	return model_;
	//initialize
}


int save_model_poly(const char *model_file_name, const struct model *model_) // This saves the SVM model
{
	int i;
	int nr_feature=model_->nr_feature;
	int n;
	int j;
	const parameter& param = model_->param;

	if(model_->bias>=0)
		n=nr_feature+1;
	else
		n=nr_feature;
	if (model_->param.flag_poly==1)
	{
		int w_size = (n+2)*(n+1)/2;
	}
	else
	{
		int w_size = n;
	}
	FILE *fp = fopen(model_file_name,"w");
	if(fp==NULL) return -1;

	int nr_w;
	if(model_->nr_class==2 && model_->param.solver_type != MCSVM_CS)
		nr_w=1;
	else
		nr_w=model_->nr_class;

	fprintf(fp, "solver_type %s\n", solver_type_table[param.solver_type]);
	fprintf(fp, "nr_class %d\n", model_->nr_class);
	fprintf(fp, "label");
	for(i=0; i<model_->nr_class; i++)
		fprintf(fp, " %d", model_->label[i]);
	fprintf(fp, "\n");

	fprintf(fp, "nr_feature %d\n", nr_feature);

	fprintf(fp, "bias %.16g\n", model_->bias);

	fprintf(fp, "B %d\n", model_->B);

	fprintf(fp,"flag_poly %d\n", param.flag_poly);

	fprintf(fp, "coef0 %.16g\n", param.coef0);

	fprintf(fp, "gamma %.16g\n", param.gamma);

	fprintf(fp, "t %d\n", param.t);

	fprintf(fp, "feature_pair %d\n", model_->feature_pair);

	fprintf(fp, "train_time %f\n", model_->run_time);

	fprintf(fp, "w %d\n", model_->feature_pair);

	j = 0;
	while (model_->solution_->w_FGM[j].index1 != -1)
	{
		fprintf(fp, "%ld ",(long) model_->solution_->w_FGM[j].index1); //typecasted to long by Poojit (porting issue from Win32 to x64 Linux
		fprintf(fp, "%ld:%.16g\n", (long)model_->solution_->w_FGM[j].index2,model_->solution_->w_FGM[j].value);
		j++;
	}
	if (param.t==1)
	{
		fprintf(fp, "\n");
		j = 0;
		while (model_->solution_->w_FGM_retrain[j].index1 != -1)
		{
			fprintf(fp, "%ld ", (long)model_->solution_->w_FGM_retrain[j].index1);
			fprintf(fp, "%ld:%.16g\n",(long) model_->solution_->w_FGM_retrain[j].index2,model_->solution_->w_FGM_retrain[j].value);
			j++;
		}

		fprintf(fp, "\n");
		j = 0;

		while (model_->solution_->w_FGM_B[j].index1 != -1)
		{
			fprintf(fp, "%ld ", (long)model_->solution_->w_FGM_B[j].index1);
			fprintf(fp, "%ld:%.16g\n",(long) model_->solution_->w_FGM_B[j].index2,model_->solution_->w_FGM_B[j].value);
			j++;
		}
	}
	if (ferror(fp) != 0 || fclose(fp) != 0) return -1;
	else return 0;
}
int save_model(const char *model_file_name, const struct model *model_)
{
	int i;
	int nr_feature=model_->nr_feature;
	int n;
	const parameter& param = model_->param;

	if(model_->bias>=0)
		n=nr_feature+1;
	else
		n=nr_feature;
	FILE *fp = fopen(model_file_name,"w");
	if(fp==NULL) return -1;

	int nr_w;
	if(model_->nr_class==2 && model_->param.solver_type != MCSVM_CS)
		nr_w=1;
	else
		nr_w=model_->nr_class;

	fprintf(fp, "solver_type %s\n", solver_type_table[param.solver_type]);
	fprintf(fp, "nr_class %d\n", model_->nr_class);
	fprintf(fp, "label");
	for(i=0; i<model_->nr_class; i++)
		fprintf(fp, " %d", model_->label[i]);
	fprintf(fp, "\n");

	fprintf(fp, "nr_feature %d\n", nr_feature);

	fprintf(fp, "bias %.16g\n", model_->bias);

	fprintf(fp, "w\n");
	for(i=0; i<n; i++)
	{
		int j;
		for(j=0; j<nr_w; j++)
			fprintf(fp, "%.16g ", model_->w[i*nr_w+j]);
		fprintf(fp, "\n");
	}

	if (ferror(fp) != 0 || fclose(fp) != 0) return -1;
	else return 0;
}

struct model *load_model(const char *model_file_name)
{
	FILE *fp = fopen(model_file_name,"r");
	if(fp==NULL) return NULL;

	int i;
	int nr_feature;
	int n;
	int nr_class;
	double bias;
	model *model_ = Malloc(model,1);
	parameter& param = model_->param;

	model_->label = NULL;

	char cmd[81];
	while(1)
	{
		fscanf(fp,"%80s",cmd);
		if(strcmp(cmd,"solver_type")==0)
		{
			fscanf(fp,"%80s",cmd);
			int i;
			for(i=0;solver_type_table[i];i++)
			{
				if(strcmp(solver_type_table[i],cmd)==0)
				{
					param.solver_type=i;
					break;
				}
			}
			if(solver_type_table[i] == NULL)
			{
				fprintf(stderr,"unknown solver type.\n");
				free(model_->label);
				free(model_);
				return NULL;
			}
		}
		else if(strcmp(cmd,"nr_class")==0)
		{
			fscanf(fp,"%d",&nr_class);
			model_->nr_class=nr_class;
		}
		else if(strcmp(cmd,"nr_feature")==0)
		{
			fscanf(fp,"%d",&nr_feature);
			model_->nr_feature=nr_feature;
		}
		else if(strcmp(cmd,"bias")==0)
		{
			fscanf(fp,"%lf",&bias);
			model_->bias=bias;
		}
		else if(strcmp(cmd,"w")==0)
		{
			break;
		}
		else if(strcmp(cmd,"label")==0)
		{
			int nr_class = model_->nr_class;
			model_->label = Malloc(int,nr_class);
			for(int i=0;i<nr_class;i++)
				fscanf(fp,"%d",&model_->label[i]);
		}
		else
		{
			fprintf(stderr,"unknown text in model file: [%s]\n",cmd);
			free(model_);
			return NULL;
		}
	}

	nr_feature=model_->nr_feature;
	if(model_->bias>=0)
		n=nr_feature+1;
	else
		n=nr_feature;

	int nr_w;
	if(nr_class==2 && param.solver_type != MCSVM_CS)
		nr_w = 1;
	else
		nr_w = nr_class;


	model_->w=Malloc(double, n*nr_w);
	for(i=0; i<n; i++)
	{
		int j;
		for(j=0; j<nr_w; j++)
			fscanf(fp, "%lf ", &model_->w[i*nr_w+j]);
		fscanf(fp, "\n");
	}
	if (ferror(fp) != 0 || fclose(fp) != 0) return NULL;

	return model_;
}

struct model *load_model_poly(const char *model_file_name)
{
	FILE *fp = fopen(model_file_name,"r");
	if(fp==NULL) return NULL;

	int i;
	int nr_feature;
	int n;
	int nr_class;
	double bias;
	int t;
	int B;
	int flag_poly;
	double coef0;
	double gamma;
	int feature_pair;
	double run_time;

	model *model_ = Malloc(model,1);
	parameter& param = model_->param;

	model_->label = NULL;
	model_->w = NULL;
	model_->count = NULL;
	model_->w2s = NULL;
	model_->alpha = NULL;
	model_->sigma = NULL;
	model_->solution_ = Malloc(Solution,1); //EDIT-Poojit: Solution
	model_->solution_->w_FGM_retrain = NULL;
	model_->solution_->w_FGM_B = NULL;
	param.flag_poly = 0;
	char cmd[81];
	while(1)
	{
		fscanf(fp,"%80s",cmd);
		if(strcmp(cmd,"solver_type")==0)
		{
			fscanf(fp,"%80s",cmd);
			int i;
			for(i=0;solver_type_table[i];i++)
			{
				if(strcmp(solver_type_table[i],cmd)==0)
				{
					param.solver_type=i;
					break;
				}
			}
			if(solver_type_table[i] == NULL)
			{
				fprintf(stderr,"unknown solver type.\n");
				free(model_->label);
				free(model_);
				return NULL;
			}
		}
		else if(strcmp(cmd,"nr_class")==0)
		{
			fscanf(fp,"%d",&nr_class);
			model_->nr_class=nr_class;
		}
		else if(strcmp(cmd,"nr_feature")==0)
		{
			fscanf(fp,"%d",&nr_feature);
			model_->nr_feature=nr_feature;
		}
		else if(strcmp(cmd,"bias")==0)
		{
			fscanf(fp,"%lf",&bias);
			model_->bias=bias;
		}
		else if(strcmp(cmd,"B")==0)
		{
			fscanf(fp,"%d",&B);
			model_->B = B;
		}
		else if(strcmp(cmd,"flag_poly")==0)
		{
			fscanf(fp,"%d",&flag_poly);
			param.flag_poly = flag_poly;
		}
		else if(strcmp(cmd,"coef0")==0)
		{
			fscanf(fp,"%lf",&coef0);
			param.coef0 = coef0;
		}
		else if(strcmp(cmd,"gamma")==0)
		{
			fscanf(fp,"%lf",&gamma);
			param.gamma = gamma;
		}
		else if(strcmp(cmd,"t")==0)
		{
			fscanf(fp,"%d",&t);
			param.t = t;
		}
		else if(strcmp(cmd,"feature_pair")==0)
		{
			fscanf(fp,"%d",&feature_pair);
			model_->feature_pair = feature_pair;
		}
		else if(strcmp(cmd,"train_time")==0)
		{
			fscanf(fp,"%lf",&run_time);
			model_->run_time = run_time;
		}
		else if(strcmp(cmd,"w")==0)
		{
			fscanf(fp,"%d",&feature_pair);
			break;
		}
		else if(strcmp(cmd,"label")==0)
		{
			int nr_class = model_->nr_class;
			model_->label = Malloc(int,nr_class);
			for(int i=0;i<nr_class;i++)
				fscanf(fp,"%d",&model_->label[i]);
		}
		else
		{
			fprintf(stderr,"unknown text in model file: [%s]\n",cmd);
			free(model_);
			return NULL;
		}
	}
	nr_feature=model_->nr_feature;
	if(model_->bias>=0)
		n=nr_feature+1;
	else
		n=nr_feature;
	if(flag_poly ==1)
	{
		int w_size = (n+2)*(n+1)/2;
	}else
	{
		int w_size = n;
	}
	int nr_w;
	if(nr_class==2 && param.solver_type != MCSVM_CS)
		nr_w = 1;
	else
		nr_w = nr_class;
	model_->solution_->w_FGM=Malloc(weight, model_->feature_pair+1);

	for(i=0; i<model_->feature_pair; i++)
	{
		fscanf(fp, "%d %d:%lf", &model_->solution_->w_FGM[i].index1,
			&model_->solution_->w_FGM[i].index2,&model_->solution_->w_FGM[i].value);
		fscanf(fp, "\n");
	}
	model_->solution_->w_FGM[i].index1 = -1;
	if (param.t ==1)
	{
		model_->solution_->w_FGM_retrain = Malloc(weight, model_->feature_pair+1);
		model_->solution_->w_FGM_B = Malloc(weight, model_->B+1);
		for(i=0; i<model_->feature_pair; i++)
		{
			fscanf(fp, "%d %d:%lf", &model_->solution_->w_FGM_retrain[i].index1,
				&model_->solution_->w_FGM_retrain[i].index2,&model_->solution_->w_FGM_retrain[i].value);
			fscanf(fp, "\n");
		}
		model_->solution_->w_FGM_retrain[i].index1 = -1;

		for(i=0; i<model_->B; i++)
		{
			fscanf(fp, "%d %d:%lf", &model_->solution_->w_FGM_B[i].index1,
				&model_->solution_->w_FGM_B[i].index2,&model_->solution_->w_FGM_B[i].value);
			fscanf(fp, "\n");
		}
		model_->solution_->w_FGM_B[i].index1 = -1;
	}
	if (ferror(fp) != 0 || fclose(fp) != 0) return NULL;

	return model_;
}

int predict_values(const struct model *model_, const struct feature_node *x, double *dec_values)
{
	int idx;
	int n;
	if(model_->bias>=0)
		n=model_->nr_feature+1;
	else
		n=model_->nr_feature;
	double *w=model_->w;
	int nr_class=model_->nr_class;
	int i;
	int nr_w;
	if(nr_class==2 && model_->param.solver_type != MCSVM_CS)
		nr_w = 1;
	else
		nr_w = nr_class;

	const feature_node *lx=x;
	if (model_->param.flag_poly==1)
	{
		int idx2;
		const feature_node *lx2;
		double *tmp_values = Malloc(double, nr_w);
		double coef0 = model_->param.coef0;
		double gamma = model_->param.gamma;
		double sqrt2 = sqrt(2.0);
		double sqrt2_coef0_g = sqrt2*sqrt(coef0*gamma);
		double sqrt2_g = sqrt2*gamma;


		for(i=0;i<nr_w;i++)
			dec_values[i] = w[i]*coef0;
		for(; (idx=lx->index)!=-1; lx++)
			// the dimension of testing data may exceed that of training
			if(idx<=n)
			{
				for (i=0; i<nr_w; i++)
					tmp_values[i] = 0;
				lx2 = lx+1;
				int w_idx = (idx*(2*n-idx+1))/2;
				for(; (idx2=lx2->index)!=-1; lx2++)
					if(idx2<=n)
						for(i=0; i<nr_w;i++)
							tmp_values[i] += w[(w_idx+idx2)*nr_w+i]*lx2->value;
				for(i=0; i<nr_w; i++)
				{
					tmp_values[i] *= sqrt2_g;
					tmp_values[i] += w[(w_idx+idx)*nr_w+i]*(lx->value)*gamma;
					tmp_values[i] += w[idx*nr_w+i]*sqrt2_coef0_g;
					dec_values[i] += tmp_values[i]*(lx->value);
				}
			}
			free(tmp_values);
	}else
	{
		for(i=0;i<nr_w;i++)
			dec_values[i] = 0;
		for(; (idx=lx->index)!=-1; lx++)
		{
			// the dimension of testing data may exceed that of training
			if(idx<=n)
				for(i=0;i<nr_w;i++)
					dec_values[i] += w[(idx-1)*nr_w+i]*lx->value;
		}
	}

	//for(i=0;i<nr_w;i++)
	//	dec_values[i] = 0;
	//for(; (idx=lx->index)!=-1; lx++)
	//{
	//	// the dimension of testing data may exceed that of training
	//	if(idx<=n)
	//		for(i=0;i<nr_w;i++)
	//			dec_values[i] += w[(idx-1)*nr_w+i]*lx->value;
	//}

	if(nr_class==2)
	{
		if (dec_values[0]>0)
		{
			return model_->label[0];
		}else if (dec_values[0]<0)
		{
			return model_->label[1];
		}else
		{
			return (dec_values[0]>0)?model_->label[0]:model_->label[1];
			//return (model_->count[0]>model_->count[1])?model_->label[0]:model_->label[1];
		}
	}
	else
	{
		int dec_max_idx = 0;
		for(i=1;i<nr_class;i++)
		{
			if(dec_values[i] > dec_values[dec_max_idx])
				dec_max_idx = i;
		}
		return model_->label[dec_max_idx];
	}
}
int predict_values_poly(const struct model *model_, const struct feature_node *x, double *dec_values, int flag=0)
{
	int n;
	if(model_->bias>=0)
		n=model_->nr_feature+1;
	else
		n=model_->nr_feature;
	double *w=model_->w;
	int nr_class=model_->nr_class;
	int i;
	int nr_w;
	if(nr_class==2 && model_->param.solver_type != MCSVM_CS)
		nr_w = 1;
	else
		nr_w = nr_class;

	const feature_node *lx=x;

	double *tmp_values = Malloc(double, nr_w);
	double coef0 = model_->param.coef0;
	double gamma = model_->param.gamma;
	double sqrt2 = sqrt(2.0);
	double sqrt2_coef0_g = sqrt2*sqrt(coef0*gamma);
	double sqrt2_g = sqrt2*gamma;

	int iw_temp=0;
	int cursor1 =0;
	int cursor2 =0;
	int iw;
	int jw;
	const feature_node *xi;
	const feature_node *xj;
	tmp_values[0] = 0;
	dec_values[0] = 0;
	int i_start = 0;
	int j_start = 0;
	weight *w_FGM;
	int num_w;
	int bias = 0;
	if (model_->param.flag_poly==0)
	{
		sqrt2_coef0_g = 1.0;
	}

	if (flag==0)
	{
		w_FGM = model_->solution_->w_FGM;
		num_w = model_->feature_pair;
	}
	else if (flag ==1)
	{
		w_FGM = model_->solution_->w_FGM_retrain;
		num_w = model_->feature_pair;
	}
	else
	{
		w_FGM = model_->solution_->w_FGM_B;
		num_w = model_->B+1;
		bias = 0;
	}
	if (bias)
	{
		for(i=0;i<num_w;i++)
		{
			iw = w_FGM[i].index1;
			jw = w_FGM[i].index2;
			if (iw ==-2)
			{
				tmp_values[0] = tmp_values[0] + w_FGM[i].value*1;
				continue;
			}
			if (jw==-1)// this is for the linear features where w2b[iw].index2==-1
			{
				//record the linear feature here
				xi = x;
				i_start = cursor1; //record the position of the array

				while(xi[i_start].index-1<iw&&xi[i_start].index!=-1)//find the corresponding xi and xj
				{
					i_start++;
				}
				if (xi[i_start].index==-1)
				{
					continue;
				}

				if (xi[i_start].index-1==iw)
				{
					tmp_values[0] = tmp_values[0] + w_FGM[i].value*xi[i_start].value*sqrt2_coef0_g;
				}
				cursor1 = i_start;
				iw_temp = iw;
			}
			else
			{
				if(iw>iw_temp) //if i changes
				{
					cursor2 = cursor1; //record the position of the array
				}
				//quadratic term

				xi = x;
				xj = x;
				i_start = cursor1; //record the position of the array
				j_start = cursor2;

				while(xi[i_start].index-1<iw&&xi[i_start].index!=-1)//find the corresponding xi and xj
				{
					i_start++;
				}
				if (xi[i_start].index==-1)
				{
					continue;
				}
				while(xj[j_start].index-1<jw&&xj[j_start].index!=-1)//find the corresponding xi and xj
				{
					j_start++;
				}
				if (xj[j_start].index==-1)
				{
					continue;
				}

				if (xi[i_start].index-1==iw&&xj[j_start].index-1==jw)
				{
					if (iw == jw)
					{
						tmp_values[0] = tmp_values[0] + w_FGM[i].value*xj[j_start].value *xi[i_start].value *gamma;
					}
					else
					{
						tmp_values[0] = tmp_values[0] + w_FGM[i].value*xj[j_start].value *xi[i_start].value*sqrt2_g;
					}
					j_start++;
					//iw++;
				}
				cursor1 = i_start;
				cursor2 = j_start;

				iw_temp = iw;
			} //end for else
		}
	}
	else
	{
		for(i=0;i<num_w;i++)
		{
			iw = w_FGM[i].index1;
			jw = w_FGM[i].index2;

			if (jw==-1)// this is for the linear features where w2b[iw].index2==-1
			{
				//record the linear feature here
				xi = x;
				i_start = cursor1; //record the position of the array

				while(xi[i_start].index-1<iw&&xi[i_start].index!=-1)//find the corresponding xi and xj
				{
					i_start++;
				}
				if (xi[i_start].index==-1)
				{
					continue;
				}

				if (xi[i_start].index-1==iw)
				{
					tmp_values[0] = tmp_values[0] + w_FGM[i].value*xi[i_start].value*sqrt2_coef0_g;
				}
				cursor1 = i_start;
				iw_temp = iw;
			}
			else
			{
				if(iw>iw_temp) //if i changes
				{
					cursor2 = cursor1; //record the position of the array
				}
				//quadratic term

				xi = x;
				xj = x;
				i_start = cursor1; //record the position of the array
				j_start = cursor2;

				while(xi[i_start].index-1<iw&&xi[i_start].index!=-1)//find the corresponding xi and xj
				{
					i_start++;
				}
				if (xi[i_start].index==-1)
				{
					continue;
				}
				while(xj[j_start].index-1<jw&&xj[j_start].index!=-1)//find the corresponding xi and xj
				{
					j_start++;
				}
				if (xj[j_start].index==-1)
				{
					continue;
				}

				if (xi[i_start].index-1==iw&&xj[j_start].index-1==jw)
				{
					if (iw == jw)
					{
						tmp_values[0] = tmp_values[0] + w_FGM[i].value*xj[j_start].value *xi[i_start].value *gamma;
					}
					else
					{
						tmp_values[0] = tmp_values[0] + w_FGM[i].value*xj[j_start].value *xi[i_start].value*sqrt2_g;
					}
					j_start++;
					//iw++;
				}
				cursor1 = i_start;
				cursor2 = j_start;

				iw_temp = iw;
			} //end for else
		}
	}

	dec_values[0] = tmp_values[0];
	free(tmp_values);
	//else
	//	{
	//		for(i=0;i<nr_w;i++)
	//			dec_values[i] = 0;
	//		for(; (idx=lx->index)!=-1; lx++)
	//		{
	//			// the dimension of testing data may exceed that of training
	//			if(idx<=n)
	//				for(i=0;i<nr_w;i++)
	//					dec_values[i] += w[(idx-1)*nr_w+i]*lx->value;
	//		}
	//	}

	//for(i=0;i<nr_w;i++)
	//	dec_values[i] = 0;
	//for(; (idx=lx->index)!=-1; lx++)
	//{
	//	// the dimension of testing data may exceed that of training
	//	if(idx<=n)
	//		for(i=0;i<nr_w;i++)
	//			dec_values[i] += w[(idx-1)*nr_w+i]*lx->value;
	//}

	if(nr_class==2)
	{
		if (dec_values[0]>0)
		{
			return model_->label[0];
		}else if (dec_values[0]<=0)
		{
			return model_->label[1];
		}
		//}else
		//{
		//	return 0;
		//	//return (model_->count[0]>model_->count[1])?model_->label[0]:model_->label[1];(dec_values[0]>0)?model_->label[0]:model_->label[1]
		//}
		//return (dec_values[0]>0)?model_->label[0]:model_->label[1];
	}
	else
	{
		int dec_max_idx = 0;
		for(i=1;i<nr_class;i++)
		{
			if(dec_values[i] > dec_values[dec_max_idx])
				dec_max_idx = i;
		}
		return model_->label[dec_max_idx];
	}
}

int predict(const model *model_, const feature_node *x)
{
	double *dec_values = Malloc(double, model_->nr_class);
	int label=predict_values(model_, x, dec_values);
	free(dec_values);
	return label;
}
int predict_poly(const model *model_, const feature_node *x, int flag=0)
{
	double *dec_values = Malloc(double, model_->nr_class);
	int label=predict_values_poly(model_, x, dec_values, flag);
	free(dec_values);
	return label;
}

int predict_poly_value(const model *model_, const feature_node *x, double &value,int flag=0)
{
	//double value;
	double *dec_values = Malloc(double, model_->nr_class);
	int label=predict_values_poly(model_, x, dec_values, flag);
	value = dec_values[0];
	free(dec_values);
	return label;
}
//int predict_probability(const struct model *model_, const struct feature_node *x, double* prob_estimates)
//{
//	if(model_->param.solver_type==L2_LR)
//	{
//		int i;
//		int nr_class=model_->nr_class;
//		int nr_w;
//		if(nr_class==2)
//			nr_w = 1;
//		else
//			nr_w = nr_class;
//
//		int label=predict_values(model_, x, prob_estimates);
//		for(i=0;i<nr_w;i++)
//			prob_estimates[i]=1/(1+exp(-prob_estimates[i]));
//
//		if(nr_class==2) // for binary classification
//			prob_estimates[1]=1.-prob_estimates[0];
//		else
//		{
//			double sum=0;
//			for(i=0; i<nr_class; i++)
//				sum+=prob_estimates[i];
//
//			for(i=0; i<nr_class; i++)
//				prob_estimates[i]=prob_estimates[i]/sum;
//		}
//
//		return label;
//	}
//	else
//		return 0;
//}

void destroy_param(parameter* param)
{
	if(param->weight_label != NULL)
		free(param->weight_label);
	if(param->weight != NULL)
		free(param->weight);
}

const char *check_parameter(const problem *prob, const parameter *param)
{
	if(param->eps <= 0)
		return "eps <= 0";

	if(param->C <= 0)
		return "C <= 0";

	//L2R_LR, L2R_L2LOSS_SVC_DUAL, L2R_L2LOSS_SVC, L2R_L1LOSS_SVC_DUAL, MCSVM_CS, L1R_L2LOSS_SVC, L1R_LR, L2R_LR_DUAL,
	if(param->solver_type != L2R_LR
		&& param->solver_type != L2R_L2LOSS_SVC_DUAL
		&& param->solver_type != L2R_L2LOSS_SVC
		&& param->solver_type != L2R_L1LOSS_SVC_DUAL
		&& param->solver_type != MCSVM_CS
		&& param->solver_type != L1R_L2LOSS_SVC
		&& param->solver_type != L1R_LR
		&& param->solver_type != L2R_LR_DUAL
		&& param->solver_type != SVMFGM
		&& param->solver_type != PSVMFGM
		&& param->solver_type != LRFGM
		&& param->solver_type != MINLR
		&& param->solver_type != PROXFGM
		&& param->solver_type != PROXFGMLR
		&& param->solver_type != STOCHFGM
		&& param->solver_type != STOCHLR
		&& param->solver_type != OMPLR
		&& param->solver_type != OMPL2)
		return "unknown solver type";

	return NULL;
}

void cross_validation(const problem *prob, const parameter *param, int nr_fold, int *target)
{
	int i;
	int *fold_start = Malloc(int,nr_fold+1);
	int l = prob->l;
	int *perm = Malloc(int,l);

	for(i=0;i<l;i++) perm[i]=i;
	for(i=0;i<l;i++)
	{
		int j = i+rand()%(l-i);
		swap(perm[i],perm[j]);
	}
	for(i=0;i<=nr_fold;i++)
		fold_start[i]=i*l/nr_fold;

	for(i=0;i<nr_fold;i++)
	{
		int begin = fold_start[i];
		int end = fold_start[i+1];
		int j,k;
		struct problem subprob;

		subprob.bias = prob->bias;
		subprob.n = prob->n;
		subprob.l = l-(end-begin);
		subprob.x = Malloc(struct feature_node*,subprob.l);
		subprob.y = Malloc(int,subprob.l);

		k=0;
		for(j=0;j<begin;j++)
		{
			subprob.x[k] = prob->x[perm[j]];
			subprob.y[k] = prob->y[perm[j]];
			++k;
		}
		for(j=end;j<l;j++)
		{
			subprob.x[k] = prob->x[perm[j]];
			subprob.y[k] = prob->y[perm[j]];
			++k;
		}
		//struct model *submodel = train(&subprob,param);
		//needs to be further modefied
		struct model *submodel;
		for(j=begin;j<end;j++)
			target[perm[j]] = predict(submodel,prob->x[perm[j]]);
		destroy_model(submodel);
		free(subprob.x);
		free(subprob.y);
	}
	free(fold_start);
	free(perm);
}

int get_nr_feature(const model *model_)
{
	return model_->nr_feature;
}

int get_nr_class(const model *model_)
{
	return model_->nr_class;
}

void get_labels(const model *model_, int* label)
{
	if (model_->label != NULL)
		for(int i=0;i<model_->nr_class;i++)
			label[i] = model_->label[i];
}



class l2r_lr_fun : public function
{
public:
	l2r_lr_fun(const problem *prob, double Cp, double Cn);
	~l2r_lr_fun();

	double fun(double *w);
	void grad(double *w, double *g);
	void Hv(double *s, double *Hs);

	int get_nr_variable(void);

private:
	void Xv(double *v, double *Xv);
	void XTv(double *v, double *XTv);

	double *C;
	double *z;
	double *D;
	const problem *prob;
};

l2r_lr_fun::l2r_lr_fun(const problem *prob, double Cp, double Cn)
{
	int i;
	int l=prob->l;
	int *y=prob->y;

	this->prob = prob;

	z = new double[l];
	D = new double[l];
	C = new double[l];

	for (i=0; i<l; i++)
	{
		if (y[i] == 1)
			C[i] = Cp;
		else
			C[i] = Cn;
	}
}

l2r_lr_fun::~l2r_lr_fun()
{
	delete[] z;
	delete[] D;
	delete[] C;
}


double l2r_lr_fun::fun(double *w)
{
	int i;
	double f=0;
	int *y=prob->y;
	int l=prob->l;
	int w_size=get_nr_variable();

	Xv(w, z);
	for(i=0;i<l;i++)
	{
		double yz = y[i]*z[i];
		if (yz >= 0)
			f += C[i]*log(1 + exp(-yz));
		else
			f += C[i]*(-yz+log(1 + exp(yz)));
	}
	f = 2*f;
	for(i=0;i<w_size;i++)
		f += w[i]*w[i];
	f /= 2.0;

	return(f);
}

void l2r_lr_fun::grad(double *w, double *g)
{
	int i;
	int *y=prob->y;
	int l=prob->l;
	int w_size=get_nr_variable();

	for(i=0;i<l;i++)
	{
		z[i] = 1/(1 + exp(-y[i]*z[i]));
		D[i] = z[i]*(1-z[i]);
		z[i] = C[i]*(z[i]-1)*y[i];
	}
	XTv(z, g);

	for(i=0;i<w_size;i++)
		g[i] = w[i] + g[i];
}

int l2r_lr_fun::get_nr_variable(void)
{
	return prob->n;
}

void l2r_lr_fun::Hv(double *s, double *Hs)
{
	int i;
	int l=prob->l;
	int w_size=get_nr_variable();
	double *wa = new double[l];

	Xv(s, wa);
	for(i=0;i<l;i++)
		wa[i] = C[i]*D[i]*wa[i];

	XTv(wa, Hs);
	for(i=0;i<w_size;i++)
		Hs[i] = s[i] + Hs[i];
	delete[] wa;
}

void l2r_lr_fun::Xv(double *v, double *Xv)
{
	int i;
	int l=prob->l;
	feature_node **x=prob->x;

	for(i=0;i<l;i++)
	{
		feature_node *s=x[i];
		Xv[i]=0;
		while(s->index!=-1)
		{
			Xv[i]+=v[s->index-1]*s->value;
			s++;
		}
	}
}

void l2r_lr_fun::XTv(double *v, double *XTv)
{
	int i;
	int l=prob->l;
	int w_size=get_nr_variable();
	feature_node **x=prob->x;

	for(i=0;i<w_size;i++)
		XTv[i]=0;
	for(i=0;i<l;i++)
	{
		feature_node *s=x[i];
		while(s->index!=-1)
		{
			XTv[s->index-1]+=v[i]*s->value;
			s++;
		}
	}
}

class l2r_l2_svc_fun : public function
{
public:
	l2r_l2_svc_fun(const problem *prob, double Cp, double Cn);
	~l2r_l2_svc_fun();

	double fun(double *w);
	void grad(double *w, double *g);
	void Hv(double *s, double *Hs);

	int get_nr_variable(void);

private:
	void Xv(double *v, double *Xv);
	void subXv(double *v, double *Xv);
	void subXTv(double *v, double *XTv);

	double *C;
	double *z;
	double *D;
	int *I;
	int sizeI;
	const problem *prob;
};

l2r_l2_svc_fun::l2r_l2_svc_fun(const problem *prob, double Cp, double Cn)
{
	int i;
	int l=prob->l;
	int *y=prob->y;

	this->prob = prob;

	z = new double[l];
	D = new double[l];
	C = new double[l];
	I = new int[l];

	for (i=0; i<l; i++)
	{
		if (y[i] == 1)
			C[i] = Cp;
		else
			C[i] = Cn;
	}
}

l2r_l2_svc_fun::~l2r_l2_svc_fun()
{
	delete[] z;
	delete[] D;
	delete[] C;
	delete[] I;
}

double l2r_l2_svc_fun::fun(double *w)
{
	int i;
	double f=0;
	int *y=prob->y;
	int l=prob->l;
	int w_size=get_nr_variable();

	Xv(w, z);
	for(i=0;i<l;i++)
	{
		z[i] = y[i]*z[i];
		double d = 1-z[i];
		if (d > 0)
			f += C[i]*d*d;
	}
	f = 2*f;
	for(i=0;i<w_size;i++)
		f += w[i]*w[i];
	f /= 2.0;

	return(f);
}

void l2r_l2_svc_fun::grad(double *w, double *g)
{
	int i;
	int *y=prob->y;
	int l=prob->l;
	int w_size=get_nr_variable();

	sizeI = 0;
	for (i=0;i<l;i++)
		if (z[i] < 1)
		{
			z[sizeI] = C[i]*y[i]*(z[i]-1);
			I[sizeI] = i;
			sizeI++;
		}
		subXTv(z, g);

		for(i=0;i<w_size;i++)
			g[i] = w[i] + 2*g[i];
}

int l2r_l2_svc_fun::get_nr_variable(void)
{
	return prob->n;
}

void l2r_l2_svc_fun::Hv(double *s, double *Hs)
{
	int i;
	int l=prob->l;
	int w_size=get_nr_variable();
	double *wa = new double[l];

	subXv(s, wa);
	for(i=0;i<sizeI;i++)
		wa[i] = C[I[i]]*wa[i];

	subXTv(wa, Hs);
	for(i=0;i<w_size;i++)
		Hs[i] = s[i] + 2*Hs[i];
	delete[] wa;
}

void l2r_l2_svc_fun::Xv(double *v, double *Xv)
{
	int i;
	int l=prob->l;
	feature_node **x=prob->x;

	for(i=0;i<l;i++)
	{
		feature_node *s=x[i];
		Xv[i]=0;
		while(s->index!=-1)
		{
			Xv[i]+=v[s->index-1]*s->value;
			s++;
		}
	}
}

void l2r_l2_svc_fun::subXv(double *v, double *Xv)
{
	int i;
	feature_node **x=prob->x;

	for(i=0;i<sizeI;i++)
	{
		feature_node *s=x[I[i]];
		Xv[i]=0;
		while(s->index!=-1)
		{
			Xv[i]+=v[s->index-1]*s->value;
			s++;
		}
	}
}

void l2r_l2_svc_fun::subXTv(double *v, double *XTv)
{
	int i;
	int w_size=get_nr_variable();
	feature_node **x=prob->x;

	for(i=0;i<w_size;i++)
		XTv[i]=0;
	for(i=0;i<sizeI;i++)
	{
		feature_node *s=x[I[i]];
		while(s->index!=-1)
		{
			XTv[s->index-1]+=v[i]*s->value;
			s++;
		}
	}
}

// A coordinate descent algorithm for
// multi-class support vector machines by Crammer and Singer
//
//  min_{\alpha}  0.5 \sum_m ||w_m(\alpha)||^2 + \sum_i \sum_m e^m_i alpha^m_i
//    s.t.     \alpha^m_i <= C^m_i \forall m,i , \sum_m \alpha^m_i=0 \forall i
//
//  where e^m_i = 0 if y_i  = m,
//        e^m_i = 1 if y_i != m,
//  C^m_i = C if m  = y_i,
//  C^m_i = 0 if m != y_i,
//  and w_m(\alpha) = \sum_i \alpha^m_i x_i
//
// Given:
// x, y, C
// eps is the stopping tolerance
//
// solution will be put in w
//
// See Appendix of LIBLINEAR paper, Fan et al. (2008)

#define GETI(i) (prob->y[i])
// To support weights for instances, use GETI(i) (i)

class Solver_MCSVM_CS
{
public:
	Solver_MCSVM_CS(const problem *prob, int nr_class, double *C, double eps=0.1, int max_iter=100000);
	~Solver_MCSVM_CS();
	void Solve(double *w);
private:
	void solve_sub_problem(double A_i, int yi, double C_yi, int active_i, double *alpha_new);
	bool be_shrunk(int i, int m, int yi, double alpha_i, double minG);
	double *B, *C, *G;
	int w_size, l;
	int nr_class;
	int max_iter;
	double eps;
	const problem *prob;
};

Solver_MCSVM_CS::Solver_MCSVM_CS(const problem *prob, int nr_class, double *weighted_C, double eps, int max_iter)
{
	this->w_size = prob->n;
	this->l = prob->l;
	this->nr_class = nr_class;
	this->eps = eps;
	this->max_iter = max_iter;
	this->prob = prob;
	this->B = new double[nr_class];
	this->G = new double[nr_class];
	this->C = weighted_C;
}

Solver_MCSVM_CS::~Solver_MCSVM_CS()
{
	delete[] B;
	delete[] G;
}

int compare_double(const void *a, const void *b)
{
	if(*(double *)a > *(double *)b)
		return -1;
	if(*(double *)a < *(double *)b)
		return 1;
	return 0;
}

void Solver_MCSVM_CS::solve_sub_problem(double A_i, int yi, double C_yi, int active_i, double *alpha_new)
{
	int r;
	double *D;

	clone(D, B, active_i);
	if(yi < active_i)
		D[yi] += A_i*C_yi;
	qsort(D, active_i, sizeof(double), compare_double);

	double beta = D[0] - A_i*C_yi;
	for(r=1;r<active_i && beta<r*D[r];r++)
		beta += D[r];

	beta /= r;
	for(r=0;r<active_i;r++)
	{
		if(r == yi)
			alpha_new[r] = min(C_yi, (beta-B[r])/A_i);
		else
			alpha_new[r] = min((double)0, (beta - B[r])/A_i);
	}
	delete[] D;
}

bool Solver_MCSVM_CS::be_shrunk(int i, int m, int yi, double alpha_i, double minG)
{
	double bound = 0;
	if(m == yi)
		bound = C[GETI(i)];
	if(alpha_i == bound && G[m] < minG)
		return true;
	return false;
}

void Solver_MCSVM_CS::Solve(double *w)
{
	int i, m, s;
	int iter = 0;
	double *alpha =  new double[l*nr_class];
	double *alpha_new = new double[nr_class];
	int *index = new int[l];
	double *QD = new double[l];
	int *d_ind = new int[nr_class];
	double *d_val = new double[nr_class];
	int *alpha_index = new int[nr_class*l];
	int *y_index = new int[l];
	int active_size = l;
	int *active_size_i = new int[l];
	double eps_shrink = max(10.0*eps, 1.0); // stopping tolerance for shrinking
	bool start_from_all = true;
	// initial
	for(i=0;i<l*nr_class;i++)
		alpha[i] = 0;
	for(i=0;i<w_size*nr_class;i++)
		w[i] = 0;
	for(i=0;i<l;i++)
	{
		for(m=0;m<nr_class;m++)
			alpha_index[i*nr_class+m] = m;
		feature_node *xi = prob->x[i];
		QD[i] = 0;
		while(xi->index != -1)
		{
			QD[i] += (xi->value)*(xi->value);
			xi++;
		}
		active_size_i[i] = nr_class;
		y_index[i] = prob->y[i];
		index[i] = i;
	}

	while(iter < max_iter)
	{
		double stopping = -INF;
		for(i=0;i<active_size;i++)
		{
			int j = i+rand()%(active_size-i);
			swap(index[i], index[j]);
		}
		for(s=0;s<active_size;s++)
		{
			i = index[s];
			double Ai = QD[i];
			double *alpha_i = &alpha[i*nr_class];
			int *alpha_index_i = &alpha_index[i*nr_class];

			if(Ai > 0)
			{
				for(m=0;m<active_size_i[i];m++)
					G[m] = 1;
				if(y_index[i] < active_size_i[i])
					G[y_index[i]] = 0;

				feature_node *xi = prob->x[i];
				while(xi->index!= -1)
				{
					double *w_i = &w[(xi->index-1)*nr_class];
					for(m=0;m<active_size_i[i];m++)
						G[m] += w_i[alpha_index_i[m]]*(xi->value);
					xi++;
				}

				double minG = INF;
				double maxG = -INF;
				for(m=0;m<active_size_i[i];m++)
				{
					if(alpha_i[alpha_index_i[m]] < 0 && G[m] < minG)
						minG = G[m];
					if(G[m] > maxG)
						maxG = G[m];
				}
				if(y_index[i] < active_size_i[i])
					if(alpha_i[prob->y[i]] < C[GETI(i)] && G[y_index[i]] < minG)
						minG = G[y_index[i]];

				for(m=0;m<active_size_i[i];m++)
				{
					if(be_shrunk(i, m, y_index[i], alpha_i[alpha_index_i[m]], minG))
					{
						active_size_i[i]--;
						while(active_size_i[i]>m)
						{
							if(!be_shrunk(i, active_size_i[i], y_index[i],
								alpha_i[alpha_index_i[active_size_i[i]]], minG))
							{
								swap(alpha_index_i[m], alpha_index_i[active_size_i[i]]);
								swap(G[m], G[active_size_i[i]]);
								if(y_index[i] == active_size_i[i])
									y_index[i] = m;
								else if(y_index[i] == m)
									y_index[i] = active_size_i[i];
								break;
							}
							active_size_i[i]--;
						}
					}
				}

				if(active_size_i[i] <= 1)
				{
					active_size--;
					swap(index[s], index[active_size]);
					s--;
					continue;
				}

				if(maxG-minG <= 1e-12)
					continue;
				else
					stopping = max(maxG - minG, stopping);

				for(m=0;m<active_size_i[i];m++)
					B[m] = G[m] - Ai*alpha_i[alpha_index_i[m]] ;

				solve_sub_problem(Ai, y_index[i], C[GETI(i)], active_size_i[i], alpha_new);
				int nz_d = 0;
				for(m=0;m<active_size_i[i];m++)
				{
					double d = alpha_new[m] - alpha_i[alpha_index_i[m]];
					alpha_i[alpha_index_i[m]] = alpha_new[m];
					if(fabs(d) >= 1e-12)
					{
						d_ind[nz_d] = alpha_index_i[m];
						d_val[nz_d] = d;
						nz_d++;
					}
				}

				xi = prob->x[i];
				while(xi->index != -1)
				{
					double *w_i = &w[(xi->index-1)*nr_class];
					for(m=0;m<nz_d;m++)
						w_i[d_ind[m]] += d_val[m]*xi->value;
					xi++;
				}
			}
		}

		iter++;
		if(iter % 10 == 0)
		{
			info(".");
		}

		if(stopping < eps_shrink)
		{
			if(stopping < eps && start_from_all == true)
				break;
			else
			{
				active_size = l;
				for(i=0;i<l;i++)
					active_size_i[i] = nr_class;
				info("*");
				eps_shrink = max(eps_shrink/2, eps);
				start_from_all = true;
			}
		}
		else
			start_from_all = false;
	}

	info("\noptimization finished, #iter = %d\n",iter);
	if (iter >= max_iter)
		info("\nWARNING: reaching max number of iterations\n");

	// calculate objective value
	double v = 0;
	int nSV = 0;
	for(i=0;i<w_size*nr_class;i++)
		v += w[i]*w[i];
	v = 0.5*v;
	for(i=0;i<l*nr_class;i++)
	{
		v += alpha[i];
		if(fabs(alpha[i]) > 0)
			nSV++;
	}
	for(i=0;i<l;i++)
		v -= alpha[i*nr_class+prob->y[i]];
	info("Objective value = %lf\n",v);
	info("nSV = %d\n",nSV);

	delete [] alpha;
	delete [] alpha_new;
	delete [] index;
	delete [] QD;
	delete [] d_ind;
	delete [] d_val;
	delete [] alpha_index;
	delete [] y_index;
	delete [] active_size_i;
}

// A coordinate descent algorithm for
// L1-loss and L2-loss SVM dual problems
//
//  min_\alpha  0.5(\alpha^T (Q + D)\alpha) - e^T \alpha,
//    s.t.      0 <= alpha_i <= upper_bound_i,
//
//  where Qij = yi yj xi^T xj and
//  D is a diagonal matrix
//
// In L1-SVM case:
// 		upper_bound_i = Cp if y_i = 1
// 		upper_bound_i = Cn if y_i = -1
// 		D_ii = 0
// In L2-SVM case:
// 		upper_bound_i = INF
// 		D_ii = 1/(2*Cp)	if y_i = 1
// 		D_ii = 1/(2*Cn)	if y_i = -1
//
// Given:
// x, y, Cp, Cn
// eps is the stopping tolerance
//
// solution will be put in w
//
// See Algorithm 3 of Hsieh et al., ICML 2008

#undef GETI
#define GETI(i) (y[i]+1)
// To support weights for instances, use GETI(i) (i)

static void solve_l2r_l1l2_svc(
							   const problem *prob, double *w, double eps,
							   double Cp, double Cn, int solver_type)
{
	int l = prob->l;
	int w_size = prob->n;
	int i, s, iter = 0;
	double C, d, G;
	double *QD = new double[l];
	int max_iter = 1000;
	int *index = new int[l];
	double *alpha = new double[l];
	schar *y = new schar[l];
	int active_size = l;

	// PG: projected gradient, for shrinking and stopping
	double PG;
	double PGmax_old = INF;
	double PGmin_old = -INF;
	double PGmax_new, PGmin_new;

	// default solver_type: L2R_L2LOSS_SVC_DUAL
	double diag[3] = {0.5/Cn, 0, 0.5/Cp};
	double upper_bound[3] = {INF, 0, INF};
	if(solver_type == L2R_L1LOSS_SVC_DUAL)
	{
		diag[0] = 0;
		diag[2] = 0;
		upper_bound[0] = Cn;
		upper_bound[2] = Cp;
	}

	for(i=0; i<w_size; i++)
		w[i] = 0;
	for(i=0; i<l; i++)
	{
		alpha[i] = 0;
		if(prob->y[i] > 0)
		{
			y[i] = +1;
		}
		else
		{
			y[i] = -1;
		}
		QD[i] = diag[GETI(i)];

		feature_node *xi = prob->x[i];
		while (xi->index != -1)
		{
			QD[i] += (xi->value)*(xi->value);
			xi++;
		}
		index[i] = i;
	}

	while (iter < max_iter)
	{
		PGmax_new = -INF;
		PGmin_new = INF;

		for (i=0; i<active_size; i++)
		{
			int j = i+rand()%(active_size-i);
			swap(index[i], index[j]);
		}

		for (s=0; s<active_size; s++)
		{
			i = index[s];
			G = 0;
			schar yi = y[i];

			feature_node *xi = prob->x[i];
			while(xi->index!= -1)
			{
				G += w[xi->index-1]*(xi->value);
				xi++;
			}
			G = G*yi-1;

			C = upper_bound[GETI(i)];
			G += alpha[i]*diag[GETI(i)];

			PG = 0;
			if (alpha[i] == 0)
			{
				if (G > PGmax_old)
				{
					active_size--;
					swap(index[s], index[active_size]);
					s--;
					continue;
				}
				else if (G < 0)
					PG = G;
			}
			else if (alpha[i] == C)
			{
				if (G < PGmin_old)
				{
					active_size--;
					swap(index[s], index[active_size]);
					s--;
					continue;
				}
				else if (G > 0)
					PG = G;
			}
			else
				PG = G;

			PGmax_new = max(PGmax_new, PG);
			PGmin_new = min(PGmin_new, PG);

			if(fabs(PG) > 1.0e-12)
			{
				double alpha_old = alpha[i];
				alpha[i] = min(max(alpha[i] - G/QD[i], 0.0), C);
				d = (alpha[i] - alpha_old)*yi;
				xi = prob->x[i];
				while (xi->index != -1)
				{
					w[xi->index-1] += d*xi->value;
					xi++;
				}
			}
		}

		iter++;
		if(iter % 10 == 0)
			info(".");

		if(PGmax_new - PGmin_new <= eps)
		{
			if(active_size == l)
				break;
			else
			{
				active_size = l;
				info("*");
				PGmax_old = INF;
				PGmin_old = -INF;
				continue;
			}
		}
		PGmax_old = PGmax_new;
		PGmin_old = PGmin_new;
		if (PGmax_old <= 0)
			PGmax_old = INF;
		if (PGmin_old >= 0)
			PGmin_old = -INF;
	}

	info("\noptimization finished, #iter = %d\n",iter);
	if (iter >= max_iter)
		info("\nWARNING: reaching max number of iterations\nUsing -s 2 may be faster (also see FAQ)\n\n");

	// calculate objective value

	double v = 0;
	int nSV = 0;
	for(i=0; i<w_size; i++)
		v += w[i]*w[i];
	for(i=0; i<l; i++)
	{
		v += alpha[i]*(alpha[i]*diag[GETI(i)] - 2);
		if(alpha[i] > 0)
			++nSV;
	}
	info("Objective value = %lf\n",v/2);
	info("nSV = %d\n",nSV);

	delete [] QD;
	delete [] alpha;
	delete [] y;
	delete [] index;
}

// A coordinate descent algorithm for
// the dual of L2-regularized logistic regression problems
//
//  min_\alpha  0.5(\alpha^T Q \alpha) + \sum \alpha_i log (\alpha_i) + (upper_bound_i - alpha_i) log (upper_bound_i - alpha_i) ,
//    s.t.      0 <= alpha_i <= upper_bound_i,
//
//  where Qij = yi yj xi^T xj and
//  upper_bound_i = Cp if y_i = 1
//  upper_bound_i = Cn if y_i = -1
//
// Given:
// x, y, Cp, Cn
// eps is the stopping tolerance
//
// solution will be put in w
//
// See Algorithm 5 of Yu et al., MLJ 2010

#undef GETI
#define GETI(i) (y[i]+1)
// To support weights for instances, use GETI(i) (i)

void solve_l2r_lr_dual(const problem *prob, double *w, double eps, double Cp, double Cn)
{
	int l = prob->l;
	int w_size = prob->n;
	int i, s, iter = 0;
	double *xTx = new double[l];
	int max_iter = 1000;
	int *index = new int[l];
	double *alpha = new double[2*l]; // store alpha and C - alpha
	schar *y = new schar[l];
	int max_inner_iter = 100; // for inner Newton
	double innereps = 1e-2;
	double innereps_min = min(1e-8, eps);
	double upper_bound[3] = {Cn, 0, Cp};

	for(i=0; i<w_size; i++)
		w[i] = 0;
	for(i=0; i<l; i++)
	{
		if(prob->y[i] > 0)
		{
			y[i] = +1;
		}
		else
		{
			y[i] = -1;
		}
		alpha[2*i] = min(0.001*upper_bound[GETI(i)], 1e-8);
		alpha[2*i+1] = upper_bound[GETI(i)] - alpha[2*i];

		xTx[i] = 0;
		feature_node *xi = prob->x[i];
		while (xi->index != -1)
		{
			xTx[i] += (xi->value)*(xi->value);
			w[xi->index-1] += y[i]*alpha[2*i]*xi->value;
			xi++;
		}
		index[i] = i;
	}

	while (iter < max_iter)
	{
		for (i=0; i<l; i++)
		{
			int j = i+rand()%(l-i);
			swap(index[i], index[j]);
		}
		int newton_iter = 0;
		double Gmax = 0;
		for (s=0; s<l; s++)
		{
			i = index[s];
			schar yi = y[i];
			double C = upper_bound[GETI(i)];
			double ywTx = 0, xisq = xTx[i];
			feature_node *xi = prob->x[i];
			while (xi->index != -1)
			{
				ywTx += w[xi->index-1]*xi->value;
				xi++;
			}
			ywTx *= y[i];
			double a = xisq, b = ywTx;

			// Decide to minimize g_1(z) or g_2(z)
			int ind1 = 2*i, ind2 = 2*i+1, sign = 1;
			if(0.5*a*(alpha[ind2]-alpha[ind1])+b < 0)
			{
				ind1 = 2*i+1;
				ind2 = 2*i;
				sign = -1;
			}

			//  g_t(z) = z*log(z) + (C-z)*log(C-z) + 0.5a(z-alpha_old)^2 + sign*b(z-alpha_old)
			double alpha_old = alpha[ind1];
			double z = alpha_old;
			if(C - z < 0.5 * C)
				z = 0.1*z;
			double gp = a*(z-alpha_old)+sign*b+log(z/(C-z));
			Gmax = max(Gmax, fabs(gp));

			// Newton method on the sub-problem
			const double eta = 0.1; // xi in the paper
			int inner_iter = 0;
			while (inner_iter <= max_inner_iter)
			{
				if(fabs(gp) < innereps)
					break;
				double gpp = a + C/(C-z)/z;
				double tmpz = z - gp/gpp;
				if(tmpz <= 0)
					z *= eta;
				else // tmpz in (0, C)
					z = tmpz;
				gp = a*(z-alpha_old)+sign*b+log(z/(C-z));
				newton_iter++;
				inner_iter++;
			}

			if (inner_iter>20)
			{
				printf("inner_iter = %d\n",inner_iter);
			}

			if(inner_iter > 0) // update w
			{
				alpha[ind1] = z;
				alpha[ind2] = C-z;
				xi = prob->x[i];
				while (xi->index != -1)
				{
					w[xi->index-1] += sign*(z-alpha_old)*yi*xi->value;
					xi++;
				}
			}
			if(inner_iter > 20)
			{
				info("inner_iter = %d\n \n",inner_iter);
			}
		}

		iter++;
		if(iter % 10 == 0)
			info(".");

		if(Gmax < eps)
			break;

		if(newton_iter < l/10)
			innereps = max(innereps_min, 0.1*innereps);

	}

	info("\noptimization finished, #iter = %d\n",iter);
	if (iter >= max_iter)
		info("\nWARNING: reaching max number of iterations\nUsing -s 0 may be faster (also see FAQ)\n\n");

	// calculate objective value

	double v = 0;
	for(i=0; i<w_size; i++)
		v += w[i] * w[i];
	v *= 0.5;
	for(i=0; i<l; i++)
		v += alpha[2*i] * log(alpha[2*i]) + alpha[2*i+1] * log(alpha[2*i+1])
		- upper_bound[GETI(i)] * log(upper_bound[GETI(i)]);
	info("Objective value = %lf\n", v);

	delete [] xTx;
	delete [] alpha;
	delete [] y;
	delete [] index;
}

// A coordinate descent algorithm for
// L1-regularized L2-loss support vector classification
//
//  min_w \sum |wj| + C \sum max(0, 1-yi w^T xi)^2,
//
// Given:
// x, y, Cp, Cn
// eps is the stopping tolerance
//
// solution will be put in w
//
// See Yuan et al. (2010) and appendix of LIBLINEAR paper, Fan et al. (2008)

#undef GETI
#define GETI(i) (y[i]+1)
// To support weights for instances, use GETI(i) (i)

static void solve_l1r_l2_svc(
							 problem *prob_col, double *w, double eps,
							 double Cp, double Cn)
{
	int l = prob_col->l;
	int w_size = prob_col->n;
	int j, s, iter = 0;
	int max_iter = 1000;
	int active_size = w_size;
	int max_num_linesearch = 20;

	double sigma = 0.01;
	double d, G_loss, G, H;
	double Gmax_old = INF;
	double Gmax_new;
	double Gmax_init;
	double d_old, d_diff;
	double loss_old, loss_new;
	double appxcond, cond;

	int *index = new int[w_size];
	schar *y = new schar[l];
	double *b = new double[l]; // b = 1-ywTx
	double *xj_sq = new double[w_size];
	feature_node *x;

	double C[3] = {Cn,0,Cp};

	for(j=0; j<l; j++)
	{
		b[j] = 1;
		if(prob_col->y[j] > 0)
			y[j] = 1;
		else
			y[j] = -1;
	}
	for(j=0; j<w_size; j++)
	{
		w[j] = 0;
		index[j] = j;
		xj_sq[j] = 0;
		x = prob_col->x[j];
		while(x->index != -1)
		{
			int ind = x->index-1;
			double val = x->value;
			x->value *= y[ind]; // x->value stores yi*xij
			xj_sq[j] += C[GETI(ind)]*val*val;
			x++;
		}
	}

	while(iter < max_iter)
	{
		Gmax_new  = 0;

		for(j=0; j<active_size; j++)
		{
			int i = j+rand()%(active_size-j);
			swap(index[i], index[j]);
		}

		for(s=0; s<active_size; s++)
		{
			j = index[s];
			G_loss = 0;
			H = 0;

			x = prob_col->x[j];
			while(x->index != -1)
			{
				int ind = x->index-1;
				if(b[ind] > 0)
				{
					double val = x->value;
					double tmp = C[GETI(ind)]*val;
					G_loss -= tmp*b[ind];
					H += tmp*val;
				}
				x++;
			}
			G_loss *= 2;

			G = G_loss;
			H *= 2;
			H = max(H, 1e-12);

			double Gp = G+1;
			double Gn = G-1;
			double violation = 0;
			if(w[j] == 0)
			{
				if(Gp < 0)
					violation = -Gp;
				else if(Gn > 0)
					violation = Gn;
				else if(Gp>Gmax_old/l && Gn<-Gmax_old/l)
				{
					active_size--;
					swap(index[s], index[active_size]);
					s--;
					continue;
				}
			}
			else if(w[j] > 0)
				violation = fabs(Gp);
			else
				violation = fabs(Gn);

			Gmax_new = max(Gmax_new, violation);

			// obtain Newton direction d
			if(Gp <= H*w[j])
				d = -Gp/H;
			else if(Gn >= H*w[j])
				d = -Gn/H;
			else
				d = -w[j];

			if(fabs(d) < 1.0e-12)
				continue;

			double delta = fabs(w[j]+d)-fabs(w[j]) + G*d;
			d_old = 0;
			int num_linesearch;
			for(num_linesearch=0; num_linesearch < max_num_linesearch; num_linesearch++)
			{
				d_diff = d_old - d;
				cond = fabs(w[j]+d)-fabs(w[j]) - sigma*delta;

				appxcond = xj_sq[j]*d*d + G_loss*d + cond;
				if(appxcond <= 0)
				{
					x = prob_col->x[j];
					while(x->index != -1)
					{
						b[x->index-1] += d_diff*x->value;
						x++;
					}
					break;
				}

				if(num_linesearch == 0)
				{
					loss_old = 0;
					loss_new = 0;
					x = prob_col->x[j];
					while(x->index != -1)
					{
						int ind = x->index-1;
						if(b[ind] > 0)
							loss_old += C[GETI(ind)]*b[ind]*b[ind];
						double b_new = b[ind] + d_diff*x->value;
						b[ind] = b_new;
						if(b_new > 0)
							loss_new += C[GETI(ind)]*b_new*b_new;
						x++;
					}
				}
				else
				{
					loss_new = 0;
					x = prob_col->x[j];
					while(x->index != -1)
					{
						int ind = x->index-1;
						double b_new = b[ind] + d_diff*x->value;
						b[ind] = b_new;
						if(b_new > 0)
							loss_new += C[GETI(ind)]*b_new*b_new;
						x++;
					}
				}

				cond = cond + loss_new - loss_old;
				if(cond <= 0)
					break;
				else
				{
					d_old = d;
					d *= 0.5;
					delta *= 0.5;
				}
			}

			w[j] += d;

			// recompute b[] if line search takes too many steps
			if(num_linesearch >= max_num_linesearch)
			{
				info("#");
				for(int i=0; i<l; i++)
					b[i] = 1;

				for(int i=0; i<w_size; i++)
				{
					if(w[i]==0) continue;
					x = prob_col->x[i];
					while(x->index != -1)
					{
						b[x->index-1] -= w[i]*x->value;
						x++;
					}
				}
			}
		}

		if(iter == 0)
			Gmax_init = Gmax_new;
		iter++;
		if(iter % 10 == 0)
			info(".");

		if(Gmax_new <= eps*Gmax_init)
		{
			if(active_size == w_size)
				break;
			else
			{
				active_size = w_size;
				info("*");
				Gmax_old = INF;
				continue;
			}
		}

		Gmax_old = Gmax_new;
	}

	info("\noptimization finished, #iter = %d\n", iter);
	if(iter >= max_iter)
		info("\nWARNING: reaching max number of iterations\n");

	// calculate objective value

	double v = 0;
	int nnz = 0;
	for(j=0; j<w_size; j++)
	{
		x = prob_col->x[j];
		while(x->index != -1)
		{
			x->value *= prob_col->y[x->index-1]; // restore x->value
			x++;
		}
		if(w[j] != 0)
		{
			v += fabs(w[j]);
			nnz++;
		}
	}
	for(j=0; j<l; j++)
		if(b[j] > 0)
			v += C[GETI(j)]*b[j]*b[j];

	info("Objective value = %lf\n", v);
	info("#nonzeros/#features = %d/%d\n", nnz, w_size);

	delete [] index;
	delete [] y;
	delete [] b;
	delete [] xj_sq;
}

// A coordinate descent algorithm for
// L1-regularized logistic regression problems
//
//  min_w \sum |wj| + C \sum log(1+exp(-yi w^T xi)),
//
// Given:
// x, y, Cp, Cn
// eps is the stopping tolerance
//
// solution will be put in w
//
// See Yuan et al. (2010) and appendix of LIBLINEAR paper, Fan et al. (2008)

#undef GETI
#define GETI(i) (y[i]+1)
// To support weights for instances, use GETI(i) (i)
static void solve_l1r_lr_(
						  const problem *prob_col, double *w, double eps,
						  double Cp, double Cn)
{
	int l = prob_col->l;
	int w_size = prob_col->n;
	int j, s, newton_iter=0, iter=0;
	int max_newton_iter = 100;
	int max_iter = 1000;
	int max_num_linesearch = 20;
	int active_size;
	int QP_active_size;

	double nu = 1e-12;
	double inner_eps = 1;
	double sigma = 0.01;
	double w_norm=0, w_norm_new;
	double z, G, H;
	double Gnorm1_init;
	double Gmax_old = INF;
	double Gmax_new, Gnorm1_new;
	double QP_Gmax_old = INF;
	double QP_Gmax_new, QP_Gnorm1_new;
	double delta, negsum_xTd, cond;

	int *index = new int[w_size];
	schar *y = new schar[l];
	double *Hdiag = new double[w_size];
	double *Grad = new double[w_size];
	double *wpd = new double[w_size];
	double *xjneg_sum = new double[w_size];
	double *xTd = new double[l];
	double *exp_wTx = new double[l];
	double *exp_wTx_new = new double[l];
	double *tau = new double[l];
	double *D = new double[l];
	feature_node *x;

	double C[3] = {Cn,0,Cp};

	for(j=0; j<l; j++)
	{
		if(prob_col->y[j] > 0)
			y[j] = 1;
		else
			y[j] = -1;

		// assume initial w is 0
		exp_wTx[j] = 1;
		tau[j] = C[GETI(j)]*0.5;
		D[j] = C[GETI(j)]*0.25;
	}
	for(j=0; j<w_size; j++)
	{
		w[j] = 0;
		wpd[j] = w[j];
		index[j] = j;
		xjneg_sum[j] = 0;
		x = prob_col->x[j];
		while(x->index != -1)
		{
			int ind = x->index-1;
			if(y[ind] == -1)
				xjneg_sum[j] += C[GETI(ind)]*x->value;
			x++;
		}
	}

	while(newton_iter < max_newton_iter)
	{
		Gmax_new = 0;
		Gnorm1_new = 0;
		active_size = w_size;

		for(s=0; s<active_size; s++)
		{
			j = index[s];
			Hdiag[j] = nu;
			Grad[j] = 0;

			double tmp = 0;
			x = prob_col->x[j];
			while(x->index != -1)
			{
				int ind = x->index-1;
				Hdiag[j] += x->value*x->value*D[ind];
				tmp += x->value*tau[ind];
				x++;
			}
			Grad[j] = -tmp + xjneg_sum[j];

			double Gp = Grad[j]+1;
			double Gn = Grad[j]-1;
			double violation = 0;
			if(w[j] == 0)
			{
				if(Gp < 0)
					violation = -Gp;
				else if(Gn > 0)
					violation = Gn;
				//outer-level shrinking
				else if(Gp>Gmax_old/l && Gn<-Gmax_old/l)
				{
					active_size--;
					swap(index[s], index[active_size]);
					s--;
					continue;
				}
			}
			else if(w[j] > 0)
				violation = fabs(Gp);
			else
				violation = fabs(Gn);

			Gmax_new = max(Gmax_new, violation);
			Gnorm1_new += violation;
		}

		if(newton_iter == 0)
			Gnorm1_init = Gnorm1_new;

		if(Gnorm1_new <= eps*Gnorm1_init)
			break;

		iter = 0;
		QP_Gmax_old = INF;
		QP_active_size = active_size;

		for(int i=0; i<l; i++)
			xTd[i] = 0;

		// optimize QP over wpd
		while(iter < max_iter)
		{
			QP_Gmax_new = 0;
			QP_Gnorm1_new = 0;

			for(j=0; j<QP_active_size; j++)
			{
				int i = j+rand()%(QP_active_size-j);
				swap(index[i], index[j]);
			}

			for(s=0; s<QP_active_size; s++)
			{
				j = index[s];
				H = Hdiag[j];

				x = prob_col->x[j];
				G = Grad[j] + (wpd[j]-w[j])*nu;
				while(x->index != -1)
				{
					int ind = x->index-1;
					G += x->value*D[ind]*xTd[ind];
					x++;
				}

				double Gp = G+1;
				double Gn = G-1;
				double violation = 0;
				if(wpd[j] == 0)
				{
					if(Gp < 0)
						violation = -Gp;
					else if(Gn > 0)
						violation = Gn;
					//inner-level shrinking
					else if(Gp>QP_Gmax_old/l && Gn<-QP_Gmax_old/l)
					{
						QP_active_size--;
						swap(index[s], index[QP_active_size]);
						s--;
						continue;
					}
				}
				else if(wpd[j] > 0)
					violation = fabs(Gp);
				else
					violation = fabs(Gn);

				QP_Gmax_new = max(QP_Gmax_new, violation);
				QP_Gnorm1_new += violation;

				// obtain solution of one-variable problem
				if(Gp <= H*wpd[j])
					z = -Gp/H;
				else if(Gn >= H*wpd[j])
					z = -Gn/H;
				else
					z = -wpd[j];

				if(fabs(z) < 1.0e-12)
					continue;
				z = min(max(z,-10.0),10.0);

				wpd[j] += z;

				x = prob_col->x[j];
				while(x->index != -1)
				{
					int ind = x->index-1;
					xTd[ind] += x->value*z;
					x++;
				}
			}

			iter++;

			if(QP_Gnorm1_new <= inner_eps*Gnorm1_init)
			{
				//inner stopping
				if(QP_active_size == active_size)
					break;
				//active set reactivation
				else
				{
					QP_active_size = active_size;
					QP_Gmax_old = INF;
					continue;
				}
			}

			QP_Gmax_old = QP_Gmax_new;
		}

		if(iter >= max_iter)
			info("WARNING: reaching max number of inner iterations\n");

		delta = 0;
		w_norm_new = 0;
		for(j=0; j<w_size; j++)
		{
			delta += Grad[j]*(wpd[j]-w[j]);
			if(wpd[j] != 0)
				w_norm_new += fabs(wpd[j]);
		}
		delta += (w_norm_new-w_norm);

		negsum_xTd = 0;
		for(int i=0; i<l; i++)
			if(y[i] == -1)
				negsum_xTd += C[GETI(i)]*xTd[i];

		int num_linesearch;
		for(num_linesearch=0; num_linesearch < max_num_linesearch; num_linesearch++)
		{
			cond = w_norm_new - w_norm + negsum_xTd - sigma*delta;

			for(int i=0; i<l; i++)
			{
				double exp_xTd = exp(xTd[i]);
				exp_wTx_new[i] = exp_wTx[i]*exp_xTd;
				cond += C[GETI(i)]*log((1+exp_wTx_new[i])/(exp_xTd+exp_wTx_new[i]));
			}

			if(cond <= 0)
			{
				w_norm = w_norm_new;
				for(j=0; j<w_size; j++)
					w[j] = wpd[j];
				for(int i=0; i<l; i++)
				{
					exp_wTx[i] = exp_wTx_new[i];
					double tau_tmp = 1/(1+exp_wTx[i]);
					tau[i] = C[GETI(i)]*tau_tmp;
					D[i] = C[GETI(i)]*exp_wTx[i]*tau_tmp*tau_tmp;
				}
				break;
			}
			else
			{
				w_norm_new = 0;
				for(j=0; j<w_size; j++)
				{
					wpd[j] = (w[j]+wpd[j])*0.5;
					if(wpd[j] != 0)
						w_norm_new += fabs(wpd[j]);
				}
				delta *= 0.5;
				negsum_xTd *= 0.5;
				for(int i=0; i<l; i++)
					xTd[i] *= 0.5;
			}
		}

		// Recompute some info due to too many line search steps
		if(num_linesearch >= max_num_linesearch)
		{
			for(int i=0; i<l; i++)
				exp_wTx[i] = 0;

			for(int i=0; i<w_size; i++)
			{
				if(w[i]==0) continue;
				x = prob_col->x[i];
				while(x->index != -1)
				{
					exp_wTx[x->index-1] += w[i]*x->value;
					x++;
				}
			}

			for(int i=0; i<l; i++)
				exp_wTx[i] = exp(exp_wTx[i]);
		}

		if(iter == 1)
			inner_eps *= 0.25;

		newton_iter++;
		Gmax_old = Gmax_new;

		info("iter %3d  #CD cycles %d\n", newton_iter, iter);
	}

	info("=========================\n");
	info("optimization finished, #iter = %d\n", newton_iter);
	if(newton_iter >= max_newton_iter)
		info("WARNING: reaching max number of iterations\n");

	// calculate objective value

	double v = 0;
	int nnz = 0;
	for(j=0; j<w_size; j++)
		if(w[j] != 0)
		{
			v += fabs(w[j]);
			nnz++;
		}
		for(j=0; j<l; j++)
			if(y[j] == 1)
				v += C[GETI(j)]*log(1+1/exp_wTx[j]);
			else
				v += C[GETI(j)]*log(1+exp_wTx[j]);

		info("Objective value = %lf\n", v);
		info("#nonzeros/#features = %d/%d\n", nnz, w_size);

		delete [] index;
		delete [] y;
		delete [] Hdiag;
		delete [] Grad;
		delete [] wpd;
		delete [] xjneg_sum;
		delete [] xTd;
		delete [] exp_wTx;
		delete [] exp_wTx_new;
		delete [] tau;
		delete [] D;
}
static void solve_l1r_lr(
						 const problem *prob_col, double *w, double eps,
						 double Cp, double Cn)
{
	int l = prob_col->l;
	int w_size = prob_col->n;
	int j, s, iter = 0;
	int max_iter = 1000;
	int active_size = w_size;
	int max_num_linesearch = 20;

	double x_min = 0;
	double sigma = 0.01;
	double d, G, H;
	double Gmax_old = INF;
	double Gmax_new;
	double Gmax_init;
	double sum1, appxcond1;
	double sum2, appxcond2;
	double cond;

	int *index = new int[w_size];
	schar *y = new schar[l];
	double *exp_wTx = new double[l];
	double *exp_wTx_new = new double[l];
	double *xj_max = new double[w_size];
	double *C_sum = new double[w_size];
	double *xjneg_sum = new double[w_size];
	double *xjpos_sum = new double[w_size];
	feature_node *x;

	double C[3] = {Cn,0,Cp};

	for(j=0; j<l; j++)
	{
		exp_wTx[j] = 1;
		if(prob_col->y[j] > 0)
			y[j] = 1;
		else
			y[j] = -1;
	}
	for(j=0; j<w_size; j++)
	{
		w[j] = 0;
		index[j] = j;
		xj_max[j] = 0;
		C_sum[j] = 0;
		xjneg_sum[j] = 0;
		xjpos_sum[j] = 0;
		x = prob_col->x[j];
		while(x->index != -1)
		{
			int ind = x->index-1;
			double val = x->value;
			x_min = min(x_min, val);
			xj_max[j] = max(xj_max[j], val);
			C_sum[j] += C[GETI(ind)];
			if(y[ind] == -1)
				xjneg_sum[j] += C[GETI(ind)]*val;
			else
				xjpos_sum[j] += C[GETI(ind)]*val;
			x++;
		}
	}

	while(iter < max_iter)
	{
		Gmax_new = 0;

		for(j=0; j<active_size; j++)
		{
			int i = j+rand()%(active_size-j);
			swap(index[i], index[j]);
		}

		for(s=0; s<active_size; s++)
		{
			j = index[s];
			sum1 = 0;
			sum2 = 0;
			H = 0;

			x = prob_col->x[j];
			while(x->index != -1)
			{
				int ind = x->index-1;
				double exp_wTxind = exp_wTx[ind];
				double tmp1 = x->value/(1+exp_wTxind);
				double tmp2 = C[GETI(ind)]*tmp1;
				double tmp3 = tmp2*exp_wTxind;
				sum2 += tmp2;
				sum1 += tmp3;
				H += tmp1*tmp3;
				x++;
			}

			G = -sum2 + xjneg_sum[j];

			double Gp = G+1;
			double Gn = G-1;
			double violation = 0;
			if(w[j] == 0)
			{
				if(Gp < 0)
					violation = -Gp;
				else if(Gn > 0)
					violation = Gn;
				else if(Gp>Gmax_old/l && Gn<-Gmax_old/l)
				{
					active_size--;
					swap(index[s], index[active_size]);
					s--;
					continue;
				}
			}
			else if(w[j] > 0)
				violation = fabs(Gp);
			else
				violation = fabs(Gn);

			Gmax_new = max(Gmax_new, violation);

			// obtain Newton direction d
			if(Gp <= H*w[j])
				d = -Gp/H;
			else if(Gn >= H*w[j])
				d = -Gn/H;
			else
				d = -w[j];

			if(fabs(d) < 1.0e-12)
				continue;

			d = min(max(d,-10.0),10.0);

			double delta = fabs(w[j]+d)-fabs(w[j]) + G*d;
			int num_linesearch;
			for(num_linesearch=0; num_linesearch < max_num_linesearch; num_linesearch++)
			{
				cond = fabs(w[j]+d)-fabs(w[j]) - sigma*delta;

				if(x_min >= 0)
				{
					double tmp = exp(d*xj_max[j]);
					appxcond1 = log(1+sum1*(tmp-1)/xj_max[j]/C_sum[j])*C_sum[j] + cond - d*xjpos_sum[j];
					appxcond2 = log(1+sum2*(1/tmp-1)/xj_max[j]/C_sum[j])*C_sum[j] + cond + d*xjneg_sum[j];
					if(min(appxcond1,appxcond2) <= 0)
					{
						x = prob_col->x[j];
						while(x->index != -1)
						{
							exp_wTx[x->index-1] *= exp(d*x->value);
							x++;
						}
						break;
					}
				}

				cond += d*xjneg_sum[j];

				int i = 0;
				x = prob_col->x[j];
				while(x->index != -1)
				{
					int ind = x->index-1;
					double exp_dx = exp(d*x->value);
					exp_wTx_new[i] = exp_wTx[ind]*exp_dx;
					cond += C[GETI(ind)]*log((1+exp_wTx_new[i])/(exp_dx+exp_wTx_new[i]));
					x++; i++;
				}

				if(cond <= 0)
				{
					int i = 0;
					x = prob_col->x[j];
					while(x->index != -1)
					{
						int ind = x->index-1;
						exp_wTx[ind] = exp_wTx_new[i];
						x++; i++;
					}
					break;
				}
				else
				{
					d *= 0.5;
					delta *= 0.5;
				}
			}

			w[j] += d;

			// recompute exp_wTx[] if line search takes too many steps
			if(num_linesearch >= max_num_linesearch)
			{
				info("#");
				for(int i=0; i<l; i++)
					exp_wTx[i] = 0;

				for(int i=0; i<w_size; i++)
				{
					if(w[i]==0) continue;
					x = prob_col->x[i];
					while(x->index != -1)
					{
						exp_wTx[x->index-1] += w[i]*x->value;
						x++;
					}
				}

				for(int i=0; i<l; i++)
					exp_wTx[i] = exp(exp_wTx[i]);
			}
		}

		if(iter == 0)
			Gmax_init = Gmax_new;
		iter++;
		if(iter % 10 == 0)
			info(".");

		if(Gmax_new <= eps*Gmax_init)
		{
			if(active_size == w_size)
				break;
			else
			{
				active_size = w_size;
				info("*");
				Gmax_old = INF;
				continue;
			}
		}

		Gmax_old = Gmax_new;
	}

	info("\noptimization finished, #iter = %d\n", iter);
	if(iter >= max_iter)
		info("\nWARNING: reaching max number of iterations\n");

	// calculate objective value

	double v = 0;
	int nnz = 0;
	for(j=0; j<w_size; j++)
		if(w[j] != 0)
		{
			v += fabs(w[j]);
			nnz++;
		}
		for(j=0; j<l; j++)
			if(y[j] == 1)
				v += C[GETI(j)]*log(1+1/exp_wTx[j]);
			else
				v += C[GETI(j)]*log(1+exp_wTx[j]);

		info("Objective value = %lf\n", v);
		info("#nonzeros/#features = %d/%d\n", nnz, w_size);

		delete [] index;
		delete [] y;
		delete [] exp_wTx;
		delete [] exp_wTx_new;
		delete [] xj_max;
		delete [] C_sum;
		delete [] xjneg_sum;
		delete [] xjpos_sum;
}

double solve_l1r_lr_weight(const problem *prob_col, double *w, double *lambda, double eps,
						   double Cp, double Cn,int &nnz_)
{
	int l = prob_col->l;
	int w_size = prob_col->n;
	int j, s, iter = 0;
	int max_iter = 1000;
	int active_size = w_size;
	int max_num_linesearch = 20;

	double x_min = 0;
	double sigma = 0.01;
	double d, G, H;
	double Gmax_old = INF;
	double Gmax_new;
	double Gmax_init;
	double sum1, appxcond1;
	double sum2, appxcond2;
	double cond;

	int *index = new int[w_size];
	schar *y = new schar[l];
	double *exp_wTx = new double[l];
	double *exp_wTx_new = new double[l];
	double *xj_max = new double[w_size];
	double *C_sum = new double[w_size];
	double *xjneg_sum = new double[w_size];
	double *xjpos_sum = new double[w_size];
	feature_node *x;

	double C[3] = {Cn,0,Cp};

	for(j=0; j<l; j++)
	{
		exp_wTx[j] = 1;
		if(prob_col->y[j] > 0)
			y[j] = 1;
		else
			y[j] = -1;
	}
	for(j=0; j<w_size; j++)
	{
		w[j] = 0;
		index[j] = j;
		xj_max[j] = 0;
		C_sum[j] = 0;
		xjneg_sum[j] = 0;
		xjpos_sum[j] = 0;
		x = prob_col->x[j];
		while(x->index != -1)
		{
			int ind = x->index-1;
			double val = x->value;
			x_min = min(x_min, val);
			xj_max[j] = max(xj_max[j], val);
			C_sum[j] += C[GETI(ind)];
			if(y[ind] == -1)
				xjneg_sum[j] += C[GETI(ind)]*val;
			else
				xjpos_sum[j] += C[GETI(ind)]*val;
			x++;
		}
	}

	while(iter < max_iter)
	{
		Gmax_new = 0;

		for(j=0; j<active_size; j++)
		{
			int i = j+rand()%(active_size-j);
			swap(index[i], index[j]);
		}

		for(s=0; s<active_size; s++)
		{
			j = index[s];
			sum1 = 0;
			sum2 = 0;
			H = 0;

			x = prob_col->x[j];
			while(x->index != -1)
			{
				int ind = x->index-1;
				double exp_wTxind = exp_wTx[ind];
				double tmp1 = x->value/(1+exp_wTxind);
				double tmp2 = C[GETI(ind)]*tmp1;
				double tmp3 = tmp2*exp_wTxind;
				sum2 += tmp2;
				sum1 += tmp3;
				H += tmp1*tmp3;
				x++;
			}

			G = -sum2 + xjneg_sum[j];

			double Gp = G+lambda[j];
			double Gn = G-lambda[j];
			double violation = 0;
			if(w[j] == 0)
			{
				if(Gp < 0)
					violation = -Gp;
				else if(Gn > 0)
					violation = Gn;
				else if(Gp>Gmax_old/l && Gn<-Gmax_old/l)
				{
					active_size--;
					swap(index[s], index[active_size]);
					s--;
					continue;
				}
			}
			else if(w[j] > 0)
				violation = fabs(Gp);
			else
				violation = fabs(Gn);

			Gmax_new = max(Gmax_new, violation);

			// obtain Newton direction d
			if(Gp <= H*w[j])
				d = -Gp/H;
			else if(Gn >= H*w[j])
				d = -Gn/H;
			else
				d = -w[j];

			if(fabs(d) < 1.0e-12)
				continue;

			d = min(max(d,-10.0),10.0);

			double delta = lambda[j]*(fabs(w[j]+d)-fabs(w[j])) + G*d;
			int num_linesearch;
			for(num_linesearch=0; num_linesearch < max_num_linesearch; num_linesearch++)
			{
				cond = lambda[j]*(fabs(w[j]+d)-fabs(w[j])) - sigma*delta;

				if(x_min >= 0)
				{
					double tmp = exp(d*xj_max[j]);
					appxcond1 = log(1+sum1*(tmp-1)/xj_max[j]/C_sum[j])*C_sum[j] + cond - d*xjpos_sum[j];
					appxcond2 = log(1+sum2*(1/tmp-1)/xj_max[j]/C_sum[j])*C_sum[j] + cond + d*xjneg_sum[j];
					if(min(appxcond1,appxcond2) <= 0)
					{
						x = prob_col->x[j];
						while(x->index != -1)
						{
							exp_wTx[x->index-1] *= exp(d*x->value);
							x++;
						}
						break;
					}
				}

				cond += d*xjneg_sum[j];

				int i = 0;
				x = prob_col->x[j];
				while(x->index != -1)
				{
					int ind = x->index-1;
					double exp_dx = exp(d*x->value);
					exp_wTx_new[i] = exp_wTx[ind]*exp_dx;
					cond += C[GETI(ind)]*log((1+exp_wTx_new[i])/(exp_dx+exp_wTx_new[i]));
					x++;
					i++;
				}

				if(cond <= 0)
				{
					int i = 0;
					x = prob_col->x[j];
					while(x->index != -1)
					{
						int ind = x->index-1;
						exp_wTx[ind] = exp_wTx_new[i];
						x++;
						i++;
					}
					break;
				}
				else
				{
					d *= 0.5;
					delta *= 0.5;
				}
			}

			w[j] += d;
			if (lambda[j]>0)
			{
				// recompute exp_wTx[] if line search takes too many steps
				if(num_linesearch >= max_num_linesearch)
				{
					info("#");
					for(int i=0; i<l; i++)
						exp_wTx[i] = 0;

					for(int i=0; i<w_size; i++)
					{
						if(w[i]==0) continue;
						x = prob_col->x[i];
						while(x->index != -1)
						{
							exp_wTx[x->index-1] += w[i]*x->value;
							x++;
						}
					}

					for(int i=0; i<l; i++)
						exp_wTx[i] = exp(exp_wTx[i]);
				}
			}
		}

		if(iter == 0)
			Gmax_init = Gmax_new;
		iter++;
		if(iter % 10 == 0)
			info(".");

		if(Gmax_new <= eps*Gmax_init)
		{
			if(active_size == w_size)
				break;
			else
			{
				active_size = w_size;
				info("*");
				Gmax_old = INF;
				continue;
			}
		}

		Gmax_old = Gmax_new;
	}

	info("\noptimization finished, #iter = %d\n", iter);
	if(iter >= max_iter)
		info("\nWARNING: reaching max number of iterations\n");

	// calculate objective value

	double v = 0;
	int nnz = 0;
	for(j=0; j<w_size; j++)
		if(w[j] != 0)
		{
			v += lambda[j]*fabs(w[j]);
			nnz++;
		}
		for(j=0; j<l; j++)
			if(y[j] == 1)
				v += C[GETI(j)]*log(1+1/exp_wTx[j]);
			else
				v += C[GETI(j)]*log(1+exp_wTx[j]);

		nnz_ = nnz;
		info("Objective value = %lf\n", v);
		info("#nonzeros/#features = %d/%d\n", nnz, w_size);

		delete [] index;
		delete [] y;
		delete [] exp_wTx;
		delete [] exp_wTx_new;
		delete [] xj_max;
		delete [] C_sum;
		delete [] xjneg_sum;
		delete [] xjpos_sum;
		return v;
}
void heap_sort(weight *h,double X,int K, int i)
{
	int p = 0;
	int q = 0;
	double tv;
	int tindex1;
	if(X > h[0].value)
	{
		h[0].value = X;
		h[0].index1 = i;
		int p = 0;
		while(p < K)
		{
			q = 2 * p + 1;
			if(q >= K)
				break;
			if((q < K - 1) && (h[q + 1].value < h[q].value))
				q = q + 1;
			if(h[q].value < h[p].value)
			{
				tv = h[p].value;
				tindex1 = h[p].index1;
				h[p].index1 = h[q].index1;
				h[p].value = h[q].value;
				h[q].value = tv;
				h[q].index1 = tindex1;
				p = q;
			}
			else
				break;
		}
	}


}


static void solve_l1r_lr_iter(
							  const problem *prob_col, double *w, double eps,
							  double Cp, double Cn)
{
	int w_size = prob_col->n;
	double *lambda = new double [w_size];
	for (int i = 0; i<w_size; i++)
	{
		lambda[i] = 1;
	}
	int iter_idx = 0;
	double obj = 0.0;
	double obj_temp = 0.0;
	int B = prob_col->B;
	double threshold = 0.0;
	double eps_iter = 0.01;

	int nnz;
	while (iter_idx<4)
	{
		obj =  solve_l1r_lr_weight(prob_col, w, lambda,  0.01,Cp, Cn,nnz);
		if (fabs(obj-obj_temp)/fabs(obj)<1e-4) //Changed to fabs by Poojt
		{
			printf("success!\n");
			break;
		}
		//calculate the weight
		//do shrinking
		for (int i = 1; i<w_size; i++)
		{
			{
				lambda[i] = 1/(fabs(w[i])+eps_iter);
			}
		}
		iter_idx++;
	}
	delete []lambda;
}
static void solve_l1r_lr_multiconvex(
									 const problem *prob_col, double *w, double eps,
									 double Cp, double Cn)
{
	//
	printf("Doing multiple convex L1!\n");
	int w_size = prob_col->n;
	double *lambda = new double [w_size];
	for (int i = 0; i<w_size; i++)
	{
		lambda[i] = 1;
	}
	int iter_idx = 0;
	double obj = 0.0;
	double obj_temp = 0.0;
	int B = prob_col->B;
	weight *weight_heap = new weight[prob_col->B];
	double threshold = 0.0;
	int nnz;
	while (iter_idx<20)
	{
		obj =  solve_l1r_lr_weight(prob_col, w, lambda,  eps,Cp, Cn,nnz);
		if (fabs(obj-obj_temp)<1e-6) //Changed to fabs by Poojt
		{
			break;
		}
		//calculate the weight
		for(int i=0; i<B;i++)
		{
			weight_heap[i].index1 = 0;
			weight_heap[i].value = 0.0;
		}
		int count = 0;
		for (int i = 1; i<w_size; i++)
		{
			//heap_sort(weight_heap ,fabs(w[i]), B, i);
			if(fabs(w[i])>0)
			{
				heap_sort(weight_heap ,fabs(w[i]), B, i);
				count++;
			}
		}
		if (iter_idx<1)
		{
			if (count>B+30)
			{
				threshold = weight_heap[0].value;
			}
			else
			{
				break;
			}
			printf("threshold = %f\n",threshold);
		}
		//do shrinking
		for (int i = 1; i<w_size; i++)
		{
			if(fabs(w[0])>threshold)
			{
				lambda[i] = 0;
			}else
			{
				lambda[i] = 1.0;
			}
		}
		iter_idx++;
	}
	delete []lambda;
	delete []weight_heap;
}

// transpose matrix X from row format to column format
static void transpose(const problem *prob, feature_node **x_space_ret, problem *prob_col)
{
	int i;
	int l = prob->l;
	int n = prob->n;
	int nnz = 0;
	int *col_ptr = new int[n+1];
	feature_node *x_space;
	prob_col->l = l;
	prob_col->n = n;
	prob_col->y = new int[l];
	prob_col->x = new feature_node*[n];

	for(i=0; i<l; i++)
		prob_col->y[i] = prob->y[i];

	for(i=0; i<n+1; i++)
		col_ptr[i] = 0;
	for(i=0; i<l; i++)
	{
		feature_node *x = prob->x[i];
		while(x->index != -1)
		{
			nnz++;
			col_ptr[x->index]++;
			x++;
		}
	}
	for(i=1; i<n+1; i++)
		col_ptr[i] += col_ptr[i-1] + 1;

	x_space = new feature_node[nnz+n];
	for(i=0; i<n; i++)
		prob_col->x[i] = &x_space[col_ptr[i]];

	for(i=0; i<l; i++)
	{
		feature_node *x = prob->x[i];
		while(x->index != -1)
		{
			int ind = x->index-1;
			x_space[col_ptr[ind]].index = i+1; // starts from 1
			x_space[col_ptr[ind]].value = x->value;
			col_ptr[ind]++;
			x++;
		}
	}
	for(i=0; i<n; i++)
		x_space[col_ptr[i]].index = -1;

	*x_space_ret = x_space;

	delete [] col_ptr;
}

// label: label name, start: begin of each class, count: #data of classes, perm: indices to the original data
// perm, length l, must be allocated before calling this subroutine
static void group_classes(const problem *prob, int *nr_class_ret, int **label_ret, int **start_ret, int **count_ret, int *perm)
{
	int l = prob->l;
	int max_nr_class = 16;
	int nr_class = 0;
	int *label = Malloc(int,max_nr_class);
	int *count = Malloc(int,max_nr_class);
	int *data_label = Malloc(int,l);
	int i;

	for(i=0;i<l;i++)
	{
		int this_label = prob->y[i];
		int j;
		for(j=0;j<nr_class;j++)
		{
			if(this_label == label[j])
			{
				++count[j];
				break;
			}
		}
		data_label[i] = j;
		if(j == nr_class)
		{
			if(nr_class == max_nr_class)
			{
				max_nr_class *= 2;
				label = (int *)realloc(label,max_nr_class*sizeof(int));
				count = (int *)realloc(count,max_nr_class*sizeof(int));
			}
			label[nr_class] = this_label;
			count[nr_class] = 1;
			++nr_class;
		}
	}

	int *start = Malloc(int,nr_class);
	start[0] = 0;
	for(i=1;i<nr_class;i++)
		start[i] = start[i-1]+count[i-1];
	for(i=0;i<l;i++)
	{
		perm[start[data_label[i]]] = i;
		++start[data_label[i]];
	}
	start[0] = 0;
	for(i=1;i<nr_class;i++)
		start[i] = start[i-1]+count[i-1];

	*nr_class_ret = nr_class;
	*label_ret = label;
	*start_ret = start;
	*count_ret = count;
	free(data_label);
}

static double train_one(const problem *prob, const parameter *param, double *w, double Cp, double Cn)
{
	long int t_start = clock();
	long int t_finish;
	double runtime = 0;

	double eps=param->eps;
	eps = 0.0000001;
	int pos = 0;
	int neg = 0;
	for(int i=0;i<prob->l;i++)
		if(prob->y[i]==+1)
			pos++;
	neg = prob->l - pos;

	function *fun_obj=NULL;
	switch(param->solver_type)
	{
	case L2R_LR:
		{
			problem prob_col;
			prob_col.B = prob->B;
			feature_node *x_space = NULL;
			transpose(prob, &x_space ,&prob_col);
			solve_l1r_lr_multiconvex(&prob_col, w, eps*min(pos,neg)/prob->l, Cp, Cn);
			delete [] prob_col.y;
			delete [] prob_col.x;
			delete [] x_space;
			break;
		}
	case L2R_L2LOSS_SVC:
		{
			//fun_obj=new l2r_l2_svc_fun(prob, Cp, Cn);
			//TRON tron_obj(fun_obj, eps*min(pos,neg)/prob->l);
			////tron_obj.set_print_string(liblinear_print_string);
			//tron_obj.tron(w);
			//delete fun_obj;
			//break;
		}
	case L2R_L2LOSS_SVC_DUAL:
		solve_l2r_l1l2_svc(prob, w, eps, Cp, Cn, L2R_L2LOSS_SVC_DUAL);
		break;
	case L2R_L1LOSS_SVC_DUAL:
		solve_l2r_l1l2_svc(prob, w, eps, Cp, Cn, L2R_L1LOSS_SVC_DUAL);
		break;
	case L1R_L2LOSS_SVC:
		{
			problem prob_col;
			prob_col.B = prob->B;
			feature_node *x_space = NULL;
			transpose(prob, &x_space ,&prob_col);
			t_start = clock();
			solve_l1r_l2_svc(&prob_col, w, eps*min(pos,neg)/prob->l, Cp, Cn);
			delete [] prob_col.y;
			delete [] prob_col.x;
			delete [] x_space;
			break;
		}
	case L1R_LR:
		{
			problem prob_col;
			prob_col.B = prob->B;
			feature_node *x_space = NULL;
			transpose(prob, &x_space ,&prob_col);
			t_start = clock();
			solve_l1r_lr_(&prob_col, w, eps*min(pos,neg)/prob->l, Cp, Cn);
			delete [] prob_col.y;
			delete [] prob_col.x;
			delete [] x_space;
			break;
		}
	case L2R_LR_DUAL:
		{
			problem prob_col;
			prob_col.B = prob->B;
			feature_node *x_space = NULL;
			transpose(prob, &x_space ,&prob_col);
			solve_l1r_lr_iter(&prob_col, w, eps*min(pos,neg)/prob->l, Cp, Cn);
			delete [] prob_col.y;
			delete [] prob_col.x;
			delete [] x_space;
			break;
		}
	default:
		fprintf(stderr, "Error: unknown solver_type\n");
		break;
	}
	t_finish = clock();
	runtime = (double(t_finish-t_start)/CLOCKS_PER_SEC);
	printf("elaps time is %f\n",runtime);
	return runtime;
}

double SearchSparseElement(feature_node *xi,int index)
{
	double node_value = 0;
	while(xi->index!=-1)
	{
		if (xi->index == index+1)
		{
			node_value = xi->value;
			break;
		}
		xi++;
	}
	return node_value;
}

//
// Interface functions
//

void solve_l2r_lr_dual_retrain(problem *prob, double eps,
							   double Cp, double Cn, int solver_type, weight *w2, int feature_num, int bias)
{
	int l = prob->l;
	int n = feature_num;
	int w_size = feature_num;
	int i, s, iter = 0;
	double *xTx = new double[l];
	int max_iter = 1000;
	int *index = new int[l];
	double *alpha = new double[2*l]; // store alpha and C - alpha
	schar *y = new schar[l];
	int max_inner_iter = 40; // for inner Newton
	double innereps = 1e-2;
	double innereps_min = min(1e-8, eps);
	double upper_bound[3] = {Cn, 0, Cp};
	double tmp_value;
	feature_node *xi;

	int j;
	for(i=0; i<w_size; i++)
		w2[i].value = 0;
	for(i=0; i<l; i++)
	{
		if(prob->y[i] > 0)
		{
			y[i] = +1;
		}
		else
		{
			y[i] = -1;
		}
		alpha[2*i] = min(0.001*upper_bound[GETI(i)], 1e-8);
		alpha[2*i+1] = upper_bound[GETI(i)] - alpha[2*i];

		xTx[i] = 0;
		j = 0;
		xi = &prob->x[i][0];
		{
			while (w2[j].index1!=-1)
			{
				tmp_value = SearchSparseElement(xi,w2[j].index1);
				w2[j].value += y[i]*alpha[2*i]*tmp_value;
				xTx[i] += tmp_value *tmp_value;
				j++;
			}
		}
		index[i] = i;
	}

	while (iter < max_iter)
	{
		for (i=0; i<l; i++)
		{
			int j = i+rand()%(l-i);
			swap(index[i], index[j]);
		}
		int newton_iter = 0;
		double Gmax = 0;
		for (s=0; s<l; s++)
		{
			i = index[s];
			schar yi = y[i];
			double C = upper_bound[GETI(i)];
			double ywTx = 0, xisq = xTx[i];


			j = 0;
			xi = &prob->x[i][0];
			if (bias)
			{
				while (w2[j].index1!=-2)
				{
					tmp_value =  SearchSparseElement(xi,w2[j].index1);
					ywTx += w2[j].value*tmp_value;
					j++;
				}
				ywTx += w2[j].value*y[i];
			}else
			{
				while (w2[j].index1!=-1)
				{
					tmp_value =  SearchSparseElement(xi,w2[j].index1);
					ywTx += w2[j].value*tmp_value;
					j++;
				}
			}
			ywTx *= y[i];
			double a = xisq, b = ywTx;

			// Decide to minimize g_1(z) or g_2(z)
			int ind1 = 2*i, ind2 = 2*i+1, sign = 1;
			if(0.5*a*(alpha[ind2]-alpha[ind1])+b < 0)
			{
				ind1 = 2*i+1;
				ind2 = 2*i;
				sign = -1;
			}

			//  g_t(z) = z*log(z) + (C-z)*log(C-z) + 0.5a(z-alpha_old)^2 + sign*b(z-alpha_old)
			double alpha_old = alpha[ind1];
			double z = alpha_old;
			if(C - z < 0.5 * C)
				z = 0.1*z;
			double gp = a*(z-alpha_old)+sign*b+log(z/(C-z));
			Gmax = max(Gmax, fabs(gp));

			// Newton method on the sub-problem
			const double eta = 0.1; // xi in the paper
			int inner_iter = 0;
			while (inner_iter <= max_inner_iter)
			{
				if(fabs(gp) < innereps)
					break;
				double gpp = a + C/(C-z)/z;
				double tmpz = z - gp/gpp;
				if(tmpz <= 0)
					z *= eta;
				else // tmpz in (0, C)
					z = tmpz;
				gp = a*(z-alpha_old)+sign*b+log(z/(C-z));
				newton_iter++;
				inner_iter++;
			}

			if (inner_iter>20)
			{
				printf("inner_iter = %d\n",inner_iter);
			}

			if(inner_iter > 0) // update w
			{
				alpha[ind1] = z;
				alpha[ind2] = C-z;

				j = 0;
				xi = &prob->x[i][0];
				if (bias)
				{

				}else
				{
					while (w2[j].index1!=-1)
					{
						tmp_value =  SearchSparseElement(xi,w2[j].index1);
						w2[j].value += sign*(z-alpha_old)*yi*tmp_value;
						j++;
					}
				}

				//xi = prob->x[i];
				//while (xi->index != -1)
				//{
				//	w[xi->index-1] += sign*(z-alpha_old)*yi*xi->value;
				//	xi++;
				//}
			}
			if(inner_iter > 20)
			{
				info("inner_iter = %d\n \n",inner_iter);
			}
		}

		iter++;
		if(iter % 10 == 0)
			info(".");

		if(Gmax < eps)
			break;

		if(newton_iter < l/10)
			innereps = max(innereps_min, 0.1*innereps);

	}

	info("\noptimization finished, #iter = %d\n",iter);
	if (iter >= max_iter)
		info("\nWARNING: reaching max number of iterations\nUsing -s 0 may be faster (also see FAQ)\n\n");

	// calculate objective value

	double v = 0;
	for(i=0; i<w_size; i++)
		v += w2[i].value * w2[i].value;
	v *= 0.5;
	for(i=0; i<l; i++)
		v += alpha[2*i] * log(alpha[2*i]) + alpha[2*i+1] * log(alpha[2*i+1])
		- upper_bound[GETI(i)] * log(upper_bound[GETI(i)]);
	info("Objective value = %lf\n", v);

	delete [] xTx;
	delete [] alpha;
	delete [] y;
	delete [] index;
}

void svm_retrain(problem *prob, double eps,
				 double Cp, double Cn, int solver_type, weight *w2, int feature_num, int bias)
{

	int l = prob->l;
	int n = feature_num;
	int w_size = feature_num;

	int i, s,iter = 0;//,
	double C, d, G;
	double *QD = new double[l];
	int max_iter = 1000;
	int *index = new int[l];
	double temp = 0;
	double *alpha = new double[l];
	schar *y = new schar[l];
	int active_size = l;

	// PG: projected gradient, for shrinking and stopping
	double PG;
	double PGmax_old = INF;
	double PGmin_old = -INF;
	double PGmax_new, PGmin_new;

	// default solver_type: L2LOSS_SVM_DUAL
	double diag_p = 0.5/Cp, diag_n = 0.5/Cn;
	double upper_bound_p = INF, upper_bound_n = INF;

	// w: 1: bias, n:linear, (n+1)n/2: quadratic
	double tmp_value;
	int j = 0;
	feature_node *xi;
	if(solver_type == L2R_L1LOSS_SVC_DUAL)
	{
		diag_p = 0; diag_n = 0;
		upper_bound_p = Cp; upper_bound_n = Cn;
	}

	for(i=0; i<l; i++)
	{
		{
			alpha[i] =0;
		}

		if(prob->y[i] > 0)
		{
			y[i] = +1;
			QD[i] = diag_p;
		}
		else
		{
			y[i] = -1;
			QD[i] = diag_n;
		}
		if (bias)// add bias here
		{
			QD[i] = QD[i]+1;
		}
		j = 0;
		xi = &prob->x[i][0];
		if (bias)
		{
			while (w2[j].index1!=-2)
			{
				tmp_value = SearchSparseElement(xi,w2[j].index1);
				QD[i] += tmp_value *tmp_value;
				j++;
			}
		}else
		{
			while (w2[j].index1!=-1)
			{
				tmp_value = SearchSparseElement(xi,w2[j].index1);
				QD[i] += tmp_value *tmp_value;
				j++;
			}
		}
		index[i] = i;
	}


	for(i=0; i<w_size; i++)
	{
		w2[i].value = 0;
	}
	int l_new = min(prob->l,20000);
	while (iter < max_iter)
	{
		PGmax_new = -INF;
		PGmin_new = INF;

		for (i=0; i<active_size; i++)
		{
			int j = i+rand()%(active_size-i);
			swap(index[i], index[j]);
		}

		for (s=0;s<active_size;s++)
		{
			i = index[s];
			G = 0;
			schar yi = y[i];

			j = 0;
			xi = &prob->x[i][0];
			if (bias)
			{
				while (w2[j].index1!=-2)
				{
					tmp_value =  SearchSparseElement(xi,w2[j].index1);
					G += w2[j].value*tmp_value;
					j++;
				}
				G += w2[j].value*y[i];
			}else
			{
				while (w2[j].index1!=-1)
				{
					tmp_value =  SearchSparseElement(xi,w2[j].index1);
					G += w2[j].value*tmp_value;
					j++;
				}
			}
			G = G*yi-1;

			if(y[i] == 1)
			{
				C = upper_bound_p;
				G += alpha[i]*diag_p;
			}
			else
			{
				C = upper_bound_n;
				G += alpha[i]*diag_n;
			}

			PG = 0;
			if (alpha[i] == 0)
			{
				if (G > PGmax_old)
				{
					active_size--;
					swap(index[s], index[active_size]);
					s--;
					continue;
				}
				else if (G < 0)
					PG = G;
			}
			else if (alpha[i] == C)
			{
				if (G < PGmin_old)
				{
					active_size--;
					swap(index[s], index[active_size]);
					s--;
					continue;
				}
				else if (G > 0)
					PG = G;
			}
			else
				PG = G;

			PGmax_new = max(PGmax_new, PG);
			PGmin_new = min(PGmin_new, PG);

			if(fabs(PG) > 1.0e-12)
			{
				double alpha_old = alpha[i];
				alpha[i] = min(max(alpha[i] - G/QD[i], 0.0), C);
				d = (alpha[i] - alpha_old)*yi;

				j = 0;
				xi = &prob->x[i][0];
				if (bias)
				{
					while (w2[j].index1!=-2)
					{
						tmp_value =  SearchSparseElement(xi,w2[j].index1);
						w2[j].value += d*tmp_value;//dt[xi->index-1].value*dt[xi->index-1].value;
						j++;
					}
					w2[j].value += d*1*y[i];
				}
				else
				{
					while (w2[j].index1!=-1)
					{
						tmp_value =  SearchSparseElement(xi,w2[j].index1);
						w2[j].value += d*tmp_value;//dt[xi->index-1].value*dt[xi->index-1].value;
						j++;
					}
				}

			}
		}

		iter++;
		if(iter % 10 == 0)
		{
			info(".");
		}

		if(PGmax_new - PGmin_new <= eps)
		{
			if(active_size == l)
				break;
			else
			{
				active_size = l;
				printf("*");
				PGmax_old = INF;
				PGmin_old = -INF;
				continue;
			}
		}
		PGmax_old = PGmax_new;
		PGmin_old = PGmin_new;
		if (PGmax_old <= 0)
			PGmax_old = INF;
		if (PGmin_old >= 0)
			PGmin_old = -INF;
	}


	// calculate objective value

	double v = 0;
	double tmp = 0;
	int nSV = 0;

	tmp = 0;
	for(i=0; i<w_size; i++)
	{
		tmp += w2[i].value*w2[i].value;
	}
	v += tmp;

	for(i=0; i<l; i++)
	{
		if (y[i] == 1)
			v += alpha[i]*(alpha[i]*diag_p - 2);
		else
			v += alpha[i]*(alpha[i]*diag_n - 2);
		if(alpha[i] > 0)
			++nSV;
	}

	printf("Objective value = %lf\n",v/2);
	printf("nSV = %d\n",nSV);


	delete [] QD;
	delete [] alpha;
	delete [] y;
	delete [] index;
}

model* train(const problem *prob, const parameter *param)
{
	int i,j;
	int l = prob->l;
	int n = prob->n;
	int w_size = prob->n;
	model *model_ = Malloc(model,1);

	if(prob->bias>=0)
		model_->nr_feature=n-1;
	else
		model_->nr_feature=n;
	model_->param = *param;
	model_->bias = prob->bias;

	int nr_class;
	int *label = NULL;
	int *start = NULL;
	int *count = NULL;
	int *perm = Malloc(int,l);

	// group training data of the same class
	group_classes(prob,&nr_class,&label,&start,&count,perm);

	model_->nr_class=nr_class;
	model_->label = Malloc(int,nr_class);
	for(i=0;i<nr_class;i++)
		model_->label[i] = label[i];

	// calculate weighted C
	double *weighted_C = Malloc(double, nr_class);
	for(i=0;i<nr_class;i++)
		weighted_C[i] = param->C;
	for(i=0;i<param->nr_weight;i++)
	{
		for(j=0;j<nr_class;j++)
			if(param->weight_label[i] == label[j])
				break;
		if(j == nr_class)
			fprintf(stderr,"WARNING: class label %d specified in weight is not found\n", param->weight_label[i]);
		else
			weighted_C[j] *= param->weight[i];
	}

	// constructing the subproblem
	feature_node **x = Malloc(feature_node *,l);
	for(i=0;i<l;i++)
		x[i] = prob->x[perm[i]];

	int k;
	problem sub_prob;
	sub_prob.l = l;
	sub_prob.n = n;
	sub_prob.x = Malloc(feature_node *,sub_prob.l);
	sub_prob.y = Malloc(int,sub_prob.l);

	for(k=0; k<sub_prob.l; k++)
		sub_prob.x[k] = x[k];

	// multi-class svm by Crammer and Singer
	if(param->solver_type == MCSVM_CS)
	{
		model_->w=Malloc(double, n*nr_class);
		for(i=0;i<nr_class;i++)
			for(j=start[i];j<start[i]+count[i];j++)
				sub_prob.y[j] = i;
		Solver_MCSVM_CS Solver(&sub_prob, nr_class, weighted_C, param->eps);
		Solver.Solve(model_->w);
	}
	else
	{
		if(nr_class == 2)
		{
			model_->w=Malloc(double, w_size);

			int e0 = start[0]+count[0];
			k=0;
			for(; k<e0; k++)
				sub_prob.y[k] = +1;
			for(; k<sub_prob.l; k++)
				sub_prob.y[k] = -1;

			train_one(&sub_prob, param, &model_->w[0], weighted_C[0], weighted_C[1]);
		}
		else
		{
			model_->w=Malloc(double, w_size*nr_class);
			double *w=Malloc(double, w_size);
			for(i=0;i<nr_class;i++)
			{
				int si = start[i];
				int ei = si+count[i];

				k=0;
				for(; k<si; k++)
					sub_prob.y[k] = -1;
				for(; k<ei; k++)
					sub_prob.y[k] = +1;
				for(; k<sub_prob.l; k++)
					sub_prob.y[k] = -1;

				train_one(&sub_prob, param, w, weighted_C[i], param->C);

				for(int j=0;j<w_size;j++)
					model_->w[j*nr_class+i] = w[j];
			}
			free(w);
		}

	}

	free(x);
	free(label);
	free(start);
	free(count);
	free(perm);
	free(sub_prob.x);
	free(sub_prob.y);
	free(weighted_C);
	return model_;
}

int predict_probability(const struct model *model_, const struct feature_node *x, double* prob_estimates)
{
	if(check_probability_model(model_))
	{
		int i;
		int nr_class=model_->nr_class;
		int nr_w;
		if(nr_class==2)
			nr_w = 1;
		else
			nr_w = nr_class;

		int label=predict_values(model_, x, prob_estimates);
		for(i=0;i<nr_w;i++)
			prob_estimates[i]=1/(1+exp(-prob_estimates[i]));

		if(nr_class==2) // for binary classification
			prob_estimates[1]=1.-prob_estimates[0];
		else
		{
			double sum=0;
			for(i=0; i<nr_class; i++)
				sum+=prob_estimates[i];

			for(i=0; i<nr_class; i++)
				prob_estimates[i]=prob_estimates[i]/sum;
		}

		return label;
	}
	else
		return 0;
}
int check_probability_model(const struct model *model_)
{
	return (model_->param.solver_type==L2R_LR ||
		model_->param.solver_type==L2R_LR_DUAL ||
		model_->param.solver_type==L1R_LR);
}
