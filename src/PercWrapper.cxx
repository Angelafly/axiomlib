/****************************************************************************
*				PercWrapper.cxx
*
*	Description:	����, ���������� ���������� ����, ��� ������� � PercWrapper.h
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef PERCWRAPPER_CXX_
#define PERCWRAPPER_CXX_

#include "AxiomLibException.h"
#include "PercWrapper.h"

namespace AxiomLib {

// �������� ����� �������, ������� �������� � ������������ 0 � ��������� ������
const double zeroValue = 0.0;

/****************************************************************************
*						PercWrapper<Holder>::construct
*
*       Description:	��������� ���������� � �������� ����
*       Parameters:		const Settings &settings - ���������
*						��� Settings ������ ����� ���� 
*							CommonSettings commonSettings,
*							PercSettings percSettings,
*						� ����� ������������� ����������� Holder::construct()
*       Returns:		-
*       Throws:			AxiomLibException, ���� dstQ ����������� �� ����� 2
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Holder>
template<class Settings>
//��� Settings ������ ����� ���� PercSettings percSettings
void PercWrapper<Holder>::construct(const Settings & s) {
	// ������������� �������� Holder
	Holder::construct(s);
	
	// ��������
	if(Holder::getCoderDstQ()!=2) {
		throw AxiomLibException("PercWrapper<Holder>::construct(): dstQ value is not 2");
	}
	
	// ����� ��� �������� ����������� (��� ��������� ������)
	const PercSettings &settings = s.percSettings;
	
	// ���������� ����� ������� � �������� ��������
	// ����� ������ - ������������ ����� �������������� ��������� ������
	numInp = Holder::getCoderDstLen(Holder::getMaxTrainDim());
	// ����� ������� - ����� ��������� �����
	numOutp = Holder::getNumTrain();
	
	// ���������� ������� ����� �������� � �����
	unsigned int *layers=new unsigned int[settings.numLay+1];
	layers[0]=numInp;
	for(int i=1; i<settings.numLay; i++)
		// ����� �������� �� ���������� ���� - "����������" �����,
		// ���������� �� ��������� numNeuronsF[i-1] �� ��������
		layers[i]=static_cast<int>(std::floor(settings.numNeuronsF[i-1]*
		(calcAveNumNeurons(i, settings.numLay, numInp, numOutp))));
	layers[settings.numLay]=numOutp;
	
	// �������� ����
	net.create_standard_array(settings.numLay+1, layers);
	
	// ������������ ������������� �������
	delete [] layers;

	// ��������� ������������� ������� � �� ��������� ��� ������� ����
	for(int i=1; i<=settings.numLay; i++)
	{
		net.set_activation_function_layer(settings.actFuncs[i-1], i);
		net.set_activation_steepness_layer(settings.actFuncsS[i-1], i);
	}

	// ��������� �������� ������
	desiredError=settings.desiredError;
	
	// ��������� ���������� ���������
	net.set_training_algorithm(settings.trainingAlgorithm);
	// �� ������������� learningRate � learningMomentum,
	// ������ ���� ������������ ������� �������� ��������������� ������
	// (����� ��������� ���� �� ����������, � ������������� �� ������� ��
	// �������������)
	if(settings.trainingAlgorithm==FANN::TRAIN_BATCH || settings.trainingAlgorithm==FANN::TRAIN_INCREMENTAL
		||settings.trainingAlgorithm==FANN::TRAIN_QUICKPROP)
	{
		net.set_learning_rate((float) settings.learningRate);
		net.set_learning_momentum((float) settings.learningMomentum);
	}
	
	// ��������� ��������� ��������
	maxIters = settings.maxIters;
	itersBetweenReports = settings.itersBetweenReports;
	
	// ���� ���������� �� �����, �� ������ ������ defaultCallback
	// (����� �� ��������� ����� ��������� callback, ��������� ����������)
	if(!s.commonSettings.comments) {
		net.set_callback(&(PercWrapper<Holder>::defaultCallback), 0);
	}
	
	// ������������ data
	constructTrainingData();
	
	// ��������� ���������
	stage = 1;
}

/****************************************************************************
*						PercWrapper<Holder>::train
*
*       Description:	������� ������� ���� �� ����� ��������������� �� ��������� �������
*       Parameters:		-
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Holder>
void PercWrapper<Holder>::train() {
	// TODO: �������� ������ ����������, ������� � Holder ���� comments
	
	// ������������� �����
	net.init_weights(data);
	// ��������
	net.train_on_data(data, maxIters,
		  itersBetweenReports, (float) desiredError);
	// ��������� ���������
	stage = 2;
}

/****************************************************************************
*						PercWrapper<Holder>::constructTrainingData
*
*       Description:	���������� ��������� ������� � ��� ����, � ������� ��
*						��������� FANN
*       Parameters:		-
*       Returns:		-
*       Throws:			AxiomLibException, ���� ��������� ������� �����
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Holder>
void PercWrapper<Holder>::constructTrainingData() {
	int sz=Holder::getNumTrain();
	
	numInp = Holder::getCoderDstLen(Holder::getMaxTrainDim());
	numOutp = Holder::getNumTrain();

	int aux_sz = 0;
	//int sz2=0;

	// ������� ����� �������������� ����� ��� ��������
	for(int i = 0; i<Holder::getNumAuxTrain(); i++)
	{
		aux_sz+=Holder::getNumAuxTrain(i);
	}


	//sz2 = sz+aux_sz;
	
	// ��������� ����� ������ ��������� ������� (������� ���. ������)
	sz += aux_sz;
	
	//sz += inhibiting_data.size();
	
	// ��������
	if(sz==0) {
		throw AxiomLibException("PercWrapper<Holder>::constructTrainingData() : no training data supplied");
	}
	
	// ������� ������ � ������� ���� (fann_type - ��� double)
	fann_type **input=new fann_type *[sz];
	fann_type **output=new fann_type *[sz];

	// �������� ������� ��������� ��������� �������
	// (��� ��������, ��� � ��������������)
	int i=0;
	
	// ���� �� ���� �������� ������� ��� ��������
	for (int ii=0; ii<Holder::getNumTrain(); ii++)
	{
		// ���� � ��������������� ��� �����
		input[i]=new fann_type[numInp];
		output[i]=new fann_type[numOutp];
		
		// ��������� ����
		for(int j=0; j < numInp; j++) {
			input[i][j]=transform(Holder::getTranslated(ii), j);
		}
		
		// ��������� �����
		for(int j=0; j < numOutp; j++)
		{
			output[i][j]= (ii==j) ? 1.0 : zeroValue;
		}
		
		// ����������� �������
		i++;
		
		// ���� �� ���� �������������� ������� ��� ��������,
		// ��������������� ������ �������� ������
		// ��� ��� ����������
		for(int iii=0; iii<Holder::getNumAuxTrain(ii); iii++)
		{
			input[i]=new fann_type[numInp];
			output[i]=new fann_type[numOutp];
			
			for (int j = 0; i < numInp; j++) {
				input[i][j]=transform(Holder::getAuxTranslated(ii, iii), j);			
			}

			for (int j = 0; i < numOutp; j++) {
				// ����� �� ������ �� 1, � ����� �������,
				// �.�. ��� �� �������� �������, �� ������� ����� �����������
				output[i][j]= (ii==j) ? 
					determineOutput(Holder::getAuxTranslated(ii, iii), Holder::getTranslated(ii)) : 
					zeroValue;
			}
			
			i++;
		}
	}

	//for( ;i<sz; i++)
	//{
	//	input[i]=new fann_type[numInp];
	//	output[i]=new fann_type[numOutp];
	//	for(int j=0; j< max(numInp, numOutp); j++)
	//	{
	//		if(j<numInp) input[i][j]=inhibiting_data[i - sz2](j);
	//		if(j<numOutp) output[i][j]= zero_value;
	//	}
	//}

	// ��������� data
	data.set_train_data(sz, numInp, input, numOutp, output);

	// ����������� ������
	for(int i=0; i<sz; i++)
	{
		delete [] input[i];
		delete [] output[i];
	}
	delete [] input;
	delete [] output;
}

/****************************************************************************
*						PercWrapper<Holder>::run
*
*       Description:	������� ��������� ������������ ��������
*						
*       Parameters:		const Qstring &inp - ���� ���� ()
*						double &p - �����, ������� ����� ���������� ���� ��� ������ (���� �� ������������)
*       Returns:		����� ��������� ������ (� ��� �������, � ������� ��� ���� ������ �������� supplyTrainingData()), 
*						���� -1, ���� ���� ������ �����, ��������� �� ����� ��������� ��������� �����
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Holder>
int PercWrapper<Holder>::run(const Qstring &pattern, double &probability) const {
	// ���� � ��� ����, � ������� �������� ��� FANN
	fann_type *input=new fann_type[numInp];
	
	// ����������� ������� ������
	Qstring patternCoded = Holder::coderTranslate(pattern);
	
	// ���������� ������� ������ � ���� �������
	for(int i=0; i< numInp; i++)
	{
		input[i]=transform(patternCoded,i);
	}

	// ������ ����
	fann_type *res=net.run(input);

	// ������������ ������
	delete [] input;
	
	// ����������� ������������� ������� � ������ ����
	int max_index=0;
	for(int i=1; i<numOutp; i++) {
		if(res[i]>res[max_index]) max_index=i;
	}
	
	// ��������� ����������� �������� ������������� ������ �������
	probability=res[max_index];
	
	return max_index;
}

/****************************************************************************
*						PercWrapper<Holder>::PercWrapper
*
*       Description:	����������� ����������� - �� �����������, �.�.
*						��� FANN::training_data �� copy-constructible
*						� ������ ������������ data �� ����������,
*						� ��������� ������, ���� �����
*       Parameters:		const PercWrapper<Holder> &other
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Holder>
PercWrapper<Holder>::PercWrapper(const PercWrapper<Holder> &other):
	Holder(other),
	net(other.net),
	numInp(other.numInp),
	numOutp(other.numOutp),
	maxIters(other.maxIters),
	itersBetweenReports(other.itersBetweenReports),
	desiredError(other.desiredError),
	stage(other.stage) {
	if(stage >= 1)  {
		constructTrainingData();
	}
}

};

#endif
