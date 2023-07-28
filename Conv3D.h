#ifndef __CONV3D__
#define __CONV3D__

#include<stdio.h>
#include "image.h"
#include <vector>
#include "util.h"

#include "Pooling.h"

#include <fstream>
#include<ctime>



using namespace std;
//class Pooling;

namespace cpr {
	class Conv3D
	{
	private:
		int stride = 1;
		int padding = 1;
		int height{0};
		int width{0};
        
		Image image;
		vector<Image> filters{};
		vector<double> biais;
		Image filter;
	public:

        int getWidth(){
            return this->width;
        }
        int getHeight(){
            return this->height;
        }

		Conv3D(int dim=3, int n=2, int c=3, int st=1, int pad=0) {
			this->padding = pad;
			this->stride = st;
			this->filters.resize(n);
			this->biais = Util::genBiais(n);
			for (int i = 0; i < n; i++)
			{
				this->filters[i]=Util::genPixel3D(dim,dim,c);
			}
		}

        ImageGray convolutionOneImageOneFilter(Image& image, double b);
        Image convolutionOnImageManyFilter(Image&);
        


		inline vector<Image> getFilters() {
			return this->filters;
		}
		inline Image getFilter() {
			return this->filter;
		}
		inline Image getImage() {
			return this->image;
		}

		inline void setImage(Image& i) {
			this->image = i;
		}
		inline void setFilters(vector<Image>& f) {
			this->filters.resize(f.size());
			this->filters = f;
		}
		inline void setFilter(Image& f) {

			this->filter = f;
		}
       
        //void updateManyFilter(vector<Image>&, double);
        void updateFilterOutputLayer(vector<Image>&, double);

		Image dw(vector<Image>&);



        void saveBiais(int indexLayer);

        //void updateManyBiais(vector<vector<Image>>& dy, double alpha);
        void updateBiaisOutputLayer(vector<Image>& dy, double alpha);

		Conv3D(const vector<Image>& filters, const vector<double>& b)
			:filters(filters), biais(b)
		{

		}

		inline vector<double> getBiais() {
			return this->biais;
		}

		inline void setBiais(const vector<double>& b) {
			this->biais = b;
		}

		void saveFilter(int);
        
		void saveManyFilter(int);
        void backpropagationInputLayer(vector<Image>&,double);
        vector<Image> backpropagationInterLayers(vector<Image>&,double);
        vector<Image> backpropagationOutputLayer(Image&,double);

		vector<Image> dxOutUpLayer(Image&);
		vector<Image> dwOutUpLayer(vector<Image>&);
		
  
		vector<Image>dxManyFilter(vector<Image>&);

		Image dxHelper(Image&, vector<Image>&);

        ~Conv3D(){
        }

	};
}

#endif // !__CONV3D__
