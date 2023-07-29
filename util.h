#pragma once

#include <vector>
#include <random>
#include <cmath>
#include "image.h"
#include <iostream>
#include <fstream>
#include <string>
#include<chrono>
#include <functional>
#include <algorithm>
#include <ctime>



namespace cpr {
    class Util
    {

    public:
        
        Util() {}


        ~Util()
        {
        }



        static vector<double>NormalisationMinMax(vector<double>& vec) {
            double max = 0.0;
            for (auto& val : vec)
            {
                if (val>max)
                {
                    max = val;
                }
            }
            double min = max;
            for (auto& val : vec)
            {
                if (val < min)
                {
                    min = val;
                }
            }
            for (int i = 0; i < vec.size(); i++)
            {
                vec[i] = (vec[i] - min) / (max - min);
            }
            return vec;
        }

        static double sum(Pixel& p) {
            double s = 0;
            for (auto& i : p.pixel)
            {
                s += i;
            }
            return s;
        }

        static int Relu(double val) {
            return (val) >= 0 ? (val) : 0;
        }

        static Image genPixel3D(int w, int h,int nc=3) {
            //cout << "genPixel3D" << endl;
            Image F;
            F.height = h;
            F.width = w;
            double MIN = -2.5/(w*h);
            double MAX = 2.5 / (w * h);

            //random_device rd;
            default_random_engine re(chrono::system_clock::now().time_since_epoch().count());
            uniform_real_distribution<double> dist(MIN, MAX);
            auto rd = bind(dist, re);
            F.p.resize(w);
            for (int i = 0; i < w; i++)
            {
                F.p[i].resize(h);
                for (int j = 0; j < h; j++)
                {
                    F.p[i][j].pixel.resize(nc);
                    for (int c = 0; c < nc; c++)
                    {
                        double b;
                        b= rd();
                        if (b <= 0.0) {
                            b = rd();
                        }
                        if (isIn(F.p[i][j].pixel, b)) {
                            b = rd();
                        }
                        F.p[i][j].pixel[c] = b;
                    }
                }
            }

            
            return F;
        }


        static Image LoadImage(const string filename,int nc=3) {


            ifstream f(filename);
            if (!f.is_open())
            {
                cout << "ouverture du fichier impossible" << endl;
                exit(EXIT_FAILURE);
            }
            else
            {
                Image I{};
                string header, com;
                string pixelMax;
                getline(f, header);
                getline(f, com);
                f >> I.width >> I.height;
                int pixel;
                getline(f, pixelMax);
                getline(f, pixelMax);

                int i = 0;
                int size = I.width * I.height * 3 + 1;
                vector<int> px;
                px.resize(size);
                while (!f.eof())
                {

                    f >> pixel;
                    px[i] = pixel;

                    i++;
                }
                int k = 0;
                I.p.resize(I.width);
                for (int i = 0; i < I.width; i++)
                {
                    I.p[i].resize(I.height);
                    for (int j = 0; j < I.height; j++)
                    {
                        if (k <= px.size()) {
                            I.p[i][j].pixel.resize(nc);
                            for (int c = 0; c < nc; c++)
                            {
                                I.p[i][j].pixel[c] = px[k];

                                k++;
                            }
                        }
                    }
                }
                return I;

            }
        }

        static vector<double> genBiais(int n) {
            vector<double> biais;
            biais.resize(n);
            double MIN = -2.5/(n);
            double MAX = 2.5/(n);
            
            default_random_engine re(chrono::system_clock::now().time_since_epoch().count());
            uniform_real_distribution<double> dist(MIN, MAX);
            auto rd = bind(dist, re);
            srand(time(nullptr));
            for (int i = 0; i < n; i++) {
                double b = rd();
                if (b <= 0.0) {
                    b = rd();
                }
                if (isIn(biais, b)) {
                    b = rd();
                }
                biais[i] = b;
            }
            return biais;
        }

        
        static vector<Pixel> reverseVector(vector<Pixel>& V) {
            vector<Pixel> V1;
            V1.resize(V.size());
            int k = V.size() - 1;
            for (int i = 0; i < V.size(); i++) {
                V1[i].pixel.resize(V[k].pixel.size());
                for (int j = 0; j < V[k].pixel.size(); j++)
                {
                    V1[i].pixel[j] = V[k].pixel[j]; 
                }
                k--;
                
            }
            return V1;
        }

        
        
        static Image reverseFilter(Image& F) {
            //cout << "begin reverseFilter" <<endl;

           Image F1;
            F1.width = F.width;
            F1.height = F.height;
            F1.p.resize(F.p.size());
            vector<Pixel> V;
            V.resize(F.p.size() * F.p[0].size());
            int k = 0;
            
            for (int i = 0; i < F.p.size(); i++) {
                F1.p[i].resize(F.p[i].size());
                for (int j = 0; j < F.p[i].size(); j++) {

                    V[k] = F.p[i][j];
                    k++;
                }
            }

            vector<Pixel> V1= reverseVector(V);
            int t = 0;
            for (int i = 0; i < F.p.size(); i++) {
                for (int j = 0; j < F.p[i].size(); j++) {
                    F1.p[i][j]= V1[t];
                    t++;
                }
            }
            //cout << "end reverseFilter" << endl;
            return F1;
        }




        static Image addPaddingFilter(Image& F, int w,int h,int dim) {
            //cout << "begin addPaddingFilter" << endl;
            Image F1;
            int height = h+dim-1;
            int width = w+dim-1;
            int t = height - F.height;
            
            int padding= 0;
            if (t % 2 == 0) {
                padding = t / 2;
            }
            else
            {
                padding = (static_cast<int>(t / 2)) + 1;
            }
            

            F1.height = height;
            F1.width = width;
            //cout << F1.height << "--" << F1.width << endl;
            F1.p.resize(width);
            for (int i = 0; i < width; i++) {
                F1.p[i].resize(height);
                for (int j = 0; j < height; j++) {
                    F1.p[i][j].pixel.resize(F.p[0][0].pixel.size());
                    for (int c = 0; c <F1.p[i][j].pixel.size(); c++)
                    {
                        F1.p[i][j].pixel[c] = 0.0;
                        
                    }
                }
            }

            
            for (int i = padding; i < width - padding; i++) {
                for (int j = padding; j < height - padding; j++) {
                    int e = j - padding;
                    int k = i - padding;
                    for (int c = 0; c < F1.p[i][j].pixel.size(); c++)
                    {
                        F1.p[i][j].pixel[c] = F.p[k][e].pixel[c];
                        
                    }
                }
            }
            //cout << "end addPaddingFilter" << endl;
            return F1;
        }
       

        static Image convolutionFilterFilter(Image& image, Image& f) {
            int height = (1 + (image.height - f.height));
            int width = (1 + (image.width - f.width));

            Image currentI;
            currentI.width = width;
            currentI.height = height;
            currentI.p.resize(width);
            int canaux = image.p[0][0].pixel.size();
            for (int i = 0; i < width; i++)
            {

                currentI.p[i].resize(height);
                for (int j = 0; j < height; j++)
                {
                    currentI.p[i][j].pixel.resize(canaux);
                    for (int c = 0; c < canaux; c++)
                    {
                        currentI.p[i][j].pixel[c] = 0.0;
                    }
                }
            }

            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {

                    for (int k = 0; k < f.width; k++)
                    {
                        for (int t = 0; t < f.height; t++)
                        {
                            int e = i + k;
                            int z = j + t;

                            for (int c = 0; c < canaux; c++)
                            {
                                currentI.p[i][j].pixel[c] += image.p[e][z].pixel[c] * f.p[k][t].pixel[c];

                            }

                        }
                    }
                }
            }
            return currentI;
        }


        static vector<Image> readFilters(string filename,int j,int n) {
            vector<Image> filters;
            filters.resize(n);
            for (int i = 0; i < n; i++)
            {
                string file= filename+"filter(" + to_string(j+1) + "," + to_string(i+1) + ").txt";
                ifstream f(file);
                Image filter;
                if (!f.is_open()) {
                    cout<<"ouverture impossible!"<<endl;

                    exit(EXIT_FAILURE);
                }
                else {
                    
                    f >> filter.width>> filter.height>> filter.nc;

                    filter.p.resize(filter.width);
                    vector<double> fil;
                    fil.resize(filter.height * filter.width * filter.nc);
                    int k = 0;
                    while (!f.eof())
                    {
                        f >> fil[k];
                        k++;
                    }
                    k = 0;
                    for (int i = 0; i < filter.width; i++)
                    {
                        filter.p[i].resize(filter.height);
                        for (int j = 0; j < filter.height; j++)
                        {
                            filter.p[i][j].pixel.resize(filter.nc);
                            for (int c = 0; c < filter.nc; c++)
                            {
                                 filter.p[i][j].pixel[c]=fil[k];
                                 k++;
                            }
                        }
                    }
                }
                filters[i] = filter;
            }
            return filters;
        }
        static vector<vector<Image>>readAllFilter(string filename,int n, vector<int>& nbf) {
            vector<vector<Image>> res;
            res.resize(n);
            for (int i = 0; i < n; i++)
            {
                res[i].resize(nbf[i]);
                res[i] = Util::readFilters(filename,i, nbf[i]);
            }
            return res;
        }

        static vector<double>readBiais(string filename,int i) {
            string file = filename+"biaisLayer_" + to_string(i+1) +".txt";
            ifstream f(file);
            
            vector<double> biais{};
            if (!f.is_open()) {
                cout<<"ouverture impossible!"<<endl;
                exit(EXIT_FAILURE);
            }
            else {
                int dim;
                f >> dim;
                biais.resize(dim);
                int k = 0;
                double b;
                while (!f.eof())
                {
                    f >> b;
                    biais[k] = b;
                    k++;
                }
            }
            return biais;
        }
        static vector<vector<double>>readAllBiais(string filename,int n){
            vector<vector<double>> biais;
            biais.resize(n);
            for (int i = 0; i < n; i++) {
                biais[i]=Util::readBiais(filename,i);
            }
            return biais;
        }
        static double somme(Image& dy) {
            double som = 0;
            for (auto& i : dy.p) {
                for (auto& j : i) {
                    for (auto& c : j.pixel)
                    {
                        som += c;
                    }
                }
            }
            return som;
        }
        

       
        static Input flatten(Image& image) {

            //cout << "begin flatten" << endl;
            Input currentInput;
            currentInput.y = image.label;
            currentInput.x.resize(image.p.size() * image.p[0].size()*image.p[0][0].pixel.size());
            int k{ 0 };
            for (int c = 0; c < image.p[0][0].pixel.size(); c++)
            {
                for (int i = 0; i < image.p.size(); i++)
                {
                    for (int j = 0; j < image.p[0].size(); j++)
                    {
                        currentInput.x[k] = image.p[i][j].pixel[c];
                        k ++;
                    }
                }
            }
            //vector<vector<Pixel>>().swap(image.p);
            
            //cout << "end flatten input size:" << currentInput.x.size()<< endl;
            return currentInput;
        }

        static double Mean(vector<double>& vec) {
            double som = 0.0;
            for (auto& i : vec)
            {
                som += i;
            }
            return (som / double(vec.size()));
        }

        static double variance(vector<double>& vec) {
            double mean = Util::Mean(vec);
            double som=0;
            for (auto& i : vec)
            {
                double d = i - mean;
                som += pow(d,2.0);
            }
            return (som / double(vec.size()));
        }

        static double sd(vector<double>& vec) {
            double var = Util::variance(vec);
            return sqrt(var);
        }

        static vector<double> normalisation(vector<double>& vec) {
            double mean = Util::Mean(vec);
            double sd = Util::sd(vec);
            vector<double> norm;
            norm.resize(vec.size());
            for (int i = 0; i < vec.size(); i++)
            {
                norm[i] = (vec[i] - mean) / sd;
            }
            return norm;
        }

        static vector<double> MeanVec(Image& image) {
            vector<double> som;
            som.resize(image.p[0][0].pixel.size());
            for (int i = 0; i < som.size(); i++)
            {
                som[i] = 0;
            }

            for (int i = 0; i < image.p.size(); i++)
            {
                for (int j = 0; j < image.p[i].size(); j++)
                {
                    for (int c = 0; c < som.size(); c++)
                    {
                        som[c] += image.p[i][j].pixel[c];
                    }
                }
            }

            
            int size = image.p[0].size() * image.p.size()*image.p[0][0].pixel.size();
            return{som[0]/double(size),som[1] / double(size),som[2] / double(size)};
        }

        static vector<double> varianceVec(Image& image) {
            vector<double> mean = Util::MeanVec(image);
            vector<double> som(image.p[0][0].pixel.size(),0.0);
            for (int i = 0; i < image.p.size(); i++)
            {
                for (int j = 0; j < image.p[i].size(); j++)
                {
                    for (int c = 0; c < som.size(); c++)
                    {
                        som[c] += image.p[i][j].pixel[c]-mean[c];
                    }
                }
            }
            
            int size = image.p[0].size() * image.p.size() * image.p[0][0].pixel.size();
            for (int i = 0; i < som.size(); i++)
            {
                som[i] /= size;
            }
            return som;
        }

        static vector<double>sdVec(Image& image) {
            vector<double> var = Util::varianceVec(image);
            vector<double> sd;
            sd.resize(var.size());
            for (int i = 0; i < sd.size(); i++)
            {
                sd[i] = sqrt(var[i]);
            }
            return sd;
        }

        static Image normalize(Image& image) {
            Image img;
            img.height = image.height;
            img.width = image.width;
            vector<double> mean = Util::MeanVec(image);
            vector<double> sd = Util::sdVec(image);
            img.p.resize(image.p.size());
            img.label = image.label;
            for (int i = 0; i < image.p.size(); i++)
            {
                img.p[i].resize(image.p[i].size());
                for (int j = 0; j < image.p[i].size(); j++)
                {
                    img.p[i][j].pixel.resize(image.p[i][j].pixel.size());
                    for (int c = 0; c < image.p[i][j].pixel.size(); c++)
                    {
                        img.p[i][j].pixel[c] = (double(image.p[i][j].pixel[c]) - mean[c]) / sd[c];
                    }
                    
                }

            }
            return img;
        }

        static vector<double> genWeight(int n) {
            
            vector<double> w;
            w.resize(n);
            double MIN = -2.5/n;
            double MAX = 2.5/n;
            
            default_random_engine re(chrono::system_clock::now().time_since_epoch().count());
            uniform_real_distribution<double> dist(MIN, MAX);
            auto rd = bind(dist, re);
            for (int i = 0; i < n; i++) {
                double b = rd();
                /*if (b <= 0.0) {
                    b = rd();
                }
                if (isIn(w, b)) {
                    b = rd();
                }*/
                w[i] = b;
            }
            
            return w;
        }
        static double sigmoid(double x) {
            return (1 / (1 + exp(-x)));
        }
        static double sigmoidPrime(double x) {
            return Util::sigmoid(x) * (1 - Util::sigmoid(x));
        }
        static double lossFunction(double x,double y) {
            return (-(y * log(x) + (1 - y) * log(1 - x)));
        }

        static double lossFunctionPrime(double x, double y) {
            return((y + x - 2 * y * x) / (x * (1 - x)));
        }



        static vector<Image>loadImages(string filename,int n){
            vector<Image> res;
            
            res.resize(n);
            for (int i = 0; i < n; i++) {
                string str= filename+to_string(i)+".ppm";
                
                res[i]=Util::LoadImage(str);
            }
            return res;
        }

        static vector<Image> shuffleImges(string filename,int np,int nn){
            
            vector<Image> shuffleImg;
            string nf=filename+"neg/n";
            vector<Image> negatif=Util::loadImages(nf,nn);
            
            for (int i=0;i<negatif.size();i++) {
                negatif[i].label=0;
                shuffleImg.push_back(negatif[i]);
            }
            

            string pf=filename+"pos/p";
            vector<Image> positif=Util::loadImages(pf,np);
            for (int i=0;i<positif.size();i++) {
                positif[i].label=1;
                shuffleImg.push_back(positif[i]);
            }
            
            random_shuffle(shuffleImg.begin(),shuffleImg.end());

            return shuffleImg;
        }

        static vector<double> readBiaisfc(string filename,int j){
            vector<double> b;

            string file = filename+"biaisLayer" + to_string(j+1) + ".txt";
            ifstream f(file);
            
            if (!f.is_open()) {
                cout<<"ouverture impossible"<<endl;
                exit(EXIT_FAILURE);
            }
            else
            {
                int dim{0};
                f >> dim;
                int k=0;
                double t;
                b.resize(dim);
                while (!f.eof()) {
                    f >> t;
                    b[k] = t;
                    k++;
                }
            }
            return b;
        }

        static vector<vector<double>> readAllBiaisfc(string filename,int n){
            vector<vector<double>> biais;
            biais.resize(n);
            for (int i = 0; i < n; i++) {
                biais[i]=Util::readBiaisfc(filename,i);
            }
            return biais;
        }
        static vector<vector<double>> readWeight(string filename,int j){
            vector<vector<double>> W;

            string file = filename+"weightLayer" + to_string(j) + ".txt";
            ifstream f(file);
            
            if (!f.is_open()) {
                cout<<"ouverture impossible"<<endl;
                exit(EXIT_FAILURE);
            }
            else
            {
                int x{0},y{0};
                vector<double> temp;
                f >> x>>y;
                
                temp.resize(x*y);
                if(temp.size()!=0){
                    int k=0;
                    while(!f.eof()){
                        f>>temp[k];
                        k++;
                    }
                }
                int k=0;
                W.resize(x);
                for (int i = 0; i < x; i++) {
                    W[i].resize(y);
                    for (int j = 0; j < y;j++) {
                        W[i][j]=temp[k];
                        k++;
                    }
                }
            }
            return W;
        }

        static vector<vector<vector<double>>> readAllWeight(string filename,int n){
            vector<vector<vector<double>>> temp;
            temp.resize(n);
            for (int i = 0; i < n; i++) {
                temp[i]=Util::readWeight(filename,i);
            }
            return temp;
        }
        static Image addImage(vector<Image> images){
            Image temp;
            temp.height=images[0].height;
            temp.width=images[0].width;

            temp.p.resize(images[0].p.size());
            for (int i = 0; i < images[0].p.size(); i++) {
                temp.p[i].resize(images[0].p[0].size());
                for (int j = 0; j <images[0].p[i].size(); j++) {
                    temp.p[i][j].pixel.resize(images[0].p[0][0].pixel.size());
                    for (int c = 0; c < images[0].p[0][0].pixel.size(); c++)
                    {
                        temp.p[i][j].pixel[c]=0.0;
                    }
                }
            }

            for (int var = 0; var < images.size(); var++) {
                for (int i = 0; i < temp.p.size(); i++) {
                    
                    for (int j = 0; j <images[0].p[i].size(); j++) {
                        
                        for (int c = 0; c < images[0].p[0][0].pixel.size(); c++)
                        {
                            temp.p[i][j].pixel[c] += images[var].p[i][j].pixel[c];
                        }
                    }
                }
            }


            return temp;
        }
        

        static bool isIn(vector<double>& vec, double val) {
            bool b = false;
            for (auto& v : vec)
            {
                if (v == val) {
                    b = true;
                }
            }
            return b;
        }


        static ImageGray addImageCanaux(Image& images) {

            
            ImageGray currentImage;
            
            currentImage.height = images.height;
            currentImage.width= images.width;
            currentImage.label = images.label;
            currentImage.p.resize(images.p.size());
            for (int i = 0; i < images.p.size(); i++)
            {
                currentImage.p[i].resize(images.p[i].size());
                for (int j = 0; j < images.p[i].size(); j++) {
                    double s = 0.0;
                    for (int c = 0; c < images.p[i][j].pixel.size(); c++)
                    {
                        s+= images.p[i][j].pixel[c];
                    }
                    currentImage.p[i][j] = s;
                }
            }
            return currentImage;
        }


        static Image createImage(vector<ImageGray>& images) {
            Image currentImage;
            currentImage.height = images[0].height;
            currentImage.width = images[0].width;
            currentImage.p.resize(images[0].width);
            currentImage.label = images[0].label;
            for (int i = 0; i < images[0].width; i++)
            {
                currentImage.p[i].resize(images[0].height);
                for (int j = 0; j < images[0].height; j++)
                {
                    currentImage.p[i][j].pixel.resize(images.size());
                }
            }

            for (int c = 0; c < images.size(); c++)
            {
                for (int i = 0; i < images[0].width; i++)
                {
                    for (int j = 0; j < images[0].height; j++)
                    {
                        currentImage.p[i][j].pixel[c]=images[c].p[i][j];
                    }
                }
            }
            return currentImage;
        }


        static Image convolutionImageFilter(Image& image, Image& f) {

            //cout << "begin convolutionImageFilterMany canal" << endl;

            int height = (1 + (image.p[0].size() - f.height));
            int width = (1 + (image.p.size() - f.width));
            

            
            
            vector<ImageGray> res;
            res.resize(f.p[0][0].pixel.size());

            for (int c = 0; c < f.p[0][0].pixel.size(); c++)
            {
                ImageGray currentI;
                currentI.width = width;
                currentI.height = height;
                currentI.p.resize(width);
                for (int i = 0; i < width; i++)
                {

                    currentI.p[i].resize(height);
                    for (int j = 0; j < height; j++)
                    {
                        double temp = 0.0;
                        for (int k = 0; k < f.width; k++)
                        {
                            for (int t = 0; t < f.height; t++)
                            {
                                int e = i + k;
                                int z = j + t;

                                for (int u = 0; u < image.p[e][z].pixel.size(); u++)
                                {
                                    temp += image.p[e][z].pixel[u] * f.p[k][t].pixel[c] ;
                                }
                            }
                        }

                        currentI.p[i][j]= temp;
                    }
                }
                res[c] = currentI;
            }
            
            //cout << "end convolutionImageFilter" << endl;

            return Util::createImage(res);
        }

        static Image convolution(Image& image, Image& f) {
            int height = (1 + (image.p[0].size() - f.height));
            int width = (1 + (image.p.size() - f.width));
            Image currentI;
            currentI.width = width;
            currentI.height = height;
            currentI.p.resize(width);

            int canaux = image.p[0][0].pixel.size();
            for (int i = 0; i < width; i++)
            {

                currentI.p[i].resize(height);
                for (int j = 0; j < height; j++)
                {
                    currentI.p[i][j].pixel.resize(canaux);
                    for (int c = 0; c < canaux; c++)
                    {
                        currentI.p[i][j].pixel[c] = 0.0;
                    }
                }
            }

            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {

                    for (int k = 0; k < f.width; k++)
                    {
                        for (int t = 0; t < f.height; t++)
                        {
                            int e = i + k;
                            int z = j + t;

                            for (int c = 0; c < canaux; c++)
                            {
                                currentI.p[i][j].pixel[c] += image.p[e][z].pixel[c] * f.p[k][t].pixel[c];

                            }

                        }
                    }
                }
            }
            return currentI;
        }

        static double sommeImage(Image& image) {
            double som = 0.0;
            for (auto& i : image.p)
            {
                for (auto& j : i)
                {
                    for (auto& c : j.pixel)
                    {
                        som += c;
                    }
                }
            }
            return som;
        }


        static vector<Image> convertMatToVec(vector<vector<Image>>& dy) {
            vector<Image> temp;
            temp.resize(dy.size());
            for (int i = 0; i < dy.size(); i++)
            {
                temp[i] = addImage(dy[i]);
            }
            return temp;
        }

        static double accurency(vector<int>& y, vector<int>& yp) {
            int vp = 0;
            for (int i = 0; i < y.size(); i++)
            {
                if (y[i] == yp[i]) {
                    vp+=1;
                }
            }
            return(vp / y.size()) * 100;
        }
    };
}

