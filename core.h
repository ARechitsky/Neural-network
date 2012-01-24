#ifndef _CORE_H
#define _CORE_H

#include <stdlib.h>
#include <math.h>

#define to_threshold(_neuron) dynamic_cast<neuron_threshold *>(_neuron)

typedef float (*functype) (float); //Активационная функция

class neuron //Базовая модель нейрона
{
protected:
	float _status; //Состояние
public:
	neuron();
	virtual ~neuron();
	float get_status(); //Возвращает состояние нейрона
};

class synapse //Связь между нейронами
{
private:
	neuron *_start; //Входной нейрон
	neuron *_end; //Выходной нейрон
	float _weight; //Вес связи
public:
	synapse();
	virtual ~synapse();

	void set_start(neuron *start); //Задает входной нейрон
	neuron *get_start(); //Возвращает входной нейрон

	void set_end(neuron *end); //Задает выходной нейрон
	neuron *get_end(); //Возвращает выходной нейрон

	void set_weight(float weight); //Задает вес связи
	void add_weight(float weight); //Изменяет вес связи
	float get_weight(); //Возвращает вес связи
};

class neuron_with_output : public virtual neuron //Нейрон, имеющий выходные связи
{
protected:
	synapse **_output; //Массив связей
	int _outlen; //Количество связей
public:
	neuron_with_output();
	virtual ~neuron_with_output();

	void add_output(synapse *out_synapse); //Добавляет выходную связь
	int get_output_length(); //Возвращает количество связей
	synapse *get_output_at(int num); //Возвращает связь с номером num
};

class neuron_without_output : public virtual neuron //Нейрон, не имеющий выходных связей
{
public:
	neuron_without_output();
	virtual ~neuron_without_output();
};

class neuron_with_input : public virtual neuron //Нейрон, имеющий входные связи
{
protected:
	synapse **_input; //Массив связей
	int _inlen; //Количество связей
	functype _function;  //Активационная функция
	float _summator(); //Сумматор, вычисляющий взвешенную сумму входов
public:
	neuron_with_input();
	virtual ~neuron_with_input();

	void add_input(synapse *in_synapse); //Добавляет выходную связь
	int get_input_length(); //Возвращает количество связей
	synapse *get_input_at(int num); //Возвращает связь с номером num

	void set_function(functype function); //Задает активационную функцию
	void refresh_status(); //Вычисляет новое состояние
};

class neuron_without_input : public virtual neuron //Нейрон, не имеющий входных связей
{
public:
	neuron_without_input();
	virtual ~neuron_without_input();
	void set_status(float status); //Задает состояние нейрона
};

class neuron_input : public virtual neuron_with_output, public virtual neuron_without_input
//Нейрон входного слоя
{
public:
	neuron_input();
	virtual ~neuron_input();
};

class neuron_output : public virtual neuron_without_output, public virtual neuron_with_input
//Нейрон выходного слоя
{
public:
	neuron_output();
	virtual ~neuron_output();
};

class neuron_hide : public virtual neuron_with_output, public virtual neuron_with_input
//Нейрон скрытого слоя
{
public:
	neuron_hide();
	virtual ~neuron_hide();
};

class neuron_threshold : public virtual neuron_with_output //Пороговый нейрон, состояние всегда равно 1
{
public:
	neuron_threshold();
	virtual ~neuron_threshold();
};

float sigmoid_func(float x); //Сигмоидальная активационная функция
float threshhold_func(float x); //Пороговая активационная функция
float linear_func(float x); //Линейная активационная функция

#endif