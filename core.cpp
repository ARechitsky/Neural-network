#include "core.h"

neuron::neuron()
{
	_status = 0;
}

neuron::~neuron()
{
}

float neuron::get_status()
{
	return _status;
}

neuron_with_output::neuron_with_output() : neuron()
{
	_output = 0;
	_outlen = 0;
}

neuron_with_output::~neuron_with_output()
{
}

void neuron_with_output::add_output(synapse *out_synapse)
{
	synapse **tmp;
	tmp = _output;
	_output = new synapse *[_outlen + 1];
	for (int i = 0; i < _outlen; i++)
	{
		_output[i] = tmp[i];
	}
	//	delete [] tmp;
	_output[_outlen] = out_synapse;
	_outlen++;
}

int neuron_with_output::get_output_length()
{
	return _outlen;
}

synapse *neuron_with_output::get_output_at(int num)
{
	return _output[num];
}

neuron_without_output::neuron_without_output() : neuron()
{
}

neuron_without_output::~neuron_without_output()
{
}

neuron_with_input::neuron_with_input() : neuron()
{
	_input = 0;
	_inlen = 0;
}

neuron_with_input::~neuron_with_input()
{
}

float neuron_with_input::_summator()
{
	float s = 0;
	for (int i = 0; i < _inlen; i++)
	{
		s += _input[i] -> get_start() -> get_status() * _input[i] -> get_weight();
	}
	return s;
}

void neuron_with_input::add_input(synapse *in_synapse)
{
	synapse **tmp;
	tmp = _input;
	_input = new synapse *[_inlen + 1];
	for (int i = 0; i < _inlen; i++)
	{
		_input[i] = tmp[i];
	}
	//	delete [] tmp;
	_input[_inlen] = in_synapse;
	_inlen++;
}

int neuron_with_input::get_input_length()
{
	return _inlen;
}

synapse *neuron_with_input::get_input_at(int num)
{
	return _input[num];
}

void neuron_with_input::set_function(functype function)
{
	_function = function;
}

void neuron_with_input::refresh_status()
{
	_status = _function(_summator());
}

neuron_without_input::neuron_without_input() : neuron()
{
}

neuron_without_input::~neuron_without_input()
{
}

void neuron_without_input::set_status(float status)
{
	_status = status;
}

neuron_input::neuron_input() : neuron_with_output(), neuron_without_input()
{
}

neuron_input::~neuron_input()
{
}

neuron_output::neuron_output() : neuron_without_output(), neuron_with_input()
{
}

neuron_output::~neuron_output()
{
}

neuron_hide::neuron_hide() : neuron_with_output(), neuron_with_input()
{
}

neuron_hide::~neuron_hide()
{
}

neuron_threshold::neuron_threshold() : neuron_with_output()
{
	_status = 1;
}

neuron_threshold::~neuron_threshold()
{
}

synapse::synapse()
{
	_weight = 0;
	//	_start = new neuron;
	//	_end = new neuron;
}

synapse::~synapse()
{
}

void synapse::set_start(neuron *start)
{
	_start = start;
}

neuron *synapse::get_start()
{
	return _start;
}

void synapse::set_end(neuron *end)
{
	_end = end;
}

neuron *synapse::get_end()
{
	return _end;
}

void synapse::set_weight(float weight)
{
	_weight = weight;
}

void synapse::add_weight(float weight)
{
	_weight += weight;
}

float synapse::get_weight()
{
	return _weight;
}

float sigmoid_func(float x)
{
	return 1.0 / (1.0 + exp(-x));
}

float threshhold_func(float x)
{
	return x > 0;
}

float linear_func(float x)
{
	return x;
}
