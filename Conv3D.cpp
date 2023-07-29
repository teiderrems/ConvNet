#include "Conv3D.h"
#include "util.h"


using namespace cpr;

ImageGray cpr::Conv3D::convolutionOneImageOneFilter(Image& img, double b)
{

	
		int height = (1 + (img.p[0].size() - this->filter.height) / this->stride);
	    int width = (1 + (img.p.size() - this->filter.width ) / this->stride);
	    this->height=height;
	    this->width=width;
	    Image currentI;
	    currentI.label = img.label;
	    currentI.width = width;
	    currentI.height = height;
		currentI.p.resize(width);

		int canaux = img.p[0][0].pixel.size();
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
	        for (int j = 0; j < height; j ++)
	        {
				
	            for (int k = 0; k < this->filter.width; k++)
	            {
	                for (int t = 0; t < this->filter.height; t++)
	                {
	                    int e = i*this->stride + k;
	                    int z = j*this->stride + t;
	                    
						for (int c = 0; c < canaux; c++)
						{
							currentI.p[i][j].pixel[c] += img.p[e][z].pixel[c] * this->filter.p[k][t].pixel[c];
							
						}
	
	                }
	            }
	        }
	    }

		for (int i = 0; i < width; i++)
		{

			currentI.p[i].resize(height);
			for (int j = 0; j < height; j++)
			{
				currentI.p[i][j].pixel.resize(canaux);
				for (int c = 0; c < img.p[0][0].pixel.size(); c++)
				{
					currentI.p[i][j].pixel[c] += b;
					currentI.p[i][j].pixel[c]=Util::Relu(currentI.p[i][j].pixel[c]);
					
				}
			}
		}
		
	    return Util::addImageCanaux(currentI);
}

Image cpr::Conv3D::convolutionOnImageManyFilter(Image& img)
{

	vector<ImageGray> res;
	res.resize(this->filters.size());
	
	this->image = img;

	
	for (int i = 0; i < this->filters.size(); i++)
	{
		
		this->filter = this->filters[i];
		res[i] = this->convolutionOneImageOneFilter(img, this->biais[i]);
	}
	
	this->height = res[0].p[0].size();
	this->width = res[0].p.size();
	return Util::createImage(res);
}


void cpr::Conv3D::updateFilterOutputLayer(vector<Image>& dw, double alpha)
{
	for (int i = 0; i < this->filters.size(); i++)
	{
		for (int j = 0; j < this->filters[i].p.size(); j++)
		{

			for (int k = 0; k < this->filters[i].p[j].size(); k++)
			{
				for (int c = 0; c < this->filters[i].p[j][k].pixel.size(); c++)
				{
					this->filters[i].p[j][k].pixel[c] -= dw[i].p[j][k].pixel[c] * alpha;
				}
			}
		}
	}
}

Image cpr::Conv3D::dw(vector<Image>& dy)
{

	vector<Image>res;
	res.resize(dy.size());
	for (int i = 0; i < dy.size(); i++)
	{
		res[i] = Util::convolution(this->image, dy[i]);
	}

	return Util::addImage(res);
}

void cpr::Conv3D::saveBiais(int indexLayer)
{
	string filename = "cpr/biaisLayer_" + to_string(indexLayer+1)+".txt";
	ofstream f(filename);
	if (!f.is_open())
	{
		cout << "error d'ouverture" << endl; 
		exit(EXIT_FAILURE);
	}
	else
	{
		f << this->biais.size()<<"\n";
		for (int i = 0; i < this->biais.size(); i++)
		{
			f << this->biais[i] << "\n";
		}
	}
}


void cpr::Conv3D::updateBiaisOutputLayer(vector<Image>& dy, double alpha)
{
	for (int i = 0; i < this->biais.size(); i++)
	{
		this->biais[i] -= Util::sommeImage(dy[i]) * alpha;
	}
}

void cpr::Conv3D::saveFilter(int i)
{
	for (int j = 0; j < this->filters.size(); j++)
	{
		string filename = "cpr/filter(" + to_string(i+1) + "," + to_string(j+1) + ").txt";
		ofstream f(filename);
		if (!f.is_open())
		{
			cout << "error d'ouverture" << endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			f << this->filters[j].p.size() << " " << this->filters[j].p[0].size() <<" " << this->filters[j].p[0][0].pixel.size() << "\n";
			for (int t = 0; t < this->filters[j].p.size(); t++)
			{
				for (int k = 0; k < this->filters[j].p[t].size(); k++)
				{
					for (int c = 0; c < this->filters[j].p[t][k].pixel.size(); c++)
					{
						f << this->filters[j].p[t][k].pixel[c] << "\n";
					}
				}
			}

		}
	}
}


void cpr::Conv3D::backpropagationInputLayer(vector<Image>& d, double alpha)
{
	vector<Image> dy = this->dxManyFilter(d);
	this->updateBiaisOutputLayer(dy, alpha);
	vector<Image> dw = this->dwOutUpLayer(dy);
	this->updateFilterOutputLayer(dw, alpha);

	for (auto& i : dw)
	{
		for (auto& j : i.p)
		{
			for (auto& c : j)
			{
				vector<double>().swap(c.pixel);
			}
		}
	}
	vector<Image>().swap(dw);

	for (auto& i : dy)
	{
		for (auto& j : i.p)
		{
			for (auto& c : j)
			{
				vector<double>().swap(c.pixel);
			}
		}
	}
	vector<Image>().swap(dy);
	
	vector<Image>().swap(dy);
}

vector<Image> cpr::Conv3D::backpropagationInterLayers(vector<Image>& d, double alpha)
{

	
	vector<Image> dy = this->dxManyFilter(d);
	this->updateBiaisOutputLayer(dy, alpha);
	vector<Image> dw = this->dwOutUpLayer(dy);
	this->updateFilterOutputLayer(dw, alpha);

	
	for (auto& i : dw)
	{
		for (auto& j : i.p)
		{
			for (auto& c : j)
			{
				vector<double>().swap(c.pixel);
			}
		}
	}
	vector<Image>().swap(dw);
	
	return dy;
}

vector<Image> cpr::Conv3D::backpropagationOutputLayer(Image& dy, double alpha)
{
	vector<Image> dx = this->dxOutUpLayer(dy);
	vector<Image> dw = this->dwOutUpLayer(dx);
	this->updateBiaisOutputLayer(dx,alpha);
	this->updateFilterOutputLayer(dw, alpha);

	for (auto& i : dw)
	{
		for (auto& j : i.p)
		{
			for (auto& c : j)
			{
				vector<double>().swap(c.pixel);
			}
			vector<Pixel>().swap(j);
		}
	}
	vector<Image>().swap(dw);
	return dx;
}

vector<Image> cpr::Conv3D::dxOutUpLayer(Image& dy)
{
	

	vector<Image> res;
	vector<Image> f;

	res.resize(this->filters.size());
	f.resize(this->filters.size());

	for (int i = 0; i < f.size(); i++)
	{
		f[i] = Util::reverseFilter(this->filters[i]);
	}

	dy = Util::addPaddingFilter(dy, this->width, this->height, this->filters[0].p.size());
	int nc = this->filters[0].p[0][0].pixel.size();

	for (int i = 0; i < this->filters.size(); i++)
	{
		res[i] = Util::convolutionImageFilter(dy, f[i]);
	}


	for (auto& i : f)
	{
		for (auto& j : i.p)
		{
			for (auto& c : j)
			{
				vector<double>().swap(c.pixel);
			}
			vector<Pixel>().swap(j);
		}
	}
	vector<Image>().swap(f);

	

	return res;
}

vector<Image> cpr::Conv3D::dwOutUpLayer(vector<Image>& dx)
{
	

	vector<Image> dwO;
	dwO.resize(this->filters.size());
	for (int i = 0; i < this->filters.size(); i++)
	{
		dwO[i] = Util::convolution(this->image, dx[i]);
	}

	for (auto& i : image.p)
	{
		for (auto& j : i)
		{
			vector<double>().swap(j.pixel);
		}
		vector<Pixel>().swap(i);
	}
	

	return dwO;
}

vector<Image>cpr::Conv3D::dxManyFilter(vector<Image>& dy)
{
	

	vector<Image>res;
	res.resize(this->filters.size());
	vector<Image> f;
	f.resize(this->filters.size());
	for (int i = 0; i < this->filters.size(); i++)
	{
		f[i] = Util::reverseFilter(this->filters[i]);
	}
	for (int i = 0; i < dy.size(); i++)
	{
		dy[i] = Util::addPaddingFilter(dy[i], this->width, this->height, this->filters[i].p.size());
	}

	for (int i = 0; i < this->filters.size(); i++)
	{

		res[i] = this->dxHelper(f[i], dy);

	}
	
	return res;
}

Image cpr::Conv3D::dxHelper(Image& f, vector<Image>& dy)
{

	

	vector<Image> res;
	res.resize(dy.size());
	for (int i = 0; i < dy.size(); i++)
	{
		res[i] = Util::convolutionImageFilter(dy[i], f);
	}
	

	return Util::addImage(res);
}
