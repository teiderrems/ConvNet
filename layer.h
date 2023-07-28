#ifndef ___LAYER___
#define ___LAYER___
#include "neurone.h"
#include "util.h"
namespace cpr {
	class Layer
	{
	private:
		vector<vector<double>> Weight;
		vector<double> agregas;
		vector<Neurone*> neurones;
		int nbNeurone{ 27 };
        vector<double> biais;
	public:

		Layer() {}

		Layer(int n) {
            this->biais = cpr::Util::genBiais(n);
			this->nbNeurone = n;
            //this->agregas.resize(n);
			this->neurones.resize(n);
			for (int i = 0; i < n; i++)
			{
				this->neurones[i] = new Neurone();
                this->neurones[i]->setBiais(this->biais[i]);
			}
		}

		vector<Neurone*> getNeurones() {
			return this->neurones;
		}
		int getNumber() {
			return this->nbNeurone;
		}
		vector<double> forward(vector<double>&);
        Layer(const vector<vector<double>>& w, vector<double>& biais)
			:Weight(w), biais(biais)
		{
            this->neurones.resize(w.size());
            for (int i = 0; i < this->neurones.size(); i++)
            {

                this->neurones[i]->setWeight(this->Weight[i]);
                this->neurones[i]->setBiais(this->biais[i]);
            }
		}
		//void backward(Input&);
		vector<double> extractVector(Layer& prev);
		vector<double> extractWeight(Layer& next,int);
		void saveWeight(int);
		void saveBiais(int);
        ~Layer() {
//            vector<vector<double>>().swap(this->Weight);
//            vector<double>().swap(this->agregas);
//            vector<Neurone>().swap(this->neurones);
//            vector<double>().swap(this->biais);
        }
	};
}
#endif // !___LAYER___



