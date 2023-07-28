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




int main()
{
    Util u;
     /*string filename="cpr/";
     int n=2;
     vector<int>lenght{4,2};
     vector<vector<Image>> filters=Util::readAllFilter(filename,n,lenght);

     vector<vector<double>> biais=Util::readAllBiais(filename,n);
     vector<Conv3D> convs;
     convs.resize(n);

     for (int i = 0; i < n; i++) {
        Conv3D* temp = new Conv3D(filters[i],biais[i]);
        convs[i]=*temp;
        delete temp;
     }*/
    //cout<<convs[1].getBiais().size()<<endl;
    Conv3D* c = new Conv3D(5,3);
    Conv3D* c1 = new Conv3D(5, 3,3);
    Conv3D* c2 = new Conv3D(3, 1,3);
    /*Conv3D* c3 = new Conv3D(3, 1);*/
    Pooling* p = new Pooling(2,AveragePooling);

   vector<Conv3D*> conv{c,c1,c2};
   Layer* l = new Layer(9);
   Layer* l1 = new Layer(3);
   Layer* l2 = new Layer(2);

   Layer* output = new Layer(1);
   vector<Layer*> layers{l,  l1, l2};



     /*string file="fc/";
     int nblfc=4;
     vector<vector<vector<double>>> W=Util::readAllWeight(file,nblfc);

     vector<vector<double>> biaisfc=Util::readAllBiaisfc(file,nblfc);

     Layer* l = new Layer(W[0],biaisfc[0]);
     Layer* l1 = new Layer(W[1],biaisfc[1]);
     Layer* l2 = new Layer(W[2],biaisfc[2]);

     Layer* output = new Layer(W[3],biaisfc[3]);
     vector<Layer> layers{*l, * l1, * l2};*/
// //    layer.resize(nblfc);
//    for (int i = 0; i <nblfc; i++) {
//        Layer* l=new Layer(W[i],biaisfc[i]);
//        layer[i]=*l;
//        delete l;
//    }
//    vector<Layer> layer{};
//    cout<<layer.size()<<endl;
//    FullConnected* fcon = new FullConnected({layer[0],layer[1],layer[2]}, layer[3]);
    FullConnected* fcon = new FullConnected(layers, output);
    //cout<<layers[0].getNeurones()[0].getWeight().size()<<endl;
    // CNN* cnn = new CNN(convs, { *p,*p,*p,*p},*fcon);
    CNN* cnn = new CNN(conv, { p,p,p },fcon,MinMax);
    cout << "hello world" << endl;
//    //vector<Image> images = Util::loadManyImage();, I4, I3
    vector<Image> shuffleImg=Util::shuffleImges("images/",2,2);
    cout << shuffleImg.size() << endl;
    cnn->fit(shuffleImg, 2);

//    cout<<positif.size()<<endl;
//    for (int j = 0; j < cnn->test(negatif).size(); j++) {
//        cout<<cnn->test(negatif)[j]<<endl;
//    }
//    for (auto& val : cnn->test(negatif)) {
//        cout<<val<<endl;
//    }
//    cout<<endl;
//    cout<<endl;
//    cout<<endl;

//    for (int j = 0; j < cnn->test(positif).size(); j++) {
//        cout<<cnn->test(negatif)[j]<<endl;
//    }

//    for (auto& val : cnn->test(positif)) {
//        cout<<val<<endl;
//    }

     vector<Image> positif;
     positif.resize(15);
     for (int i = 15; i < 31; i++) {
         string filename="images/pos/p"+to_string(i)+".ppm";
         positif[i]=Util::LoadImage(filename);
     }

     vector<Image> negatif;
     negatif.resize(15);
     for (int i = 15; i < 31; i++) {
         string filename="images/neg/n"+to_string(i)+".ppm";
         //cout<<filename<<endl;
         negatif[i]=Util::LoadImage(filename);
     }

     vector<double> resPos=cnn->test(positif);
     for (auto& i : resPos) {
         cout<<i<<"\t";
     }
     cout<<endl;
     cout<<endl;
     cout<<endl;
     vector<double> resNeg=cnn->test(negatif);
     for (auto& i : resNeg) {
         cout<<i<<"\t";
     }

    /*Image image = Util::LoadImage("C:/projet/ConvNet/images/pos/p0.ppm");
    image.label = 1;
    vector<Image> arr{image};*/
    
    vector<Layer*>().swap(layers);
    //vector<Layer>().swap(layer);
    //vector<Image>().swap(shuffleImg);
    cout << "ex�cution �ffectuer avec success." << endl;
    vector<Conv3D*>().swap(conv);
    //cnn->fit(arr, 2);

    delete c1,c2,c,l,l1,l2,cnn, fcon,output, p;//c1,c2,c,,l,l1,l2, output, cnn,fcon
}
