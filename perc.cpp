#include <stdio.h>

#include "perc.h"

#define to_perc_input(_neuron) dynamic_cast<neuron_perc_input *>(_neuron)
#define to_perc_hide(_neuron) dynamic_cast<neuron_perc_hide *>(_neuron)
#define to_perc_output(_neuron) dynamic_cast<neuron_perc_output *>(_neuron)

neuron_backprop::neuron_backprop() : neuron_with_input()
{
	_grad = 0;
}

neuron_backprop::~neuron_backprop()
{
}

float neuron_backprop::get_grad()
{
	return _grad;
}

void neuron_backprop::set_derivative(functype derivative)
{
	_derivative = derivative;
}

neuron_perc_input::neuron_perc_input() : neuron_input()
{
}

neuron_perc_input::~neuron_perc_input()
{
}

neuron_perc_hide::neuron_perc_hide() : neuron_hide(), neuron_backprop()
{
}

neuron_perc_hide::~neuron_perc_hide()
{
}

float neuron_perc_hide::_grad_summator()
{
	float s = 0;
	for (int i = 0; i < _outlen; i++)
	{
		if (to_perc_hide(_output[i] -> get_end()))
		{
			s += to_perc_hide(_output[i] -> get_end()) -> get_grad() * _output[i] -> get_weight();
		}
		else
		{
			s += to_perc_output(_output[i] -> get_end()) -> get_grad() * _output[i] -> get_weight();
		}
	}
	return s;
}

void neuron_perc_hide::refresh_grad()
{
	_grad = _grad_summator() * _derivative(_summator());
}

neuron_perc_output::neuron_perc_output() : neuron_output(), neuron_backprop()
{
}

neuron_perc_output::~neuron_perc_output()
{
}

void neuron_perc_output::calc_grad(float err)
{
	_grad = err * _derivative(_summator());
}

perceptron::perceptron(int *layers_lengths, int n)
{
	_neurons = new neuron **[n];

	for (int i = 0; i < n; i++)
	{
		_neurons[i] = new neuron *[layers_lengths[i]];
		for (int j = 0; j < layers_lengths[i]; j++)
		{
			if (i == 0)
			{
				_neurons[i][j] = new neuron_perc_input; //первый слой - входные нейроны
			}
			else if (i < n - 1)
			{
				//промежуточные слои - скрытые нейроны, сигмоидальная активационная функция
				_neurons[i][j] = new neuron_perc_hide;
				to_perc_hide(_neurons[i][j]) -> set_function(sigmoid_func);
				to_perc_hide(_neurons[i][j]) -> set_derivative(sigmoid_deriv);
			}
			else
			{
				//последний слой - выходной
				_neurons[i][j] = new neuron_perc_output;
				to_perc_output(_neurons[i][j]) -> set_function(sigmoid_func);
				to_perc_output(_neurons[i][j]) -> set_derivative(sigmoid_deriv);
			}
		}
	}

	_synapses = new synapse **[n - 1];

	int k;
	int j1, j2;
	for (int i = 0; i < n - 1; i++)
	{
		k = layers_lengths[i] * layers_lengths[i + 1];
		_synapses[i] = new synapse *[k];
		for (int j = 0; j < k; j++)
		{
			j1 = j / layers_lengths[i + 1];
			j2 = j % layers_lengths[i + 1];
			_synapses[i][j] = new synapse;
			_synapses[i][j] -> set_start(_neurons[i][j1]);
			_synapses[i][j] -> set_end(_neurons[i + 1][j2]);
			_synapses[i][j] -> set_weight((rand() % 2) * 2 - 1); //веса случайные, -1 или 1

			if (i == 0) to_perc_input(_neurons[i][j1]) ->add_output(_synapses[i][j]);
			else to_perc_hide(_neurons[i][j1]) ->add_output(_synapses[i][j]);

			if (i == n - 2) to_perc_output(_neurons[i + 1][j2]) ->add_input(_synapses[i][j]);
			else to_perc_hide(_neurons[i + 1][j2]) ->add_input(_synapses[i][j]);

		}
	}
	_layers_count = n;
	_layers_lengths = new int [n];
	for (int i = 0; i < n; i++)
	{
		_layers_lengths[i] = layers_lengths[i];
	}
}

perceptron::~perceptron()
{
}

bool perceptron::learn(float **learning_set, float **results, int set_length, float err, float speed)
{
	int x; //Number of iteration
	int success = 0;
	float cur_err;
	float *tmp, tmp2;
	for (x = 0; x < 1000; x++)
	{
		//Считаем ошибку модели на обучающем множестве
		cur_err = 0;
		for (int k = 0; k < set_length; k++)
		{
			tmp = get_result(learning_set[k]);
			for (int j = 0; j < _layers_lengths[_layers_count - 1]; j++)
			{
				tmp2 = results[k][j] - tmp[j];
				cur_err += tmp2 * tmp2;
			}
		}
		cur_err /= 2 * set_length;
		//Если ошибка меньше допустимой - успех
		if (cur_err < err)
		{
			success = 1;
			break;
		}
		for (int t = 0; t < 10; t++)
		{
			//Берем случайную обучающую пару, прогоняем через сеть
			int k;
			k = rand() % set_length;
			tmp = get_result(learning_set[k]);
			//Считаем отклонение от желаемого результата
			for (int j = 0; j < _layers_lengths[_layers_count - 1]; j++)
			{
				tmp2 = results[k][j] - tmp[j];
				to_perc_output(_neurons[_layers_count - 1][j]) -> calc_grad(tmp2);
			}
			//Выполняем обратное распространение ошибки
			for (int i = _layers_count - 2; i > 0; i--)
			{
				for (int j = 0; j < _layers_lengths[i]; j++)
				{
					to_perc_hide(_neurons[i][j]) -> refresh_grad();
				}
			}
			//Подстраиваем веса
			for (int i = 0; i < _layers_count - 1; i++)
			{
				for (int j = 0; j < _layers_lengths[i] * _layers_lengths[i + 1]; j++)
				{
					if (to_perc_hide(_synapses[i][j] -> get_end()))
					{
						_synapses[i][j] -> add_weight(speed * _synapses[i][j] -> get_start() -> get_status() * to_perc_hide(_synapses[i][j] -> get_end()) -> get_grad());
					}
					else
					{
						_synapses[i][j] -> add_weight(speed * _synapses[i][j] -> get_start() -> get_status() * to_perc_output(_synapses[i][j] -> get_end()) -> get_grad());
					}
				}
			}
		}
	}
	printf("%5d\t%5.3f\t",x*10, cur_err);
	return success;
}

float *perceptron::get_result(float *input)
{
	float *res = new float [_layers_lengths[_layers_count - 1]];
	for (int i = 0; i < _layers_count; i++)
	{
		for (int j = 0; j < _layers_lengths[i]; j++)
		{
			if (i == 0)
			{
				to_perc_input(_neurons[i][j]) -> set_status(input[j]); //Задаем значения входов
			}
			else if (i < _layers_count - 1)
			{
				to_perc_hide(_neurons[i][j]) -> refresh_status(); //обеспечиваем прохождение сигнала
			}
			else
			{
				to_perc_output(_neurons[i][j]) -> refresh_status();
				res[j] = to_perc_output(_neurons[i][j]) -> get_status();
			}
		}
	}
	return res; // и возвращаем результат
}

void perceptron::sendDiff(float *diff, float speed)
{
	//Выполняем обратное распространение ошибки
	for (int j = 0; j < _layers_lengths[_layers_count - 1]; j++)
	{
		to_perc_output(_neurons[_layers_count - 1][j]) -> calc_grad(diff[j]);
	}
	for (int i = _layers_count - 2; i > 0; i--)
	{
		for (int j = 0; j < _layers_lengths[i]; j++)
		{
			to_perc_hide(_neurons[i][j]) -> refresh_grad();
		}
	}
	//Подстраиваем веса
	for (int i = 0; i < _layers_count - 1; i++)
	{
		for (int j = 0; j < _layers_lengths[i] * _layers_lengths[i + 1]; j++)
		{
			if (to_perc_hide(_synapses[i][j] -> get_end()))
			{
				_synapses[i][j] -> add_weight(speed * _synapses[i][j] -> get_start() -> get_status() * to_perc_hide(_synapses[i][j] -> get_end()) -> get_grad());
			}
			else
			{
				_synapses[i][j] -> add_weight(speed * _synapses[i][j] -> get_start() -> get_status() * to_perc_output(_synapses[i][j] -> get_end()) -> get_grad());
			}
		}
	}
}

void perceptron::save(char* filename)
{
	FILE *f = fopen(filename, "w");
	fprintf(f, "%d\n", _layers_count);
	for (int i = 0; i < _layers_count; i++)
	{
		fprintf(f, "%d ", _layers_lengths[i]);
	}
	fprintf(f, "\n");

	for (int i = 0; i < _layers_count - 1; i++)
	{
		for (int j = 0; j < _layers_lengths[i] * _layers_lengths[i + 1]; j++)
		{
			fprintf(f, "%f ", _synapses[i][j] -> get_weight());
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

void perceptron::load(char* filename)
{
	FILE *f = fopen(filename, "r");
	int lc, *ls;
	fscanf(f, "%d\n", &lc);
	ls = new int [lc];
	for (int i = 0; i < lc; i++)
	{
		fscanf(f, "%d ", &(ls[i]));
	}
	fscanf(f, "\n");
	perceptron(ls, lc);

	float tmp;
	for (int i = 0; i < _layers_count - 1; i++)
	{
		for (int j = 0; j < _layers_lengths[i] * _layers_lengths[i + 1]; j++)
		{
			fscanf(f, "%f ", &tmp);
			_synapses[i][j] -> set_weight(tmp);
		}
		fscanf(f, "\n");
	}
	fclose(f);
}

float sigmoid_deriv(float x)
{
	return sigmoid_func(x) * (1.0 - sigmoid_func(x));
}