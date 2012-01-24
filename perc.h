#ifndef _PERC_H
#define _PERC_H

#include "core.h"

class neuron_backprop : public virtual neuron_with_input //Нейрон, обучаемый по алгоритму обратного распространения
{
protected:
	float _grad; //Градиент поверхности ошибки
	functype _derivative; //Производная активационной функции
public:
	neuron_backprop();
	virtual ~neuron_backprop();
	float get_grad(); //Возвращает величину градиента
	void set_derivative(functype derivative); //Задает производную активационной функции
};

class neuron_perc_input : public virtual neuron_input //Нейрон входного слоя персептрона
{
public:
	neuron_perc_input();
	virtual ~neuron_perc_input();
};

class neuron_perc_hide : public virtual neuron_hide, public virtual neuron_backprop
//Нейрон скрытого слоя персептрона
{
protected:
	float _grad_summator(); //Возвращает взвешенную сумму градиентов при обратном распространении
public:
	neuron_perc_hide();
	virtual ~neuron_perc_hide();
	void refresh_grad(); //Пересчитывает градиент для нейрона
};

class neuron_perc_output : public virtual neuron_output, public virtual neuron_backprop
//Нейрон выходного слоя персептрона
{
public:
	neuron_perc_output();
	virtual ~neuron_perc_output();
	void calc_grad(float err); //Вычисляет градиент основываясь на ошибке
};

class perceptron //Модель персептрона
{
protected:
	int _layers_count; //Количество слоев
	int *_layers_lengths; //Размеры слоев
	neuron ***_neurons; //Массив нейронов (по слоям)
	synapse ***_synapses; //Массив связей (по слоям)
public:
	perceptron(int *layers_lengths, int n); //Конструктор с параметрами, ...
	//... принимает массив размеров слоев и число слоев
	virtual ~perceptron();
	bool learn(float **learning_set, float **results, int set_length, float err, float speed);
	//Обучает модель, принимает массив обучающих примеров, массив желаемых откликов на примеры, ...
	//... количество примеров в массиве, допустимую погрешность модели и скорость обучения
	float *get_result(float *input); //Возвращает отклик сети
	void sendDiff(float *diff, float speed); //Принимает отклонение последнего отклика сети от
	//значения желаемого выхода (реального объекта) и скорость обучения, дообучает сеть
	void save(char *filename); //Сохраняет нейронную сеть в файл
	void load(char *filename); //Загружает нейронную сеть из файла
};

float sigmoid_deriv(float x); //Derivative of sigma-function.

#endif