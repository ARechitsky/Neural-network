#include "cognitive.h"

neuron_cogn_input::neuron_cogn_input() : neuron_input()
{

}

neuron_cogn_input::~neuron_cogn_input()
{

}

neuron_cogn_excitive::neuron_cogn_excitive() : neuron_with_input()
{
	set_function(0);
}

neuron_cogn_excitive::~neuron_cogn_excitive()
{

}

float neuron_cogn_excitive::_inh_summator()
{
	float s = 0;
	for (int i = 0; i < _inhlen; i++)
	{
		s += _inhinp[i] -> get_start() -> get_status() * _inhinp[i] -> get_weight();
	}
	return s;
}

void neuron_cogn_excitive::add_inh_input(synapse *in_synapse)
{
	synapse **tmp;
	tmp = _inhinp;
	_inhinp = new synapse *[_inhlen + 1];
	for (int i = 0; i < _inhlen; i++)
	{
		_inhinp[i] = tmp[i];
	}
	//	delete [] tmp;
	_inhinp[_inhlen] = in_synapse;
	_inhlen++;
}

int neuron_cogn_excitive::get_inh_input_length()
{
	return _inhlen;
}

synapse *neuron_cogn_excitive::get_inh_input_at(int num)
{
	return _inhinp[num];
}

void neuron_cogn_excitive::refresh_status()
{
	_status = (_summator() + 1.0) / (_inh_summator() + 1.0) - 1.0;
	if (_status < 0) _status = 0;
}

neuron_cogn_inhibitory::neuron_cogn_inhibitory() : neuron_hide()
{
	set_function(linear_func);
}

neuron_cogn_inhibitory::~neuron_cogn_inhibitory()
{

}

neuron_cogn_exc_hide::neuron_cogn_exc_hide() : neuron_hide(), neuron_cogn_excitive()
{

}

neuron_cogn_exc_hide::~neuron_cogn_exc_hide()
{

}

neuron_cogn_output::neuron_cogn_output() : neuron_output(), neuron_cogn_excitive()
{

}

neuron_cogn_output::~neuron_cogn_output()
{

}

