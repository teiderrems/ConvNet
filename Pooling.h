#ifndef __POOLING__
#define __POOLING__


#include "image.h"

#include "util.h"

#include <vector>




enum TypePoooling
{
	MaxPooling,
	AveragePooling
};


using namespace std;


namespace cpr {
	class Pooling
	{
	private:
		int s{1}, dim{ 2 };
		TypePoooling type = MaxPooling;
        
	public:
		Pooling() {}
		Pooling(int dim, TypePoooling type = MaxPooling) : dim{ dim }, type{ type } {}

		
		Image maxPoolingOneImage(Image&);
		vector<Image> maxPoolingManyImage(vector<Image>&);
		

		Image averagePooling(Image& I);


		vector<Image> averagePoolings(vector<Image>&);


		inline TypePoooling getType() {
			return this->type;
		}
	private:
		Pixel averagePixel(vector<vector<Pixel>>&);

		Pixel maxPixel(vector<vector<Pixel>>&);
	};
}


#endif // !__POOLING__
