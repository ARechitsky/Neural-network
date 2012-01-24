#ifndef _COGNITRON_H
#define	_COGNITRON_H

#include "cognitive.h"

class cognitron_one_dim
{
public:
	cognitron_one_dim(int input_size, int layers_count, int link_area_size, int comp_area_size);
	virtual ~cognitron_one_dim();
	float *get_result(float *input);
	void learn(float **learning_set, int count, int length, float q, float q2);
protected:
	int _layers_count;
	int *_layers_lengths;
	int _input_size;
	neuron ** _input;
	neuron ***_excitive;
	neuron ***_inhibitory;
	neuron ***_lateral_inhibitory;
	synapse ****_exc_to_lat;
	synapse **** _lat_to_exc;
	synapse **** _lat_to_inh;
	synapse ***_inh_to_exc;
};

class cognitron_square
{
public:
	cognitron_square(int input_size, int layers_count, int link_area_size, int comp_area_size);
	virtual ~cognitron_square();
	float **get_result(float **input);
	void learn(float ***learning_set, int count, int length, float q, float q2);
protected:
	int _layers_count;
	int *_layers_lengths;
	int _input_size;
	neuron *** _input;
	neuron ****_excitive;
	neuron ****_inhibitory;
	neuron ****_lateral_inhibitory;
	synapse ******_exc_to_lat;
	synapse ****** _lat_to_exc;
	synapse ****** _lat_to_inh;
	synapse ****_inh_to_exc;
};


#endif	/* _COGNITRON_H */
