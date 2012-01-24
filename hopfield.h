#ifndef _HOPFIELD_H
#define	_HOPFIELD_H

#include "core.h"

class neuron_hopf_input : public virtual neuron_input, public virtual neuron_with_input
{
public:
	neuron_hopf_input();
	virtual ~neuron_hopf_input();
};

class neuron_hopf_output : public virtual neuron_hide
{
public:
	neuron_hopf_output();
	virtual ~neuron_hopf_output();
};

class hopfield_net
{
public:
	hopfield_net(int dimensionality);
	virtual ~hopfield_net();
	float *get_result(float *input);
	void learn(float **learning_set, int n);
protected:
	int _len;
	neuron **_input;
	neuron **_output;
	neuron *_threshold;
	synapse **_inp_to_out;
	synapse **_out_to_inp;
	synapse **_thr_to_out;
};

#endif	/* _HOPFIELD_H */

