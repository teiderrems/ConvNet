#include "cnn.h"
#include "full-connected.h"
#include <algorithm>
#include<functional>


using namespace cpr;
CNN::CNN(const vector<Conv3D*> c, const vector<Pooling*> p) {
    this->conv.resize(c.size());
    this->conv = c;

    this->pool.resize(p.size());
    this->pool = p;
}



double cpr::CNN::predict(Image& image)
{
    Image currentImage = this->helper(image);
    
    
    vector<vector<Pixel>>().swap(image.p);
    
    Input currentInput = Util::flatten(currentImage);

    for (auto& i : currentImage.p)
    {
        for (auto& j : i)
        {
            vector<double>().swap(j.pixel);
        }
    }

    for (auto& i : currentImage.p)
    {
        vector<Pixel>().swap(i);
    }
    cout<<currentInput.x.size()<<endl;

    
    double val=this->fc->forward(currentInput);
    return val;
}
vector<double> cpr::CNN::test(vector<Image>& images)
{
    vector<double> res;
    res.resize(images.size());
    for (int i = 0; i < images.size(); i++)
    {
        res[i] = this->predict(images[i]);
    }
    for (auto& i :images) {
        vector<vector<Pixel>>().swap(i.p);
    }
    vector<Image>().swap(images);
    return res;
}





/// <summary>
/// Fonction pour convertir un vecteur en une image
/// </summary>
/// <param name="vec"></param>
/// <returns >Image2</returns>
Image cpr::CNN::convertVectorToImage(vector<double>& vec,int nc)
{
    Image current;
    int w = sqrt(int(vec.size()/nc));
    int h = sqrt(int(vec.size()/nc));

    current.height = h;
    current.width = w;
    int k = 0;
    current.p.resize(w);
    for (int i = 0; i < w; i++)
    {
        current.p[i].resize(h);
        for (int j = 0; j < h; j++)
        {
            current.p[i][j].pixel.resize(nc);
        }
    }
    
    return current;
}

void cpr::CNN::save()
{
    fc->save();
    for (int i = 0; i < this->conv.size(); i++)
    {
        this->conv[i]->saveBiais(i);
        this->conv[i]->saveFilter(i);
    }
}


Image CNN::helper(Image& image){
    //cout << "begin helper"<< endl;
    Image currentImage = this->conv[0]->convolutionOnImageManyFilter(image);
    currentImage=this->pool[0]->getType()==MaxPooling? this->pool[0]->maxPoolingOneImage(currentImage): this->pool[0]->averagePooling(currentImage);
    for (int i = 1; i < this->conv.size(); i++) {
        currentImage = this->conv[i]->convolutionOnImageManyFilter(currentImage);
        currentImage= this->pool[i]->getType() == MaxPooling ? this->pool[i]->maxPoolingOneImage(currentImage) : this->pool[i]->averagePooling(currentImage);
    }
    //cout << "end helper" <<currentImage.p.size()<< endl;
    return currentImage;
}



void CNN::preFit(Image& image, double alpha){
    //image = Util::normalize(image);
    Image images=this->helper(image);
    Input currentInput=Util::flatten(images);
    currentInput.x = this->type != Z_score ? Util::NormalisationMinMax(currentInput.x) : Util::normalisation(currentInput.x);
    cout << endl;

    for (int i = 0; i < 20; i++)
    {
        cout << currentInput.x[i] << "\t";
    }

    cout << endl;

    int nc = images.p[0][0].pixel.size();

    

    this->fc->forward(currentInput);

    vector<double> temp= this->fc->backward(currentInput, alpha);
    
    
    Image dy =this->convertVectorToImage(temp,nc);

    

    int size=this->conv.size()-1;
    
    vector<Image> d = this->conv[size]->backpropagationOutputLayer(dy, alpha);

    for (int i = size - 1; i > 0; i--)
    {
        d=this->conv[i]->backpropagationInterLayers(d, alpha);
    }

    this->conv[0]->backpropagationInputLayer(d, alpha);

    for (auto& i : d)
    {
        for (auto& j : i.p)
        {
            for (auto& c : j)
            {
                vector<double>().swap(c.pixel);
            }
        }
    }
    vector<Image>().swap(d);
}


void CNN::fit(vector<Image>& images,int epoch,double learning_rate) {
    int k = 0,i=0;
    while (k<epoch)
    {
        
        for (auto& image : images) {
            cout << endl;
            cout << endl;
            cout << "Epoch :" + to_string(k + 1)+" image :"+ to_string(i + 1) << endl;
            this->preFit(image, learning_rate);
            i++;
        }
        k++;
        random_shuffle(images.begin(), images.end());
        i = 0;
    }
    this->save();
}

CNN::~CNN(){

    for (auto& i : this->conv)
    {
        delete i;
    }
    
    for (auto& i : this->pool)
    {
        delete i;
    }

    for (auto& i : this->fc->getLayers())
    {
        for (auto& j : i->getNeurones())
        {
            delete j;
        }
        delete[] i;
    }

    delete this->fc;
    
    vector<Conv3D*>().swap(this->conv);
    vector<Pooling*>().swap(this->pool);

}
