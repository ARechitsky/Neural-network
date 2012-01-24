/* 
 * File:   cognitive.h
 * Author: xamejieoh
 *
 * Created on 12 Сентябрь 2010 г., 2:32
 */

#ifndef _COGNITIVE_H
#define	_COGNITIVE_H

#include "core.h"

class neuron_cogn_input : public virtual neuron_input
{
public:
	neuron_cogn_input();
	virtual ~neuron_cogn_input();
};

class neuron_cogn_excitive : public virtual neuron_with_input
{
public:
	neuron_cogn_excitive();
	virtual ~neuron_cogn_excitive();
	void refresh_status();
	void add_inh_input(synapse *in_synapse);
	int get_inh_input_length();
	synapse *get_inh_input_at(int num);
private:
	float _inh_summator();
	int _inhlen;
	synapse **_inhinp;
};

class neuron_cogn_inhibitory : public virtual neuron_hide
{
public:
	neuron_cogn_inhibitory();
	virtual ~neuron_cogn_inhibitory();
};

class neuron_cogn_exc_hide : public virtual neuron_hide, public virtual neuron_cogn_excitive
{
public:
	neuron_cogn_exc_hide();
	virtual ~neuron_cogn_exc_hide();
};

class neuron_cogn_output : public virtual neuron_output, public virtual neuron_cogn_excitive
{
public:
	neuron_cogn_output();
	virtual ~neuron_cogn_output();
};

#endif	/* _COGNITIVE_H */

