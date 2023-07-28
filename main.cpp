#include <iostream>
#include <vector>
#include <fstream>
#include<string>
#include <stdio.h>

#include "util.h"
#include "Conv3D.h"

#include "image.h"


#include "Pooling.h"

#include "cnn.h"
#include "full-connected.h"
#include <algorithm>




using namespace std;
using namespace cpr;




int main(int argc, char* argv[])
{
    Util u;
     
   
    int epoch, nbImgPos,nbImgNeg;
    float alpha;


    if(argc==5)
    {
	    epoch = atoi(argv[1]);
    	alpha = atof(argv[2]);
	    nbImgPos = atoi(argv[3]);
	    nbImgNeg = atoi(argv[4]); 
    }
    else
    {
       cout<<"Nombre de paramètres insuffisant\n Usagr: ./med_vir epoch alpha nbImgPos nbImgNeg\n";	 
    }
    Conv3D* c = new Conv3D(5,3);
    Conv3D* c1 = new Conv3D(5, 3,3);
    Conv3D* c2 = new Conv3D(3, 3,3);
    /*Conv3D* c3 = new Conv3D(3, 1);*/
    Pooling* p = new Pooling(2,AveragePooling);

   vector<Conv3D*> conv{c,c1,c2};
   Layer* l = new Layer(27);
   Layer* l1 = new Layer(3);
   Layer* l2 = new Layer(2);

   Layer* output = new Layer(1);
   vector<Layer*> layers{l,  l1, l2};


     FullConnected* fcon = new FullConnected(layers, output);
    
    CNN* cnn = new CNN(conv, { p,p,p },fcon);
    cout << "hello world" << endl;

    vector<Image> shuffleImg = Util::shuffleImges("images/", nbImgPos, nbImgNeg);

    cnn->fit(shuffleImg, epoch, alpha);


    string filename = "cpr/";
    int n = 3;
    vector<int>lenght{3, 3, 3};
    vector<vector<Image>> filters = Util::readAllFilter(filename, n, lenght);

    vector<vector<double>> biais = Util::readAllBiais(filename, n);
    vector<Conv3D*> convs;
    convs.resize(n);

    for (int i = 0; i < n; i++) {
        Conv3D* temp = new Conv3D(filters[i], biais[i]);
        convs[i] = temp;
    }



    string file = "fc/";
    int nblfc = 4;
    vector<vector<vector<double>>> W = Util::readAllWeight(file, nblfc);

    vector<vector<double>> biaisfc = Util::readAllBiaisfc(file, nblfc);


    

    Layer* lt = new Layer(W[0], biaisfc[0]);
    Layer* lt1 = new Layer(W[1], biaisfc[1]);
    Layer* lt2 = new Layer(W[2], biaisfc[2]);

    Layer* outputT = new Layer(W[3], biaisfc[3]);
    vector<Layer*> layersT{lt, lt1, lt2};

    FullConnected* fconT = new FullConnected(layersT, outputT);


    CNN* cnnT = new CNN(convs, { p,p,p }, fconT);

    vector<Image> positif;
    positif.resize(30);
    for (int i = 0; i < 30; i++) {
        string filename = "images/pos/p" + to_string(i) + ".ppm";
        cout << filename << endl;
        positif[i] = Util::LoadImage(filename);
    }


    vector<Image> negatif;
    negatif.resize(30);
    for (int i = 0; i < 30; i++) {
        string filename = "images/neg/n" + to_string(i) + ".ppm";
        cout << filename << endl;
        negatif[i] = Util::LoadImage(filename);
    }

    cout << "Resultat des positifs" << endl;

    vector<double> resPos = cnnT->test(positif);
    for (auto& i : resPos) {
        cout << i << "\t";
    }
    cout << endl;
    cout << endl;
    cout <<"Resultat des négatifs"<< endl;
    vector<double> resNeg = cnnT->test(negatif);
    for (auto& i : resNeg) {
        cout << i << "\t";
    }
    cout << endl;
    vector<Layer*>().swap(layers);
    vector<Layer*>().swap(layersT);
    vector<Image>().swap(shuffleImg);
    vector<Conv3D*>().swap(convs);
    vector<Conv3D*>().swap(conv);
    cout << "compilation éffectuer avec success." << endl;
    delete l,l1,l2,c,c1,c2,cnn, fcon,output, p , lt,lt1,lt2, outputT, fconT, cnnT;//c1,c2,c,,l,l1,l2, output, cnn,fcon
}                                       
