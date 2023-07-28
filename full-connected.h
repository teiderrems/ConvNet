#ifndef ___FULLCONNECTED___
#define ___FULLCONNECTED___
#include "layer.h"
namespace cpr {
	class FullConnected
	{
	private:
		vector<Layer*> layers;
		Layer* output;
	public:

		FullConnected() = default;
        double forward(cpr::Input&);
        vector<Layer*> getLayers(){
            return this->layers;
        }
		vector<double> backward(cpr::Input&, double);
		FullConnected(const vector<Layer*>& layers,Layer*& out)
			: layers(layers),output(out)
		{
		}

		void save();
        ~FullConnected();
	};
}
#endif // !___FULLCONNECTED___



