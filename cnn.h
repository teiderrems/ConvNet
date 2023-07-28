#ifndef ___CNN___
#define ___CNN___

#include "Conv3D.h"
#include<vector>
#include "Pooling.h"
#include "util.h"
#include "image.h"
#include "full-connected.h"


using namespace std;
namespace cpr {
    enum Norm
    {
        MinMax,
        Z_score
    };
    class CNN
    {
    private:
        vector<Conv3D*> conv;
        
        vector<Pooling*> pool;
        FullConnected* fc;
        Norm type = Z_score;
        double prob=0.0;
        int label=0;
    public:
        CNN() = default;
        CNN(const vector<Conv3D*>, const vector<Pooling*>);
        void fit(vector<Image>&, int, double learning_rate = 0.005);
        void preFit(Image&, double);
        double predict(Image&);
        vector<double> test(vector<Image>&);
        Image helper(Image&);
        Image convertVectorToImage(vector<double>&,int);
        void save();
        ~CNN();

        CNN(const vector<Conv3D*>& conv, const vector<Pooling*>& pool,FullConnected *fc,Norm type=Z_score)
            : conv(conv), pool(pool), fc(fc)
        {
        }
    };
}

#endif // !___CNN___



