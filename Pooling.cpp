#include "Pooling.h"

using namespace cpr;


/// <summary>
/// < Fonction pour calculer la moyenne des pixels>
/// </summary>
/// <param name="p"></param>
/// <returns></returns>
Pixel Pooling::averagePixel(vector<vector<Pixel>>& p) {
    Pixel h;
    vector<double> s;
    s.resize(p[0][0].pixel.size());
    for (int i = 0; i < p[0][0].pixel.size(); i++)
    {
        s[i] = 0.0;
    }

    for (int c = 0; c < p[0][0].pixel.size(); c++)
    {
        for (int i = 0; i < this->dim; i++)
        {
            for (int j = 0; j < this->dim; j++)
            {
                s[c] += p[i][j].pixel[c];
            }
        }
        s[c] = s[c] / (this->dim * this->dim * p[0][0].pixel.size());
    }
    h.pixel = s;
    for (auto& i : p)
    {
        for (auto& j : i)
        {
            vector<double>().swap(j.pixel);
        }
    }
    vector<vector<Pixel>>().swap(p);
    return h;
}


 /// <summary>
 /// <Fonction pour d�terminer le pixel maximal>
 /// </summary>
 /// <param name="p"></param>
 /// <returns></returns>
 Pixel Pooling::maxPixel(vector<vector<Pixel>>& p) {
    
        vector<int>som(this->dim * this->dim, 0);
        int k = 0, z = 0;
        int x{ 0 }, y{ 0 };

        for (int i = 0; i < this->dim; i++)
        {
            for (int j = 0; j < this->dim; j++)
            {
                som[k] += Util::sum(p[i][j]);
                k++;
            }
        }

        int max = 0.0;
        for (int j = 0; j < som.size(); j++)
        {
            if (som[j] >= max) {
                max = som[j];
            }
        }

        for (int i = 0; i < this->dim; i++)
            {
                for (int j = 0; j < this->dim; j++)
                {
                    if (Util::sum(p[i][j]) == max) {

                        Pixel current= p[i][j];
                        for (auto& i : p)
                        {
                            for (auto& j : i)
                            {
                                vector<double>().swap(j.pixel);
                            }
                        }
                        vector<vector<Pixel>>().swap(p);
                        
                        return current;
                    }
                }
            }
}



 /// <summary>
 /// <Fonction pour faire le max pooling d'une image pr�cise>
 /// </summary>
 /// <param name="I"></param>
 /// <returns></returns>
 Image Pooling::maxPoolingOneImage(Image& I) {

     
     Image image;
     int width = 1 + ((I.width - this->dim) / this->s);
     int height = 1 + ((I.height - this->dim) / this->s);
     image.width = width;
     image.height = height;
     image.label = I.label;
     image.p.resize(width);
     for (int i = 0; i < width; i++)
     {
         image.p[i].resize(height);
     }

     for (int i = 0; i < width; i++)
     {
         for (int j = 0; j < height; j++)
         {
            vector<vector<Pixel>> V;
             V.resize(this->dim);
             for (int k = 0; k < this->dim; k++)
             {
                 V[k].resize(this->dim);
                 for (int t = 0; t < this->dim; t++)
                 {
                     int e = i *this->s + k;
                     int z = j * this->s + t;
                     V[k][t] = I.p[e][z];
                 }
             }
             image.p[i][j] = this->maxPixel(V);
         }
     }
    
     return image;
 }


 /// <summary>
 ///
 /// </summary>
 /// <param name="images"></param>
 /// < Fonction pour faire le maxpooling de plusieurs images>
 /// <returns vector<Image></returns>
 vector<Image> Pooling::maxPoolingManyImage(vector<Image>& images)
 {


     vector<Image> Images;
     Images.resize(images.size());

     for (int i=0; i< images.size();i++)
     {
         Images[i]=this->maxPoolingOneImage(images[i]);
     }
     
     return Images;
 }
 
 






 /// <summary>
 ///
 /// </summary>
 /// <param name="I"></param>
 /// < fonction pour faire de pooling par la moyenne des pixels>
 /// <returns></returns>
 Image Pooling::averagePooling(Image& I)
 {
     Image image;

     int width = 1 + ((I.width - this->dim) / this->s);
     int height = 1 + ((I.height - this->dim) / this->s);

     image.width = width;
     image.height = height;
     image.label = I.label;
     image.p.resize(width);
     for (int i = 0; i < width; i++)
     {
         image.p[i].resize(height);
     }



     for (int i = 0; i < width; i++)
     {
         for (int j = 0; j <height; j ++)
         {
             vector < vector< Pixel>>V;
             V.resize(this->dim);

             for (int k = 0; k < this->dim; k++)
             {
                 V[k].resize(this->dim);
                 for (int t = 0; t < this->dim; t++)
                 {
                     int e = i* this->s + k;
                     int z = j * this->s + t;
                     V[k][t] = I.p[e][z];
                     
                 }
             }
             image.p[i][j] = this->averagePixel(V);
             
         }
     }
     return image;
 }







 vector<Image> Pooling::averagePoolings(vector<Image>& images)
 {
     vector<Image> Images;

     Images.resize(images.size());

     for (int i = 0; i < images.size(); i++)
     {
         Images[i] = this->averagePooling(images[i]);
     }

     return Images;
 }

