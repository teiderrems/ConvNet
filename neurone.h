#ifndef ___NEURONE___
#define ___NEURONE___
#include <vector>

using namespace std;
namespace cpr {
	class Neurone
	{

	private:
		vector<double> Weight{};
		double y{0.0};
		double b{0.0};
		double agrega{0.0};

	public:
		Neurone(double);
		Neurone(){}
		Neurone(vector<double>& w, double b) :Weight{ w }, b{b} {}
		void setWeight(vector<double>& w) {
			this->Weight = w;
		}
		void setBiais(double biais) {
			this->b = biais;
		}
		double activate(vector<double>&);
		double sum(vector<double>& vec) {
			double som = 0.0;
			for (int i = 0; i < vec.size(); i++)
			{
				som += this->Weight[i] * vec[i];
			}
			return som;
		}
		double gradient(double,double);
		double getBiais() {
			return this->b;
		}
		double gradient(vector<double>&, vector<double>&);
		vector<vector<double>> dw(vector<double>&, vector<double>&);
		vector<double> dwOutput(vector<double>&,double);
		double gradientOutput(double);
		vector<double> getWeight() {
			return this->Weight;
		}
		double getY() {
			return this->y;
		}
		double getAgrega() {
			return this->agrega;
		}
		/*Neurone() {
			this->Weight.resize(0);
		}*/

		void updateWeight(vector<double>&,double);
		void updateBiais(double,double);
        ~Neurone() {
//            
        }

	private:
	};
}
#endif // !___NEURONE___
