#ifndef _COUNTERPROP_H
#define	_COUNTERPROP_H

#include "core.h"

class neuron_counterprop_input : public virtual neuron_input
{
public:
	neuron_counterprop_input();
	virtual ~neuron_counterprop_input();
};

class neuron_counterprop_kohonen : public virtual neuron_hide
{
public:
	neuron_counterprop_kohonen();
	virtual ~neuron_counterprop_kohonen();
	void set_active();
	void set_nonactive();
};

class neuron_counterprop_grossberg : public virtual neuron_output
{
public:
	neuron_counterprop_grossberg();
	virtual ~neuron_counterprop_grossberg();
};

class counter_propagation_net
{
protected:
	int _inplen;
	neuron **_input;
	int _kohlen;
	neuron **_kohonen;
	int _groslen;
	neuron **_grossberg;
	neuron *_threshold;
	synapse **_inp_to_koh;
	synapse **_koh_to_gros;
	synapse **_thr_to_koh;
	neuron_counterprop_kohonen *_active_kohonen;
public:
	counter_propagation_net(int inplen, int kohlen, int groslen);
	virtual ~counter_propagation_net();
	void set_active_kohonen(neuron_counterprop_kohonen *active);
	neuron_counterprop_kohonen *get_active_cohonen();
	float *get_result(float *input);
	void learn(float **learning_set, float **results, int n, float speedkoh, float speedgros);
};

#endif	/* _COUNTERPROP_H */

