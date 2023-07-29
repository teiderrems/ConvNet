#include "full-connected.h"

double cpr::FullConnected::forward(cpr::Input& I)
{
    //cout<<I.x.size()<<endl;
	vector<double> temp = layers[0]->forward(I.x);
	for (int i = 1; i < this->layers.size(); i++)
	{
		temp = this->layers[i]->forward(temp);
	}
	temp = this->output->forward(temp);
    return temp[0];
}

vector<double> cpr::FullConnected::backward(cpr::Input& I,double alpha)
{
	
	int size = this->layers.size() - 1;
	double delta=this->output->getNeurones()[0]->gradientOutput(I.y);
	vector<double> A = this->output->extractVector(*this->layers[size]);
	vector<double> dw=this->output->getNeurones()[0]->dwOutput(A, delta);
	this->output->getNeurones()[0]->updateWeight(dw, alpha);
	this->output->getNeurones()[0]->updateBiais(delta, alpha);


	vector<double>del;
	del.resize(this->layers[size]->getNeurones().size());
	for (int i = 0; i < this->layers[size]->getNeurones().size(); i++)
	{
		del[i] = this->layers[size]->getNeurones()[i]->gradient(this->output->getNeurones()[0]->getWeight()[i], delta);
		A = layers[size]->extractVector(*this->layers[size - 1]);
		dw = this->layers[size]->getNeurones()[i]->dwOutput(A, del[i]);
		this->layers[size]->getNeurones()[i]->updateWeight(dw, alpha);
		this->layers[size]->getNeurones()[i]->updateBiais(del[i], alpha);
		

    }
    vector<double> temp;
	for (int i = size - 1; i > 0; i--)
    {
        temp = del;
		del.resize(this->layers[i]->getNeurones().size());
		for (int j = 0; j < this->layers[i]->getNeurones().size(); j++)
		{
			
			vector<double>w = this->layers[i]->extractWeight(*this->layers[i + 1], j);
			del[j] = this->layers[i]->getNeurones()[j]->gradient(w, temp);
            vector<double>().swap(w);
			A = layers[i]->extractVector(*this->layers[i-1]);
			dw=this->layers[i]->getNeurones()[j]->dwOutput(A, del[j]);
			this->layers[i]->getNeurones()[j]->updateBiais(del[j], alpha);
			this->layers[i]->getNeurones()[j]->updateWeight(dw,alpha);
			
		}
    }
    temp = del;
	del.resize(this->layers[0]->getNeurones().size());
	for (int i = 0; i < this->layers[0]->getNeurones().size(); i++)
	{
		
		vector<double>w = this->layers[0]->extractWeight(*this->layers[1], i);
		del[i] = this->layers[0]->getNeurones()[i]->gradient(w, temp);
        vector<double>().swap(w);
		dw = this->layers[0]->getNeurones()[i]->dwOutput(I.x, del[i]);
		this->layers[0]->getNeurones()[i]->updateWeight(dw, alpha);
		this->layers[0]->getNeurones()[i]->updateBiais(del[i], alpha);
		
	}
    vector<double>().swap(temp);
    vector<double>().swap(A);
    vector<double>().swap(dw);

	return del;
}

void cpr::FullConnected::save()
{
	for (int i = 0; i < this->layers.size(); i++)
	{
		this->layers[i]->saveBiais(i);
		this->layers[i]->saveWeight(i);
	}
	this->output->saveBiais(this->layers.size());
	this->output->saveWeight(this->layers.size());
}

cpr::FullConnected::~FullConnected(){

}
