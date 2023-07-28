#include "layer.h"

vector<double> cpr::Layer::forward(vector<double>& x)
{
	vector<double> temp;
    //cout<<this->neurones.size()<<endl;
	temp.resize(this->neurones.size());
	for (int i = 0; i < this->neurones.size(); i++)
	{
		temp[i]=this->neurones[i]->activate(x);
	}
	return temp;
}



vector<double> cpr::Layer::extractVector(Layer& prev) {
	vector<double> vec;
	vec.resize(prev.getNumber());
	for (int i = 0; i < prev.getNumber(); i++)
	{
		vec[i] = prev.getNeurones()[i]->getY();
	}
	return vec;
}

vector<double> cpr::Layer::extractWeight(Layer& next,int j)
{
	vector<double> w;
	w.resize(next.getNeurones().size());
	for (int i = 0; i < next.getNeurones().size(); i++)
	{
		w[i] = next.getNeurones()[i]->getWeight()[j];
	}

	return w;
}

void cpr::Layer::saveWeight(int j)
{
	
    string filename = "fc/weightLayer" + to_string(j) +".txt";
	ofstream f(filename);
	
	if (!f.is_open()) {

		exit(EXIT_FAILURE);
	}
	else
	{
        f << this->neurones.size() << "\t"<<this->neurones[0]->getWeight().size()<<"\n";
		for (int i = 0; i < this->neurones.size(); i++)
		{
			for (auto& w:this->neurones[i]->getWeight())
			{
                f << w << "\n";
			}
			f << "\n";
		}
	}
	
}

void cpr::Layer::saveBiais(int j)
{
    string filename = "fc/biaisLayer" + to_string(j+1) + ".txt";
	ofstream f(filename);
	if (!f.is_open()) {

		exit(EXIT_FAILURE);
	}
	else
	{
        f <<this->neurones.size()<<"\n";
		for (int i = 0; i < this->neurones.size(); i++)
		{
            f << this->neurones[i]->getBiais() << "\n";
		}
	}
}
