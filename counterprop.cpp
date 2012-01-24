#include "counterprop.h"

#define to_counterprop_input(_neuron) dynamic_cast<neuron_counterprop_input *>(_neuron)
#define to_counterprop_kohonen(_neuron) dynamic_cast<neuron_counterprop_kohonen *>(_neuron)
#define to_counterprop_grossberg(_neuron) dynamic_cast<neuron_counterprop_grossberg *>(_neuron)

neuron_counterprop_input::neuron_counterprop_input() : neuron_input()
{

}

neuron_counterprop_input::~neuron_counterprop_input()
{

}

neuron_counterprop_kohonen::neuron_counterprop_kohonen() : neuron_hide()
{

}

neuron_counterprop_kohonen::~neuron_counterprop_kohonen()
{

}

void neuron_counterprop_kohonen::set_active()
{
	_status = 1;
}

void neuron_counterprop_kohonen::set_nonactive()
{
	_status = 0;
}

neuron_counterprop_grossberg::neuron_counterprop_grossberg() : neuron_output()
{

}

neuron_counterprop_grossberg::~neuron_counterprop_grossberg()
{

}

counter_propagation_net::counter_propagation_net(int inplen, int kohlen, int groslen)
{
	//TODO: Verifying
	_inplen = inplen;
	_input = new neuron *[_inplen];
	for (int i = 0; i < _inplen; i++)
	{
		_input[i] = new neuron_counterprop_input;
	}
	_kohlen = kohlen;
	_kohonen = new neuron *[_kohlen];
	_threshold = new neuron_threshold;
	_thr_to_koh = new synapse *[_kohlen];
	for (int i = 0; i < _kohlen; i++)
	{
		_kohonen[i] = new neuron_counterprop_kohonen;
		to_counterprop_kohonen(_kohonen[i]) -> set_function(linear_func);
		_thr_to_koh[i] = new synapse;
		_thr_to_koh[i] -> set_weight(0);
		_thr_to_koh[i] -> set_start(_threshold);
		_thr_to_koh[i] -> set_end(_kohonen[i]);
		to_threshold(_threshold) -> add_output(_thr_to_koh[i]);
		to_counterprop_kohonen(_kohonen[i]) -> add_input(_thr_to_koh[i]);
	}
	_active_kohonen = 0;
	_groslen = groslen;
	_grossberg = new neuron *[_groslen];
	for (int i = 0; i < _groslen; i++)
	{
		_grossberg[i] = new neuron_counterprop_grossberg;
		to_counterprop_grossberg(_grossberg[i]) -> set_function(linear_func);
	}

	_inp_to_koh = new synapse *[_inplen * _kohlen];
	_koh_to_gros = new synapse *[_kohlen * _groslen];

	for (int j = 0; j < _kohlen; j++)
	{
		for (int i = 0; i < _inplen; i++)
		{
			_inp_to_koh[i * _kohlen + j] = new synapse;
			_inp_to_koh[i * _kohlen + j] -> set_start(_input[i]);
			_inp_to_koh[i * _kohlen + j] -> set_end(_kohonen[j]);
			_inp_to_koh[i * _kohlen + j] -> set_weight(1 / sqrt(_inplen));
			//			_inp_to_koh[i * _kohlen + j] -> set_weight(rand() % 1000);
			to_counterprop_input(_input[i]) -> add_output(_inp_to_koh[i * _kohlen + j]);
			to_counterprop_kohonen(_kohonen[j]) -> add_input(_inp_to_koh[i * _kohlen + j]);
		}
		for (int k = 0; k < _groslen; k++)
		{
			_koh_to_gros[j * _groslen + k] = new synapse;
			_koh_to_gros[j * _groslen + k] -> set_start(_kohonen[j]);
			_koh_to_gros[j * _groslen + k] -> set_end(_grossberg[k]);
			_koh_to_gros[j * _groslen + k] -> set_weight(0);
			//			_koh_to_gros[j * _groslen + k] -> set_weight((rand() % 2) * 2 - 1);
			//			printf("%d -> %d: %f\n", j, k, _koh_to_gros[j * _groslen + k] -> get_weight());
			to_counterprop_kohonen(_kohonen[j]) -> add_output(_koh_to_gros[j * _groslen + k]);
			to_counterprop_grossberg(_grossberg[k]) -> add_input(_koh_to_gros[j * _groslen + k]);
		}
		//		neuron_counterprop_kohonen *cur_koh;
		//		synapse *cur;
		//		int s;
		//		for (int i = 0; i < _kohlen; i++)
		//		{
		//			cur_koh = to_counterprop_kohonen(_kohonen[i]);
		//			s = 0;
		//			for (int j = 0; j < _inplen; j++)
		//			{
		//				cur = cur_koh->get_input_at(j);
		//				s += cur->get_weight() * cur->get_weight();
		//			}
		//			s = sqrt(s);
		//			for (int j = 0; j < _inplen; j++)
		//			{
		//				cur = cur_koh->get_input_at(j);
		//				cur->set_weight(cur->get_weight() / s);
		//			}
		//		}
	}
}

counter_propagation_net::~counter_propagation_net()
{

}

void counter_propagation_net::set_active_kohonen(neuron_counterprop_kohonen *active)
{
	_active_kohonen = active;
	for (int i = 0; i < _kohlen; i++)
	{
		to_counterprop_kohonen(_kohonen[i]) -> set_nonactive();
	}
	active -> set_active();
}

neuron_counterprop_kohonen *counter_propagation_net::get_active_cohonen()
{
	return _active_kohonen;
}

float *counter_propagation_net::get_result(float* input)
{
	for (int i = 0; i < _inplen; i++)
	{
		to_counterprop_input(_input[i]) -> set_status(input[i]);
	}

	for (int i = 0; i < _kohlen; i++)
	{
		to_counterprop_kohonen(_kohonen[i]) -> refresh_status();
	}
	int active_koh = 0;
	for (int i = 1; i < _kohlen; i++)
	{
		if (_kohonen[i] -> get_status() > _kohonen[active_koh] -> get_status()) active_koh = i;
	}
	set_active_kohonen(to_counterprop_kohonen(_kohonen[active_koh]));

	float *res = new float [_groslen];
	for (int i = 0; i < _groslen; i++)
	{
		to_counterprop_grossberg(_grossberg[i]) -> refresh_status();
		res[i] = _grossberg[i] -> get_status();
	}
	return res;
}

void counter_propagation_net::learn(float** learning_set, float** results, int n, float speedkoh, float speedgros)
{
	//TODO: Veryfying
	float koef_of_inp = 0.0001;
	float koef_multiplier = 1;
	float koef_addition = .0001;
	float threshold = 1;
	float *tmp_inp = new float [_inplen];
	float *tmp_res;
	neuron_counterprop_kohonen *active_koh;
	synapse *cur;
	float s;
	//	for (int i = 0; i < _kohlen; i++)
	//	{
	//		active_koh = to_counterprop_kohonen(_kohonen[i]);
	//		s = 0;
	//		for (int j = 0; j < _inplen; j++)
	//		{
	//			cur = active_koh->get_input_at(j);
	//			s += cur->get_weight() * cur->get_weight();
	//		}
	//		s = sqrt(s);
	//		//		printf("%f\n", s);
	//		for (int j = 0; j < _inplen; j++)
	//		{
	//			cur = active_koh->get_input_at(j);
	//			printf("%10f ", cur -> get_weight());
	//			cur -> set_weight(cur->get_weight() / s /* * koef_of_inp + (1 - koef_of_inp) / sqrt(_inplen)*/);
	//		}
	//		printf("\n");
	//		for (int j = 0; j < _inplen; j++)
	//		{
	//			cur = active_koh->get_input_at(j);
	//			printf("%10f ", cur -> get_weight());
	//		}
	//		printf("\n");
	//	}
	for (int x = 0; x < 10000; x++)
	{
		for (int k = 0; k < n; k++)
		{
			for (int i = 0; i < _inplen; i++)
			{
				tmp_inp[i] = learning_set[k][i] /** koef_of_inp + (1 - koef_of_inp) / sqrt(_inplen)*/;
			}
			tmp_res = get_result(tmp_inp);
			active_koh = get_active_cohonen();
			//				printf("%d:  %d - %x \n", x, k, active_koh);
			for (int i = 0; i < active_koh->get_input_length(); i++)
			{
				cur = active_koh->get_input_at(i);
				//				cur->set_weight(cur->get_start()->get_status() * speed + cur->get_weight()*(1 - speed));
				if (to_threshold(cur -> get_start())) cur -> add_weight(-threshold);
				else cur -> add_weight((cur -> get_start() -> get_status() - cur -> get_weight()) * speedkoh * (1 - x / 10000.0));
			}
			for (int i = 0; i < _kohlen; i++)
			{
				to_threshold(_threshold) -> get_output_at(i) -> add_weight(threshold / _kohlen);
			}
			for (int i = 0; i < active_koh -> get_output_length(); i++)
			{
				cur = active_koh->get_output_at(i);
				cur->add_weight((results[k][i] - tmp_res[i]) * speedgros);
			}
		}
		koef_of_inp *= koef_multiplier;
		koef_of_inp += koef_addition;
	}
}
