//#include <stdio.h>

#include "cognitron.h"

#define to_cogn_input(_neuron) dynamic_cast<neuron_cogn_input *>(_neuron)
#define to_cogn_exc_hide(_neuron) dynamic_cast<neuron_cogn_exc_hide *>(_neuron)
#define to_cogn_inhibitory(_neuron) dynamic_cast<neuron_cogn_inhibitory *>(_neuron)
#define to_cogn_output(_neuron) dynamic_cast<neuron_cogn_output *>(_neuron)


cognitron_one_dim::cognitron_one_dim(int input_size, int layers_count, int link_area_size, int comp_area_size)
{
	_layers_count = layers_count;
	_layers_lengths = new int [_layers_count];
	_input_size = input_size;
	_input = new neuron *[_input_size];
	for (int j = 0; j < _input_size; j++)
	{
		_input[j] = new neuron_cogn_input;
	}
	_excitive = new neuron **[_layers_count];
	_inhibitory = new neuron **[_layers_count];
	_lateral_inhibitory = new neuron **[_layers_count];
	for (int i = 0; i < _layers_count; i++)
	{
		_layers_lengths[i] = _input_size;
		_excitive[i] = new neuron *[_layers_lengths[i]];
		_inhibitory[i] = new neuron *[_layers_lengths[i]];
		_lateral_inhibitory[i] = new neuron *[_layers_lengths[i]];
		for (int j = 0; j < _layers_lengths[i]; j++)
		{
			_excitive[i][j] = new neuron_cogn_exc_hide;
			_inhibitory[i][j] = new neuron_cogn_inhibitory;
			if (i != _layers_count - 1) _lateral_inhibitory[i][j] = new neuron_cogn_exc_hide;
		}
	}
	for (int j = 0; j < _layers_lengths[_layers_count - 1]; j++)
	{
		_lateral_inhibitory[_layers_count - 1][j] = new neuron_cogn_output;
	}

	int cur_area_size, cur_presyn_addition;
	float s;
	int tmp;
	_lat_to_exc = new synapse ***[_layers_count];
	_lat_to_inh = new synapse ***[_layers_count];
	_inh_to_exc = new synapse **[_layers_count];
	_exc_to_lat = new synapse ***[_layers_count];
	for (int i = 0; i < _layers_count; i++)
	{
		_lat_to_exc[i] = new synapse **[_layers_lengths[i]];
		_lat_to_inh[i] = new synapse **[_layers_lengths[i]];
		_inh_to_exc[i] = new synapse *[_layers_lengths[i]];
		_exc_to_lat[i] = new synapse **[_layers_lengths[i]];
		for (int j = 0; j < _layers_lengths[i]; j++)
		{
			cur_area_size = link_area_size;
			cur_presyn_addition = j - link_area_size / 2;
			if (j < link_area_size / 2)
			{
				cur_area_size -= link_area_size / 2 - j;
				cur_presyn_addition = 0;
			}
			if (i == 0)
			{
				if (j > _input_size - (link_area_size + 1) / 2)
				{
					cur_area_size -= (link_area_size + 1) / 2 + j - _input_size;
				}
			}
			else
			{
				if (j > _layers_lengths[i - 1]-(link_area_size + 1) / 2)
				{
					cur_area_size -= (link_area_size + 1) / 2 + j - _layers_lengths[i - 1];
				}
			}
			_lat_to_exc[i][j] = new synapse *[cur_area_size];
			_lat_to_inh[i][j] = new synapse *[cur_area_size];
			//			printf("le %2d, %2d: ", i, j);
			for (int k = 0; k < cur_area_size; k++)
			{
				_lat_to_exc[i][j][k] = new synapse;
				_lat_to_inh[i][j][k] = new synapse;
				if (i == 0)
				{
					_lat_to_exc[i][j][k] -> set_start(_input[k + cur_presyn_addition]);
					to_cogn_input(_input[k + cur_presyn_addition]) -> add_output(_lat_to_exc[i][j][k]);
					_lat_to_inh[i][j][k] -> set_start(_input[k + cur_presyn_addition]);
					to_cogn_input(_input[k + cur_presyn_addition]) -> add_output(_lat_to_inh[i][j][k]);
				}
				else
				{
					_lat_to_exc[i][j][k] -> set_start(_lateral_inhibitory[i - 1][k + cur_presyn_addition]);
					to_cogn_exc_hide(_lateral_inhibitory[i - 1][k + cur_presyn_addition]) -> add_output(_lat_to_exc[i][j][k]);
					_lat_to_inh[i][j][k] -> set_start(_lateral_inhibitory[i - 1][k + cur_presyn_addition]);
					to_cogn_exc_hide(_lateral_inhibitory[i - 1][k + cur_presyn_addition]) -> add_output(_lat_to_inh[i][j][k]);
				}
				_lat_to_exc[i][j][k] -> set_end(_excitive[i][j]);
				to_cogn_exc_hide(_excitive[i][j]) -> add_input(_lat_to_exc[i][j][k]);
				_lat_to_inh[i][j][k] -> set_end(_inhibitory[i][j]);
				to_cogn_inhibitory(_inhibitory[i][j]) -> add_input(_lat_to_inh[i][j][k]);
				_lat_to_exc[i][j][k] -> set_weight(rand());
				//				printf("%6.4f ", _lat_to_exc[i][j][k]->get_weight());
				_lat_to_inh[i][j][k] -> set_weight(rand());
			}
			//			printf("\n");
			s = 0;
			for (int k = 0; k < cur_area_size; k++)
			{
				s += _lat_to_inh[i][j][k] -> get_weight();
			}
			//			printf("li %2d, %2d: ", i, j);
			for (int k = 0; k < cur_area_size; k++)
			{
				_lat_to_inh[i][j][k] ->set_weight(_lat_to_inh[i][j][k] -> get_weight() / s);
				//				printf("%6.4f ", _lat_to_inh[i][j][k]->get_weight());
			}
			//			printf("\n");

			_inh_to_exc[i][j] = new synapse;
			_inh_to_exc[i][j] -> set_start(_inhibitory[i][j]);
			to_cogn_inhibitory(_inhibitory[i][j]) -> add_output(_inh_to_exc[i][j]);
			_inh_to_exc[i][j] -> set_end(_excitive[i][j]);
			to_cogn_exc_hide(_excitive[i][j]) -> add_inh_input(_inh_to_exc[i][j]);
			_inh_to_exc[i][j] -> set_weight(rand());
			//			printf("ie %2d, %2d: %6.4f\n", i, j, _inh_to_exc[i][j] -> get_weight());

			_exc_to_lat[i][j] = new synapse *[comp_area_size];
			cur_area_size = comp_area_size;
			cur_presyn_addition = j - comp_area_size / 2;
			if (j < comp_area_size / 2)
			{
				cur_area_size -= comp_area_size / 2 - j;
				cur_presyn_addition = 0;
			}
			if (j > _layers_lengths[i] - (comp_area_size + 1) / 2)
			{
				cur_area_size -= (comp_area_size + 1) / 2 + j - _layers_lengths[i];
			}
			_exc_to_lat[i][j] = new synapse *[cur_area_size];
			_exc_to_lat[i][j] = new synapse *[cur_area_size];
			//			printf("el %2d, %2d: ", i, j);
			for (int k = 0; k < cur_area_size; k++)
			{
				_exc_to_lat[i][j][k] = new synapse;
				_exc_to_lat[i][j][k] -> set_start(_excitive[i][k + cur_presyn_addition]);
				to_cogn_exc_hide(_excitive[i][k + cur_presyn_addition]) -> add_output(_exc_to_lat[i][j][k]);
				_exc_to_lat[i][j][k] -> set_end(_lateral_inhibitory[i][j]);
				if (i == _layers_count - 1)
				{
					if (k + cur_presyn_addition == j)
					{
						to_cogn_output(_lateral_inhibitory[i][j]) -> add_input(_exc_to_lat[i][j][k]);
					}
					else
					{
						to_cogn_output(_lateral_inhibitory[i][j]) -> add_inh_input(_exc_to_lat[i][j][k]);
					}
				}
				else
				{
					if (k + cur_presyn_addition == j)
					{
						to_cogn_exc_hide(_lateral_inhibitory[i][j]) -> add_input(_exc_to_lat[i][j][k]);
					}
					else
					{
						to_cogn_exc_hide(_lateral_inhibitory[i][j]) -> add_inh_input(_exc_to_lat[i][j][k]);
					}
				}
				tmp = rand();

				if (k + cur_presyn_addition == j)
				{
					_exc_to_lat[i][j][k] -> set_weight(1.0);
				}
				else
				{
					_exc_to_lat[i][j][k] -> set_weight(rand());
				}
			}
			s = 0;
			for (int k = 0; k < cur_area_size; k++)
			{
				if (k + cur_presyn_addition == j) continue;
				s += _exc_to_lat[i][j][k] -> get_weight();
			}
			for (int k = 0; k < cur_area_size; k++)
			{
				if (k + cur_presyn_addition == j) continue;
				_exc_to_lat[i][j][k] ->set_weight(_exc_to_lat[i][j][k] -> get_weight() / s);
				//				printf("%6.4f ", _exc_to_lat[i][j][k] -> get_weight());
			}
			//			printf("\n");
		}
	}
	for (int i = 0; i < _layers_count; i++)
	{
		for (int j = 0; j < _layers_lengths[i]; j++)
		{
			for (int k = 0; k < to_cogn_inhibitory(_inhibitory[i][j]) -> get_input_length(); k++)
			{
				//				printf("%2d, %2d, %2d: %6.4f\n", i, j, k, to_cogn_inhibitory(_inhibitory[i][j]) -> get_input_at(k) -> get_weight());
			}
		}
	}
}

cognitron_one_dim::~cognitron_one_dim()
{

}

float *cognitron_one_dim::get_result(float *input)
{
	for (int i = 0; i < _input_size; i++)
	{
		to_cogn_input(_input[i]) -> set_status(input[i]);
		//				printf("> %2d    : %.4f\n", i, _input[i] -> get_status());
	}
	float *res = new float [_layers_lengths[_layers_count - 1]];
	for (int i = 0; i < _layers_count - 1; i++)
	{
		for (int j = 0; j < _layers_lengths[i]; j++)
		{
			to_cogn_inhibitory(_inhibitory[i][j]) -> refresh_status();
			//			printf("i %2d, %2d: %6.4f\n", i, j, _inhibitory[i][j] -> get_status());
			for (int k = 0; k < to_cogn_inhibitory(_inhibitory[i][j])->get_input_length(); k++)
			{
				//				printf("%6.4f ", to_cogn_inhibitory(_inhibitory[i][j])->get_input_at(k)->get_weight());
			}
			//			printf("\n");
		}
		for (int j = 0; j < _layers_lengths[i]; j++)
		{
			to_cogn_exc_hide(_excitive[i][j]) -> refresh_status();
			//			printf("e %2d, %2d: %.4f\n", i, j, _excitive[i][j] -> get_status());
		}
		for (int j = 0; j < _layers_lengths[i]; j++)
		{
			to_cogn_exc_hide(_lateral_inhibitory[i][j]) -> refresh_status();
			//			printf("l %2d, %2d: %.4f\n", i, j, _lateral_inhibitory[i][j] -> get_status());
		}
	}
	for (int j = 0; j < _layers_lengths[ _layers_count - 1]; j++)
	{
		to_cogn_inhibitory(_inhibitory[_layers_count - 1][j]) -> refresh_status();
		//		printf("i %2d, %2d: %.4f\n", _layers_count - 1, j, _inhibitory[_layers_count - 1][j] -> get_status());
	}
	for (int j = 0; j < _layers_lengths[ _layers_count - 1]; j++)
	{
		to_cogn_exc_hide(_excitive[_layers_count - 1][j]) -> refresh_status();
		//		printf("e %2d, %2d: %.4f\n", _layers_count - 1, j, _excitive[_layers_count - 1][j] -> get_status());
	}
	for (int j = 0; j < _layers_lengths[ _layers_count - 1]; j++)
	{
		to_cogn_output(_lateral_inhibitory[_layers_count - 1][j]) -> refresh_status();
		//		printf("o %2d    : %.4f\n", j, _lateral_inhibitory[_layers_count - 1][j] -> get_status());
		res[j] = _lateral_inhibitory[_layers_count - 1][j] -> get_status();
	}
	return res;
}

void cognitron_one_dim::learn(float** learning_set, int count, int length, float q, float q2)
{
	float *res;
	float s;
	neuron_cogn_exc_hide *cur_lat;
	neuron_cogn_exc_hide *cur_exc;
	neuron_cogn_output *cur_out;
	int tmp;
	for (int k = 0; k < length; k++)
	{
		res = get_result(learning_set[rand() % count]);
		for (int i = 0; i < _layers_count; i++)
		{
			for (int j = 0; j < _layers_lengths[i]; j++)
			{
				tmp = 1;
				cur_lat = to_cogn_exc_hide(_lateral_inhibitory[i][j]);
				cur_out = to_cogn_output(_lateral_inhibitory[i][j]);
				cur_exc = to_cogn_exc_hide(_excitive[i][j]);
				if (cur_lat)
				{
					for (int t = 0; t < cur_lat -> get_inh_input_length(); t++)
					{
						if (cur_lat -> get_inh_input_at(t) -> get_start() -> get_status() >= cur_exc -> get_status())
						{
							tmp = 0;
							break;
						}
					}
				}
				else
				{
					for (int t = 0; t < cur_out -> get_inh_input_length(); t++)
					{
						if (cur_out -> get_inh_input_at(t) -> get_start() -> get_status() >= cur_exc -> get_status())
						{
							tmp = 0;
							break;
						}
					}
				}

				if (tmp)
				{
					s = 0;
					for (int t = 0; t < cur_exc -> get_input_length(); t++)
					{
						s += cur_exc->get_input_at(t)->get_start()->get_status() * cur_exc->get_input_at(t)->get_weight();
					}
					_inh_to_exc[i][j] -> add_weight(q * s / 2 / _inhibitory[i][j]->get_status());
					for (int t = 0; t < cur_exc -> get_input_length(); t++)
					{
						_lat_to_exc[i][j][t]->add_weight(q * _lat_to_inh[i][j][t]->get_weight() * _lat_to_inh[i][j][t]->get_start()->get_status());
					}
				}
				else
				{
					for (int t = 0; t < cur_exc -> get_input_length(); t++)
					{
						_lat_to_exc[i][j][t]->add_weight(q2 * _lat_to_inh[i][j][t]->get_weight() * _lat_to_inh[i][j][t]->get_start()->get_status());
					}
					_inh_to_exc[i][j] -> add_weight(q2 * _inhibitory[i][j]->get_status());
				}
			}
		}
	}
}

cognitron_square::cognitron_square(int input_size, int layers_count, int link_area_size, int comp_area_size)
{
	// i is index of layer
	// x is index of row in current layer
	// y is index of neuron in current row
	// j is index of row in last layer
	// k is index of neuron in row at last layer
	_layers_count = layers_count;
	_layers_lengths = new int [_layers_count];
	_input_size = input_size;
	_input = new neuron **[_input_size];
	for (int x = 0; x < _input_size; x++)
	{
		_input[x] = new neuron *[_input_size];
		for (int y = 0; y < _input_size; y++)
		{
			_input[x][y] = new neuron_cogn_input;
		}
	}
	_excitive = new neuron ***[_layers_count];
	_inhibitory = new neuron ***[_layers_count];
	_lateral_inhibitory = new neuron ***[_layers_count];
	for (int i = 0; i < _layers_count; i++)
	{
		_layers_lengths[i] = _input_size;
		_excitive[i] = new neuron **[_layers_lengths[i]];
		_inhibitory[i] = new neuron **[_layers_lengths[i]];
		_lateral_inhibitory[i] = new neuron **[_layers_lengths[i]];
		for (int x = 0; x < _layers_lengths[i]; x++)
		{
			_excitive[i][x] = new neuron *[_layers_lengths[i]];
			_inhibitory[i][x] = new neuron *[_layers_lengths[i]];
			_lateral_inhibitory[i][x] = new neuron *[_layers_lengths[i]];
			for (int y = 0; y < _layers_lengths[i]; y++)
			{
				_excitive[i][x][y] = new neuron_cogn_exc_hide;
				_inhibitory[i][x][y] = new neuron_cogn_inhibitory;
				if (i != _layers_count - 1) _lateral_inhibitory[i][x][y] = new neuron_cogn_exc_hide;
			}
		}
	}
	for (int x = 0; x < _layers_lengths[_layers_count - 1]; x++)
	{
		for (int y = 0; y < _layers_lengths[_layers_count - 1]; y++)
		{
			_lateral_inhibitory[_layers_count - 1][x][y] = new neuron_cogn_output;
		}
	}

	int cur_area_size_x, cur_area_size_y;
	int cur_presyn_addition_x, cur_presyn_addition_y;
	float s;
	int tmp;
	_lat_to_exc = new synapse *****[_layers_count];
	_lat_to_inh = new synapse *****[_layers_count];
	_inh_to_exc = new synapse ***[_layers_count];
	_exc_to_lat = new synapse *****[_layers_count];
	for (int i = 0; i < _layers_count; i++)
	{
		_lat_to_exc[i] = new synapse ****[_layers_lengths[i]];
		_lat_to_inh[i] = new synapse ****[_layers_lengths[i]];
		_inh_to_exc[i] = new synapse **[_layers_lengths[i]];
		_exc_to_lat[i] = new synapse ****[_layers_lengths[i]];
		for (int x = 0; x < _layers_lengths[i]; x++)
		{
			_lat_to_exc[i][x] = new synapse ***[_layers_lengths[i]];
			_lat_to_inh[i][x] = new synapse ***[_layers_lengths[i]];
			cur_area_size_x = link_area_size;
			cur_presyn_addition_x = x - link_area_size / 2;
			if (x < link_area_size / 2)
			{
				cur_area_size_x -= link_area_size / 2 - x;
				cur_presyn_addition_x = 0;
			}
			if (i == 0)
			{
				if (x > _input_size - (link_area_size + 1) / 2)
				{
					cur_area_size_x -= (link_area_size + 1) / 2 + x - _input_size;
				}
			}
			else
			{
				if (x > _layers_lengths[i - 1] - (link_area_size + 1) / 2)
				{
					cur_area_size_x -= (link_area_size + 1) / 2 + x - _layers_lengths[i - 1];
				}
			}
			for (int y = 0; y < _layers_lengths[i]; y++)
			{
				cur_area_size_y = link_area_size;
				cur_presyn_addition_y = y - link_area_size / 2;
				if (y < link_area_size / 2)
				{
					cur_area_size_y -= link_area_size / 2 - y;
					cur_presyn_addition_y = 0;
				}
				if (i == 0)
				{
					if (y > _input_size - (link_area_size + 1) / 2)
					{
						cur_area_size_y -= (link_area_size + 1) / 2 + y - _input_size;
					}
				}
				else
				{
					if (y > _layers_lengths[i - 1] - (link_area_size + 1) / 2)
					{
						cur_area_size_y -= (link_area_size + 1) / 2 + y - _layers_lengths[i - 1];
					}
				}
				_lat_to_exc[i][x][y] = new synapse **[cur_area_size_x];
				_lat_to_inh[i][x][y] = new synapse **[cur_area_size_x];
				//			printf("le %2d, %2d: ", i, j);
				for (int j = 0; j < cur_area_size_x; j++)
				{
					_lat_to_exc[i][x][y][j] = new synapse *[cur_area_size_y];
					_lat_to_inh[i][x][y][j] = new synapse *[cur_area_size_y];
					for (int k = 0; k < cur_area_size_y; k++)
					{
						_lat_to_exc[i][x][y][j][k] = new synapse;
						_lat_to_inh[i][x][y][j][k] = new synapse;
						if (i == 0)
						{
							_lat_to_exc[i][x][y][j][k] -> set_start(_input[j + cur_presyn_addition_x][k + cur_presyn_addition_y]);
							to_cogn_input(_input[j + cur_presyn_addition_x][k + cur_presyn_addition_y]) -> add_output(_lat_to_exc[i][x][y][j][k]);
							_lat_to_inh[i][x][y][j][k] -> set_start(_input[j + cur_presyn_addition_x][k + cur_presyn_addition_y]);
							to_cogn_input(_input[j + cur_presyn_addition_x][k + cur_presyn_addition_y]) -> add_output(_lat_to_inh[i][x][y][j][k]);
						}
						else
						{
							_lat_to_exc[i][x][y][j][k] -> set_start(_lateral_inhibitory[i - 1][j + cur_presyn_addition_x][k + cur_presyn_addition_y]);
							to_cogn_exc_hide(_lateral_inhibitory[i - 1][j + cur_presyn_addition_x][k + cur_presyn_addition_y]) -> add_output(_lat_to_exc[i][x][y][j][k]);
							_lat_to_inh[i][x][y][j][k] -> set_start(_lateral_inhibitory[i - 1][j + cur_presyn_addition_x][k + cur_presyn_addition_y]);
							to_cogn_exc_hide(_lateral_inhibitory[i - 1][j + cur_presyn_addition_x][k + cur_presyn_addition_y]) -> add_output(_lat_to_inh[i][x][y][j][k]);
						}
						_lat_to_exc[i][x][y][j][k] -> set_end(_excitive[i][x][y]);
						to_cogn_exc_hide(_excitive[i][x][y]) -> add_input(_lat_to_exc[i][x][y][j][k]);
						_lat_to_inh[i][x][y][j][k] -> set_end(_inhibitory[i][x][y]);
						to_cogn_inhibitory(_inhibitory[i][x][y]) -> add_input(_lat_to_inh[i][x][y][j][k]);
						_lat_to_exc[i][x][y][j][k] -> set_weight(rand());
						//				printf("%6.4f ", _lat_to_exc[i][j][k]->get_weight());
						_lat_to_inh[i][x][y][j][k] -> set_weight(rand());
					}
				}
				//			printf("\n");
				s = 0;
				for (int j = 0; j < cur_area_size_x; j++)
				{
					for (int k = 0; k < cur_area_size_y; k++)
					{
						s += _lat_to_inh[i][x][y][j][k] -> get_weight();
					}
				}
				//			printf("li %2d, %2d: ", i, j);
				for (int j = 0; j < cur_area_size_x; j++)
				{
					for (int k = 0; k < cur_area_size_y; k++)
					{
						_lat_to_inh[i][x][y][j][k] ->set_weight(_lat_to_inh[i][x][y][j][k] -> get_weight() / s);
						//				printf("%6.4f ", _lat_to_inh[i][j][k]->get_weight());
					}
					//			printf("\n");
				}
			}

			_inh_to_exc[i][x] = new synapse *[_layers_lengths[i]];
			for (int y = 0; y < _layers_lengths[i]; y++)
			{
				_inh_to_exc[i][x][y] = new synapse;
				_inh_to_exc[i][x][y] -> set_start(_inhibitory[i][x][y]);
				to_cogn_inhibitory(_inhibitory[i][x][y]) -> add_output(_inh_to_exc[i][x][y]);
				_inh_to_exc[i][x][y] -> set_end(_excitive[i][x][y]);
				to_cogn_exc_hide(_excitive[i][x][y]) -> add_inh_input(_inh_to_exc[i][x][y]);
				_inh_to_exc[i][x][y] -> set_weight(rand());
				//			printf("ie %2d, %2d: %6.4f\n", i, j, _inh_to_exc[i][j] -> get_weight());
			}

			_exc_to_lat[i][x] = new synapse ***[_layers_lengths[i]];
			cur_area_size_x = comp_area_size;
			cur_presyn_addition_x = x - comp_area_size / 2;
			if (x < comp_area_size / 2)
			{
				cur_area_size_x -= comp_area_size / 2 - x;
				cur_presyn_addition_x = 0;
			}
			if (x > _layers_lengths[i] - (comp_area_size + 1) / 2)
			{
				cur_area_size_x -= (comp_area_size + 1) / 2 + x - _layers_lengths[i];
			}
			for (int y = 0; y < _layers_lengths[i]; y++)
			{
				cur_area_size_y = comp_area_size;
				cur_presyn_addition_y = y - comp_area_size / 2;
				if (y < comp_area_size / 2)
				{
					cur_area_size_y -= comp_area_size / 2 - y;
					cur_presyn_addition_y = 0;
				}
				if (y > _layers_lengths[i] - (comp_area_size + 1) / 2)
				{
					cur_area_size_y -= (comp_area_size + 1) / 2 + y - _layers_lengths[i];
				}
				_exc_to_lat[i][x][y] = new synapse **[cur_area_size_x];
				_exc_to_lat[i][x][y] = new synapse **[cur_area_size_x];
				//			printf("el %2d, %2d: ", i, j);
				for (int j = 0; j < cur_area_size_x; j++)
				{
					_exc_to_lat[i][x][y][j] = new synapse *[cur_area_size_x];
					_exc_to_lat[i][x][y][j] = new synapse *[cur_area_size_x];
					for (int k = 0; k < cur_area_size_y; k++)
					{
						_exc_to_lat[i][x][y][j][k] = new synapse;
						_exc_to_lat[i][x][y][j][k] -> set_start(_excitive[i][j + cur_presyn_addition_x][k + cur_presyn_addition_y]);
						to_cogn_exc_hide(_excitive[i][j + cur_presyn_addition_x][k + cur_presyn_addition_y]) -> add_output(_exc_to_lat[i][x][y][j][k]);
						_exc_to_lat[i][x][y][j][k] -> set_end(_lateral_inhibitory[i][x][y]);
						if (i == _layers_count - 1)
						{
							if (j + cur_presyn_addition_x == x && k + cur_presyn_addition_y == y)
							{
								to_cogn_output(_lateral_inhibitory[i][x][y]) -> add_input(_exc_to_lat[i][x][y][j][k]);
							}
							else
							{
								to_cogn_output(_lateral_inhibitory[i][x][y]) -> add_inh_input(_exc_to_lat[i][x][y][j][k]);
							}
						}
						else
						{
							if (j + cur_presyn_addition_x == x && k + cur_presyn_addition_y == y)
							{
								to_cogn_exc_hide(_lateral_inhibitory[i][x][y]) -> add_input(_exc_to_lat[i][x][y][j][k]);
							}
							else
							{
								to_cogn_exc_hide(_lateral_inhibitory[i][x][y]) -> add_inh_input(_exc_to_lat[i][x][y][j][k]);
							}
						}
						tmp = rand();

						if (j + cur_presyn_addition_x == x && k + cur_presyn_addition_y == y)
						{
							_exc_to_lat[i][x][y][j][k] -> set_weight(1.0);
						}
						else
						{
							_exc_to_lat[i][x][y][j][k] -> set_weight(rand());
						}
					}
				}
				s = 0;
				for (int j = 0; j < cur_area_size_x; j++)
				{
					for (int k = 0; k < cur_area_size_y; k++)
					{
						if (j + cur_presyn_addition_x == x && k + cur_presyn_addition_y == y) continue;
						s += _exc_to_lat[i][x][y][j][k] -> get_weight();
					}
				}
				for (int j = 0; j < cur_area_size_x; j++)
				{
					for (int k = 0; k < cur_area_size_y; k++)
					{
						if (j + cur_presyn_addition_x == x && k + cur_presyn_addition_y == y) continue;
						_exc_to_lat[i][x][y][j][k] ->set_weight(_exc_to_lat[i][x][y][j][k] -> get_weight() / s);
						//				printf("%6.4f ", _exc_to_lat[i][j][k] -> get_weight());
					}
				}
				//			printf("\n");
			}
		}
	}
	//	for (int i = 0; i < _layers_count; i++)
	//	{
	//		for (int j = 0; j < _layers_lengths[i]; j++)
	//		{
	//			for (int k = 0; k < to_cogn_inhibitory(_inhibitory[i][j]) -> get_input_length(); k++)
	//			{
	//				printf("%2d, %2d, %2d: %6.4f\n", i, j, k, to_cogn_inhibitory(_inhibitory[i][j]) -> get_input_at(k) -> get_weight());
	//			}
	//		}
	//	}
}

cognitron_square::~cognitron_square()
{

}

float **cognitron_square::get_result(float **input)
{
	for (int i = 0; i < _input_size; i++)
	{
		for (int j = 0; j < _input_size; j++)
		{
			to_cogn_input(_input[i][j]) -> set_status(input[i][j]);
			//				printf("> %2d    : %.4f\n", i, _input[i] -> get_status());
		}
	}
	float **res = new float *[_layers_lengths[_layers_count - 1]];
	for (int i = 0; i < _layers_lengths[_layers_count - 1]; i++)
	{
		res[i] = new float [_layers_lengths[_layers_count - 1]];
	}

	for (int i = 0; i < _layers_count - 1; i++)
	{
		for (int x = 0; x < _layers_lengths[i]; x++)
		{
			for (int y = 0; y < _layers_lengths[i]; y++)
			{
				to_cogn_inhibitory(_inhibitory[i][x][y]) -> refresh_status();
				//			printf("i %2d, %2d: %6.4f\n", i, j, _inhibitory[i][j] -> get_status());
				for (int k = 0; k < to_cogn_inhibitory(_inhibitory[i][x][y])->get_input_length(); k++)
				{
					//				printf("%6.4f ", to_cogn_inhibitory(_inhibitory[i][j])->get_input_at(k)->get_weight());
				}
				//			printf("\n");
			}
		}
		for (int x = 0; x < _layers_lengths[i]; x++)
		{
			for (int y = 0; y < _layers_lengths[i]; y++)
			{
				to_cogn_exc_hide(_excitive[i][x][y]) -> refresh_status();
				//			printf("e %2d, %2d: %.4f\n", i, j, _excitive[i][j] -> get_status());
			}
		}
		for (int x = 0; x < _layers_lengths[i]; x++)
		{
			for (int y = 0; y < _layers_lengths[i]; y++)
			{
				to_cogn_exc_hide(_lateral_inhibitory[i][x][y]) -> refresh_status();
				//			printf("l %2d, %2d: %.4f\n", i, j, _lateral_inhibitory[i][x][y] -> get_status());
			}
		}
	}
	for (int x = 0; x < _layers_lengths[_layers_count - 1]; x++)
	{
		for (int y = 0; y < _layers_lengths[_layers_count - 1]; y++)
		{
			to_cogn_inhibitory(_inhibitory[_layers_count - 1][x][y]) -> refresh_status();
			//		printf("i %2d, %2d: %.4f\n", _layers_count - 1, j, _inhibitory[_layers_count - 1][x][y] -> get_status());
		}
	}
	for (int x = 0; x < _layers_lengths[_layers_count - 1]; x++)
	{
		for (int y = 0; y < _layers_lengths[_layers_count - 1]; y++)
		{
			to_cogn_exc_hide(_excitive[_layers_count - 1][x][y]) -> refresh_status();
			//		printf("e %2d, %2d: %.4f\n", _layers_count - 1, j, _excitive[_layers_count - 1][x][y] -> get_status());
		}
	}
	for (int x = 0; x < _layers_lengths[_layers_count - 1]; x++)
	{
		for (int y = 0; y < _layers_lengths[_layers_count - 1]; y++)
		{
			to_cogn_output(_lateral_inhibitory[_layers_count - 1][x][y]) -> refresh_status();
			//		printf("o %2d    : %.4f\n", j, _lateral_inhibitory[_layers_count - 1][x][y] -> get_status());
			res[x][y] = _lateral_inhibitory[_layers_count - 1][x][y] -> get_status();
		}
	}
	return res;
}

void cognitron_square::learn(float*** learning_set, int count, int length, float q, float q2)
{
	float **res;
	float s;
	neuron_cogn_exc_hide *cur_lat;
	neuron_cogn_exc_hide *cur_exc;
	neuron_cogn_output *cur_out;
	synapse *cur1, *cur2;
	int tmp;
	for (int k = 0; k < length; k++)
	{
		res = get_result(learning_set[rand() % count]);
		for (int i = 0; i < _layers_count; i++)
		{
			for (int x = 0; x < _layers_lengths[i]; x++)
			{
				for (int y = 0; y < _layers_lengths[i]; y++)
				{
					tmp = 1;
					cur_lat = to_cogn_exc_hide(_lateral_inhibitory[i][x][y]);
					cur_out = to_cogn_output(_lateral_inhibitory[i][x][y]);
					cur_exc = to_cogn_exc_hide(_excitive[i][x][y]);
					if (cur_lat)
					{
						for (int t = 0; t < cur_lat -> get_inh_input_length(); t++)
						{
							if (cur_lat -> get_inh_input_at(t) -> get_start() -> get_status() >= cur_exc -> get_status())
							{
								tmp = 0;
								break;
							}
						}
					}
					else
					{
						for (int t = 0; t < cur_out -> get_inh_input_length(); t++)
						{
							if (cur_out -> get_inh_input_at(t) -> get_start() -> get_status() >= cur_exc -> get_status())
							{
								tmp = 0;
								break;
							}
						}
					}

					if (tmp)
					{
						s = 0;
						for (int t = 0; t < cur_exc -> get_input_length(); t++)
						{
							s += cur_exc->get_input_at(t)->get_start()->get_status() * cur_exc->get_input_at(t)->get_weight();
						}
						_inh_to_exc[i][x][y] -> add_weight(q * s / 2 / _inhibitory[i][x][y]->get_status());
						for (int t = 0; t < cur_exc -> get_input_length(); t++)
						{
							cur1 = to_cogn_exc_hide(_excitive[i][x][y]) -> get_input_at(t);
							cur2 = to_cogn_inhibitory(_inhibitory[i][x][y]) -> get_input_at(t);
							cur1->add_weight(q * cur2->get_weight() * cur2->get_start()->get_status());
						}
					}
					else
					{
						_inh_to_exc[i][x][y] -> add_weight(q2 * _inhibitory[i][x][y]->get_status());
						for (int t = 0; t < cur_exc -> get_input_length(); t++)
						{
							cur1 = to_cogn_exc_hide(_excitive[i][x][y]) -> get_input_at(t);
							cur2 = to_cogn_inhibitory(_inhibitory[i][x][y]) -> get_input_at(t);
							cur1->add_weight(q2 * cur2->get_weight() * cur2->get_start()->get_status());
						}
					}
				}
			}
		}
	}
}

