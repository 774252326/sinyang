#include "SurfaceMeshA.hpp"
#include "BHS.hpp"

class SurfaceMeshB : public SurfaceMesh
{
public:
	std::vector< std::vector<Point2D> > contourp;
	std::vector<double> contourValue;

	std::vector<double> triangleValue;
	SurfaceMeshA sfma;

public:
	SurfaceMeshB(void){};
	~SurfaceMeshB(void){};

	void Clear()
	{
		SurfaceMesh::Clear();
		triangleValue.clear();		
	};

	void ClearContour()
	{
		contourp.clear();
		contourValue.clear();
	};


	void GetEdgePoint(std::vector<size_t> &edgep, std::vector<double> &edgepValue)
	{
		std::vector<size_t> edgep0;
		std::vector<size_t> edgep1;
		std::vector<size_t> ti;
		FindEdge(edgep0,edgep1,ti);

		edgep0.resize(edgep0.size()+edgep1.size());
		std::copy_backward(edgep1.begin(),edgep1.end(),edgep0.end());


		std::sort(edgep0.begin(),edgep0.end());
		std::vector<size_t>::iterator it;
		it=std::unique(edgep0.begin(),edgep0.end());

		edgep.assign(edgep0.begin(),it);
		edgepValue.assign(edgep.size(),0);

		for(size_t i=0;i<edgep.size();i++){
			std::vector<double> dist2(point2triangle[edgep[i]].size());
			for(size_t j=0;j<dist2.size();j++){
				dist2[j]=Point2D::Dist2(point[edgep[i]],sfma.point[point2triangle[edgep[i]][j]]);
			}
			size_t mi=std::min_element(dist2.begin(),dist2.end())-dist2.begin();
			edgepValue[i]=sfma.pointValue[point2triangle[edgep[i]][mi]];
		}


	};

	void InitA()
	{
		sfma.point.assign(triangle.size(),Point2D());
		for(size_t i=0;i<sfma.point.size();i++){
			sfma.point[i]=point[triangle[i][0]]+point[triangle[i][1]]+point[triangle[i][2]];
			sfma.point[i].x[0]/=3;
			sfma.point[i].x[1]/=3;
		}
		sfma.pointValue.assign(triangleValue.begin(),triangleValue.end());


		//std::vector<size_t> edgepoint;
		//std::vector<double> edgepointv;

		//GenPointToFaceMap();
		//GetEdgePoint(edgepoint,edgepointv);

		//for(size_t i=0;i<edgepoint.size();i++){
		//	sfma.point.push_back(point[edgepoint[i]]);
		//	sfma.pointValue.push_back(edgepointv[i]);
		//}

		bool flag=sfma.Delaunay2DMesh();

		sfma.GenPointToFaceMap();
		sfma.ClearContour();
	};

	void InitB()
	{		
		sfma.point.assign(point.begin(),point.end());
		sfma.triangle.assign(triangle.begin(),triangle.end());
		sfma.pointValue.assign(sfma.point.size(),0);

		BHS bhs;
		std::vector<double> pt(2);
		bhs.point.assign(triangle.size(),pt);
		for(size_t i=0;i<bhs.point.size();i++){
			Point2D tmp=point[triangle[i][0]]+point[triangle[i][1]]+point[triangle[i][2]];
			tmp.x[0]/=3;
			tmp.x[1]/=3;
			bhs.point[i].assign(tmp.x,tmp.x+2);
		}
		bhs.pointValue.assign(triangleValue.begin(),triangleValue.end());

		bhs.CalCoef();

		for(size_t i=0;i<sfma.pointValue.size();i++){
			std::vector<double> ptx(sfma.point[i].x,sfma.point[i].x+2);
			sfma.pointValue[i]=bhs.BHSInt(ptx);
		}

		sfma.GenPointToFaceMap();
		sfma.ClearContour();
	};

	void InitC()
	{		
		sfma.point.assign(point.begin(),point.end());
		sfma.triangle.assign(triangle.begin(),triangle.end());
		sfma.pointValue.assign(sfma.point.size(),0);
		sfma.GenPointToFaceMap();


		for(size_t i=0;i<sfma.pointValue.size();i++){

			if(sfma.point2triangle[i].size()>1){
				BHS bhs;
				std::vector<double> pt(2);
				bhs.point.assign(sfma.point2triangle[i].size(),pt);
				bhs.pointValue.assign(sfma.point2triangle[i].size(),0);

				for(size_t j=0;j<bhs.point.size();j++){
					size_t ti=sfma.point2triangle[i][j];
					Point2D tmp=point[triangle[ti][0]]+point[triangle[ti][1]]+point[triangle[ti][2]];
					tmp.x[0]/=3;
					tmp.x[1]/=3;
					bhs.point[j].assign(tmp.x,tmp.x+2);
					bhs.pointValue[j]=triangleValue[ti];
				}

				bhs.CalCoef();
				std::vector<double> ptx(sfma.point[i].x,sfma.point[i].x+2);
				sfma.pointValue[i]=bhs.BHSInt(ptx);
			}
			else{
				sfma.pointValue[i]=triangleValue[sfma.point2triangle[i].back()];
			}
		}


		sfma.ClearContour();
	};

	void InitD()
	{		
		sfma.point.assign(point.begin(),point.end());
		sfma.triangle.assign(triangle.begin(),triangle.end());
		sfma.pointValue.assign(sfma.point.size(),0);
		sfma.GenPointToFaceMap();


		for(size_t i=0;i<sfma.pointValue.size();i++){
				for(size_t j=0;j<sfma.point2triangle[i].size();j++){
				sfma.pointValue[i]+=triangleValue[sfma.point2triangle[i][j]];
				}
				sfma.pointValue[i]/=sfma.point2triangle[i].size();
		}
		sfma.ClearContour();
	};


	void GenContourMap(const std::vector<double> &vl)
	{
		sfma.GenContourMap(vl);
		contourp.resize(contourp.size()+sfma.contourp.size());
		std::copy_backward(sfma.contourp.begin(),sfma.contourp.end(),contourp.end());
		contourValue.resize(contourValue.size()+sfma.contourValue.size());
		std::copy_backward(sfma.contourValue.begin(),sfma.contourValue.end(),contourValue.end());
	};

	void GenContourMap(size_t Ncontour, double vmin, double vmax)
	{
		sfma.GenContourMap(Ncontour,vmin,vmax);
		contourp.resize(contourp.size()+sfma.contourp.size());
		std::copy_backward(sfma.contourp.begin(),sfma.contourp.end(),contourp.end());
		contourValue.resize(contourValue.size()+sfma.contourValue.size());
		std::copy_backward(sfma.contourValue.begin(),sfma.contourValue.end(),contourValue.end());
	};

	void GenContourMap(size_t Ncontour=10)
	{
		sfma.GenContourMap(Ncontour);
		contourp.resize(contourp.size()+sfma.contourp.size());
		std::copy_backward(sfma.contourp.begin(),sfma.contourp.end(),contourp.end());
		contourValue.resize(contourValue.size()+sfma.contourValue.size());
		std::copy_backward(sfma.contourValue.begin(),sfma.contourValue.end(),contourValue.end());
	};

};