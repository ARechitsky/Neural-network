#include "hopfield.h"

#define to_hopf_input(_neuron) dynamic_cast<neuron_hopf_input *>(_neuron)
#define to_hopf_output(_neuron) dynamic_cast<neuron_hopf_output *>(_neuron)

neuron_hopf_input::neuron_hopf_input() : neuron_input(), neuron_with_input()
{
	set_function(linear_func);
}

neuron_hopf_input::~neuron_hopf_input()
{

}

neuron_hopf_output::neuron_hopf_output() : neuron_hide()
{
	set_function(sigmoid_func);
}

neuron_hopf_output::~neuron_hopf_output()
{

}

hopfield_net::hopfield_net(int dimensionality)
{
	_len = dimensionality;
	_input = new neuron *[_len];
	_output = new neuron *[_len];
	_threshold = new neuron_threshold;
	_inp_to_out = new synapse *[_len * _len];
	_out_to_inp = new synapse *[_len];
	_thr_to_out = new synapse *[_len];
	for (int i = 0; i < _len; i++)
	{
		_input[i] = new neuron_hopf_input;
		_output[i] = new neuron_hopf_output;
	}
	for (int i = 0; i < _len; i++)
	{
		for (int j = 0; j < _len; j++)
		{
			_inp_to_out[i * _len + j] = new synapse;
			_inp_to_out[i * _len + j] -> set_weight(0);
			_inp_to_out[i * _len + j] -> set_start(_input[i]);
			_inp_to_out[i * _len + j] -> set_end(_output[j]);
			to_hopf_input(_input[i]) -> add_output(_inp_to_out[i * _len + j]);
			to_hopf_output(_output[j]) -> add_input(_inp_to_out[i * _len + j]);
		}
		_out_to_inp[i] = new synapse;
		_out_to_inp[i] -> set_weight(1);
		_out_to_inp[i] -> set_start(_output[i]);
		_out_to_inp[i] -> set_end(_input[i]);
		to_hopf_output(_output[i]) -> add_output(_out_to_inp[i]);
		to_hopf_input(_input[i]) -> add_input(_out_to_inp[i]);

		_thr_to_out[i] = new synapse;
		_thr_to_out[i] -> set_weight(0);
		_thr_to_out[i] -> set_start(_threshold);
		_thr_to_out[i] -> set_end(_output[i]);
		to_threshold(_threshold) -> add_output(_thr_to_out[i]);
		to_hopf_output(_output[i]) -> add_input(_thr_to_out[i]);
	}
}

hopfield_net::~hopfield_net()
{

}

float *hopfield_net::get_result(float* input)
{
	for (int i = 0; i < _len; i++)
	{
		to_hopf_input(_input[i]) -> set_status(input[i]);
	}
	for (int i = 0; i < _len; i++)
	{
		to_hopf_output(_output[i]) -> refresh_status();
	}
	float *tmp = new float [_len];
	int flag = 1;
	while (flag)
	{
		for (int i = 0; i < _len; i++)
		{
			tmp[i] = _output[i] -> get_status();
		}
		for (int i = 0; i < _len; i++)
		{
			to_hopf_input(_input[i]) -> refresh_status();
		}
		for (int i = 0; i < _len; i++)
		{
			to_hopf_output(_output[i]) -> refresh_status();
		}
		flag = 0;
		for (int i = 0; i < _len; i++)
		{
			if (tmp[i] != _output[i] -> get_status())
			{
				flag = 1;
				break;
			}
		}
	}
	return tmp;
}

void hopfield_net::learn(float** learning_set, int n)
{


	//	for (int i = 0; i < n; i++)
	//	{
	//		for (int j = 0; j < _len; j++)
	//		{
	//			printf("%10f", learning_set[i][j]);
	//		}
	//		printf("\n");
	//	}

	float s;
	for (int k = 0; k < n; k++)
	{
		for (int i = 0; i < _len; i++)
		{
			s += learning_set[k][i];
			for (int j = 0; j < _len; j++)
			{
				if (i == j) continue;
				_inp_to_out[i * _len + j] -> add_weight(learning_set[k][i] * learning_set[k][j]);
			}
		}
	}
	s /= -n * 3;
	for (int i = 0; i < _len; i++)
	{
		_thr_to_out[i] -> set_weight(s);
	}
	//	for (int i = 0; i < _len; i++)
	//	{
	//		for (int j = 0; j < _len; j++)
	//		{
	//			printf("%10f", _inp_to_out[i * _len + j] -> get_weight());
	//		}
	//		printf("\n");
	//	}
}
